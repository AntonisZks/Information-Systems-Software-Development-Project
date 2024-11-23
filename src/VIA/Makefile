CC = g++
FLAGS = -g -Wall -std=c++11 -O3

INC_DIR = ../../include
OBJ_DIR = ../../build

all: $(OBJ_DIR)/GreedySearch.o $(OBJ_DIR)/RobustPrune.o $(OBJ_DIR)/VamanaIndex.o $(OBJ_DIR)/recall.o

$(OBJ_DIR)/GreedySearch.o: Algorithms/GreedySearch.cpp
	$(CC) $(FLAGS) -o $(OBJ_DIR)/GreedySearch.o -c Algorithms/GreedySearch.cpp -I$(INC_DIR)

$(OBJ_DIR)/RobustPrune.o: Algorithms/RobustPrune.cpp
	$(CC) $(FLAGS) -o $(OBJ_DIR)/RobustPrune.o -c Algorithms/RobustPrune.cpp -I$(INC_DIR)

$(OBJ_DIR)/VamanaIndex.o: Algorithms/VamanaIndex.cpp
	$(CC) $(FLAGS) -o $(OBJ_DIR)/VamanaIndex.o -c Algorithms/VamanaIndex.cpp -I$(INC_DIR)

$(OBJ_DIR)/recall.o: Evaluation/recall.cpp
	$(CC) $(FLAGS) -o $(OBJ_DIR)/recall.o -c Evaluation/recall.cpp -I$(INC_DIR)
