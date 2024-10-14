#ifndef GRAPH_NODE_H
#define GRAPH_NODE_H

#include <iostream>
#include <algorithm> 
#include <vector>

/**
 * @brief Struct that represents the node of the Graph Data Structure. It is part of the
 * graph implementation, and consists of some data and its neighbor nodes. Data could be any
 * type the user would like, and the neighbors is a vector containing the data of every neighbor 
 * node inside the graph.
 */
template <typename node_t> struct GraphNode {

private:
  node_t data;
  std::vector<node_t> neighbors;

public:

  /**
   * @brief Default constructor of the graph node. Exists just to avoid errors.
   */
  GraphNode(void) {}

  /**
   * @brief Constructor of the graph node. Sets the data of the node.
   */
  GraphNode(node_t data) { this->data = data; }

  /**
   * @brief Destructor of the graph node. Exists just to avoid errors.
   */
  ~GraphNode(void) {}

  void setData(node_t data) {
    this->data = data;
  }

  /**
   * @brief Adds a neighbor to the node. Specifically it adds the data of the neighbor 
   * at the back of the vector structure containing the neighbors.
   * 
   * @param data the data of the neighbor
   */
  void addNeighbor(node_t data) {
    this->neighbors.push_back(data);
  }

  /**
   * @brief Removes the specific data from the specific node's neighbors.
   * 
   * @param data the data to remove
   */
  void removeNeighbor(node_t data) {
    auto it = std::find(this->neighbors.begin(), this->neighbors.end(), data);
    if (it != this->neighbors.end()) {
      this->neighbors.erase(it);
    }
  }

  /**
   * @brief Returns the data of the specific node.
   * 
   * @return the node's data
   */
  node_t getData(void) {
    return this->data;
  }

  /**
   * @brief Returns the neighbors of the node. Specifically it returns a vector containing
   * the data of each neighbor node.
   * 
   * @return a vector containing the neighbors of the node
   */
  std::vector<node_t> getNeighbors(void) {
    return this->neighbors;
  }

};

#endif /* GRAPH_NODE_H */
