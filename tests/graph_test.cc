#include <iostream>
#include <vector>
#include <string>

#include "../include/acutest.h"

#include "../include/Graph/graph_node.h"
#include "../include/Graph/graph.h"


/**
 * @brief Simple test function that checks if the neighbors of a single graph node of integer values
 * are received successfully. It provides two simple cases of checking. One with small amount 
 * of neighbors, and one with a large amount of neighbors.
*/
void test_node_integer_neighbors_fetching(void) {

    // --- SMALL AMOUNT OF NEIGHBORS ---

    GraphNode<int> node1(1);
    std::vector<int> computedNeighbors, correctNeighbors;
    
    // Add some neighbors
    node1.addNeighbor(2);
    node1.addNeighbor(3);
    node1.addNeighbor(4);
    
    // Real neighbors
    correctNeighbors = { 2, 3, 4 };

    computedNeighbors = node1.getNeighbors();
    TEST_CHECK(computedNeighbors == correctNeighbors);

    // --- LARGE AMOUNT OF NEIGHBORS ---

    GraphNode<int> node2(1);
    computedNeighbors.clear(); correctNeighbors.clear();

    for (unsigned int i = 2; i < 10000; i++) {
        node2.addNeighbor(i);
        correctNeighbors.push_back(i);
    }

    computedNeighbors = node2.getNeighbors();
    TEST_CHECK(computedNeighbors == correctNeighbors);
    TEST_CHECK(node1.getNeighbors() != node2.getNeighbors());

}

/**
 * @brief This functions checks if the neighbors of a single graph node of string values are received
 * successfully. It provides an equality checking and an non-equality checking.
*/
void test_node_string_neighbors_fetching(void) {

    GraphNode<std::string> node1("Mark");
    std::vector<std::string> computedNeighbors, correctNeighbors, wrongNeighbors;

    // Add neighbors
    node1.addNeighbor("Peter");
    node1.addNeighbor("Mary");
    node1.addNeighbor("Michael");
    
    // Real and wrong ones
    correctNeighbors = { "Peter", "Mary", "Michael" };
    wrongNeighbors = { "Harry", "George", "Helen" };

    computedNeighbors = node1.getNeighbors();
    TEST_CHECK(computedNeighbors == correctNeighbors);
    TEST_CHECK(computedNeighbors != wrongNeighbors);

}

TEST_LIST = {
    { "neighbors_fetching_integers", test_node_integer_neighbors_fetching },
    { "neighbors_fetching_strings", test_node_string_neighbors_fetching },
    { NULL, NULL }
};
