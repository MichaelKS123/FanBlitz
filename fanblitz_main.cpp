/*
 * FanBlitz Main Application
 * Author: Michael Semera
 * 
 * Usage: ./fanblitz <data_file.csv>
 */

#include "fanblitz.h"

void print_banner() {
    std::cout << "\n";
    std::cout << "╔══════════════════════════════════════════════════════════════╗\n";
    std::cout << "║                      ⚡ FANBLITZ ⚡                          ║\n";
    std::cout << "║         Advanced Sports Performance Analytics               ║\n";
    std::cout << "║                 Author: Michael Semera                       ║\n";
    std::cout << "╚══════════════════════════════════════════════════════════════╝\n";
    std::cout << "\n";
}

void print_menu() {
    std::cout << "\n╔══════════════════════════════════════════════════════════════╗\n";
    std::cout << "║                      ANALYSIS MENU                           ║\n";
    std::cout << "╠══════════════════════════════════════════════════════════════╣\n";
    std::cout << "║  1. Show Top 10 Most Consistent Players                      ║\n";
    std::cout << "║  2. Analyze Specific Player Performance                      ║\n";
    std::cout << "║  3. Compare Two Players                                      ║\n";
    std::cout << "║  4. Team Performance Analysis                                ║\n";
    std::cout << "║  5. Position-based Statistics                                ║\n";
    std::cout << "║  6. Generate Full Report                                     ║\n";
    std::cout << "║  7. Export Data to CSV                                       ║\n";
    std::cout << "║  8. Exit                                                     ║\n";
    std::cout << "╚══════════════════════════════════════════════════════════════╝\n";
    std::cout << "Enter your choice: ";
}

void display_top_consistent_players(PerformanceAnalyzer& analyzer) {
    std::cout << "\n" << std::string(80, '=') << "\n";
    std::cout << "TOP 10 MOST CONSISTENT PLAYERS\n";
    std::cout << std::string(80, '=') << "\n\n";
    
    auto rankings = analyzer.get_top_consistent_players(10);
    
    std::cout << std::left << std::setw(5) << "Rank"
              << std::setw(25) << "Player Name"
              << std::setw(15) << "Position"
              << std::setw(12) << "Avg Rating"
              << std::setw(12) << "Consistency"
              << std::setw(10) << "Seasons" << "\n";
    std::cout << std::string(80, '-') << "\n";
    
    for (const auto& rank : rankings) {
        std::cout << std::left << std::setw(5) << rank.rank
                  << std::setw(25) << rank.player_name
                  << std::setw(15) << rank.position
                  << std::setw(12) << std::fixed << std::setprecision(2) 
                  << rank.average_rating
                  << std::setw(12) << std::fixed << std::setprecision(2) 
                  << rank.consistency_score
                  << std::setw(10) << rank.seasons_played << "\n";
    }
    
    std::cout << "\n💡 Insight: " << rankings[0].player_name 
              << " shows the most consistent performance with a consistency score of "
              << std::fixed << std::setprecision(1) << rankings[0].consistency_score
              << " across " << rankings[0].seasons_played << " seasons.\n";
}

