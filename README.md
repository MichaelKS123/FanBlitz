# FanBlitz ⚡

**Advanced Sports Performance Analysis Platform**

*Author: Michael Semera*

---

## 🎯 Project Overview

FanBlitz is a high-performance sports analytics platform built in C++ that provides comprehensive player and team performance analysis across multiple seasons. The system leverages advanced statistical methods, time series analysis, and consistency metrics to deliver actionable insights for coaches, analysts, and sports enthusiasts.

### Why FanBlitz?

Traditional sports analytics tools often lack the depth needed for true performance insights. FanBlitz offers:
- **Consistency Analysis**: Identify players with reliable performance across seasons
- **Time Series Tracking**: Monitor performance trends and trajectory
- **Correlation Analysis**: Compare players and find performance patterns
- **Team Analytics**: Aggregate statistics for team-level insights
- **Statistical Rigor**: Coefficient of variation, standard deviation, percentile analysis
- **Fast Processing**: C++ implementation for handling large datasets efficiently

---

## ✨ Key Features

### 📊 Performance Analytics
- **Top 10 Rankings**: Identify most consistent performers
- **Player Analysis**: Deep dive into individual statistics
- **Player Comparison**: Head-to-head statistical matchups
- **Team Performance**: Aggregate team-level metrics
- **Position Statistics**: Role-based performance insights

### 📈 Statistical Methods
- **Consistency Scoring**: Coefficient of variation analysis
- **Time Series Analysis**: Track performance over seasons
- **Correlation Calculation**: Pearson correlation between players
- **Rolling Averages**: Smooth short-term fluctuations
- **Percentile Rankings**: Position players in overall distribution

### 📝 Reporting
- **Comprehensive Reports**: Auto-generated text analysis
- **CSV Export**: Data export for Power BI integration
- **Interactive Dashboard**: Menu-driven interface
- **Custom Queries**: Flexible analysis options

---

## 🛠️ Technologies & Skills

### Core Technologies
- **C++17**: Modern C++ with STL
- **Object-Oriented Design**: Clean class architecture
- **Statistical Libraries**: Custom statistical implementations
- **CSV Processing**: Robust data parsing
- **File I/O**: Efficient data handling

### Skills Demonstrated
1. **Advanced C++ Programming**
   - Template usage
   - Smart pointers
   - STL containers and algorithms
   - Exception handling

2. **Statistical Analysis**
   - Descriptive statistics (mean, median, std dev)
   - Correlation analysis
   - Time series methods
   - Consistency metrics

3. **Data Structures**
   - Vectors for sequential data
   - Maps for indexed lookups
   - Custom structs for type safety
   - Efficient data organization

4. **Software Architecture**
   - Separation of concerns
   - RAII (Resource Acquisition Is Initialization)
   - Modularity and reusability
   - Clean code principles

5. **Performance Optimization**
   - O(n log n) algorithms for sorting
   - Efficient memory management
   - Fast I/O operations
   - Minimal overhead design

---

## 📦 Installation

### Prerequisites

#### Linux (Ubuntu/Debian)
```bash
sudo apt-get update
sudo apt-get install build-essential g++ make
```

#### macOS
```bash
# Install Xcode Command Line Tools
xcode-select --install

# Or use Homebrew
brew install gcc make
```

#### Windows
- Install MinGW-w64 or Visual Studio with C++ support
- Or use WSL (Windows Subsystem for Linux)

### Build from Source

```bash
# Clone or download the repository
git clone <repository-url>
cd fanblitz

# Quick start (builds everything)
make quickstart

# Or build step by step
make all              # Build all components
make generate-data    # Create sample dataset
```

---

## 🚀 Quick Start Guide

### Step 1: Build the Application

```bash
make quickstart
```

This command:
1. Compiles the main application
2. Builds the data generator
3. Creates a sample dataset (`player_stats.csv`)

### Step 2: Run FanBlitz

```bash
./fanblitz player_stats.csv
```

You'll see the main menu:
```
╔══════════════════════════════════════════════════════════════╗
║                      ANALYSIS MENU                           ║
╠══════════════════════════════════════════════════════════════╣
║  1. Show Top 10 Most Consistent Players                      ║
║  2. Analyze Specific Player Performance                      ║
║  3. Compare Two Players                                      ║
║  4. Team Performance Analysis                                ║
║  5. Position-based Statistics                                ║
║  6. Generate Full Report                                     ║
║  7. Export Data to CSV                                       ║
║  8. Exit                                                     ║
╚══════════════════════════════════════════════════════════════╝
```

