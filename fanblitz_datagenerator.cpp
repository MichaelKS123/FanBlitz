/*
 * FanBlitz Data Generator
 * Author: Michael Semera
 * 
 * Generates realistic sample sports data for demonstration
 * Usage: ./data_generator <output_file.csv> <num_players>
 */

#include <iostream>
#include <fstream>
#include <random>
#include <vector>
#include <string>
#include <ctime>

class SportsDataGenerator {
private:
    std::mt19937 rng;
    std::vector<std::string> first_names;
    std::vector<std::string> last_names;
    std::vector<std::string> teams;
    std::vector<std::string> positions;
    std::vector<std::string> seasons;
    
    void initialize_data() {
        first_names = {
            "Lionel", "Cristiano", "Neymar", "Kevin", "Mohamed",
            "Robert", "Kylian", "Erling", "Harry", "Bruno",
            "Luka", "Virgil", "Sadio", "Sergio", "Paul",
            "Marcus", "Antoine", "Raheem", "Son", "Karim",
            "Trent", "Joshua", "Jadon", "Phil", "Jack",
            "Mason", "Romelu", "Luis", "Eden", "N'Golo"
        };
        
        last_names = {
            "Silva", "Santos", "Martinez", "Fernandez", "Rodriguez",
            "Garcia", "Lopez", "Hernandez", "Gonzalez", "Perez",
            "Smith", "Johnson", "Brown", "Wilson", "Davis",
            "Mueller", "Schmidt", "Fischer", "Weber", "Wagner",
            "Rossi", "Russo", "Ferrari", "Bianchi", "Costa"
        };
        
        teams = {
            "Manchester United", "Real Madrid", "Barcelona", "Bayern Munich",
            "Liverpool", "Chelsea", "Paris SG", "Juventus", "Inter Milan",
            "Manchester City", "Arsenal", "Atletico Madrid", "Borussia Dortmund",
            "AC Milan", "Tottenham", "RB Leipzig", "Sevilla", "Roma"
        };
        
        positions = {"Forward", "Midfielder", "Defender", "Goalkeeper"};
        
        seasons = {"2020-21", "2021-22", "2022-23", "2023-24"};
    }
    
    double generate_rating(const std::string& position, int base_skill) {
        std::normal_distribution<double> dist(base_skill, 5.0);
        double rating = dist(rng);
        return std::max(40.0, std::min(99.0, rating));
    }
    
    int generate_goals(const std::string& position, double rating) {
        if (position == "Goalkeeper") return 0;
        
        std::poisson_distribution<int> dist(
            position == "Forward" ? 15 : 
            position == "Midfielder" ? 6 : 2
        );
        
        int goals = dist(rng) + (rating > 80 ? 5 : 0);
        return std::max(0, goals);
    }
    
    int generate_assists(const std::string& position, double rating) {
        if (position == "Goalkeeper") return 0;
        
        std::poisson_distribution<int> dist(
            position == "Forward" ? 8 :
            position == "Midfielder" ? 9 : 3
        );
        
        int assists = dist(rng) + (rating > 80 ? 3 : 0);
        return std::max(0, assists);
    }
    
public:
    SportsDataGenerator() : rng(static_cast<unsigned>(std::time(nullptr))) {
        initialize_data();
    }
    
