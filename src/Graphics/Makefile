# Define the compiler and its flags during compilation
CC = g++
FLAGS = -g -Wall -std=c++11 -O3


# Setup constants for code directories
INC_DIR = ../../include
OBJ_DIR = ../../build


# Define the targets for the executables
all: $(OBJ_DIR)/ProgressBar.o


# Compile the source files in the current directory
$(OBJ_DIR)/ProgressBar.o: ProgressBar.cpp
	$(CC) $(FLAGS) -o $(OBJ_DIR)/ProgressBar.o -c ProgressBar.cpp -I$(INC_DIR)