void analyze_player_performance(PerformanceAnalyzer& analyzer) {
    std::cout << "\nEnter player name: ";
    std::string player_name;
    std::cin.ignore();
    std::getline(std::cin, player_name);
    
    auto metrics = analyzer.calculate_consistency(player_name);
    
    if (metrics.seasons_played == 0) {
        std::cout << "❌ Player not found or insufficient data.\n";
        return;
    }
    
    std::cout << "\n" << std::string(60, '=') << "\n";
    std::cout << "PLAYER PERFORMANCE ANALYSIS: " << player_name << "\n";
    std::cout << std::string(60, '=') << "\n\n";
    
    std::cout << "📊 Career Statistics:\n";
    std::cout << "  • Seasons Played: " << metrics.seasons_played << "\n";
    std::cout << "  • Average Rating: " << std::fixed << std::setprecision(2) 
              << metrics.average_rating << "\n";
    std::cout << "  • Consistency Score: " << std::fixed << std::setprecision(2) 
              << metrics.consistency_score << "/100\n";
    std::cout << "  • Standard Deviation: " << std::fixed << std::setprecision(2) 
              << metrics.std_deviation << "\n";
    std::cout << "  • Coefficient of Variation: " << std::fixed << std::setprecision(2) 
              << metrics.cv_percentage << "%\n\n";
    
    // Show time series data
    auto ts_rating = analyzer.get_player_time_series(player_name, "rating");
    auto ts_goals = analyzer.get_player_time_series(player_name, "goals");
    
    std::cout << "📈 Season-by-Season Performance:\n";
    std::cout << std::left << std::setw(12) << "Season" 
              << std::setw(10) << "Rating" 
              << std::setw(10) << "Goals" << "\n";
    std::cout << std::string(32, '-') << "\n";
    
    for (size_t i = 0; i < ts_rating.seasons.size(); ++i) {
        std::cout << std::left << std::setw(12) << ts_rating.seasons[i]
                  << std::setw(10) << std::fixed << std::setprecision(1) 
                  << ts_rating.values[i]
                  << std::setw(10) << static_cast<int>(ts_goals.values[i]) << "\n";
    }
    
    // Performance trend
    if (ts_rating.values.size() >= 2) {
        double first_half_avg = 0.0;
        double second_half_avg = 0.0;
        size_t mid = ts_rating.values.size() / 2;
        
        for (size_t i = 0; i < mid; ++i) {
            first_half_avg += ts_rating.values[i];
        }
        for (size_t i = mid; i < ts_rating.values.size(); ++i) {
            second_half_avg += ts_rating.values[i];
        }
        
        first_half_avg /= mid;
        second_half_avg /= (ts_rating.values.size() - mid);
        
        std::cout << "\n📉 Performance Trend:\n";
        if (second_half_avg > first_half_avg) {
            std::cout << "  ↗️ Improving: Rating increased by "
                      << std::fixed << std::setprecision(1)
                      << (second_half_avg - first_half_avg) << " points\n";
        } else if (second_half_avg < first_half_avg) {
            std::cout << "  ↘️ Declining: Rating decreased by "
                      << std::fixed << std::setprecision(1)
                      << (first_half_avg - second_half_avg) << " points\n";
        } else {
            std::cout << "  → Stable: Consistent performance maintained\n";
        }
    }
}

void compare_two_players(PerformanceAnalyzer& analyzer) {
    std::cout << "\nEnter first player name: ";
    std::string player1;
    std::cin.ignore();
    std::getline(std::cin, player1);
    
    std::cout << "Enter second player name: ";
    std::string player2;
    std::getline(std::cin, player2);
    
    auto metrics1 = analyzer.calculate_consistency(player1);
    auto metrics2 = analyzer.calculate_consistency(player2);
    
    if (metrics1.seasons_played == 0 || metrics2.seasons_played == 0) {
        std::cout << "❌ One or both players not found.\n";
        return;
    }
    
    std::cout << "\n" << std::string(70, '=') << "\n";
    std::cout << "PLAYER COMPARISON: " << player1 << " vs " << player2 << "\n";
    std::cout << std::string(70, '=') << "\n\n";
    
    std::cout << std::left << std::setw(25) << "Metric"
              << std::setw(20) << player1
              << std::setw(20) << player2 << "\n";
    std::cout << std::string(70, '-') << "\n";
    
    std::cout << std::left << std::setw(25) << "Average Rating"
              << std::setw(20) << std::fixed << std::setprecision(2) 
              << metrics1.average_rating
              << std::setw(20) << std::fixed << std::setprecision(2) 
              << metrics2.average_rating << "\n";
    
    std::cout << std::left << std::setw(25) << "Consistency Score"
              << std::setw(20) << std::fixed << std::setprecision(2) 
              << metrics1.consistency_score
              << std::setw(20) << std::fixed << std::setprecision(2) 
              << metrics2.consistency_score << "\n";
    
    std::cout << std::left << std::setw(25) << "Seasons Played"
              << std::setw(20) << metrics1.seasons_played
              << std::setw(20) << metrics2.seasons_played << "\n";
    
    std::cout << std::left << std::setw(25) << "Std Deviation"
              << std::setw(20) << std::fixed << std::setprecision(2) 
              << metrics1.std_deviation
              << std::setw(20) << std::fixed << std::setprecision(2) 
              << metrics2.std_deviation << "\n";
    
    // Calculate correlation
    double correlation = analyzer.calculate_player_correlation(player1, player2, "rating");
    
    std::cout << "\n📊 Statistical Analysis:\n";
    std::cout << "  • Correlation: " << std::fixed << std::setprecision(3) 
              << correlation << "\n";
    
    if (std::abs(correlation) > 0.7) {
        std::cout << "  • Interpretation: Strong correlation - Similar performance patterns\n";
    } else if (std::abs(correlation) > 0.4) {
        std::cout << "  • Interpretation: Moderate correlation\n";
    } else {
        std::cout << "  • Interpretation: Weak correlation - Independent performance patterns\n";
    }
    
    std::cout << "\n🏆 Winner:\n";
    if (metrics1.average_rating > metrics2.average_rating) {
        std::cout << "  • Higher Rating: " << player1 << "\n";
    } else {
        std::cout << "  • Higher Rating: " << player2 << "\n";
    }
    
    if (metrics1.consistency_score > metrics2.consistency_score) {
        std::cout << "  • More Consistent: " << player1 << "\n";
    } else {
        std::cout << "  • More Consistent: " << player2 << "\n";
    }
}

