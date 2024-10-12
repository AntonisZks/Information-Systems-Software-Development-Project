# Define the compiler and its flags during compilation
CC = g++
FLAGS = -g -Wall

# Setup constants for code directories
INC_DIR = include
SRC_DIR = src
OBJ_DIR = build
EXE_DIR = bin
TST_DIR = tests


# make all builds evrything
all: build bin $(EXE_DIR)/graph_test


# Graph test executable is being compiled here
$(EXE_DIR)/graph_test: $(OBJ_DIR)/graph_test.o
	$(CC) $(FLAGS) -o $(EXE_DIR)/graph_test $(OBJ_DIR)/graph_test.o

$(OBJ_DIR)/graph_test.o: $(TST_DIR)/graph_test.cc $(INC_DIR)/acutest.h
	$(CC) $(FLAGS) -o $(OBJ_DIR)/graph_test.o -c $(TST_DIR)/graph_test.cc


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
