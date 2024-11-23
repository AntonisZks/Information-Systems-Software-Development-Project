CC = g++
FLAGS = -g -Wall -std=c++11 -O3

INC_DIR = ../../include
OBJ_DIR = ../../build

all: $(OBJ_DIR)/DataVector.o $(OBJ_DIR)/distance_functions.o

$(OBJ_DIR)/DataVector.o: DataVector.cpp
	$(CC) $(FLAGS) -o $(OBJ_DIR)/DataVector.o -c DataVector.cpp -I$(INC_DIR)

$(OBJ_DIR)/distance_functions.o: distance_functions.cpp
	$(CC) $(FLAGS) -o $(OBJ_DIR)/distance_functions.o -c distance_functions.cpp -I$(INC_DIR)
