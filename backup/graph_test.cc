#include <iostream>
#include "../include/Graph/graph.h"
#include "../include/acutest.h"

/**
 * @brief Test function that checks the initialization of a graph.
 * It ensures that a graph with a specified number of nodes can be
 * initialized successfully without returning a null pointer.
 */
void test_graph_initialization(void) {

    // Initialize a graph with 10 nodes and check if its initialization worked successfully
    Graph<int>* graph1 = new Graph<int>(10);
    TEST_CHECK(graph1 != NULL);

    delete graph1;
}

/**
 * @brief Test function that checks setting and fetching data for graph nodes.
 * It inserts sequential data into the nodes of the graph and verifies
 * if the data can be correctly retrieved for each node.
 */
void test_graph_node_data_setting_and_fetching(void) {

    // Initialize a graph with 10 nodes
    Graph<int> graph1(10);

    // Insert some data to the graph nodes
    for (unsigned int i = 0; i < graph1.getNodesCount(); i++) {
        graph1.setNodeData(i, i + 1);
    }

    // Check whether the data are being fetched successfully
    for (unsigned int i = 0; i < 10; i++) {
        TEST_CHECK(graph1.getNodeData(i) == (int)i + 1);
    }
}

/**
 * @brief Test function that checks the connectivity of graph nodes.
 * It connects several nodes in the graph and verifies whether the correct neighbors
 * are retrieved for each node. It tests various connections, ensuring no self-connections
 * or incorrect neighbors are returned.
 */
void test_graph_nodes_connectivity(void) {

    Graph<int> graph1(10);

    // Insert some data to the graph nodes
    for (unsigned int i = 0; i < graph1.getNodesCount(); i++) {
        graph1.setNodeData(i, i + 1);
    }

    // Connect nodes within the graph
    for (unsigned int i = 1; i < graph1.getNodesCount() - 3; i++) {
        graph1.connectNodes(1, i + 1);
        graph1.connectNodes(2, i + 2);
        graph1.connectNodes(3, i - 1);
    }

    // Check the neighbors of node 0
    std::vector<int> node0_CorrectNeighbors;
    for (unsigned int i = 2; i <= 7; i++) node0_CorrectNeighbors.push_back(i);
    std::vector<int>* node0_ComputedNeighbors = graph1.getNodeNeighbors(0);
    TEST_CHECK(*node0_ComputedNeighbors == node0_CorrectNeighbors);
    
    // Check the neighbors of node 1
    std::vector<int> node1_CorrectNeighbors;
    for (unsigned int i = 3; i <= 8; i++) node1_CorrectNeighbors.push_back(i);
    std::vector<int>* node1_ComputedNeighbors = graph1.getNodeNeighbors(1);
    TEST_CHECK(*node1_ComputedNeighbors == node1_CorrectNeighbors);
    
    // Check the neighbors of node 2, ensuring no self-connections (node 3 is excluded)
    std::vector<int> node2_CorrectNeighbors;
    node2_CorrectNeighbors.push_back(1);
    node2_CorrectNeighbors.push_back(2);
    node2_CorrectNeighbors.push_back(4);
    node2_CorrectNeighbors.push_back(5);
    std::vector<int>* node2_ComputedNeighbors = graph1.getNodeNeighbors(2);
    TEST_CHECK(*node2_ComputedNeighbors == node2_CorrectNeighbors);
}

TEST_LIST = {
    { "graph_initialization_test", test_graph_initialization },
    { "graph_setting_and_fetching_data_test", test_graph_node_data_setting_and_fetching },
    { "graph_nodes_connectivity_test", test_graph_nodes_connectivity },
    { NULL, NULL }
};
