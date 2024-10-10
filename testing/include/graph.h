// Graph.h

#ifndef GRAPH_H
#define GRAPH_H

// CPP libraries
// this contains most of the STL headers files (data structures) amd iostream
// WARNING individual #inlcludes to ensure portability
// temporary placeholder for clean code
#include <bits/stdc++.h>

using namespace std;

// Class representing a graph
class Graph {
private:
    // Adjacency list to represent the graph
    vector<vector<int>> adjList;
    // Boolean flag to indicate if the graph is directed
    bool isDirected;

public:
    // Constructor to initialize the graph
    Graph(int vertices, bool directed = false);
    
    // Function to add an edge to the graph
    void addEdge(int src, int dest);
    
    // Function to print the adjacency list of the graph
    void printGraph();
};

#endif // GRAPH_H
