#include "../include/graph_node.h"

/**
 * @brief Constructs a GraphNode with specified data.
 * 
 * @param data_ The data to be stored in the node
 */
template <typename node_t> GraphNode<node_t>::GraphNode(node_t data_): data(data_) {}

/**
 * @brief Adds a neighbor to the node, if it does not already exist in the neighbors list.
 * 
 * @param data The data of the neighbor to add
 */
template <typename node_t> void GraphNode<node_t>::addNeighbor(node_t data) {

  // Add data to neighbors only if it is not already present.
  if (std::count(this->neighbors.begin(), this->neighbors.end(), data) == 0) {
    this->neighbors.push_back(data);
  }

}

/**
 * @brief Removes a specified neighbor from the node's neighbor list.
 * 
 * @param data The data of the neighbor to remove
 */
template <typename node_t> void GraphNode<node_t>::removeNeighbor(node_t data) {

  // Find and remove the neighbor data from the neighbors list.
  auto it = std::find(this->neighbors.begin(), this->neighbors.end(), data);
  if (it != this->neighbors.end()) {
    this->neighbors.erase(it);
  }

}

/**
 * @brief Overloads the << operator to output the node's data to an output stream.
 * 
 * @param node_t The data type stored in the node
 * @param out The output stream to write to
 * @param node The GraphNode whose data will be output
 * 
 * @return The output stream with node data appended
 */
template <typename node_t> 
std::ostream& operator<<(std::ostream& out, const GraphNode<node_t> node) {

  out << node.getData();
  return out;

}
