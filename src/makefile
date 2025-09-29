# Generic C++ Makefile
# Place this Makefile in the src/ directory
# Automatically finds all .cpp files in current directory and compiles them

# Compiler settings
CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++11 -g
LDFLAGS = 

# Target executable
TARGET = test

# Automatically find all .cpp files in current directory
ALL_SOURCES = $(wildcard *.cpp)

# Exclude DemoMain.cpp from regular build
SOURCES = $(filter-out DemoMain.cpp, $(ALL_SOURCES))

# Generate object file names from source files
OBJECTS = $(SOURCES:.cpp=.o)

# Demo-specific files
DEMO_SOURCES = $(filter-out TestingMain.cpp, $(ALL_SOURCES))
DEMO_OBJECTS = $(DEMO_SOURCES:.cpp=.o)
DEMO_TARGET = demo

# Default target
all: $(TARGET)

# Link object files to create executable
$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

# Compile .cpp files to .o files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Run the program
run: $(TARGET)
	@./$(TARGET)

# Build demo executable
$(DEMO_TARGET): $(DEMO_OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

# Run demo
run-demo: $(DEMO_TARGET)
	@./$(DEMO_TARGET)

# Clean build artifacts
clean:
	rm -f *.o $(TARGET) $(DEMO_TARGET)

# Clean and rebuild
rebuild: clean all

# Show variables (for debugging makefile)
debug:
	@echo "All Sources: $(ALL_SOURCES)"
	@echo "Test Sources: $(SOURCES)"
	@echo "Test Objects: $(OBJECTS)"
	@echo "Test Target: $(TARGET)"
	@echo ""
	@echo "Demo Sources: $(DEMO_SOURCES)"
	@echo "Demo Objects: $(DEMO_OBJECTS)"
	@echo "Demo Target: $(DEMO_TARGET)"

# Phony targets (not actual files)
.PHONY: all clean run run-demo rebuild debug