    void generate_dataset(const std::string& filename, int num_players) {
        std::ofstream file(filename);
        if (!file.is_open()) {
            throw std::runtime_error("Failed to create output file");
        }
        
        std::cout << "🏟️  Generating sports performance dataset...\n";
        
        // Write header
        file << "player_id,player_name,team,position,season,matches_played,"
             << "minutes_played,goals,assists,shots,shots_on_target,passes,"
             << "pass_accuracy,tackles,interceptions,rating\n";
        
        std::uniform_int_distribution<int> team_dist(0, teams.size() - 1);
        std::uniform_int_distribution<int> position_dist(0, positions.size() - 1);
        std::uniform_int_distribution<int> fname_dist(0, first_names.size() - 1);
        std::uniform_int_distribution<int> lname_dist(0, last_names.size() - 1);
        std::uniform_int_distribution<int> base_skill_dist(60, 85);
        std::uniform_int_distribution<int> matches_dist(20, 38);
        std::uniform_int_distribution<int> minutes_dist(60, 90);
        std::uniform_real_distribution<double> pass_acc_dist(70.0, 95.0);
        
        int player_id = 1;
        
        for (int p = 0; p < num_players; ++p) {
            std::string player_name = first_names[fname_dist(rng)] + " " + 
                                     last_names[lname_dist(rng)];
            std::string team = teams[team_dist(rng)];
            std::string position = positions[position_dist(rng)];
            int base_skill = base_skill_dist(rng);
            
            // Generate data for each season
            for (const auto& season : seasons) {
                int matches = matches_dist(rng);
                int minutes = matches * minutes_dist(rng);
                double rating = generate_rating(position, base_skill);
                int goals = generate_goals(position, rating);
                int assists = generate_assists(position, rating);
                
                // Calculate dependent stats
                int shots = goals > 0 ? goals * 5 + (std::rand() % 30) : std::rand() % 20;
                int shots_on_target = static_cast<int>(shots * 0.4);
                
                int passes;
                if (position == "Goalkeeper") {
                    passes = 400 + std::rand() % 600;
                } else if (position == "Midfielder") {
                    passes = 1000 + std::rand() % 1500;
                } else if (position == "Defender") {
                    passes = 800 + std::rand() % 1200;
                } else {
                    passes = 500 + std::rand() % 700;
                }
                
                double pass_accuracy = pass_acc_dist(rng);
                
                int tackles, interceptions;
                if (position == "Defender") {
                    tackles = 50 + std::rand() % 50;
                    interceptions = 30 + std::rand() % 40;
                } else if (position == "Midfielder") {
                    tackles = 30 + std::rand() % 40;
                    interceptions = 20 + std::rand() % 30;
                } else if (position == "Forward") {
                    tackles = 10 + std::rand() % 20;
                    interceptions = 5 + std::rand() % 15;
                } else {
                    tackles = 0;
                    interceptions = 0;
                }
                
                // Write to CSV
                file << "P" << std::setfill('0') << std::setw(4) << player_id << ","
                     << player_name << ","
                     << team << ","
                     << position << ","
                     << season << ","
                     << matches << ","
                     << minutes << ","
                     << goals << ","
                     << assists << ","
                     << shots << ","
                     << shots_on_target << ","
                     << passes << ","
                     << std::fixed << std::setprecision(2) << pass_accuracy << ","
                     << tackles << ","
                     << interceptions << ","
                     << std::fixed << std::setprecision(1) << rating << "\n";
            }
            
            player_id++;
            
            // Progress indicator
            if ((p + 1) % 10 == 0) {
                std::cout << "  Generated " << (p + 1) << " players...\n";
            }
        }
        
        file.close();
        
        std::cout << "\n✓ Dataset generated successfully!\n";
        std::cout << "  • File: " << filename << "\n";
        std::cout << "  • Players: " << num_players << "\n";
        std::cout << "  • Total records: " << (num_players * seasons.size()) << "\n";
        std::cout << "  • Seasons: " << seasons.size() << "\n";
    }
};

int main(int argc, char* argv[]) {
    std::cout << "\n";
    std::cout << "╔══════════════════════════════════════════════════════════════╗\n";
    std::cout << "║            FanBlitz Sports Data Generator                    ║\n";
    std::cout << "║               Author: Michael Semera                         ║\n";
    std::cout << "╚══════════════════════════════════════════════════════════════╝\n";
    std::cout << "\n";
    
    if (argc < 3) {
        std::cout << "Usage: " << argv[0] << " <output_file.csv> <num_players>\n";
        std::cout << "\nExample: " << argv[0] << " player_stats.csv 50\n";
        std::cout << "This will generate data for 50 players across 4 seasons.\n\n";
        return 1;
    }
    
    std::string filename = argv[1];
    int num_players = std::atoi(argv[2]);
    
    if (num_players < 1 || num_players > 1000) {
        std::cerr << "❌ Error: Number of players must be between 1 and 1000\n";
        return 1;
    }
    
    try {
        SportsDataGenerator generator;
        generator.generate_dataset(filename, num_players);
    } catch (const std::exception& e) {
        std::cerr << "❌ Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}