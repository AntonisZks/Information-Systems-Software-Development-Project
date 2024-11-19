#ifndef GRAPH_H
#define GRAPH_H

#include <iostream>
#include <vector>
#include <set>
#include "graph_node.h"
#include <numeric>  
#include <random>
#include "../../graphics.h"

/**
 * @brief Implements a directed, unweighted graph data structure. Each node in the graph contains 
 * data of any specified type and a list of its neighbors.
 * 
 * @param graph_t The data type stored in each node of the graph
 */
template <typename graph_t> class Graph {

private:
  GraphNode<graph_t>* nodes;
  std::set<graph_t> nodesSet;
  unsigned int nodesCount;

public:

  /**
   * @brief Default Constructor of the Grpah. Exists to avoid errors.
   */
  Graph(void) : nodesCount(0) {
    this->nodes = new GraphNode<graph_t>[0];
  }

  /**
   * @brief Constructs a graph with a specified number of nodes and allocates memory for them.
   * 
   * @param nodesCount_ Number of nodes in the graph
   */
  Graph(unsigned int nodesCount_) : nodesCount(nodesCount_) {

    this->nodes = new GraphNode<graph_t>[nodesCount_];
    for (unsigned int i = 0; i < nodesCount_; i++) {
      this->nodes[i].setIndex(i);
    }

    for (unsigned int i = 0; i < this->nodesCount; i++) {
      this->nodesSet.insert(this->nodes[i].getData());
    }

  }

  /**
   * @brief Destructor for the Graph. Releases the memory allocated for nodes.
   */
  ~Graph(void) {
    delete[] this->nodes;
  }

  /**
   * @brief Sets the data of a specific node by its index.
   * 
   * @param index Index of the node
   * @param data Data to assign to the node
   */
  void setNodeData(unsigned int index, const graph_t& data) {
    this->nodes[index].setData(data);
  }

  /**
   * @brief Sets the nodes count of the graph. Deletes the current memory
   * allocated for the nodes array and creates a new one with the updates
   * number of nodes.
   * 
   * @param nodesCount the number of nodes.
   */
  void setNodesCount(const unsigned int nodesCount) {
    this->nodesCount = nodesCount;
    delete [] this->nodes;
    this->nodes = new GraphNode<graph_t>[nodesCount];
    for (unsigned int i = 0; i < nodesCount; i++) {
      this->nodes[i].setIndex(i);
    }
  }

  /**
   * @brief Retrieves the data from all nodes in the graph.
   * 
   * @return set of data from all nodes
  */
  std::set<graph_t> getNodesSet(void) const {
    return this->nodesSet;
  }

  /**
   * @brief Retrieves the data from a specific node by its index.
   * 
   * @param index Index of the node
   * @return Data stored in the node
   */
  graph_t getNodeData(const unsigned int index) const {
    return this->nodes[index].getData();
  }

  /**
   * @brief Retrieves a pointer to a node at a specified index.
   * 
   * @param index Index of the node
   * @return Pointer to the node if index is valid, otherwise nullptr
   */
  GraphNode<graph_t>* getNode(const unsigned int index) const {

    if (index >= this->nodesCount) {
      return nullptr;
    }
    return &this->nodes[index];

  }

  /**
   * @brief Finds and returns a pointer to the first node that contains the specified data.
   * 
   * @param data Data to search for in the nodes
   * @return Pointer to the node containing the data, or nullptr if not found
   */
  GraphNode<graph_t>* getNodeWithData(const graph_t data) const {

    for (unsigned int i = 0; i < this->nodesCount; i++) {
      if (this->nodes[i].getData() == data) {
        return &this->nodes[i];
      }
    }
    return nullptr;

  }

  /**
   * @brief Retrieves the neighbors of a node by its index.
   * 
   * @param index Index of the node
   * @return Pointer to a vector of the node's neighbors, or nullptr if the index is invalid
   */
  std::vector<graph_t>* getNodeNeighbors(const unsigned int index) const {

    if (index >= this->nodesCount) {
      return nullptr;
    }
    return this->nodes[index].getNeighbors();

  }

  /**
   * @brief Retrieves the total number of nodes in the graph.
   * 
   * @return Total node count
   */
  unsigned int getNodesCount(void) const {
    return this->nodesCount;
  }

  /**
   * @brief Connects two nodes in the graph by their data, creating a directed edge from the first 
   * node to the second.
   * 
   * @param firstNodeData Data of the starting node
   * @param secondNodeData Data of the destination node
   * 
   * @return True if the connection was successful, false otherwise
   */
  bool connectNodesByData(const graph_t firstNodeData, const graph_t secondNodeData) {

    // Check if the data re not the same
    if (firstNodeData == secondNodeData) {
      return false;
    }

    GraphNode<graph_t>* firstNode = getNodeWithData(firstNodeData);
    GraphNode<graph_t>* secondNode = getNodeWithData(secondNodeData);

    if (!firstNode || !secondNode) {
      return false;
    }

    firstNode->addNeighbor(secondNode->getData());
    return true;

  }

  /**
   * @brief Connects two nodes by their indices, creating a directed edge from the first index to 
   * he second.
   * 
   * @param index1 Index of the starting node
   * @param index2 Index of the destination node
   * 
   * @return True if the connection was successful, false otherwise
   */
  bool connectNodesByIndex(const unsigned int index1, const unsigned int index2) {

    if (index1 >= this->nodesCount || index2 >= this->nodesCount || index1 == index2) {
      return false;
    }

    this->nodes[index1].addNeighbor(this->nodes[index2].getData());
    return true;

  }

  /**
   * @brief Disconnects two nodes by removing the second node's data from the neighbors of the first.
   * 
   * @param firstNodeData Data of the node to modify
   * @param secondNodeData Data of the neighbor to remove
   * 
   * @return True if the disconnection was successful, false otherwise
   */
  bool disconnectNodesByData(const graph_t firstNodeData, const graph_t secondNodeData) {

    GraphNode<graph_t>* firstNode = getNodeWithData(firstNodeData);
    if (!firstNode) {
      return false;
    }

    firstNode->removeNeighbor(secondNodeData);
    return true;
  }

};

