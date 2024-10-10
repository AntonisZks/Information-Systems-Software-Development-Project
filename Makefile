CC = g++
FLAGS = -g -Wall

INC_DIR = include
SRC_DIR = src
OBJ_DIR = build
EXE_DIR = bin
TST_DIR = tests

all: build bin $(EXE_DIR)/main


$(EXE_DIR)/main: $(OBJ_DIR)/main.o
	$(CC) $(FLAGS) -o $(EXE_DIR)/main $(OBJ_DIR)/main.o

$(OBJ_DIR)/main.o: main.cpp $(INC_DIR)/Graph/graph.h
	$(CC) $(FLAGS) -o $(OBJ_DIR)/main.o -c main.cpp


build:
	mkdir build

bin:
	mkdir bin


.PHONY: clean

clean:
	rm -r -f bin
	rm -r -f build