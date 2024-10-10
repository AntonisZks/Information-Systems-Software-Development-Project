//######################################################################################
// This is were the graph that we will be using 
// for knn will be created.
//######################################################################################

// CPP libraries
// this contains most of the STL headers files (data structures) amd iostream
// WARNING individual #inlcludes to ensure portability
// temporary placeholder for clean code
#include <bits/stdc++.h>

using namespace std;

// Class representing a graph
class Graph {
private:
    vector<vector<int>> adjList;
    bool isDirected;

public:
    Graph(int vertices, bool directed = false)
    {
        adjList.resize(vertices);
        isDirected = directed;
    }

    void addEdge(int src, int dest)
    {
        adjList[src].push_back(dest);
        if (!isDirected) {
            adjList[dest].push_back(src);
        }
    }

    string printGraph()
    {
        stringstream ss;
        for (int i = 0; i < adjList.size(); ++i) {
            ss << i << ": ";
            for (int j = 0; j < adjList[i].size(); ++j) {
                ss << adjList[i][j] << " -> ";
            }
            ss << "NULL\n";
        }
        return ss.str();
    }
};