### Step 3: Explore the Features

Try these options:
- **Option 1**: See the top 10 most consistent performers
- **Option 6**: Generate a comprehensive text report
- **Option 7**: Export rankings for Power BI visualization

---

## 📚 Detailed Usage

### Generating Custom Datasets

```bash
# Generate data for 100 players
./data_generator my_data.csv 100

# This creates:
# - 100 unique players
# - 4 seasons of data per player
# - Realistic performance statistics
```

### Analyzing Your Data

```bash
# Run analysis on your dataset
./fanblitz my_data.csv

# Or use the provided sample data
./fanblitz player_stats.csv
```

### Menu Options Explained

#### 1. Top 10 Most Consistent Players
Shows ranking based on:
- Average rating across seasons
- Consistency score (lower variation = higher score)
- Number of seasons played

Example output:
```
Rank  Player Name              Position        Avg Rating  Consistency  Seasons
─────────────────────────────────────────────────────────────────────────────
1     Cristiano Fernandez      Midfielder      84.25       92.50        4
2     Kevin Silva              Forward         82.80       91.20        4
3     Mohamed Santos           Defender        81.50       90.80        4
```

#### 2. Analyze Specific Player Performance
Detailed breakdown for individual players:
- Career statistics
- Season-by-season performance
- Performance trends (improving/declining/stable)
- Consistency metrics

#### 3. Compare Two Players
Head-to-head comparison:
- Side-by-side statistics
- Correlation analysis
- Performance winner determination

#### 4. Team Performance Analysis
Aggregate team statistics:
- Goals scored per season
- Average team rating
- Multiple season tracking

#### 5. Position-based Statistics
Compare positions:
- Average rating by position
- Goals and assists by role
- Positional strengths

#### 6. Generate Full Report
Creates `fanblitz_report.txt` with:
- Executive summary
- Top 10 rankings
- Methodology explanation
- Key insights

#### 7. Export Data to CSV
Creates `fanblitz_rankings.csv` for:
- Power BI integration
- Excel analysis
- Further processing

---

## 📊 Data Format

### Input CSV Structure

```csv
player_id,player_name,team,position,season,matches_played,minutes_played,goals,assists,shots,shots_on_target,passes,pass_accuracy,tackles,interceptions,rating
P0001,Lionel Martinez,Barcelona,Forward,2020-21,35,3150,28,12,145,58,850,87.5,25,18,88.5
P0001,Lionel Martinez,Barcelona,Forward,2021-22,38,3420,32,15,168,72,920,88.2,28,20,90.2
...
```

### Required Fields
- **player_id**: Unique identifier
- **player_name**: Full name
- **team**: Team name
- **position**: Forward/Midfielder/Defender/Goalkeeper
- **season**: Format YYYY-YY (e.g., 2023-24)
- **matches_played**: Number of matches
- **minutes_played**: Total minutes
- **goals**: Goals scored
- **assists**: Assists provided
- **shots**: Total shots taken
- **shots_on_target**: Accurate shots
- **passes**: Total passes
- **pass_accuracy**: Percentage (0-100)
- **tackles**: Successful tackles
- **interceptions**: Ball interceptions
- **rating**: Overall rating (0-100)

---

## 🏗️ Architecture

### Class Structure

```
PerformanceAnalyzer (Main Engine)
├── Data Loading & Indexing
├── Time Series Analysis
├── Consistency Calculation
├── Ranking Generation
└── Statistical Computations

StatisticalAnalyzer (Math Engine)
├── Mean, Median, Std Dev
├── Correlation Analysis
├── Percentile Calculation
└── Rolling Averages

CSVParser (Data Parser)
├── File Reading
├── Header Parsing
├── Data Validation
└── Error Handling

ReportGenerator (Output Generator)
├── Text Report Creation
├── CSV Export
└── Formatting
```

### Data Flow

```
1. CSV Input
   └── CSVParser::parse()

2. Data Indexing
   ├── By Player Name
   ├── By Team
   └── By Season

3. Analysis
   ├── Time Series Extraction
   ├── Statistical Calculations
   └── Ranking Generation

4. Output
   ├── Console Display
   ├── Text Reports
   └── CSV Export
```

