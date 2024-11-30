# Define the compiler and its flags during compilation
CC = g++
FLAGS = -g -Wall -std=c++11 -O3


# Setup constants for code directories
INC_DIR = ../../include
OBJ_DIR = ../../build


# Define the targets for the executables
all: $(OBJ_DIR)/GreedySearch.o $(OBJ_DIR)/RobustPrune.o $(OBJ_DIR)/VamanaIndex.o $(OBJ_DIR)/recall.o $(OBJ_DIR)/FilteredVamanaIndex.cpp


# Compile the source files in the current directory
$(OBJ_DIR)/GreedySearch.o: Algorithms/GreedySearch.cpp
	$(CC) $(FLAGS) -o $(OBJ_DIR)/GreedySearch.o -c Algorithms/GreedySearch.cpp -I$(INC_DIR)

$(OBJ_DIR)/RobustPrune.o: Algorithms/RobustPrune.cpp
	$(CC) $(FLAGS) -o $(OBJ_DIR)/RobustPrune.o -c Algorithms/RobustPrune.cpp -I$(INC_DIR)

$(OBJ_DIR)/VamanaIndex.o: Algorithms/VamanaIndex.cpp
	$(CC) $(FLAGS) -o $(OBJ_DIR)/VamanaIndex.o -c Algorithms/VamanaIndex.cpp -I$(INC_DIR)

$(OBJ_DIR)/recall.o: Evaluation/recall.cpp
	$(CC) $(FLAGS) -o $(OBJ_DIR)/recall.o -c Evaluation/recall.cpp -I$(INC_DIR)

$(OBJ_DIR)/FilteredVamanaIndex.cpp: Algorithms/FilteredVamanaIndex.cpp
	$(CC) $(FLAGS) -o $(OBJ_DIR)/FilteredVamanaIndex.o -c Algorithms/FilteredVamanaIndex.cpp -I$(INC_DIR)
