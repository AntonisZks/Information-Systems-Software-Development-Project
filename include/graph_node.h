#ifndef GRAPH_NODE_H
#define GRAPH_NODE_H

#include <iostream>
#include <algorithm> 
#include <vector>
#include <set>

/**
 * @brief Struct that represents a node in a graph. Each node contains data of a specified type 
 * and a list of its neighbors within the graph. This struct is designed to support a variety
 * of data types for flexible graph applications.
 * 
 * @param node_t The data type for the node's data
 */
template <typename node_t> struct GraphNode {

private:
  node_t data;
  std::vector<node_t> neighbors;
  int index;

public:

  /**
   * @brief Default constructor for GraphNode. Creates a graph node with uninitialized data. Primarily for 
   * internal use or as a placeholder.
   */
  GraphNode(void) {}

  /**
   * @brief Constructs a GraphNode with specified data.
   * 
   * @param data_ The data to be stored in the node
   */
  GraphNode(node_t data_);

  /**
   * @brief Destructor for GraphNode. Cleans up node resources if necessary.
   */
  ~GraphNode(void) {}

  /**
   * @brief Sets the data of the node.
   * 
   * @param data The data to assign to this node
   */
  inline void setData(const node_t& data) { this->data = data; }

  /**
   * @brief Adds a neighbor to the node, if it does not already exist in the neighbors list.
   * 
   * @param data The data of the neighbor to add
   */
  void addNeighbor(node_t data);

  /**
   * @brief Removes a specified neighbor from the node's neighbor list.
   * 
   * @param data The data of the neighbor to remove
   */
  void removeNeighbor(node_t data);

  /**
   * @brief Retrieves the data stored in this node.
   * 
   * @return The data contained in the node
   */
  inline node_t getData(void) const { return this->data; }

  /**
   * @brief Retrieves the list of neighbors for this node.
   * 
   * @return A pointer to a vector containing the neighbors' data
   */
  inline std::vector<node_t>* getNeighborsVector(void) { return &this->neighbors; }

  /**
   * @brief Retrieves the set of neighbors for this node.
   * 
   * @return A set containing the neighbors' data
   * 
   * TODO: This functions should be replaced with a more efficient implementation
   */
  inline std::set<node_t> getNeighborsSet(void) { 
    return std::set<node_t>(this->neighbors.begin(), this->neighbors.end()); 
  }

  /**
   * @brief Clears out all the neighbors of the node
  */
  inline void clearNeighbors(void) { this->neighbors.clear(); }

  /**
   * @brief Less-than operator to allow ordering of nodes by data.
   * 
   * @param other The other GraphNode to compare against
   * @return True if this node's data is less than the other node's data
   */
  bool operator<(const GraphNode<node_t> other) const { return this->data < other.data; }

  /**
   * @brief Equality operator to compare nodes based on their data.
   * 
   * @param other The other GraphNode to compare against
   * @return True if this node's data is equal to the other node's data
   */
  bool operator==(const GraphNode& other) const { return this->data == other.data; }

  /**
   * @brief Sets the index of the node.
   * 
   * @param idx The index to assign to this node
   */
  inline void setIndex(int idx) { this->index = idx; }

  /**
   * @brief Retrieves the index of this node.
   * 
   * @return The index of the node
   */
  inline int getIndex() const { return index; }
  
};

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

#endif /* GRAPH_NODE_H */
