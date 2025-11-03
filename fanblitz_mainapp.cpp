/*
 * FanBlitz: Advanced Sports Performance Analysis Platform
 * Author: Michael Semera
 * Description: Comprehensive sports analytics with time series analysis and ranking systems
 * 
 * This platform analyzes player and team performance across multiple seasons
 * using statistical methods, correlation analysis, and consistency metrics.
 */

#ifndef FANBLITZ_H
#define FANBLITZ_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <cmath>
#include <numeric>
#include <iomanip>
#include <memory>
#include <stdexcept>

// Performance metrics constants
constexpr double CONSISTENCY_WEIGHT = 0.4;
constexpr double AVERAGE_WEIGHT = 0.6;
constexpr int TOP_N_PLAYERS = 10;
constexpr int ROLLING_WINDOW = 5;

/*
 * PlayerStats - Core statistics for a player in a single season
 */
struct PlayerStats {
    std::string player_id;
    std::string player_name;
    std::string team;
    std::string position;
    std::string season;
    
    // Performance metrics
    int matches_played;
    int minutes_played;
    int goals;
    int assists;
    int shots;
    int shots_on_target;
    int passes;
    double pass_accuracy;
    int tackles;
    int interceptions;
    
    // Calculated metrics
    double goals_per_match;
    double assists_per_match;
    double rating;
    
    PlayerStats() : matches_played(0), minutes_played(0), goals(0), 
                   assists(0), shots(0), shots_on_target(0), passes(0),
                   pass_accuracy(0.0), tackles(0), interceptions(0),
                   goals_per_match(0.0), assists_per_match(0.0), rating(0.0) {}
};

/*
 * TimeSeriesData - Performance metrics over time
 */
struct TimeSeriesData {
    std::vector<std::string> seasons;
    std::vector<double> values;
    std::string metric_name;
    std::string player_name;
    
    double mean() const {
        if (values.empty()) return 0.0;
        return std::accumulate(values.begin(), values.end(), 0.0) / values.size();
    }
    
    double std_dev() const {
        if (values.size() < 2) return 0.0;
        double avg = mean();
        double sq_sum = 0.0;
        for (const auto& val : values) {
            sq_sum += (val - avg) * (val - avg);
        }
        return std::sqrt(sq_sum / (values.size() - 1));
    }
    
    double coefficient_of_variation() const {
        double avg = mean();
        if (avg == 0.0) return 0.0;
        return (std_dev() / avg) * 100.0;
    }
};

/*
 * ConsistencyMetrics - Player consistency analysis
 */
struct ConsistencyMetrics {
    std::string player_name;
    double average_rating;
    double std_deviation;
    double consistency_score;  // Lower is more consistent
    double cv_percentage;      // Coefficient of variation
    int seasons_played;
    
    ConsistencyMetrics() : average_rating(0.0), std_deviation(0.0),
                          consistency_score(0.0), cv_percentage(0.0),
                          seasons_played(0) {}
};

/*
 * TeamPerformance - Team-level aggregated statistics
 */
struct TeamPerformance {
    std::string team_name;
    std::string season;
    int total_goals;
    int total_assists;
    double average_rating;
    int total_matches;
    double win_rate;
    
    TeamPerformance() : total_goals(0), total_assists(0), average_rating(0.0),
                       total_matches(0), win_rate(0.0) {}
};

/*
 * PerformanceRanking - Ranking structure for players
 */
struct PerformanceRanking {
    int rank;
    std::string player_name;
    std::string position;
    double score;
    double average_rating;
    double consistency_score;
    int seasons_played;
    
    bool operator>(const PerformanceRanking& other) const {
        return score > other.score;
    }
};

/*
 * CSVParser - Parse CSV files with player statistics
 */
class CSVParser {
private:
    std::string filename;
    std::vector<std::string> headers;
    
