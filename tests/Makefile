CC = g++
FLAGS = -g -Wall -std=c++11 -O3

INC_DIR = ../include
OBJ_DIR = ../build
EXE_DIR = ../bin
SRC_DIR = ../src

OBJS = $(OBJ_DIR)/DataVector.o $(OBJ_DIR)/distance_functions.o $(OBJ_DIR)/graph_node.o $(OBJ_DIR)/Graph.o \
			 $(OBJ_DIR)/RobustPrune.o $(OBJ_DIR)/recall.o $(OBJ_DIR)/read_vectors.o $(OBJ_DIR)/GreedySearch.o $(OBJ_DIR)/graphics.o $(OBJ_DIR)/VamanaIndex.o

TEST_SRCS = $(wildcard *.cc)
TEST_OBJS = $(patsubst %.cc, $(OBJ_DIR)/%.o, $(TEST_SRCS))
TARGETS = $(patsubst %.cc, $(EXE_DIR)/%, $(TEST_SRCS))

all: $(TARGETS)

$(EXE_DIR)/%: $(OBJ_DIR)/%.o $(OBJS)
	$(CC) $(FLAGS) $< $(OBJS) -o $@ -I$(INC_DIR)

$(OBJ_DIR)/%.o: %.cc
	$(CC) $(FLAGS) -c $< -o $@

$(OBJS):
	$(MAKE) -C $(SRC_DIR)

clean:
	rm -f $(OBJ_DIR)/*.o
	rm -f $(TARGETS)
