######################################################
######### GENERAL PURPOSE CPP MAKEFILE ###############
######################################################

# Compiler and flags
CXX := g++
CXXFLAGS := -std=c++11 -Wall

# Directories
SRC_DIR := src
INC_DIR := include
BUILD_DIR := build
BIN_DIR := bin
TEST_DIR := test  # Directory for test files (create if it doesn't exist)

# Source files
SRCS := $(wildcard $(SRC_DIR)/*.cpp)
OBJS := $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(SRCS))
DEPS := $(wildcard $(INC_DIR)/*.h)

# Test source files
TEST_SRCS := $(wildcard $(SRC_DIR)/test_*.cpp) # Assuming test files start with test_
TEST_OBJS := $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/test_%.o, $(TEST_SRCS))

# Executable name
TARGET := $(BIN_DIR)/exec

# Main source file
MAIN_SRC := main.cpp
MAIN_OBJ := $(BUILD_DIR)/main.o

# Test executable name
TEST_TARGET := $(BIN_DIR)/test_graph

# Build rule for the main executable
$(TARGET): $(OBJS) $(MAIN_OBJ)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Object files rule
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp $(DEPS)
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -I$(INC_DIR) -c -o $@ $<

# Main object file rule
$(MAIN_OBJ): $(MAIN_SRC) $(DEPS)
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -I$(INC_DIR) -c -o $@ $<

# Build rule for test executable
$(TEST_TARGET): $(TEST_OBJS) $(OBJS)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $^ acutest.cpp # Include Acutest source file

# Test object files rule
$(BUILD_DIR)/test_%.o: $(SRC_DIR)/test_%.cpp $(DEPS)
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -I$(INC_DIR) -c -o $@ $<

# Clean rule
clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)

# PHONY targets
.PHONY: all clean test

# Default target
all: $(TARGET)

# Target to run tests
test: $(TEST_TARGET)
	@echo "Running tests..."
	@./$(TEST_TARGET)

