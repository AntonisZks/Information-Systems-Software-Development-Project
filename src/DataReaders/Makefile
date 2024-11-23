CC = g++
FLAGS = -g -Wall -std=c++11 -O3

INC_DIR = ../../include
OBJ_DIR = ../../build

all: $(OBJ_DIR)/read_vectors.o

$(OBJ_DIR)/read_vectors.o: read_vectors.cpp
	$(CC) $(FLAGS) -o $(OBJ_DIR)/read_vectors.o -c read_vectors.cpp -I$(INC_DIR)
