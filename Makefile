CC = g++
FLAGS = -g -Wall -std=c++11

INC_DIR = include
SRC_DIR = src
OBJ_DIR = build
EXE_DIR = bin
TST_DIR = tests
GTEST_DIR = googletest/googletest
GTEST_LIB = googletest/build/lib

# Build main program
all: build bin $(EXE_DIR)/main

$(EXE_DIR)/main: $(OBJ_DIR)/main.o
	$(CC) $(FLAGS) -o $(EXE_DIR)/main $(OBJ_DIR)/main.o

$(OBJ_DIR)/main.o: main.cpp $(INC_DIR)/graph.h
	$(CC) $(FLAGS) -I$(INC_DIR) -o $(OBJ_DIR)/main.o -c main.cpp

# Build and run tests
test: build bin $(EXE_DIR)/graph_test

$(EXE_DIR)/graph_test: $(OBJ_DIR)/graph_test.o $(OBJ_DIR)/graph.o
	$(CC) $(FLAGS) -I$(GTEST_DIR)/include -I$(INC_DIR) -o $(EXE_DIR)/graph_test $(OBJ_DIR)/graph_test.o $(OBJ_DIR)/graph.o -L$(GTEST_LIB) -lgtest -lgtest_main -lpthread
	./$(EXE_DIR)/graph_test

$(OBJ_DIR)/graph_test.o: $(TST_DIR)/graph_test.cpp $(INC_DIR)/graph.h
	$(CC) $(FLAGS) -I$(GTEST_DIR)/include -I$(INC_DIR) -c $(TST_DIR)/graph_test.cpp -o $(OBJ_DIR)/graph_test.o

$(OBJ_DIR)/graph.o: $(SRC_DIR)/create_graph.cpp $(INC_DIR)/graph.h
	$(CC) $(FLAGS) -I$(INC_DIR) -c $(SRC_DIR)/create_graph.cpp -o $(OBJ_DIR)/graph.o

# Create necessary directories
build:
	mkdir -p build

bin:
	mkdir -p bin

# Clean build and bin directories
.PHONY: clean

clean:
	rm -rf bin
	rm -rf build
