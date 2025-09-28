# Makefile for COS214 Practical 3 - PetSpace Chat System
CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++11 -g
TEST_TARGET = test

# Source files in src directory
SRCDIR = src
SOURCES = ChatRoom.cpp \
          Command.cpp \
          CtrlCat.cpp \
          Dogorithm.cpp \
          SaveMessageCommand.cpp \
          SendMessageCommand.cpp \
          Users.cpp \
          TestingMain.cpp

# Add src/ prefix to sources
SRC_FILES = $(addprefix $(SRCDIR)/, $(SOURCES))

# Object files (replace src/ and .cpp with .o)
OBJECTS = $(SRC_FILES:.cpp=.o)

# Default target
all: $(TEST_TARGET)

# Build executable
$(TEST_TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $(TEST_TARGET) $(OBJECTS)

# Pattern rule for object files
$(SRCDIR)/%.o: $(SRCDIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Run target
run: $(TEST_TARGET)
	./$(TEST_TARGET)

# Clean target
clean:
	rm -f $(SRCDIR)/*.o $(TEST_TARGET)

# Coverage target
coverage: $(TEST_TARGET)
	./$(TEST_TARGET)


docs: Doxyfile
	doxygen Doxyfile

# Clean docs
clean-docs:
	rm -rf html/ latex/

# Add docs and clean-docs to .PHONY
.PHONY: all run clean coverage docs clean-docs