/**
 * @brief Overloads the << operator to print the graph. Each node's data and its neighbors are printed.
 * 
 * @param graph_t Data type contained in the graph
 * @param output Output stream for printing
 * @param graph Graph to be printed
 * 
 * @return Reference to the output stream
 */
template <typename graph_t> 
std::ostream& operator<<(std::ostream& output, const Graph<graph_t>& graph) {
  if (graph.getNodesCount() == 0) {
    output << "Graph Empty";
    return output;
  }

  for (unsigned int i = 0; i < graph.getNodesCount(); i++) {
    std::vector<graph_t>* neighbors = graph.getNodeNeighbors(i);
    output << graph.getNodeData(i) << ": [";
    for (unsigned int j = 0; j < neighbors->size(); j++) {
      output << neighbors->at(j);
      if (j < neighbors->size() - 1) {
        output << ", ";
      }
    }
    output << "]";
    if (i < graph.getNodesCount() - 1) {
      output << std::endl;
    }    
  } 
  return output;

}


/**
 * @brief Finds the medoid node in the graph using a sample of nodes.
 *
 * The medoid is the node with the minimum average distance to all other nodes in the sample.
 * This function uses Euclidean distance to calculate the distances between nodes.
 *
 * @tparam T The type of data stored in the graph nodes.
 * @param graph The graph from which to find the medoid.
 * @param sample_size The number of nodes to sample from the graph for medoid calculation. Default is 100.
 * @return The medoid node of the sampled nodes.
 */
template<typename T>
GraphNode<T> findMedoid(const Graph<T>& graph, int sample_size = 100) {

    const int node_count = graph.getNodesCount();  // Get the total number of nodes in the graph
    sample_size = std::min(sample_size, node_count);  // Ensure the sample size doesn’t exceed the total node count

    // Create a list of all node indices and shuffle it to select a random subset
    std::vector<int> sampled_indices(node_count);  // Vector to hold indices from 0 to node_count - 1
    std::iota(sampled_indices.begin(), sampled_indices.end(), 0);  // Fill the vector with sequential numbers (0, 1, ..., node_count - 1)
    
    // Randomly shuffle the indices to select a random subset of nodes
    std::shuffle(sampled_indices.begin(), sampled_indices.end(), std::mt19937{std::random_device{}()});
    
    // Resize the vector to contain only `sample_size` elements, representing the sampled subset of nodes
    sampled_indices.resize(sample_size);

    // Initialize a distance matrix for the sampled nodes
    // This matrix will store the pairwise distances between each pair of sampled nodes
    std::vector<std::vector<float>> distance_matrix(sample_size, std::vector<float>(sample_size, 0.0));

    // Compute pairwise distances between each pair of sampled nodes
    for (int i = 0; i < sample_size; ++i) {

      double percentage = (double)(100*i)/sample_size;
      printProgressBar(percentage, "Computing the mediod point:     ");

      for (int j = i + 1; j < sample_size; ++j) {
        // Calculate the Euclidean distance between nodes `i` and `j` in the sampled subset
        float dist = euclideanDistance(graph.getNode(sampled_indices[i])->getData(), graph.getNode(sampled_indices[j])->getData());
        
        // Store the computed distance in both `distance_matrix[i][j]` and `distance_matrix[j][i]`
        // This leverages the symmetry of the matrix, as distance from i to j equals distance from j to i
        distance_matrix[i][j] = dist;
        distance_matrix[j][i] = dist;
      }
    }

    // Find the medoid node among the sampled nodes by calculating the average distance for each one
    float min_average_distance = std::numeric_limits<float>::max();  // Initialize with a large value to find minimum
    GraphNode<T>* medoid_node = nullptr;  // Pointer to the node with the minimum average distance (the medoid)

    // Loop through each sampled node to calculate its average distance to other sampled nodes
    for (int i = 0; i < sample_size; ++i) {
      // Calculate the total distance from node `i` to all other nodes in the sample
      float total_distance = std::accumulate(distance_matrix[i].begin(), distance_matrix[i].end(), 0.0f);
      
      // Compute the average distance by dividing the total distance by the number of other nodes (sample_size - 1)
      float average_distance = total_distance / (sample_size - 1);

      // Check if this node has a smaller average distance than the smallest found so far
      if (average_distance < min_average_distance) {
        // Update the minimum average distance and set this node as the current medoid candidate
        min_average_distance = average_distance;
        medoid_node = graph.getNode(sampled_indices[i]);
      }
    }

    printProgressBar(100, "Computing the mediod point:     ");

    return *medoid_node;

}


#endif /* GRAPH_H */
