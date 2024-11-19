# Define the compiler and its flags during compilation
CC = g++
FLAGS = -g -Wall -std=c++11 -I $(INC_DIR) -O3

# Setup constants for code directories
INC_DIR = include
SRC_DIR = src
OBJ_DIR = build
EXE_DIR = bin
TST_DIR = tests

# make all builds everything
all: code test

# Compile only the main code
code: build bin $(EXE_DIR)/main $(EXE_DIR)/main2

# Compile all test executables
test: build bin $(EXE_DIR)/graph_node_test $(EXE_DIR)/graph_test $(EXE_DIR)/test_distance $(EXE_DIR)/test_data_vectors $(EXE_DIR)/test_recall
	@echo "Running all tests:"
	$(EXE_DIR)/graph_node_test
	$(EXE_DIR)/graph_test
	$(EXE_DIR)/test_distance
	$(EXE_DIR)/test_data_vectors
	$(EXE_DIR)/test_recall

# Run tests with valgrind
valgrind: test
	valgrind --leak-check=full $(EXE_DIR)/graph_node_test
	valgrind --leak-check=full $(EXE_DIR)/graph_test
	valgrind --leak-check=full $(EXE_DIR)/test_distance
	valgrind --leak-check=full $(EXE_DIR)/test_data_vectors
	valgrind --leak-check=full $(EXE_DIR)/test_recall

# Define the rule for the main executable
$(EXE_DIR)/main: $(OBJ_DIR)/main.o $(OBJ_DIR)/read_vectors.o $(OBJ_DIR)/distance_functions.o
	$(CC) $(FLAGS) -o $(EXE_DIR)/main $(OBJ_DIR)/main.o $(OBJ_DIR)/read_vectors.o $(OBJ_DIR)/distance_functions.o

$(OBJ_DIR)/main.o: main.cpp $(INC_DIR)/DataStructures/Graph/graph.h $(INC_DIR)/read_data.h $(INC_DIR)/DataStructures/DataVector/DataVector.h
	$(CC) $(FLAGS) -o $(OBJ_DIR)/main.o -c main.cpp

$(EXE_DIR)/main2: $(OBJ_DIR)/main2.o $(OBJ_DIR)/read_vectors.o $(OBJ_DIR)/distance_functions.o
	$(CC) $(FLAGS) -o $(EXE_DIR)/main2 $(OBJ_DIR)/main2.o $(OBJ_DIR)/read_vectors.o $(OBJ_DIR)/distance_functions.o

$(OBJ_DIR)/main2.o: main2.cpp $(INC_DIR)/DataStructures/Graph/graph.h $(INC_DIR)/read_data.h $(INC_DIR)/DataStructures/DataVector/DataVector.h
	$(CC) $(FLAGS) -o $(OBJ_DIR)/main2.o -c main2.cpp

# Rule to create read_vectors.o
$(OBJ_DIR)/read_vectors.o: $(SRC_DIR)/read_vectors.cpp $(INC_DIR)/read_data.h $(INC_DIR)/DataStructures/DataVector/DataVector.h
	$(CC) $(FLAGS) -o $(OBJ_DIR)/read_vectors.o -c $(SRC_DIR)/read_vectors.cpp

# Rule to create distance_functions.o
$(OBJ_DIR)/distance_functions.o: $(SRC_DIR)/distance_functions.cpp $(INC_DIR)/distance.h
	$(CC) $(FLAGS) -o $(OBJ_DIR)/distance_functions.o -c $(SRC_DIR)/distance_functions.cpp

# Test executables and object file rules

# Build graph_node_test executable
$(EXE_DIR)/graph_node_test: $(OBJ_DIR)/graph_node_test.o
	$(CC) $(FLAGS) -o $(EXE_DIR)/graph_node_test $(OBJ_DIR)/graph_node_test.o

$(OBJ_DIR)/graph_node_test.o: $(TST_DIR)/graph_node_test.cc $(INC_DIR)/acutest.h $(INC_DIR)/DataStructures/Graph/graph_node.h
	$(CC) $(FLAGS) -o $(OBJ_DIR)/graph_node_test.o -c $(TST_DIR)/graph_node_test.cc

# Build graph_test executable
$(EXE_DIR)/graph_test: $(OBJ_DIR)/graph_test.o
	$(CC) $(FLAGS) -o $(EXE_DIR)/graph_test $(OBJ_DIR)/graph_test.o

$(OBJ_DIR)/graph_test.o: $(TST_DIR)/graph_test.cc $(INC_DIR)/acutest.h $(INC_DIR)/DataStructures/Graph/graph.h
	$(CC) $(FLAGS) -o $(OBJ_DIR)/graph_test.o -c $(TST_DIR)/graph_test.cc

# Build test_distance executable
$(EXE_DIR)/test_distance: $(OBJ_DIR)/test_distance.o $(OBJ_DIR)/distance_functions.o
	$(CC) $(FLAGS) -o $(EXE_DIR)/test_distance $(OBJ_DIR)/test_distance.o $(OBJ_DIR)/distance_functions.o

$(OBJ_DIR)/test_distance.o: $(TST_DIR)/test_distance.cc $(INC_DIR)/acutest.h $(INC_DIR)/distance.h
	$(CC) $(FLAGS) -o $(OBJ_DIR)/test_distance.o -c $(TST_DIR)/test_distance.cc

# Build test_data_vectors executable
$(EXE_DIR)/test_data_vectors: $(OBJ_DIR)/test_data_vectors.o $(OBJ_DIR)/read_vectors.o
	$(CC) $(FLAGS) -o $(EXE_DIR)/test_data_vectors $(OBJ_DIR)/test_data_vectors.o $(OBJ_DIR)/read_vectors.o

$(OBJ_DIR)/test_data_vectors.o: $(TST_DIR)/test_data_vectors.cc $(INC_DIR)/acutest.h $(INC_DIR)/read_data.h
	$(CC) $(FLAGS) -o $(OBJ_DIR)/test_data_vectors.o -c $(TST_DIR)/test_data_vectors.cc

# Build test_recall executable
$(EXE_DIR)/test_recall: $(OBJ_DIR)/test_recall.o
	$(CC) $(FLAGS) -o $(EXE_DIR)/test_recall $(OBJ_DIR)/test_recall.o

$(OBJ_DIR)/test_recall.o: $(TST_DIR)/test_recall.cc $(INC_DIR)/acutest.h $(INC_DIR)/Evaluation/recall.h
	$(CC) $(FLAGS) -o $(OBJ_DIR)/test_recall.o -c $(TST_DIR)/test_recall.cc

# Directories creation
build:
	mkdir -p $(OBJ_DIR)

bin:
	mkdir -p $(EXE_DIR)

# Clean command to clean the workspace
.PHONY: clean
clean:
	rm -r -f $(EXE_DIR)
	rm -r -f $(OBJ_DIR)
