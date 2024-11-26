# Define the compiler and its flags during compilation
CC = g++
FLAGS = -g -Wall -std=c++11 -O3


# Setup constants for code directories
INC_DIR = ../../include
OBJ_DIR = ../../build


# Define the targets for the executables
all: $(OBJ_DIR)/Graph.o $(OBJ_DIR)/graph_node.o


# Compile the source files in the current directory
$(OBJ_DIR)/Graph.o: Graph.cpp
	$(CC) $(FLAGS) -o $(OBJ_DIR)/Graph.o -c Graph.cpp -I$(INC_DIR)

$(OBJ_DIR)/graph_node.o: graph_node.cpp
	$(CC) $(FLAGS) -o $(OBJ_DIR)/graph_node.o -c graph_node.cpp -I$(INC_DIR)
