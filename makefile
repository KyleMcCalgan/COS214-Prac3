# Makefile for COS214 Practical 3 - PetSpace Chat System
CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++11 -g
TEST_TARGET = test
DEMO_TARGET = demo

# Source directory
SRCDIR = src

# Automatically find all .cpp files in src directory
SOURCES = $(wildcard $(SRCDIR)/*.cpp)

# Object files for all sources (replace .cpp with .o)
ALL_OBJECTS = $(SOURCES:.cpp=.o)

# Test objects (exclude demoMain.o)
TEST_OBJECTS = $(filter-out $(SRCDIR)/DemoMain.o, $(ALL_OBJECTS))

# Demo objects (exclude TestingMain.o)
DEMO_OBJECTS = $(filter-out $(SRCDIR)/TestingMain.o, $(ALL_OBJECTS))

# Default target
all: $(TEST_TARGET)

# Build test executable
$(TEST_TARGET): $(TEST_OBJECTS)
	$(CXX) $(CXXFLAGS) -o $(TEST_TARGET) $(TEST_OBJECTS)

# Build demo executable
$(DEMO_TARGET): $(DEMO_OBJECTS)
	$(CXX) $(CXXFLAGS) -o $(DEMO_TARGET) $(DEMO_OBJECTS)

# Pattern rule for object files
$(SRCDIR)/%.o: $(SRCDIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Run test target
run: $(TEST_TARGET)
	./$(TEST_TARGET)

# Run demo target
run-demo: $(DEMO_TARGET)
	./$(DEMO_TARGET)

# Run valgrind on test target
val: $(TEST_TARGET)
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./$(TEST_TARGET)

# Build both executables
both: $(TEST_TARGET) $(DEMO_TARGET)

# Clean target
clean:
	rm -f $(SRCDIR)/*.o $(TEST_TARGET) $(DEMO_TARGET)

# Coverage target
coverage: $(TEST_TARGET)
	./$(TEST_TARGET)

# Documentation target
docs: Doxyfile
	doxygen Doxyfile

# Clean docs
clean-docs:
	rm -rf html/ latex/

# Debug target to show what files will be compiled
debug:
	@echo "All source files found:"
	@echo $(SOURCES)
	@echo ""
	@echo "Test objects (excludes demoMain.o):"
	@echo $(TEST_OBJECTS)
	@echo ""
	@echo "Demo objects (excludes TestingMain.o):"
	@echo $(DEMO_OBJECTS)

# Add all targets to .PHONY
.PHONY: all run run-demo val both clean coverage docs clean-docs debug