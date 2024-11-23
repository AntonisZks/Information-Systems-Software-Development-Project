CC = g++
FLAGS = -g -Wall -std=c++11 -O3

INC_DIR = ../include
OBJ_DIR = ../build

all: $(OBJ_DIR)/DataVector.o $(OBJ_DIR)/distance_functions.o $(OBJ_DIR)/graph_node.o $(OBJ_DIR)/Graph.o \
     $(OBJ_DIR)/RobustPrune.o $(OBJ_DIR)/recall.o $(OBJ_DIR)/read_vectors.o $(OBJ_DIR)/GreedySearch.o $(OBJ_DIR)/graphics.o $(OBJ_DIR)/VamanaIndex.o

$(OBJ_DIR)/DataVector.o: DataVector.cpp
	$(CC) $(FLAGS) -o $(OBJ_DIR)/DataVector.o -c DataVector.cpp -I$(INC_DIR)

$(OBJ_DIR)/distance_functions.o: distance_functions.cpp
	$(CC) $(FLAGS) -o $(OBJ_DIR)/distance_functions.o -c distance_functions.cpp -I$(INC_DIR)

$(OBJ_DIR)/graph_node.o: graph_node.cpp
	$(CC) $(FLAGS) -o $(OBJ_DIR)/graph_node.o -c graph_node.cpp -I$(INC_DIR)

$(OBJ_DIR)/Graph.o: Graph.cpp
	$(CC) $(FLAGS) -o $(OBJ_DIR)/Graph.o -c Graph.cpp -I$(INC_DIR)

$(OBJ_DIR)/RobustPrune.o: RobustPrune.cpp
	$(CC) $(FLAGS) -o $(OBJ_DIR)/RobustPrune.o -c RobustPrune.cpp -I$(INC_DIR)

$(OBJ_DIR)/recall.o: recall.cpp
	$(CC) $(FLAGS) -o $(OBJ_DIR)/recall.o -c recall.cpp -I$(INC_DIR)

$(OBJ_DIR)/read_vectors.o: read_vectors.cpp
	$(CC) $(FLAGS) -o $(OBJ_DIR)/read_vectors.o -c read_vectors.cpp -I$(INC_DIR)

$(OBJ_DIR)/GreedySearch.o: GreedySearch.cpp
	$(CC) $(FLAGS) -o $(OBJ_DIR)/GreedySearch.o -c GreedySearch.cpp -I$(INC_DIR)

$(OBJ_DIR)/graphics.o: graphics.cpp
	$(CC) $(FLAGS) -o $(OBJ_DIR)/graphics.o -c graphics.cpp -I$(INC_DIR)

$(OBJ_DIR)/VamanaIndex.o: VamanaIndex.cpp
	$(CC) $(FLAGS) -o $(OBJ_DIR)/VamanaIndex.o -c VamanaIndex.cpp -I$(INC_DIR)

clean:
	rm -f $(OBJ_DIR)/*.o