    std::vector<std::string> split_line(const std::string& line, char delimiter = ',') {
        std::vector<std::string> tokens;
        std::stringstream ss(line);
        std::string token;
        
        while (std::getline(ss, token, delimiter)) {
            // Trim whitespace
            token.erase(0, token.find_first_not_of(" \t\r\n"));
            token.erase(token.find_last_not_of(" \t\r\n") + 1);
            tokens.push_back(token);
        }
        
        return tokens;
    }
    
public:
    CSVParser(const std::string& file) : filename(file) {}
    
    std::vector<PlayerStats> parse() {
        std::vector<PlayerStats> players;
        std::ifstream file(filename);
        
        if (!file.is_open()) {
            throw std::runtime_error("Failed to open file: " + filename);
        }
        
        std::string line;
        bool first_line = true;
        
        while (std::getline(file, line)) {
            if (line.empty()) continue;
            
            if (first_line) {
                headers = split_line(line);
                first_line = false;
                continue;
            }
            
            auto tokens = split_line(line);
            if (tokens.size() < headers.size()) continue;
            
            PlayerStats player;
            
            // Parse each field (adjust indices based on your CSV format)
            try {
                player.player_id = tokens[0];
                player.player_name = tokens[1];
                player.team = tokens[2];
                player.position = tokens[3];
                player.season = tokens[4];
                player.matches_played = std::stoi(tokens[5]);
                player.minutes_played = std::stoi(tokens[6]);
                player.goals = std::stoi(tokens[7]);
                player.assists = std::stoi(tokens[8]);
                player.shots = std::stoi(tokens[9]);
                player.shots_on_target = std::stoi(tokens[10]);
                player.passes = std::stoi(tokens[11]);
                player.pass_accuracy = std::stod(tokens[12]);
                player.tackles = std::stoi(tokens[13]);
                player.interceptions = std::stoi(tokens[14]);
                
                // Calculate derived metrics
                if (player.matches_played > 0) {
                    player.goals_per_match = static_cast<double>(player.goals) / 
                                            player.matches_played;
                    player.assists_per_match = static_cast<double>(player.assists) / 
                                              player.matches_played;
                }
                
                player.rating = std::stod(tokens[15]);
                
                players.push_back(player);
            } catch (const std::exception& e) {
                std::cerr << "Warning: Skipping invalid row: " << e.what() << std::endl;
                continue;
            }
        }
        
        file.close();
        return players;
    }
    
    const std::vector<std::string>& get_headers() const {
        return headers;
    }
};

/*
 * StatisticalAnalyzer - Performs statistical calculations
 */
class StatisticalAnalyzer {
public:
    static double calculate_mean(const std::vector<double>& data) {
        if (data.empty()) return 0.0;
        return std::accumulate(data.begin(), data.end(), 0.0) / data.size();
    }
    
    static double calculate_median(std::vector<double> data) {
        if (data.empty()) return 0.0;
        
        std::sort(data.begin(), data.end());
        size_t n = data.size();
        
        if (n % 2 == 0) {
            return (data[n/2 - 1] + data[n/2]) / 2.0;
        } else {
            return data[n/2];
        }
    }
    
    static double calculate_std_dev(const std::vector<double>& data) {
        if (data.size() < 2) return 0.0;
        
        double mean = calculate_mean(data);
        double sq_sum = 0.0;
        
        for (const auto& val : data) {
            sq_sum += (val - mean) * (val - mean);
        }
        
        return std::sqrt(sq_sum / (data.size() - 1));
    }
    
    static double calculate_correlation(const std::vector<double>& x, 
                                       const std::vector<double>& y) {
        if (x.size() != y.size() || x.size() < 2) return 0.0;
        
        double mean_x = calculate_mean(x);
        double mean_y = calculate_mean(y);
        
        double numerator = 0.0;
        double sum_sq_x = 0.0;
        double sum_sq_y = 0.0;
        
        for (size_t i = 0; i < x.size(); ++i) {
            double diff_x = x[i] - mean_x;
            double diff_y = y[i] - mean_y;
            numerator += diff_x * diff_y;
            sum_sq_x += diff_x * diff_x;
            sum_sq_y += diff_y * diff_y;
        }
        
        double denominator = std::sqrt(sum_sq_x * sum_sq_y);
        if (denominator == 0.0) return 0.0;
        
        return numerator / denominator;
    }
    
