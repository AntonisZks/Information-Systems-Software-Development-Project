# Makefile for compiling the project

# Compiler and flags
CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++11

# Source files
SRCS = readVectors.cpp readMain.cpp
OBJS = $(SRCS:.cpp=.o)

# Executable name
TARGET = myprogram

# Test files
TESTS = test_distance.cpp
TEST_OBJS = $(TESTS:.cpp=.o)

# Testing target
test_distance: $(TEST_OBJS) $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Main target
all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Pattern rule for building .o files from .cpp files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean target
clean:
	rm -f $(OBJS) $(TARGET) $(TEST_OBJS) test_distance

.PHONY: all clean

# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++11 -I.  # Include current directory for headers

# Source files
SOURCES = readVectors.cpp test_distance.cpp DataVectorNew.cpp
OBJECTS = $(SOURCES:.cpp=.o)
TARGET = test_distance

# Default target
all: $(TARGET)

# Linking
$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Rule for compiling source files into object files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean target
clean:
	rm -f $(OBJECTS) $(TARGET)