---

## 📈 Statistical Methodology

### Consistency Score Calculation

```cpp
// 1. Calculate Coefficient of Variation (CV)
CV = (standard_deviation / mean) * 100

// 2. Convert to Consistency Score (0-100 scale)
consistency_score = 100 - min(100, CV)

// Higher score = More consistent performance
```

### Combined Performance Score

```cpp
score = (average_rating × 0.6) + (consistency_score × 0.4)

// Weights:
// - 60% performance (average rating)
// - 40% consistency (reliability)
```

### Correlation Analysis

Uses Pearson correlation coefficient:
```
r = Σ((x - x̄)(y - ȳ)) / √(Σ(x - x̄)² × Σ(y - ȳ)²)

Where:
- r > 0.7: Strong positive correlation
- r > 0.4: Moderate correlation
- r < 0.4: Weak correlation
```

---

## 🎓 Learning Outcomes

This project demonstrates proficiency in:

### C++ Expertise
- Modern C++17 features
- STL containers and algorithms
- Memory management (RAII)
- Exception handling
- Template programming

### Data Structures & Algorithms
- Efficient data organization
- Sorting algorithms
- Search optimization
- Hash maps for O(1) lookups

### Statistical Analysis
- Descriptive statistics
- Time series analysis
- Correlation methods
- Percentile calculations

### Software Engineering
- Clean code architecture
- Modular design
- Documentation
- Error handling
- Testing strategies

### Performance Optimization
- Algorithmic complexity
- Memory efficiency
- I/O optimization
- Cache-friendly data structures

---

## 💼 Use Cases

### For Sports Analysts
- **Scouting**: Identify consistent performers for recruitment
- **Performance Tracking**: Monitor player development over time
- **Team Building**: Compare players for tactical decisions
- **Injury Impact**: Analyze performance before/after injuries

### For Coaches
- **Training Focus**: Identify areas needing improvement
- **Squad Selection**: Choose most reliable players
- **Tactical Planning**: Understand position-specific strengths
- **Youth Development**: Track emerging talent progress

### For Fantasy Sports
- **Player Selection**: Choose consistent performers
- **Transfer Decisions**: Identify form trends
- **Captaincy Choices**: Select reliable captain options
- **Value Analysis**: Find undervalued players

### For Media & Content
- **Article Data**: Support stories with statistics
- **Debate Points**: Back arguments with numbers
- **Season Reviews**: Comprehensive performance summaries
- **Predictions**: Use trends for forecasting

---

## 🔧 Configuration & Customization

### Adjusting Weights

Edit `fanblitz.h`:
```cpp
// Current weights
constexpr double CONSISTENCY_WEIGHT = 0.4;
constexpr double AVERAGE_WEIGHT = 0.6;

// Increase consistency importance
constexpr double CONSISTENCY_WEIGHT = 0.5;
constexpr double AVERAGE_WEIGHT = 0.5;
```

### Changing Top N Players

```cpp
constexpr int TOP_N_PLAYERS = 10;  // Change to 20, 50, etc.
```

### Custom Rating Calculation

Modify in `data_generator.cpp`:
```cpp
double generate_rating(const std::string& position, int base_skill) {
    // Add your custom logic
    // Consider factors like: age, form, injuries, etc.
}
```

---

## 📁 Project Structure

```
fanblitz/
│
├── fanblitz.h              # Main header with all classes
├── main.cpp                # Main application entry point
├── data_generator.cpp      # Sample data generator
├── Makefile                # Build system
├── README.md               # This file
│
├── player_stats.csv        # Generated sample data
├── fanblitz_report.txt     # Generated analysis report
└── fanblitz_rankings.csv   # Exported rankings for Power BI
```

---

## 🚧 Troubleshooting

### Issue: Compilation Errors

```bash
# Ensure C++17 support
g++ --version  # Should be 7.0 or higher

# Clean and rebuild
make clean
make all
```

### Issue: Data File Not Found

```bash
# Generate new sample data
make generate-data

# Or create your own
./data_generator player_stats.csv 50
```

### Issue: Invalid CSV Format

Ensure your CSV has:
- Comma-separated values
- Proper header row
- No extra quotes or special characters
- Correct number of columns (16)

### Issue: Memory Issues with Large Datasets

```bash
# Build with optimizations
make clean
CXXFLAGS="-O3 -march=native" make all
```

---