    static std::vector<double> calculate_rolling_average(const std::vector<double>& data,
                                                        int window_size) {
        std::vector<double> result;
        if (data.size() < static_cast<size_t>(window_size)) {
            return result;
        }
        
        for (size_t i = 0; i <= data.size() - window_size; ++i) {
            double sum = 0.0;
            for (int j = 0; j < window_size; ++j) {
                sum += data[i + j];
            }
            result.push_back(sum / window_size);
        }
        
        return result;
    }
    
    static double calculate_percentile(std::vector<double> data, double percentile) {
        if (data.empty()) return 0.0;
        
        std::sort(data.begin(), data.end());
        double index = (percentile / 100.0) * (data.size() - 1);
        size_t lower = static_cast<size_t>(std::floor(index));
        size_t upper = static_cast<size_t>(std::ceil(index));
        
        if (lower == upper) {
            return data[lower];
        }
        
        double weight = index - lower;
        return data[lower] * (1 - weight) + data[upper] * weight;
    }
};

/*
 * PerformanceAnalyzer - Core analysis engine
 */
class PerformanceAnalyzer {
private:
    std::vector<PlayerStats> all_players;
    std::map<std::string, std::vector<PlayerStats>> players_by_name;
    std::map<std::string, std::vector<PlayerStats>> players_by_team;
    
    void index_players() {
        players_by_name.clear();
        players_by_team.clear();
        
        for (const auto& player : all_players) {
            players_by_name[player.player_name].push_back(player);
            players_by_team[player.team].push_back(player);
        }
    }
    
public:
    PerformanceAnalyzer() = default;
    
    void load_data(const std::string& filename) {
        std::cout << "📊 Loading data from: " << filename << std::endl;
        
        CSVParser parser(filename);
        all_players = parser.parse();
        index_players();
        
        std::cout << "✓ Loaded " << all_players.size() << " player records" << std::endl;
        std::cout << "✓ Unique players: " << players_by_name.size() << std::endl;
        std::cout << "✓ Teams: " << players_by_team.size() << std::endl;
    }
    
    TimeSeriesData get_player_time_series(const std::string& player_name,
                                         const std::string& metric) {
        TimeSeriesData ts;
        ts.player_name = player_name;
        ts.metric_name = metric;
        
        auto it = players_by_name.find(player_name);
        if (it == players_by_name.end()) {
            return ts;
        }
        
        // Sort by season
        auto player_seasons = it->second;
        std::sort(player_seasons.begin(), player_seasons.end(),
                 [](const PlayerStats& a, const PlayerStats& b) {
                     return a.season < b.season;
                 });
        
        for (const auto& stat : player_seasons) {
            ts.seasons.push_back(stat.season);
            
            if (metric == "rating") {
                ts.values.push_back(stat.rating);
            } else if (metric == "goals") {
                ts.values.push_back(stat.goals);
            } else if (metric == "assists") {
                ts.values.push_back(stat.assists);
            } else if (metric == "goals_per_match") {
                ts.values.push_back(stat.goals_per_match);
            }
        }
        
        return ts;
    }
    
    ConsistencyMetrics calculate_consistency(const std::string& player_name) {
        ConsistencyMetrics metrics;
        metrics.player_name = player_name;
        
        auto ts = get_player_time_series(player_name, "rating");
        if (ts.values.empty()) {
            return metrics;
        }
        
        metrics.average_rating = ts.mean();
        metrics.std_deviation = ts.std_dev();
        metrics.cv_percentage = ts.coefficient_of_variation();
        metrics.seasons_played = static_cast<int>(ts.values.size());
        
        // Consistency score: lower CV means more consistent
        // Normalize to 0-100 scale where 100 is most consistent
        metrics.consistency_score = 100.0 - std::min(100.0, metrics.cv_percentage);
        
        return metrics;
    }
    
    std::vector<ConsistencyMetrics> get_all_consistency_metrics() {
        std::vector<ConsistencyMetrics> all_metrics;
        
        for (const auto& pair : players_by_name) {
            if (pair.second.size() >= 2) {  // At least 2 seasons
                all_metrics.push_back(calculate_consistency(pair.first));
            }
        }
        
        return all_metrics;
    }
    
