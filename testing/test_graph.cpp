#define ACUTEST_NO_MAIN // Prevent Acutest from defining its own main()

#include "graph.cpp"    // Include your Graph class implementation
#include "acutest.h"    // Include Acutest framework
#include <iostream>

// Test case for undirected graph
void test_undirected_graph()
{
    Graph undirectedGraph(3);
    undirectedGraph.addEdge(0, 1);
    undirectedGraph.addEdge(0, 2);
    undirectedGraph.addEdge(1, 2);

    std::string expected = "0: 1 -> 2 -> NULL\n1: 0 -> 2 -> NULL\n2: 0 -> 1 -> NULL\n";
    std::string output = undirectedGraph.printGraph();

    if (output == expected) {
        std::cout << "[OK] Undirected graph test passed." << std::endl;
    } else {
        std::cout << "[FAIL] Undirected graph test failed." << std::endl;
    }
}

// Test case for directed graph
void test_directed_graph()
{
    Graph directedGraph(3, true);
    directedGraph.addEdge(1, 2);
    directedGraph.addEdge(1, 0);
    directedGraph.addEdge(2, 0);

    std::string expected = "0: NULL\n1: 2 -> 0 -> NULL\n2: 0 -> NULL\n";
    std::string output = directedGraph.printGraph();

    if (output == expected) {
        std::cout << "[OK] Directed graph test passed." << std::endl;
    } else {
        std::cout << "[FAIL] Directed graph test failed." << std::endl;
    }
}

int main()
{
    std::cout << "Running tests..." << std::endl;

    // Run the undirected graph test
    test_undirected_graph();
    
    // Run the directed graph test
    test_directed_graph();

    std::cout << "All tests completed." << std::endl;

    return 0;
}
