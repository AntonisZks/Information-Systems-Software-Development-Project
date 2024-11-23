CC = g++
FLAGS = -g -Wall -std=c++11 -O3

INC_DIR = ../../include
OBJ_DIR = ../../build

all: $(OBJ_DIR)/ProgressBar.o

$(OBJ_DIR)/ProgressBar.o: ProgressBar.cpp
	$(CC) $(FLAGS) -o $(OBJ_DIR)/ProgressBar.o -c ProgressBar.cpp -I$(INC_DIR)