## 🔮 Future Enhancements

### Planned Features
- [ ] GUI interface using Qt or wxWidgets
- [ ] Real-time data import from APIs
- [ ] Machine learning predictions
- [ ] Interactive visualizations
- [ ] Web dashboard
- [ ] Mobile app integration
- [ ] Multi-sport support (NBA, NFL, Cricket)
- [ ] Advanced metrics (xG, RAPTOR, WAR)
- [ ] Video analysis integration
- [ ] Social media sentiment analysis

### Power BI Integration
The exported CSV can be directly imported into Power BI for:
- Interactive dashboards
- Custom visualizations
- Drill-down analysis
- Automated reporting
- Shareable insights

### Example Power BI Measures
```DAX
Consistency Rank = RANKX(ALL(Players), [Consistency Score],, DESC)
Performance Trend = CALCULATE(SUM(Stats[Rating]), LASTDATE(Seasons))
Top Performer = TOPN(1, Players, [Combined Score], DESC)
```

---

## 🎯 Portfolio Highlights

### Key Selling Points
1. ✅ **Production C++ Code**: Clean, maintainable, professional
2. ✅ **Statistical Rigor**: Advanced mathematical methods
3. ✅ **Real-world Application**: Solves actual analytics problems
4. ✅ **Performance Focused**: Optimized algorithms and data structures
5. ✅ **Complete System**: Data generation, analysis, and reporting
6. ✅ **Well Documented**: Extensive README and inline comments
7. ✅ **Build System**: Professional Makefile with multiple targets

### Demonstration Capabilities
- Live analysis demonstration
- Code walkthrough
- Algorithm explanation
- Performance comparison
- Architecture discussion

### Resume Impact
```
FanBlitz - Sports Performance Analysis Platform (C++)
• Developed high-performance analytics engine processing 1000+ player records
• Implemented statistical algorithms (correlation, CV, percentiles) for consistency analysis
• Achieved O(n log n) time complexity for ranking and comparison operations
• Created data pipeline from CSV parsing to Power BI integration
• Generated automated reports with actionable insights
```

---

## 🤝 Contributing

This is a portfolio project by Michael Semera. Suggestions and improvements are welcome!

### Code Style Guidelines
- Follow C++17 best practices
- Use const correctness
- Prefer STL over raw pointers
- Comment complex algorithms
- Keep functions focused

---

## 📄 License

This project is created for educational and portfolio purposes.

---

## 👤 Author

**Michael Semera**

*Software Engineer | Data Analyst | Cyber Security Analyst | Sports Analytics Enthusiast*

For questions, suggestions, or collaboration opportunities, please reach out!
- 💼 LinkedIn: [Michael Semera](https://www.linkedin.com/in/michael-semera-586737295/)
- 🐙 GitHub: [@MichaelKS123](https://github.com/MichaelKS123)
- 📧 Email: michaelsemera15@gmail.com

---

## 🙏 Acknowledgments

- C++ Standard Library for robust containers
- Sports analytics community for methodology inspiration
- Open source community for best practices

---

## 📞 References & Resources

### Documentation
- [C++ Reference](https://en.cppreference.com/)
- [STL Algorithms](https://www.cplusplus.com/reference/algorithm/)
- [Sports Analytics Papers](https://arxiv.org/list/stat.AP/recent)

### Statistical Methods
- Coefficient of Variation: [Wikipedia](https://en.wikipedia.org/wiki/Coefficient_of_variation)
- Pearson Correlation: [Statistics How To](https://www.statisticshowto.com/probability-and-statistics/correlation-coefficient-formula/)
- Time Series Analysis: [Penn State Course](https://online.stat.psu.edu/stat510/)

### Power BI Integration
- [Import CSV to Power BI](https://docs.microsoft.com/en-us/power-bi/connect-data/desktop-connect-csv)
- [DAX Functions](https://docs.microsoft.com/en-us/dax/)
- [Power BI Visuals](https://powerbi.microsoft.com/en-us/data-storytelling/)

---

**Built with ⚡ by Michael Semera**

*Transforming sports data into performance insights through advanced analytics*

---

## 🎉 Quick Command Reference

```bash
# Build everything
make quickstart

# Run application
./fanblitz player_stats.csv

# Generate custom data
./data_generator custom_data.csv 100

# Clean up
make distclean

# Get help
make help
```

**Ready to analyze! 🏆📊**