void analyze_team_performance(PerformanceAnalyzer& analyzer) {
    std::cout << "\n" << std::string(80, '=') << "\n";
    std::cout << "TEAM PERFORMANCE ANALYSIS\n";
    std::cout << std::string(80, '=') << "\n\n";
    
    auto team_perf = analyzer.get_team_performance_by_season();
    
    // Group by team
    std::map<std::string, std::vector<TeamPerformance>> teams;
    for (const auto& perf : team_perf) {
        teams[perf.team_name].push_back(perf);
    }
    
    std::cout << std::left << std::setw(25) << "Team"
              << std::setw(15) << "Seasons"
              << std::setw(15) << "Avg Rating"
              << std::setw(15) << "Total Goals" << "\n";
    std::cout << std::string(80, '-') << "\n";
    
    for (const auto& pair : teams) {
        double avg_rating = 0.0;
        int total_goals = 0;
        
        for (const auto& perf : pair.second) {
            avg_rating += perf.average_rating;
            total_goals += perf.total_goals;
        }
        avg_rating /= pair.second.size();
        
        std::cout << std::left << std::setw(25) << pair.first
                  << std::setw(15) << pair.second.size()
                  << std::setw(15) << std::fixed << std::setprecision(2) << avg_rating
                  << std::setw(15) << total_goals << "\n";
    }
}

void analyze_position_statistics(PerformanceAnalyzer& analyzer) {
    std::cout << "\n" << std::string(60, '=') << "\n";
    std::cout << "POSITION-BASED STATISTICS\n";
    std::cout << std::string(60, '=') << "\n\n";
    
    auto rating_avg = analyzer.get_position_averages("rating");
    auto goals_avg = analyzer.get_position_averages("goals");
    auto assists_avg = analyzer.get_position_averages("assists");
    
    std::cout << std::left << std::setw(20) << "Position"
              << std::setw(15) << "Avg Rating"
              << std::setw(12) << "Avg Goals"
              << std::setw(12) << "Avg Assists" << "\n";
    std::cout << std::string(60, '-') << "\n";
    
    for (const auto& pair : rating_avg) {
        std::cout << std::left << std::setw(20) << pair.first
                  << std::setw(15) << std::fixed << std::setprecision(2) << pair.second
                  << std::setw(12) << std::fixed << std::setprecision(1) << goals_avg[pair.first]
                  << std::setw(12) << std::fixed << std::setprecision(1) << assists_avg[pair.first] << "\n";
    }
}

int main(int argc, char* argv[]) {
    print_banner();
    
    if (argc < 2) {
        std::cout << "Usage: " << argv[0] << " <data_file.csv>\n";
        std::cout << "\nExample: " << argv[0] << " player_stats.csv\n";
        return 1;
    }
    
    try {
        PerformanceAnalyzer analyzer;
        analyzer.load_data(argv[1]);
        
        ReportGenerator reporter(analyzer);
        
        bool running = true;
        while (running) {
            print_menu();
            
            int choice;
            std::cin >> choice;
            
            switch (choice) {
                case 1:
                    display_top_consistent_players(analyzer);
                    break;
                    
                case 2:
                    analyze_player_performance(analyzer);
                    break;
                    
                case 3:
                    compare_two_players(analyzer);
                    break;
                    
                case 4:
                    analyze_team_performance(analyzer);
                    break;
                    
                case 5:
                    analyze_position_statistics(analyzer);
                    break;
                    
                case 6:
                    std::cout << "\n📝 Generating comprehensive report...\n";
                    reporter.generate_consistency_report("fanblitz_report.txt");
                    break;
                    
                case 7:
                    std::cout << "\n💾 Exporting data to CSV...\n";
                    reporter.generate_csv_export("fanblitz_rankings.csv");
                    break;
                    
                case 8:
                    std::cout << "\n👋 Thank you for using FanBlitz!\n\n";
                    running = false;
                    break;
                    
                default:
                    std::cout << "\n❌ Invalid choice. Please try again.\n";
            }
            
            if (running && choice >= 1 && choice <= 7) {
                std::cout << "\nPress Enter to continue...";
                std::cin.ignore();
                std::cin.get();
            }
        }
        
    } catch (const std::exception& e) {
        std::cerr << "❌ Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}