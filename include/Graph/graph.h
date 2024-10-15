#ifndef GRAPH_H
#define GRAPH_H

#include <iostream>
#include <vector>
#include "graph_node.h"


/**
 * @brief Implementation of the Graph Data Structure. This class implements the graph of the application.
 * Implementation is based on the idea of a directed and unweighted graph data structre. It receives data
 * of any type.
 */
template <typename graph_t> class Graph {

private:
  GraphNode<graph_t>* nodes;
  unsigned int nodesCount;

public:

  /**
   * @brief Constructor of the Graph. Here all the logic is being initialized. Receives and stores the 
   * number of nodes inside the graph, and allocates memory for the nodes array.
   */
  Graph(unsigned int nodesCount_): nodesCount(nodesCount_) {
    this->nodes = new GraphNode<graph_t>[nodesCount_];
  }

  /**
   * @brief Destructor of the Graph.Here all the memory allocated for the nodes is being deleted.
   */
  ~Graph(void) {
    delete [] this->nodes;
  }

  /**
   * @brief Sets the data of a specific node inside the graph. It receives the index of the node inside the
   * array and sets the data of that node.
   * 
   * @param index the index of the node inside the array
   * @param data the data to be set
   */
  void setNodeData(unsigned int index, const graph_t data) {
    GraphNode<graph_t>* currentNode = &this->nodes[index];
    currentNode->setData(data);
  }

  /**
   * @brief Returns the data of the node located at the specific given index inside the array.
   * 
   * @param index the index of the node inside the array
   * @return the data of the node
   */
  graph_t getNodeData(const unsigned int index) const {
    GraphNode<graph_t> node = this->nodes[index];
    return node.getData();
  }

  /**
   * @brief Returns the node itself located at the specific given index inside the array.
   * 
   * @param index the index of the node inside the array
   * @return the node at the given index
   */
  GraphNode<graph_t>* getNode(const unsigned int index) const {

    // Check if the given index is valid
    if (index > this->nodesCount - 1 || index < 0) {
      return nullptr;
    }

    GraphNode<graph_t>* node = &this->nodes[index];
    return node;
  }

  /**
   * @brief Returns the neighbors of a node. It finds the node containing the given data and returns its
   * neighbors inside the graph.
   * 
   * @param index the index of the node inside the array
   * @return a vector containing the neighbors of the node
   */
  std::vector<graph_t>* getNodeNeighbors(const unsigned int index) const {

    // Check if the given index is valid
    if (index > this->nodesCount - 1 || index < 0) {
      return nullptr;
    }

    GraphNode<graph_t>* node = this->getNode(index);
    std::vector<graph_t> neighbors = node->getNeighbors();

    return neighbors;
    
  }

  /**
   * @brief Returns the number of nodes inside the graph
   * 
   * @return the number of nodes
   */
  unsigned int getNodesCount(void) const {
    return this->nodesCount;
  }

  /**
   * @brief Connects two nodes inside the graph. Specifically it locates the nodes containing the given
   * data and creates a directed edge starting from the first node going to the second node.
   * 
   * @param firstNodeData the data of the first node
   * @param secondNodeData the data of the second node
   * 
   * @returns true if the connection worked successfully, false otherwise
   */
  bool connectNodes(const graph_t firstNodeData, const graph_t secondNodeData) {
    GraphNode<graph_t>* firstNode = nullptr;
    GraphNode<graph_t>* secondNode = nullptr;
    
    // Find the two nodes
    for (unsigned int i = 0; i < this->nodesCount; i++) {
      GraphNode<graph_t>* currentNode = &this->nodes[i];
      if (currentNode->getData() == firstNodeData) {
        firstNode = currentNode;
      } 
      else if (currentNode->getData() == secondNodeData) {
        secondNode = currentNode;
      }
    }

    // Check if nodes were found
    if (firstNode == nullptr || secondNode == nullptr) {
      return false;
    }
    
    // Connect the nodes
    firstNode->addNeighbor(secondNode->getData());
    std::vector<graph_t> neighbors = this->nodes[0].getNeighbors();
    
    return true;
  }

  /**
   * @brief Disconnects two nodes inside the graph. Specifically it locates the node containing the
   * given firstNodeData, and if the node exists it removes the secondNodeData from its neighbors,
   * if the data exist. If the process worked as expected it returns true, otherwise false.
   */
  bool disconnectNodes(const graph_t firstNodeData, const graph_t secondNodeData) {
    GraphNode<graph_t>* firstNode = nullptr;

    // Get the node of the first data
    for (unsigned int i = 0; i < this->nodesCount; i++) {
      GraphNode<graph_t>* currentNode = &this->nodes[i];
      if (currentNode->getData() == firstNodeData) {
        firstNode = currentNode;
      }
    }

    // Check if the node was found
    if (firstNode == nullptr) {
      return false;
    }

    // Remove the given data of the second node from the first node's neighbors
    firstNode->removeNeighbor(secondNodeData);

    return true;

  }

};

/**
 * @brief Helper function that gives a nice printing of a graph. Specifically the operator << is being
 * overloaded in order to print any graph using std::cout.
 * 
 * @param output the output stream
 * @param graph the graph to print
 * 
 * @return the output
 */
template <typename graph_t> std::ostream& operator<<(std::ostream& output, const Graph<graph_t>& graph) {
  for (unsigned int i = 0; i < graph.getNodesCount(); i++) {
    std::vector<graph_t> neighbors = graph.getNodeNeighbors(i);
    output << graph.getNodeData(i) << ": ";

    output << "[";
    for (unsigned int j = 0; j < neighbors.size(); j++) {
      output << neighbors[j];
      if (j < neighbors.size() - 1) { 
        output << ", ";
      }
    }
    output << "]";
    if (i < graph.getNodesCount()-1) {
      output << std::endl;
    }    
  } 

  return output;
}

#endif /* GRAPH_H */
