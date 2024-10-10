// Graph.cpp

#include "graph.h"

// Constructor to initialize the graph
Graph::Graph(int vertices, bool directed) {
    // Resize the adjacency list to accommodate the vertices
    adjList.resize(vertices);
    // Set the directed flag
    isDirected = directed;
}

// Function to add an edge to the graph
void Graph::addEdge(int src, int dest) {
    // Add the destination to the adjacency list of the source
    adjList[src].push_back(dest);
    // If the graph is undirected
    if (!isDirected) {
        // Add the source to the adjacency list of the destination
        adjList[dest].push_back(src);
    }
}

// Function to print the adjacency list of the graph
void Graph::printGraph() {
    // Use size_t for the loop variable to match the vector's size_type
    for (size_t i = 0; i < adjList.size(); ++i) {
        // Print the vertex
        cout << i << ": ";
        // Use size_t for the inner loop variable as well
        for (size_t j = 0; j < adjList[i].size(); ++j) {
            // Print each adjacent vertex
            cout << adjList[i][j] << " -> ";
        }
        // Indicate the end of the adjacency list
        cout << "NULL" << endl;
    }
}
