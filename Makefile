# Define the compiler and its flags during compilation
CC = g++
FLAGS = -g -Wall -std=c++11 -I $(INC_DIR) -O3

# Setup constants for code directories
INC_DIR = include
SRC_DIR = src
OBJ_DIR = build
EXE_DIR = bin

# Locate all the .cpp files in the src directory and flatten their object paths
SRC_FILES := $(shell find $(SRC_DIR) -name '*.cpp')
OBJS := $(patsubst %.cpp, $(OBJ_DIR)/%.o, $(notdir $(SRC_FILES)))

TARGET = $(EXE_DIR)/main2
TARGET_MAIN = $(EXE_DIR)/main

all: $(TARGET) $(TARGET_MAIN)

$(TARGET): $(OBJS) $(OBJ_DIR)/main2.o
	$(CC) $(FLAGS) $(OBJS) $(OBJ_DIR)/main2.o -o $(TARGET)

$(TARGET_MAIN): $(OBJS) $(OBJ_DIR)/main.o
	$(CC) $(FLAGS) $(OBJS) $(OBJ_DIR)/main.o -o $(TARGET_MAIN)

$(OBJ_DIR)/main2.o: main2.cpp
	$(CC) $(FLAGS) -c $< -o $@

$(OBJ_DIR)/main.o: main.cpp
	$(CC) $(FLAGS) -c $< -o $@

$(OBJS):
	$(MAKE) -C $(SRC_DIR)

clean:
	rm -f $(OBJ_DIR)/*.o
	rm -f $(EXE_DIR)/*
