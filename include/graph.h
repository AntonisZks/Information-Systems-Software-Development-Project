#ifndef GRAPH_H
#define GRAPH_H

#include <iostream>
#include <vector>
#include <set>
#include "graph_node.h"
#include <numeric>  
#include <random>
#include "graphics.h"

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
  Graph(void);

  /**
   * @brief Constructs a graph with a specified number of nodes and allocates memory for them.
   * 
   * @param nodesCount_ Number of nodes in the graph
   */
  Graph(unsigned int nodesCount_);

  /**
   * @brief Destructor for the Graph. Releases the memory allocated for nodes.
   */
  ~Graph(void);

  /**
   * @brief Sets the data of a specific node by its index.
   * 
   * @param index Index of the node
   * @param data Data to assign to the node
   */
  void setNodeData(unsigned int index, const graph_t& data);

  /**
   * @brief Sets the nodes count of the graph. Deletes the current memory
   * allocated for the nodes array and creates a new one with the updates
   * number of nodes.
   * 
   * @param nodesCount the number of nodes.
   */
  void setNodesCount(const unsigned int nodesCount);

  /**
   * @brief Retrieves the data from all nodes in the graph.
   * 
   * @return set of data from all nodes
  */
  std::set<graph_t> getNodesSet(void) const { return this->nodesSet; }

  /**
   * @brief Retrieves the data from a specific node by its index.
   * 
   * @param index Index of the node
   * @return Data stored in the node
   */
  graph_t getNodeData(const unsigned int index) const;

  /**
   * @brief Retrieves a pointer to a node at a specified index.
   * 
   * @param index Index of the node
   * @return Pointer to the node if index is valid, otherwise nullptr
   */
  GraphNode<graph_t>* getNode(const unsigned int index) const;

  /**
   * @brief Finds and returns a pointer to the first node that contains the specified data.
   * 
   * @param data Data to search for in the nodes
   * @return Pointer to the node containing the data, or nullptr if not found
   */
  GraphNode<graph_t>* getNodeWithData(const graph_t data) const;

  /**
   * @brief Retrieves the neighbors of a node by its index.
   * 
   * @param index Index of the node
   * @return Pointer to a vector of the node's neighbors, or nullptr if the index is invalid
   */
  std::vector<graph_t>* getNodeNeighbors(const unsigned int index) const;

  /**
   * @brief Retrieves the total number of nodes in the graph.
   * 
   * @return Total node count
   */
  unsigned int getNodesCount(void) const { return this->nodesCount; }

  /**
   * @brief Connects two nodes in the graph by their data, creating a directed edge from the first 
   * node to the second.
   * 
   * @param firstNodeData Data of the starting node
   * @param secondNodeData Data of the destination node
   * 
   * @return True if the connection was successful, false otherwise
   */
  bool connectNodesByData(const graph_t firstNodeData, const graph_t secondNodeData);

  /**
   * @brief Connects two nodes by their indices, creating a directed edge from the first index to 
   * he second.
   * 
   * @param index1 Index of the starting node
   * @param index2 Index of the destination node
   * 
   * @return True if the connection was successful, false otherwise
   */
  bool connectNodesByIndex(const unsigned int index1, const unsigned int index2);

  /**
   * @brief Disconnects two nodes by removing the second node's data from the neighbors of the first.
   * 
   * @param firstNodeData Data of the node to modify
   * @param secondNodeData Data of the neighbor to remove
   * 
   * @return True if the disconnection was successful, false otherwise
   */
  bool disconnectNodesByData(const graph_t firstNodeData, const graph_t secondNodeData);

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
template <typename graph_t> std::ostream& operator<<(std::ostream& output, const Graph<graph_t>& graph) {

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

#endif /* GRAPH_H */
