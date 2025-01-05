# Define the compiler and its flags during compilation
CC = g++
FLAGS = -g -Wall -std=c++11 -I $(INC_DIR) -O3

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
TARGET = $(EXE_DIR)/main

# Default targets
all: app tests

app: $(TARGET)
tests: $(UNIT_TESTS)

# Compile the third executable and its dependencies (objects)
$(TARGET): $(OBJS) $(OBJ_DIR)/main.o
	@mkdir -p $(EXE_DIR)
	$(CC) $(FLAGS) $(OBJS) $(OBJ_DIR)/main.o -o $@

$(OBJ_DIR)/main.o: app/main.cpp
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
-include $(OBJ_DIR)/main.d



# Execution Rules

create_simple_via:
	./bin/main --create -index-type 'simple' -base-file 'data/siftsmall/siftsmall_base.fvecs' -L 120 -R 12 -alpha 1.0 -save 'simple_index.bin' -distance-save matrix -distance-threads 1

create_filtered_via:
	./bin/main --create -index-type 'filtered' -base-file 'data/Dummy/dummy-data.bin' -L 120 -R 12 -alpha 1.0 -save 'filtered_index.bin' -distance-threads 1

create_stiched_via:
	./bin/main --create -index-type 'stiched' -base-file 'data/Dummy/dummy-data.bin' -L-small 150 -R-small 12 -R-stiched 20 -alpha 1.0 -save 'stiched_index.bin' -distance-threads 1 -computing-threads 1

compute_groundtruth:
	./bin/main --compute-gt -base-file 'data/Dummy/dummy-data.bin' -query-file 'data/Dummy/dummy-queries.bin' -gt-file 'data/Dummy/dummy-groundtruth.bin'

test_simple_via:
	./bin/main --test -index-type 'simple' -load 'simple_index.bin' -k 100 -L 120 -gt-file 'data/siftsmall/siftsmall_groundtruth.ivecs' -query-file 'data/siftsmall/siftsmall_query.fvecs' -query 1

test_filtered_via:
	./bin/main --test -index-type 'filtered' -load 'filtered_index.bin' -L 120 -k 100 -gt-file 'data/Dummy/dummy-groundtruth.bin' -query-file 'data/Dummy/dummy-queries.bin' -query 1

test_stiched_via:
	./bin/main --test -index-type 'stiched' -load 'stiched_index.bin' -L 150 -k 100 -gt-file 'data/Dummy/dummy-groundtruth.bin' -query-file 'data/Dummy/dummy-queries.bin' -query 1

test_and_save_stiched_empty_unfiltered_via:
	./bin/main --test -index-type 'stiched' -load 'models/stiched/stiched_index_empty.bin' -L 150 -k 100 -gt-file 'data/Dummy/dummy-groundtruth.bin' -query-file 'data/Dummy/dummy-queries.bin' -query -1 -test-on unfiltered -save-recalls results/empty/empty_stiched_index_unfiltered_recalls.txt

test_and_save_stiched_empty_filtered_via:
	./bin/main --test -index-type 'stiched' -load 'models/stiched/stiched_index_empty.bin' -L 150 -k 100 -gt-file 'data/Dummy/dummy-groundtruth.bin' -query-file 'data/Dummy/dummy-queries.bin' -query -1 -test-on filtered -save-recalls results/empty/empty_stiched_index_filtered_recalls.txt

test_and_save_stiched_filled_unfiltered_via:
	./bin/main --test -index-type 'stiched' -load 'models/stiched/stiched_index_filled.bin' -L 150 -k 100 -gt-file 'data/Dummy/dummy-groundtruth.bin' -query-file 'data/Dummy/dummy-queries.bin' -query -1 -test-on unfiltered -save-recalls results/filled/filled_stiched_index_unfiltered_recalls.txt

test_and_save_stiched_filled_filtered_via:
	./bin/main --test -index-type 'stiched' -load 'models/stiched/stiched_index_filled.bin' -L 150 -k 100 -gt-file 'data/Dummy/dummy-groundtruth.bin' -query-file 'data/Dummy/dummy-queries.bin' -query -1 -test-on filtered -save-recalls results/filled/filled_stiched_index_filtered_recalls.txt


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
