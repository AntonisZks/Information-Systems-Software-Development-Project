# Define the compiler and its flags during compilation
CC = g++
FLAGS = -g -Wall -std=c++11 -O3


# Setup constants for code directories
INC_DIR = ../../include
OBJ_DIR = ../../build


# Define the targets for the executables
all: $(OBJ_DIR)/DataVector.o $(OBJ_DIR)/distance_functions.o


# Compile the source files in the current directory
$(OBJ_DIR)/DataVector.o: DataVector.cpp
	$(CC) $(FLAGS) -o $(OBJ_DIR)/DataVector.o -c DataVector.cpp -I$(INC_DIR)

$(OBJ_DIR)/distance_functions.o: distance_functions.cpp
	$(CC) $(FLAGS) -o $(OBJ_DIR)/distance_functions.o -c distance_functions.cpp -I$(INC_DIR)
