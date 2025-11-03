"""
FanBlitz - Power BI Data Preparation Script
Author: Michael Semera

This script prepares FanBlitz data for Power BI visualization
Creates additional calculated columns and summary tables
"""

import pandas as pd
import numpy as np
from datetime import datetime


class PowerBIPrep:
    """Prepare FanBlitz data for Power BI dashboards."""
    
    def __init__(self, csv_file):
        """Initialize with CSV file path."""
        print("📊 FanBlitz Power BI Preparation Tool")
        print("=" * 60)
        self.df = pd.read_csv(csv_file)
        print(f"✓ Loaded {len(self.df)} records from {csv_file}")
        
    def add_calculated_columns(self):
        """Add calculated columns for Power BI."""
        print("\n🔧 Adding calculated columns...")
        
        # Performance grade
        self.df['performance_grade'] = pd.cut(
            self.df['rating'],
            bins=[0, 60, 70, 80, 90, 100],
            labels=['Poor', 'Average', 'Good', 'Excellent', 'World Class']
        )
        
        # Goals + Assists
        self.df['goal_contributions'] = self.df['goals'] + self.df['assists']
        
        # Minutes per goal involvement
        self.df['minutes_per_contribution'] = np.where(
            self.df['goal_contributions'] > 0,
            self.df['minutes_played'] / self.df['goal_contributions'],
            np.inf
        )
        
        # Shot conversion rate
        self.df['shot_conversion'] = np.where(
            self.df['shots'] > 0,
            (self.df['goals'] / self.df['shots'] * 100),
            0
        )
        
        # Efficiency score (composite metric)
        self.df['efficiency_score'] = (
            (self.df['goals'] * 5) +
            (self.df['assists'] * 3) +
            (self.df['pass_accuracy'] * 0.5) +
            (self.df['tackles'] * 0.3)
        ) / self.df['matches_played'].clip(lower=1)
        
        print("✓ Calculated columns added")
        
    def create_consistency_table(self):
        """Create player consistency summary table."""
        print("\n📈 Creating consistency table...")
        
        # Group by player
        player_summary = self.df.groupby('player_name').agg({
            'rating': ['mean', 'std', 'min', 'max', 'count'],
            'goals': 'sum',
            'assists': 'sum',
            'matches_played': 'sum',
            'position': 'first',
            'team': lambda x: x.mode()[0] if not x.empty else ''
        }).reset_index()
        
        # Flatten column names
        player_summary.columns = [
            'player_name', 'avg_rating', 'std_rating', 'min_rating',
            'max_rating', 'seasons', 'total_goals', 'total_assists',
            'total_matches', 'position', 'primary_team'
        ]
        
        # Calculate consistency metrics
        player_summary['cv_percentage'] = (
            player_summary['std_rating'] / player_summary['avg_rating'] * 100
        )
        player_summary['consistency_score'] = 100 - player_summary['cv_percentage'].clip(upper=100)
        
        # Combined score
        player_summary['combined_score'] = (
            player_summary['avg_rating'] * 0.6 +
            player_summary['consistency_score'] * 0.4
        )
        
        # Rank
        player_summary['rank'] = player_summary['combined_score'].rank(
            ascending=False, method='min'
        ).astype(int)
        
        # Sort by rank
        player_summary = player_summary.sort_values('rank')
        
        return player_summary
    
    def create_season_comparison_table(self):
        """Create season-over-season comparison."""
        print("\n📅 Creating season comparison table...")
        
        # Pivot by season
        season_pivot = self.df.pivot_table(
            index='player_name',
            columns='season',
            values=['rating', 'goals', 'assists'],
            aggfunc='first'
        )
        
        # Flatten and clean
        season_pivot.columns = ['_'.join(col).strip() for col in season_pivot.columns]
        season_pivot = season_pivot.reset_index()
        
        return season_pivot
    
    def create_team_summary(self):
        """Create team performance summary."""
        print("\n🏆 Creating team summary...")
        
        team_summary = self.df.groupby(['team', 'season']).agg({
            'rating': 'mean',
            'goals': 'sum',
            'assists': 'sum',
            'matches_played': 'sum',
            'player_name': 'count'
        }).reset_index()
        
        team_summary.columns = [
            'team', 'season', 'avg_rating', 'total_goals',
            'total_assists', 'total_matches', 'squad_size'
        ]
        
        return team_summary
    
    def create_position_analysis(self):
        """Create position-based analysis."""
        print("\n⚽ Creating position analysis...")
        
        position_summary = self.df.groupby(['position', 'season']).agg({
            'rating': 'mean',
            'goals': 'mean',
            'assists': 'mean',
            'passes': 'mean',
            'tackles': 'mean',
            'player_name': 'count'
        }).reset_index()
        
        position_summary.columns = [
            'position', 'season', 'avg_rating', 'avg_goals',
            'avg_assists', 'avg_passes', 'avg_tackles', 'player_count'
        ]
        
        return position_summary
    
    def export_for_powerbi(self, output_prefix='fanblitz_powerbi'):
        """Export all tables for Power BI."""
        print("\n💾 Exporting tables for Power BI...")
        
        # Add calculated columns
        self.add_calculated_columns()
        
        # Export main dataset
        main_file = f'{output_prefix}_main.csv'
        self.df.to_csv(main_file, index=False)
        print(f"  ✓ Main dataset: {main_file}")
        
        # Export consistency table
        consistency = self.create_consistency_table()
        consistency_file = f'{output_prefix}_consistency.csv'
        consistency.to_csv(consistency_file, index=False)
        print(f"  ✓ Consistency table: {consistency_file}")
        
        # Export season comparison
        season_comp = self.create_season_comparison_table()
        season_file = f'{output_prefix}_season_comparison.csv'
        season_comp.to_csv(season_file, index=False)
        print(f"  ✓ Season comparison: {season_file}")
        
        # Export team summary
        team_summary = self.create_team_summary()
        team_file = f'{output_prefix}_team_summary.csv'
        team_summary.to_csv(team_file, index=False)
        print(f"  ✓ Team summary: {team_file}")
        
        # Export position analysis
        position_analysis = self.create_position_analysis()
        position_file = f'{output_prefix}_position_analysis.csv'
        position_analysis.to_csv(position_file, index=False)
        print(f"  ✓ Position analysis: {position_file}")
        
        print("\n" + "=" * 60)
        print("✅ All tables exported successfully!")
        print("\nPower BI Import Instructions:")
        print("1. Open Power BI Desktop")
        print("2. Click 'Get Data' > 'Text/CSV'")
        print(f"3. Import these files:")
        print(f"   - {main_file}")
        print(f"   - {consistency_file}")
        print(f"   - {team_file}")
        print(f"   - {position_file}")
        print("4. Create relationships if needed")
        print("5. Build your dashboard!")
        
        return {
            'main': main_file,
            'consistency': consistency_file,
            'season': season_file,
            'team': team_file,
            'position': position_file
        }
    
    def generate_dashboard_template(self):
        """Generate DAX measures for Power BI."""
        print("\n📋 Generating Power BI DAX measures...")
        
        dax_measures = """
-- FanBlitz Power BI DAX Measures
-- Author: Michael Semera

-- Average Performance Rating
Avg Rating = AVERAGE('Main'[rating])

-- Total Goals
Total Goals = SUM('Main'[goals])

-- Total Assists
Total Assists = SUM('Main'[assists])

-- Goal Contributions
Goal Contributions = [Total Goals] + [Total Assists]

-- Top Performer
Top Performer = 
CALCULATE(
    MAX('Consistency'[player_name]),
    TOPN(1, 'Consistency', 'Consistency'[combined_score], DESC)
)

-- Most Consistent Player
Most Consistent = 
CALCULATE(
    MAX('Consistency'[player_name]),
    TOPN(1, 'Consistency', 'Consistency'[consistency_score], DESC)
)

-- Season Over Season Growth
Season Growth = 
VAR CurrentRating = [Avg Rating]
VAR PreviousSeason = 
    CALCULATE(
        [Avg Rating],
        DATEADD('Main'[season], -1, YEAR)
    )
RETURN
    DIVIDE(CurrentRating - PreviousSeason, PreviousSeason, 0) * 100

-- Performance Trend
Performance Trend = 
VAR Trend = LINESTX('Main', 'Main'[rating], 'Main'[season])
RETURN
    IF(Trend > 0, "↗️ Improving", IF(Trend < 0, "↘️ Declining", "→ Stable"))

-- Top 10 Rank
Top 10 Rank = 
RANKX(
    ALL('Consistency'),
    'Consistency'[combined_score],
    ,
    DESC,
    Dense
)

-- Position Best
Position Best = 
CALCULATE(
    MAX('Main'[player_name]),
    TOPN(1, 'Main', 'Main'[rating], DESC)
)

-- Team Performance Score
Team Score = 
SUMX(
    'Team Summary',
    'Team Summary'[avg_rating] * 'Team Summary'[total_goals] / 100
)

-- Consistency Badge
Consistency Badge = 
SWITCH(
    TRUE(),
    'Consistency'[consistency_score] >= 90, "🏆 Elite",
    'Consistency'[consistency_score] >= 80, "⭐ Great",
    'Consistency'[consistency_score] >= 70, "✓ Good",
    "○ Average"
)

-- Performance Grade Color
Grade Color = 
SWITCH(
    'Main'[performance_grade],
    "World Class", "#FFD700",
    "Excellent", "#32CD32",
    "Good", "#1E90FF",
    "Average", "#FFA500",
    "Poor", "#DC143C",
    "#808080"
)
"""
        
        dax_file = 'fanblitz_dax_measures.txt'
        with open(dax_file, 'w') as f:
            f.write(dax_measures)
        
        print(f"✓ DAX measures saved to: {dax_file}")
        print("\nCopy these measures into Power BI:")
        print("1. Go to 'Modeling' tab")
        print("2. Click 'New Measure'")
        print("3. Paste each measure")


def main():
    """Main execution."""
    import sys
    
    if len(sys.argv) < 2:
        print("Usage: python powerbi_prep.py <player_stats.csv>")
        print("\nExample: python powerbi_prep.py player_stats.csv")
        return
    
    csv_file = sys.argv[1]
    
    try:
        # Initialize
        prep = PowerBIPrep(csv_file)
        
        # Export all tables
        files = prep.export_for_powerbi()
        
        # Generate DAX measures
        prep.generate_dashboard_template()
        
        print("\n🎉 Power BI preparation complete!")
        print("\nNext steps:")
        print("1. Import CSV files into Power BI")
        print("2. Add DAX measures from fanblitz_dax_measures.txt")
        print("3. Create visualizations:")
        print("   - Bar chart: Top 10 Consistent Players")
        print("   - Line chart: Player Performance Over Seasons")
        print("   - Table: Player Rankings")
        print("   - Cards: Key Metrics")
        print("4. Apply filters and slicers")
        print("5. Publish your dashboard!")
        
    except Exception as e:
        print(f"\n❌ Error: {str(e)}")
        import traceback
        traceback.print_exc()


if __name__ == "__main__":
    main()
