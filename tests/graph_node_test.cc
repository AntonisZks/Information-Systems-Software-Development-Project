#include <iostream>
#include <vector>
#include <string>

#include "../include/acutest.h"

#include "../include/DataStructures/Graph/graph_node.h"
#include "../include/DataStructures/Graph/graph.h"


/**
 * @brief Test function that checks if the data of a specific graph node is being fetched successfully.
 * It provides tests fo three types of data. Integer, floating point numbers and strings. It also checks
 * whether the data are changed successfully if needed.
 */
void test_node_data_fetching(void) {

    // Create nodes with specific data (int, double, string)
    GraphNode<int> node1(1);
    GraphNode<double> node2(2.78);
    GraphNode<std::string> node3("Jimmy");

    // Test if the data are fetched successfully
    TEST_CHECK(1 == node1.getData());
    TEST_CHECK(2.78 == node2.getData());
    TEST_CHECK("Jimmy" == node3.getData());

    // Change the data of every node
    node1.setData(2);
    node2.setData(8.34);
    node3.setData("Paul");

    // Test if the data were chenged successfully
    TEST_CHECK(2 == node1.getData());
    TEST_CHECK(8.34 == node2.getData());
    TEST_CHECK("Paul" == node3.getData());

}


/**
 * @brief Simple test function that checks if the functionality of adding neighbors to a specific
 * graph node works as expected.
 */
void test_node_add_neighbors(void) {

    GraphNode<int> node1(1);
    std::vector<int> *computedNeighbors, correctNeighbors;

    node1.addNeighbor(2);
    node1.addNeighbor(3);
    node1.addNeighbor(4);
    
    computedNeighbors = node1.getNeighbors();
    correctNeighbors.push_back(2);
    correctNeighbors.push_back(3);
    correctNeighbors.push_back(4);

    TEST_CHECK(*computedNeighbors == correctNeighbors);

}

/**
 * @brief Simple test function that checks if the neighbors of a single graph node of integer values
 * are received successfully. It provides two simple cases of checking. One with small amount 
 * of neighbors, and one with a large amount of neighbors.
*/
void test_node_integer_neighbors_fetching(void) {

    // --- SMALL AMOUNT OF NEIGHBORS ---

    GraphNode<int> node1(1);
    std::vector<int> *computedNeighbors, correctNeighbors;
    
    // Add some neighbors
    node1.addNeighbor(2);
    node1.addNeighbor(3);
    node1.addNeighbor(4);
    node1.addNeighbor(2);
    
    // Real neighbors
    correctNeighbors.push_back(2);
    correctNeighbors.push_back(3);
    correctNeighbors.push_back(4);

    computedNeighbors = node1.getNeighbors();
    TEST_CHECK(*computedNeighbors == correctNeighbors);

    // --- LARGE AMOUNT OF NEIGHBORS ---

    GraphNode<int> node2(1);
    computedNeighbors->clear(); correctNeighbors.clear();

    for (unsigned int i = 2; i < 10000; i++) {
        node2.addNeighbor(i);
        correctNeighbors.push_back(i);
    }

    computedNeighbors = node2.getNeighbors();
    TEST_CHECK(*computedNeighbors == correctNeighbors);
    TEST_CHECK(node1.getNeighbors() != node2.getNeighbors());

}

/**
 * @brief This functions checks if the neighbors of a single graph node of string values are received
 * successfully. It provides an equality checking and an non-equality checking.
*/
void test_node_string_neighbors_fetching(void) {

    GraphNode<std::string> node1("Mark");
    std::vector<std::string> *computedNeighbors, correctNeighbors, wrongNeighbors;

    // Add neighbors
    node1.addNeighbor("Peter");
    node1.addNeighbor("Mary");
    node1.addNeighbor("Michael");
    node1.addNeighbor("Mary");

    // Real and wrong ones
    correctNeighbors.push_back("Peter");
    correctNeighbors.push_back("Mary");
    correctNeighbors.push_back("Michael");
 
    wrongNeighbors.push_back("Sally"); 
    wrongNeighbors.push_back("Harry");
    wrongNeighbors.push_back("Bob");
    
    computedNeighbors = node1.getNeighbors();
    TEST_CHECK(*computedNeighbors == correctNeighbors);
    TEST_CHECK(*computedNeighbors != wrongNeighbors);

}

/**
 * @brief Test function that checks whether the deletion of a specific neighbor from a graph node
 * works fine.
 */
void test_node_remove_neighbor(void) {

    GraphNode<int> node1(1);
    std::vector<int> *computedNeighbors, correctNeighbors;

    node1.addNeighbor(2);
    node1.addNeighbor(3);
    node1.addNeighbor(4);
    
    node1.removeNeighbor(3);

    correctNeighbors.push_back(2);
    correctNeighbors.push_back(4);

    computedNeighbors = node1.getNeighbors();

    TEST_CHECK(*computedNeighbors == correctNeighbors);

}

TEST_LIST = {
    { "node_data_fetching", test_node_data_fetching },
    { "node_add_neighbors", test_node_add_neighbors },
    { "neighbors_fetching_integers", test_node_integer_neighbors_fetching },
    { "neighbors_fetching_strings", test_node_string_neighbors_fetching },
    { "neighbors_remove_neighbors", test_node_remove_neighbor },
    { NULL, NULL }
};
