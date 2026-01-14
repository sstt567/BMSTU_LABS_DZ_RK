CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -I./include
LDFLAGS = -lpqxx -lpq

SRCDIR = src
INCDIR = include
SQLDIR = sql
BINDIR = .

SOURCES = $(SRCDIR)/main_full.cpp
HEADERS = $(INCDIR)/DatabaseConnection.h $(SRCDIR)/Models_full.h $(SRCDIR)/User_full.h
OBJECTS = $(SOURCES:.cpp=.o)
TARGET = $(BINDIR)/shop

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJECTS) $(LDFLAGS)
	@echo "Build complete: $(TARGET)"

%.o: %.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

setup_db:
	@echo "Setting up database..."
	createdb ekz_task 2>/dev/null || echo "Database already exists"
	psql -U postgres ekz_task < $(SQLDIR)/tables.sql
	psql -U postgres ekz_task < $(SQLDIR)/functions.sql
	psql -U postgres ekz_task < $(SQLDIR)/triggers.sql
	psql -U postgres ekz_task < $(SQLDIR)/sampledata.sql
	@echo "Database setup complete!"

run: $(TARGET)
	@echo "Running application..."
	@cd $(BINDIR) && ./shop

test: $(TARGET)
	@echo "Testing application..."
	@cd $(BINDIR) && ./shop > /dev/null && echo "✅ Test passed"

clean:
	@echo "Cleaning build files..."
	rm -f $(OBJECTS) $(TARGET) *.csv
	rm -rf build/
	@echo "Clean complete!"

rebuild: clean all

help:
	@echo "Available targets:"
	@echo "  make              - Build the project"
	@echo "  make setup_db     - Create and setup database"
	@echo "  make run          - Run the application"
	@echo "  make test         - Test the application"
	@echo "  make clean        - Clean build files"
	@echo "  make rebuild      - Clean and build"
	@echo "  make help         - Show this help message"

.PHONY: all setup_db run test clean rebuild help
