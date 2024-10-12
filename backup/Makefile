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

# Source files
SRCS := $(wildcard $(SRC_DIR)/*.cpp)
OBJS := $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(SRCS))
DEPS := $(wildcard $(INC_DIR)/*.h)

# Executable name
TARGET := $(BIN_DIR)/secretary

# Main source file
MAIN_SRC := main.cpp
MAIN_OBJ := $(BUILD_DIR)/main.o

# Build rule
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

# Clean rule
clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)

# PHONY targets
.PHONY: all clean

# Default target
all: $(TARGET)