    std::vector<PerformanceRanking> get_top_consistent_players(int top_n = TOP_N_PLAYERS) {
        std::vector<PerformanceRanking> rankings;
        auto all_metrics = get_all_consistency_metrics();
        
        for (const auto& metrics : all_metrics) {
            PerformanceRanking ranking;
            ranking.player_name = metrics.player_name;
            ranking.average_rating = metrics.average_rating;
            ranking.consistency_score = metrics.consistency_score;
            ranking.seasons_played = metrics.seasons_played;
            
            // Combined score: weighted average of performance and consistency
            ranking.score = (metrics.average_rating * AVERAGE_WEIGHT) +
                          (metrics.consistency_score * CONSISTENCY_WEIGHT);
            
            // Get position from first season data
            auto it = players_by_name.find(metrics.player_name);
            if (it != players_by_name.end() && !it->second.empty()) {
                ranking.position = it->second[0].position;
            }
            
            rankings.push_back(ranking);
        }
        
        // Sort by score
        std::sort(rankings.begin(), rankings.end(), std::greater<PerformanceRanking>());
        
        // Assign ranks
        for (size_t i = 0; i < rankings.size() && i < static_cast<size_t>(top_n); ++i) {
            rankings[i].rank = static_cast<int>(i + 1);
        }
        
        // Return only top N
        if (rankings.size() > static_cast<size_t>(top_n)) {
            rankings.resize(top_n);
        }
        
        return rankings;
    }
    
    std::map<std::string, double> get_position_averages(const std::string& metric) {
        std::map<std::string, std::vector<double>> position_data;
        
        for (const auto& player : all_players) {
            if (metric == "rating") {
                position_data[player.position].push_back(player.rating);
            } else if (metric == "goals") {
                position_data[player.position].push_back(player.goals);
            } else if (metric == "assists") {
                position_data[player.position].push_back(player.assists);
            }
        }
        
        std::map<std::string, double> averages;
        for (const auto& pair : position_data) {
            averages[pair.first] = StatisticalAnalyzer::calculate_mean(pair.second);
        }
        
        return averages;
    }
    
    std::vector<TeamPerformance> get_team_performance_by_season() {
        std::map<std::pair<std::string, std::string>, TeamPerformance> team_season_map;
        
        for (const auto& player : all_players) {
            auto key = std::make_pair(player.team, player.season);
            auto& perf = team_season_map[key];
            
            perf.team_name = player.team;
            perf.season = player.season;
            perf.total_goals += player.goals;
            perf.total_assists += player.assists;
            perf.total_matches += player.matches_played;
            perf.average_rating += player.rating;
        }
        
        std::vector<TeamPerformance> result;
        for (auto& pair : team_season_map) {
            auto& perf = pair.second;
            // Average the rating
            size_t player_count = players_by_team[perf.team_name].size();
            if (player_count > 0) {
                perf.average_rating /= player_count;
            }
            result.push_back(perf);
        }
        
        return result;
    }
    
    double calculate_player_correlation(const std::string& player1,
                                       const std::string& player2,
                                       const std::string& metric) {
        auto ts1 = get_player_time_series(player1, metric);
        auto ts2 = get_player_time_series(player2, metric);
        
        if (ts1.values.empty() || ts2.values.empty()) {
            return 0.0;
        }
        
        // Find common seasons
        std::vector<double> common_vals1, common_vals2;
        for (size_t i = 0; i < ts1.seasons.size(); ++i) {
            for (size_t j = 0; j < ts2.seasons.size(); ++j) {
                if (ts1.seasons[i] == ts2.seasons[j]) {
                    common_vals1.push_back(ts1.values[i]);
                    common_vals2.push_back(ts2.values[j]);
                    break;
                }
            }
        }
        
        return StatisticalAnalyzer::calculate_correlation(common_vals1, common_vals2);
    }
    
    const std::vector<PlayerStats>& get_all_players() const {
        return all_players;
    }
    
