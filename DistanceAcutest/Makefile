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
