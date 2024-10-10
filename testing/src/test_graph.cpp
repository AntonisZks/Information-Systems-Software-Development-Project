#include "graph.h"
#include "acutest.h"

// Test case to check the adjacency list of the undirected graph
void test_undirected_graph() {
    Graph g(3); // Create a graph with 3 vertices
    g.addEdge(0, 1);
    g.addEdge(0, 2);
    g.addEdge(1, 2);

    // Check the adjacency list
    // Expected adjacency list:
    // 0: 1 -> 2 -> NULL
    // 1: 0 -> 2 -> NULL
    // 2: 0 -> 1 -> NULL

    // Create a string stream to capture output
    std::ostringstream output;
    std::streambuf *oldCoutBuffer = std::cout.rdbuf(); // Store old buffer
    std::cout.rdbuf(output.rdbuf()); // Redirect std::cout to string stream

    g.printGraph(); // Print the graph

    std::cout.rdbuf(oldCoutBuffer); // Restore original buffer

    std::string expectedOutput = "0: 1 -> 2 -> NULL\n"
                                  "1: 0 -> 2 -> NULL\n"
                                  "2: 0 -> 1 -> NULL\n";

    TEST_CHECK(output.str() == expectedOutput);
}

// Test case to check the adjacency list of the directed graph
void test_directed_graph() {
    Graph g(3, true); // Create a directed graph with 3 vertices
    g.addEdge(1, 2);
    g.addEdge(1, 0);
    g.addEdge(2, 0);

    // Check the adjacency list
    // Expected adjacency list:
    // 0: NULL
    // 1: 2 -> 0 -> NULL
    // 2: 0 -> NULL

    // Create a string stream to capture output
    std::ostringstream output;
    std::streambuf *oldCoutBuffer = std::cout.rdbuf(); // Store old buffer
    std::cout.rdbuf(output.rdbuf()); // Redirect std::cout to string stream

    g.printGraph(); // Print the graph

    std::cout.rdbuf(oldCoutBuffer); // Restore original buffer

    std::string expectedOutput = "0: NULL\n"
                                  "1: 2 -> 0 -> NULL\n"
                                  "2: 0 -> NULL\n";

    TEST_CHECK(output.str() == expectedOutput);
}

// Main function to run all tests
TEST_LIST = {
    { "Undirected Graph Test", test_undirected_graph },
    { "Directed Graph Test", test_directed_graph },
    { NULL, NULL } // Terminate the list
};
