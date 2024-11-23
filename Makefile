# Define the compiler and its flags during compilation
CC = g++
FLAGS = -g -Wall -std=c++11 -I $(INC_DIR) -O3

# Setup constants for code directories
INC_DIR = include
SRC_DIR = src
OBJ_DIR = build
EXE_DIR = bin
TST_DIR = tests

TARGET = $(EXE_DIR)/main2
TARGET_MAIN = $(EXE_DIR)/main

OBJS = $(OBJ_DIR)/DataVector.o $(OBJ_DIR)/distance_functions.o $(OBJ_DIR)/graph_node.o $(OBJ_DIR)/Graph.o \
			 $(OBJ_DIR)/RobustPrune.o $(OBJ_DIR)/recall.o $(OBJ_DIR)/read_vectors.o $(OBJ_DIR)/GreedySearch.o $(OBJ_DIR)/graphics.o $(OBJ_DIR)/VamanaIndex.o

OBJS_MAIN = $(OBJ_DIR)/DataVector.o $(OBJ_DIR)/distance_functions.o $(OBJ_DIR)/graph_node.o $(OBJ_DIR)/Graph.o \
			 $(OBJ_DIR)/RobustPrune.o $(OBJ_DIR)/recall.o $(OBJ_DIR)/read_vectors.o $(OBJ_DIR)/GreedySearch.o $(OBJ_DIR)/graphics.o $(OBJ_DIR)/VamanaIndex.o

all: $(TARGET) $(TARGET_MAIN)

$(TARGET): $(OBJS) $(OBJ_DIR)/main2.o
	$(CC) $(FLAGS) -I$(INC_DIR) $(OBJ_DIR)/main2.o $(OBJS) -o $(TARGET)

$(TARGET_MAIN): $(OBJS_MAIN) $(OBJ_DIR)/main.o
	$(CC) $(FLAGS) -I$(INC_DIR) $(OBJ_DIR)/main.o $(OBJS_MAIN) -o $(TARGET_MAIN)

$(OBJ_DIR)/main2.o: main2.cpp
	$(CC) $(FLAGS) -I$(INC_DIR) -c main2.cpp -o $(OBJ_DIR)/main2.o

$(OBJ_DIR)/main.o: main.cpp
	$(CC) $(FLAGS) -I$(INC_DIR) -c main.cpp -o $(OBJ_DIR)/main.o

$(OBJS):
	$(MAKE) -C $(SRC_DIR)

clean:
	rm -f $(OBJ_DIR)/*.o
	rm -f $(EXE_DIR)/*
