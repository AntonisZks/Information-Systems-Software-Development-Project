# Define the compiler and its flags during compilation
CC = g++
FLAGS = -g -Wall -std=c++11 -O3


# Setup constants for code directories
INC_DIR = ../include
OBJ_DIR = ../build


# Locate all the .cpp files in the src directory and flatten their object paths
GEOMETRY_OBJS = $(OBJ_DIR)/DataVector.o
GRAPHICS_OBJS = $(OBJ_DIR)/ProgressBar.o
DATA_READERS_OBJS = $(OBJ_DIR)/read_vectors.o
GRAPH_OBJS = $(OBJ_DIR)/Graph.o $(OBJ_DIR)/graph_node.o
VIA_OBJS = $(OBJ_DIR)/GreedySearch.o $(OBJ_DIR)/RobustPrune.o $(OBJ_DIR)/VamanaIndex.o $(OBJ_DIR)/recall.o


# Define the targets for the executables
all: $(GRAPH_OBJS) $(GEOMETRY_OBJS) $(VIA_OBJS) $(DATA_READERS_OBJS) $(GRAPHICS_OBJS)


# Compile all the objects in the src directory
$(OBJ_DIR)/%.o: %.cpp
	@mkdir -p $(OBJ_DIR)
	$(CC) $(FLAGS) -I$(INC_DIR) -c $< -o $@


# Compile the objects in the subdirectories by calling their Makefiles
$(GRAPH_OBJS): | $(OBJ_DIR)
	$(MAKE) -C Graph

$(GEOMETRY_OBJS): | $(OBJ_DIR)
	$(MAKE) -C Geometry

$(VIA_OBJS): | $(OBJ_DIR)
	$(MAKE) -C VIA

$(DATA_READERS_OBJS): | $(OBJ_DIR)
	$(MAKE) -C DataReaders

$(GRAPHICS_OBJS): | $(OBJ_DIR)
	$(MAKE) -C Graphics


# Clean the object files
clean:
	rm -f $(OBJ_DIR)/*.o