    std::vector<std::string> get_all_player_names() const {
        std::vector<std::string> names;
        for (const auto& pair : players_by_name) {
            names.push_back(pair.first);
        }
        return names;
    }
};

/*
 * ReportGenerator - Generate analysis reports
 */
class ReportGenerator {
private:
    PerformanceAnalyzer& analyzer;
    
    std::string get_current_datetime() {
        auto now = std::time(nullptr);
        auto tm = *std::localtime(&now);
        std::ostringstream oss;
        oss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");
        return oss.str();
    }
    
public:
    ReportGenerator(PerformanceAnalyzer& analyzer) : analyzer(analyzer) {}
    
    void generate_consistency_report(const std::string& filename) {
        std::ofstream file(filename);
        if (!file.is_open()) {
            throw std::runtime_error("Failed to create report file");
        }
        
        file << std::string(80, '=') << "\n";
        file << "FANBLITZ: SPORTS PERFORMANCE ANALYSIS REPORT\n";
        file << "Author: Michael Semera\n";
        file << "Generated: " << get_current_datetime() << "\n";
        file << std::string(80, '=') << "\n\n";
        
        file << "TOP 10 MOST CONSISTENT PLAYERS\n";
        file << std::string(80, '-') << "\n";
        file << std::left << std::setw(5) << "Rank"
             << std::setw(25) << "Player Name"
             << std::setw(15) << "Position"
             << std::setw(12) << "Avg Rating"
             << std::setw(12) << "Consistency"
             << std::setw(10) << "Seasons" << "\n";
        file << std::string(80, '-') << "\n";
        
        auto rankings = analyzer.get_top_consistent_players(10);
        for (const auto& rank : rankings) {
            file << std::left << std::setw(5) << rank.rank
                 << std::setw(25) << rank.player_name
                 << std::setw(15) << rank.position
                 << std::setw(12) << std::fixed << std::setprecision(2) << rank.average_rating
                 << std::setw(12) << std::fixed << std::setprecision(2) << rank.consistency_score
                 << std::setw(10) << rank.seasons_played << "\n";
        }
        
        file << "\n" << std::string(80, '=') << "\n";
        file << "ANALYSIS METHODOLOGY\n";
        file << std::string(80, '-') << "\n";
        file << "Consistency Score: Calculated using coefficient of variation\n";
        file << "  - Lower variation = Higher consistency\n";
        file << "  - Score ranges from 0-100 (100 = most consistent)\n\n";
        file << "Combined Score = (Average Rating × " << AVERAGE_WEIGHT << ") + ";
        file << "(Consistency × " << CONSISTENCY_WEIGHT << ")\n\n";
        
        file << "KEY INSIGHTS:\n";
        if (!rankings.empty()) {
            file << "• Most consistent player: " << rankings[0].player_name << "\n";
            file << "  - Average Rating: " << rankings[0].average_rating << "\n";
            file << "  - Consistency Score: " << rankings[0].consistency_score << "\n";
        }
        
        file << "\n" << std::string(80, '=') << "\n";
        file << "END OF REPORT\n";
        file << std::string(80, '=') << "\n";
        
        file.close();
        std::cout << "✓ Report saved to: " << filename << std::endl;
    }
    
    void generate_csv_export(const std::string& filename) {
        std::ofstream file(filename);
        if (!file.is_open()) {
            throw std::runtime_error("Failed to create CSV file");
        }
        
        file << "Rank,Player Name,Position,Average Rating,Consistency Score,Seasons Played\n";
        
        auto rankings = analyzer.get_top_consistent_players(TOP_N_PLAYERS);
        for (const auto& rank : rankings) {
            file << rank.rank << ","
                 << rank.player_name << ","
                 << rank.position << ","
                 << std::fixed << std::setprecision(2) << rank.average_rating << ","
                 << std::fixed << std::setprecision(2) << rank.consistency_score << ","
                 << rank.seasons_played << "\n";
        }
        
        file.close();
        std::cout << "✓ CSV export saved to: " << filename << std::endl;
    }
};

#endif // FANBLITZ_H