CC = g++
FLAGS = -g -Wall -std=c++11 -O3

INC_DIR = ../../include
OBJ_DIR = ../../build

all: $(OBJ_DIR)/Graph.o $(OBJ_DIR)/graph_node.o

$(OBJ_DIR)/Graph.o: Graph.cpp
	$(CC) $(FLAGS) -o $(OBJ_DIR)/Graph.o -c Graph.cpp -I$(INC_DIR)

$(OBJ_DIR)/graph_node.o: graph_node.cpp
	$(CC) $(FLAGS) -o $(OBJ_DIR)/graph_node.o -c graph_node.cpp -I$(INC_DIR)
