# Define the compiler and its flags during compilation
CC = g++
FLAGS = -g -Wall -std=c++11 -I$(INC_DIR)

# Setup constants for code directories
INC_DIR = include
SRC_DIR = src
OBJ_DIR = build
EXE_DIR = bin
TST_DIR = tests

# make all builds evrything
all: build bin $(EXE_DIR)/graph_node_test $(EXE_DIR)/graph_test $(EXE_DIR)/test_distance $(EXE_DIR)/test_read_vectors $(EXE_DIR)/printDataSet


# Graph node test executable is being compiled here
$(EXE_DIR)/graph_node_test: $(OBJ_DIR)/graph_node_test.o $(INC_DIR)/Graph/graph_node.h $(INC_DIR)/Graph/graph.h
	$(CC) $(FLAGS) -o $(EXE_DIR)/graph_node_test $(OBJ_DIR)/graph_node_test.o

$(OBJ_DIR)/graph_node_test.o: $(TST_DIR)/graph_node_test.cc $(INC_DIR)/acutest.h $(INC_DIR)/Graph/graph_node.h $(INC_DIR)/Graph/graph.h
	$(CC) $(FLAGS) -o $(OBJ_DIR)/graph_node_test.o -c $(TST_DIR)/graph_node_test.cc

# Graph test executable is being compiled here
$(EXE_DIR)/graph_test: $(OBJ_DIR)/graph_test.o $(INC_DIR)/Graph/graph.h
	$(CC) $(FLAGS) -o $(EXE_DIR)/graph_test $(OBJ_DIR)/graph_test.o

$(OBJ_DIR)/graph_test.o: $(TST_DIR)/graph_test.cc $(INC_DIR)/acutest.h $(INC_DIR)/Graph/graph.h
	$(CC) $(FLAGS) -o $(OBJ_DIR)/graph_test.o -c $(TST_DIR)/graph_test.cc


# Test distance executable is being compiled here
$(EXE_DIR)/test_distance: $(OBJ_DIR)/test_distance.o $(OBJ_DIR)/distance_functions.o $(INC_DIR)/Graph/graph.h $(INC_DIR)/distance.h
	$(CC) $(FLAGS) -o $(EXE_DIR)/test_distance $(OBJ_DIR)/test_distance.o $(OBJ_DIR)/distance_functions.o

$(OBJ_DIR)/test_distance.o: $(TST_DIR)/test_distance.cc $(INC_DIR)/acutest.h $(INC_DIR)/Graph/graph.h
	$(CC) $(FLAGS) -o $(OBJ_DIR)/test_distance.o -c $(TST_DIR)/test_distance.cc

$(OBJ_DIR)/distance_functions.o: $(SRC_DIR)/distance_functions.cpp $(INC_DIR)/distance.h
	$(CC) $(FLAGS) -o $(OBJ_DIR)/distance_functions.o -c $(SRC_DIR)/distance_functions.cpp

# read_vector executable is being compiled here
$(EXE_DIR)/test_read_vectors: $(OBJ_DIR)/test_read_vectors.o $(OBJ_DIR)/read_vectors.o $(INC_DIR)/DataVector/DataVector.h $(INC_DIR)/read_data.h 
	$(CC) $(FLAGS) -o $(EXE_DIR)/test_read_vectors $(OBJ_DIR)/test_read_vectors.o $(OBJ_DIR)/read_vectors.o

$(OBJ_DIR)/test_read_vectors.o: $(TST_DIR)/test_read_vectors.cc $(INC_DIR)/acutest.h $(INC_DIR)/DataVector/DataVector.h
	$(CC) $(FLAGS) -o $(OBJ_DIR)/test_read_vectors.o -c $(TST_DIR)/test_read_vectors.cc

# printDataSet files
$(EXE_DIR)/printDataSet: $(OBJ_DIR)/printDataSet.o $(OBJ_DIR)/read_vectors.o $(INC_DIR)/Graph/graph.h $(INC_DIR)/read_data.h $(INC_DIR)/DataVector/DataVector.h
	$(CC) $(FLAGS) -o $(EXE_DIR)/printDataSet $(OBJ_DIR)/printDataSet.o $(OBJ_DIR)/read_vectors.o

$(OBJ_DIR)/printDataSet.o: printDataSet.cpp $(INC_DIR)/Graph/graph.h $(INC_DIR)/read_data.h $(INC_DIR)/DataVector/DataVector.h
	$(CC) $(FLAGS) -o $(OBJ_DIR)/printDataSet.o -c printDataSet.cpp

$(OBJ_DIR)/read_vectors.o: $(SRC_DIR)/read_vectors.cpp $(INC_DIR)/read_data.h $(INC_DIR)/DataVector/DataVector.h
	$(CC) $(FLAGS) -o $(OBJ_DIR)/read_vectors.o -c $(SRC_DIR)/read_vectors.cpp


# build directory creation 
# (every object file goes here)
build:
	mkdir build


# bin directory creation 
# (every executable file goes here)
bin:
	mkdir bin


# Clean command to clean the workspace
.PHONY: clean

clean:
	rm -r -f bin
	rm -r -f build
