// main.cpp

#include "graph.h"

int main() {
    // Number of vertices in the graph
    int vertices = 3;

    // Undirected Graph Example
    Graph undirectedGraph(vertices);
    undirectedGraph.addEdge(0, 1);
    undirectedGraph.addEdge(0, 2);
    undirectedGraph.addEdge(1, 2);

    // Print the adjacency list of the undirected graph
    cout << "Undirected Graph Adjacency List:" << endl;
    undirectedGraph.printGraph();

    cout << endl;

    // Directed Graph Example
    Graph directedGraph(vertices, true);
    directedGraph.addEdge(1, 2);
    directedGraph.addEdge(1, 0);
    directedGraph.addEdge(2, 0);

    // Print the adjacency list of the directed graph
    cout << "Directed Graph Adjacency List:" << endl;
    directedGraph.printGraph();

    return 0;
}
