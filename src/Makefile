CC = g++
FLAGS = -g -Wall -std=c++11 -O3

INC_DIR = ../include
OBJ_DIR = ../build

GRAPH_OBJS = $(OBJ_DIR)/Graph.o $(OBJ_DIR)/graph_node.o
GEOMETRY_OBJS = $(OBJ_DIR)/DataVector.o
VIA_OBJS = $(OBJ_DIR)/GreedySearch.o $(OBJ_DIR)/RobustPrune.o $(OBJ_DIR)/VamanaIndex.o $(OBJ_DIR)/recall.o
DATA_READERS_OBJS = $(OBJ_DIR)/read_vectors.o
GRAPHICS_OBJS = $(OBJ_DIR)/ProgressBar.o

all: $(GRAPH_OBJS) $(GEOMETRY_OBJS) $(VIA_OBJS) $(DATA_READERS_OBJS) $(GRAPHICS_OBJS)

$(GRAPH_OBJS):
	$(MAKE) -C Graph

$(GEOMETRY_OBJS):
	$(MAKE) -C Geometry

$(VIA_OBJS):
	$(MAKE) -C VIA

$(DATA_READERS_OBJS):
	$(MAKE) -C DataReaders

$(GRAPHICS_OBJS):
	$(MAKE) -C Graphics

clean:
	rm -f $(OBJ_DIR)/*.o
