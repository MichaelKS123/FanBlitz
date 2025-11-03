# FanBlitz Makefile
# Author: Michael Semera

CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O3 -pedantic
INCLUDES = -I.

# Target executables
MAIN = fanblitz
DATA_GEN = data_generator

# Source files
MAIN_SRC = main.cpp
DATA_GEN_SRC = data_generator.cpp

# Header files
HEADERS = fanblitz.h

# Default target
all: $(MAIN) $(DATA_GEN)

# Build main application
$(MAIN): $(MAIN_SRC) $(HEADERS)
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(MAIN_SRC) -o $(MAIN)
	@echo "✓ FanBlitz application built successfully"

# Build data generator
$(DATA_GEN): $(DATA_GEN_SRC)
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(DATA_GEN_SRC) -o $(DATA_GEN)
	@echo "✓ Data generator built successfully"

# Generate sample data
generate-data: $(DATA_GEN)
	@echo "Generating sample dataset..."
	@./$(DATA_GEN) player_stats.csv 50
	@echo "✓ Sample data created: player_stats.csv"

# Run the application
run: $(MAIN) generate-data
	@echo "Starting FanBlitz..."
	@./$(MAIN) player_stats.csv

# Clean build artifacts
clean:
	rm -f $(MAIN) $(DATA_GEN)
	rm -f *.o
	@echo "✓ Build artifacts cleaned"

# Deep clean (including generated data)
distclean: clean
	rm -f player_stats.csv
	rm -f fanblitz_report.txt
	rm -f fanblitz_rankings.csv
	@echo "✓ All generated files removed"

# Create documentation
docs:
	@echo "Creating documentation..."
	@doxygen Doxyfile 2>/dev/null || echo "Doxygen not installed"

# Install to system
install: all
	@echo "Installing FanBlitz..."
	@sudo cp $(MAIN) /usr/local/bin/
	@sudo cp $(DATA_GEN) /usr/local/bin/
	@echo "✓ Installed to /usr/local/bin/"

# Uninstall from system
uninstall:
	@sudo rm -f /usr/local/bin/$(MAIN)
	@sudo rm -f /usr/local/bin/$(DATA_GEN)
	@echo "✓ Uninstalled from /usr/local/bin/"

# Run tests
test: $(MAIN) $(DATA_GEN)
	@echo "Running tests..."
	@./$(DATA_GEN) test_data.csv 20
	@./$(MAIN) test_data.csv << EOF
6
7
8
EOF
	@echo "✓ Tests completed"
	@rm -f test_data.csv

# Development build with debug symbols
debug: CXXFLAGS += -g -DDEBUG
debug: clean all
	@echo "✓ Debug build completed"

# Quick start (build everything and run)
quickstart: all generate-data
	@echo ""
	@echo "╔══════════════════════════════════════════════════════════════╗"
	@echo "║                  FanBlitz is ready!                          ║"
	@echo "╚══════════════════════════════════════════════════════════════╝"
	@echo ""
	@echo "Run the application: ./$(MAIN) player_stats.csv"
	@echo "Generate more data:  ./$(DATA_GEN) custom_data.csv 100"
	@echo ""

# Help target
help:
	@echo "FanBlitz Build System"
	@echo "Author: Michael Semera"
	@echo ""
	@echo "Available targets:"
	@echo "  make              - Build all components"
	@echo "  make fanblitz     - Build main application only"
	@echo "  make data_generator - Build data generator only"
	@echo "  make generate-data  - Generate sample dataset"
	@echo "  make run          - Build and run with sample data"
	@echo "  make quickstart   - Build everything and setup"
	@echo "  make test         - Run automated tests"
	@echo "  make debug        - Build with debug symbols"
	@echo "  make clean        - Remove build artifacts"
	@echo "  make distclean    - Full cleanup including data"
	@echo "  make install      - Install to system"
	@echo "  make uninstall    - Remove from system"
	@echo "  make help         - Show this help message"

.PHONY: all clean distclean run test debug quickstart help install uninstall generate-data docs