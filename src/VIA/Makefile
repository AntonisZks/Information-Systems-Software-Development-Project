# Define the compiler and its flags during compilation
CC = g++
FLAGS = -g -Wall -std=c++11 -O3


# Setup constants for code directories
INC_DIR = ../../include
OBJ_DIR = ../../build


# Define the targets for the executables
all: $(OBJ_DIR)/GreedySearch.o $(OBJ_DIR)/RobustPrune.o $(OBJ_DIR)/VamanaIndex.o $(OBJ_DIR)/recall.o \
		 $(OBJ_DIR)/FilteredVamanaIndex.o $(OBJ_DIR)/grountruth.o $(OBJ_DIR)/StichedVamanaIndex.o


# Compile the source files in the current directory
$(OBJ_DIR)/GreedySearch.o: Algorithms/GreedySearch.cpp
	$(CC) $(FLAGS) -o $(OBJ_DIR)/GreedySearch.o -c Algorithms/GreedySearch.cpp -I$(INC_DIR)

$(OBJ_DIR)/RobustPrune.o: Algorithms/RobustPrune.cpp
	$(CC) $(FLAGS) -o $(OBJ_DIR)/RobustPrune.o -c Algorithms/RobustPrune.cpp -I$(INC_DIR)

$(OBJ_DIR)/VamanaIndex.o: Algorithms/VamanaIndex.cpp
	$(CC) $(FLAGS) -o $(OBJ_DIR)/VamanaIndex.o -c Algorithms/VamanaIndex.cpp -I$(INC_DIR)

$(OBJ_DIR)/FilteredVamanaIndex.o: Algorithms/FilteredVamanaIndex.cpp
	$(CC) $(FLAGS) -o $(OBJ_DIR)/FilteredVamanaIndex.o -c Algorithms/FilteredVamanaIndex.cpp -I$(INC_DIR)

$(OBJ_DIR)/StichedVamanaIndex.o: Algorithms/StichedVamanaIndex.cpp
	$(CC) $(FLAGS) -o $(OBJ_DIR)/StichedVamanaIndex.o -c Algorithms/StichedVamanaIndex.cpp -I$(INC_DIR)

$(OBJ_DIR)/recall.o: Evaluation/recall.cpp
	$(CC) $(FLAGS) -o $(OBJ_DIR)/recall.o -c Evaluation/recall.cpp -I$(INC_DIR)

$(OBJ_DIR)/grountruth.o: Evaluation/grountruth.cpp
	$(CC) $(FLAGS) -o $(OBJ_DIR)/grountruth.o -c Evaluation/grountruth.cpp -I$(INC_DIR)
