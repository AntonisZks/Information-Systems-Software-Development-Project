# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++11 -I.  # Include current directory for headers

# Source files
SOURCES = readVectors.cpp test_distance.cpp DataVectorNew.cpp
OBJECTS = $(SOURCES:.cpp=.o)
TARGET = test_distance

# Default target
all: $(TARGET)

# Linking
$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Rule for compiling source files into object files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean target
clean:
	rm -f $(OBJECTS) $(TARGET)
