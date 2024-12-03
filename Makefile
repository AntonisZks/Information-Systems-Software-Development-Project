# Define the compiler and its flags during compilation
CC = g++
FLAGS = -g -Wall -std=c++11 -I $(INC_DIR) -O0

# Setup constants for code directories
INC_DIR = include
SRC_DIR = src
OBJ_DIR = build
EXE_DIR = bin
TST_DIR = tests

# Locate all the .cpp files in the src directory
SRC_FILES := $(shell find $(SRC_DIR) -name '*.cpp')
OBJS := $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRC_FILES))

# Define the test executables
UNIT_TEST_SRCS := $(wildcard $(TST_DIR)/*.cc)
UNIT_TESTS := $(patsubst $(TST_DIR)/%.cc, $(EXE_DIR)/%, $(UNIT_TEST_SRCS))

# Define the application executables
TARGET_SIMPLE_VAMANA = $(EXE_DIR)/simple_vamana
TARGET_FILTERED_VAMANA = $(EXE_DIR)/filtered_vamana

# Default targets
all: app tests

app: $(TARGET_SIMPLE_VAMANA) $(TARGET_FILTERED_VAMANA)
tests: $(UNIT_TESTS)

# Compile the first executable and its dependencies (objects)
$(TARGET_SIMPLE_VAMANA): $(OBJS) $(OBJ_DIR)/simple_vamana_main.o
	@mkdir -p $(EXE_DIR)
	$(CC) $(FLAGS) $(OBJS) $(OBJ_DIR)/simple_vamana_main.o -o $@

$(OBJ_DIR)/simple_vamana_main.o: app/simple_vamana_main.cpp
	@mkdir -p $(OBJ_DIR)
	$(CC) $(FLAGS) -c $< -o $@

# Compile the second executable and its dependencies (objects)
$(TARGET_FILTERED_VAMANA): $(OBJS) $(OBJ_DIR)/filtered_vamana_main.o
	@mkdir -p $(EXE_DIR)
	$(CC) $(FLAGS) $(OBJS) $(OBJ_DIR)/filtered_vamana_main.o -o $@

$(OBJ_DIR)/filtered_vamana_main.o: app/filtered_vamana_main.cpp
	@mkdir -p $(OBJ_DIR)
	$(CC) $(FLAGS) -c $< -o $@

# Compile the objects in the src directory
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CC) $(FLAGS) -c $< -o $@

# Compile the test executables and their dependencies
$(EXE_DIR)/%: $(OBJ_DIR)/%.o $(OBJS)
	@mkdir -p $(EXE_DIR)
	$(CC) $(FLAGS) $< $(OBJS) -o $@

$(OBJ_DIR)/%.o: $(TST_DIR)/%.cc
	@mkdir -p $(OBJ_DIR)
	$(CC) $(FLAGS) -c $< -o $@

# Clean the object files
clean:
	rm -rf $(OBJ_DIR) $(EXE_DIR)

# Dependency generation
DEPFLAGS = -MMD -MP
FLAGS += $(DEPFLAGS)

-include $(OBJS:.o=.d)
-include $(OBJ_DIR)/simple_vamana_main.d
-include $(OBJ_DIR)/filtered_vamana_main.d



# Execution Rules

create_simple_vamana:
	./bin/simple_vamana --create -base-file 'data/siftsmall/siftsmall_base.fvecs' -L 20 -R 14 -alpha 1.0 -save 'index.bin'

create_simple_vamana_valgrind:
	valgrind --leak-check=full ./bin/simple_vamana --create -base-file 'data/siftsmall/siftsmall_base.fvecs' -L 10 -R 14 -alpha 1.0 -save 'index.bin'


test_simple_vamana:
	./bin/simple_vamana --test -load 'index.bin' -k 100 -L 20 -gt-file 'data/siftsmall/siftsmall_groundtruth.ivecs' -query-file 'data/siftsmall/siftsmall_query.fvecs' -query 0

test_simple_vamana_valgrind:
	valgrind --leak-check=full ./bin/simple_vamana --test -load 'index.bin' -k 100 -L 10 -gt-file 'data/siftsmall/siftsmall_groundtruth.ivecs' -query-file 'data/siftsmall/siftsmall_query.fvecs' -query 0


create_filtered_vamana:
	./bin/filtered_vamana

create_filtered_vamana_valgrind:
	./bin/filtered_vamana


run_tests:
	./bin/graph_node_test
	./bin/graph_test
	./bin/test_distance
	./bin/test_data_vectors
	./bin/test_recall
	./bin/filtered_vamana_test

run_tests_valgrind:
	valgrind --leak-check=full ./bin/graph_node_test
	valgrind --leak-check=full ./bin/graph_test
	valgrind --leak-check=full ./bin/test_distance
	valgrind --leak-check=full ./bin/test_data_vectors
	valgrind --leak-check=full ./bin/test_recall
	valgrind --leak-check=full ./bin/filtered_vamana_test
