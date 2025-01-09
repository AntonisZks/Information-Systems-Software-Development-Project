#include "../../include/graph.h"
#include "../../include/DataVector.h"
#include "../../include/BQDataVectors.h"

/**
 * @brief Default Constructor of the Grpah. Exists to avoid errors.
 */
template <typename graph_t> Graph<graph_t>::Graph(void) : nodesCount(0) {
  this->nodes = new GraphNode<graph_t>[0];
}

/**
 * @brief Constructs a graph with a specified number of nodes and allocates memory for them.
 * 
 * @param nodesCount_ Number of nodes in the graph
 */
template <typename graph_t> Graph<graph_t>::Graph(unsigned int nodesCount_) : nodesCount(nodesCount_) {

  this->nodes = new GraphNode<graph_t>[nodesCount_];
  for (unsigned int i = 0; i < nodesCount_; i++) {
    this->nodes[i].setIndex(i);
  }

  for (unsigned int i = 0; i < this->nodesCount; i++) {
    this->nodesSet.insert(this->nodes[i].getData());
    this->nodesVector.push_back(this->nodes[i]);
  }

}

/**
 * @brief Destructor for the Graph. Releases the memory allocated for nodes.
 */
template <typename graph_t> Graph<graph_t>::~Graph(void) {
  delete[] this->nodes;
}

/**
 * @brief Sets the data of a specific node by its index.
 * 
 * @param index Index of the node
 * @param data Data to assign to the node
 */
template <typename graph_t> void Graph<graph_t>::setNodeData(unsigned int index, const graph_t& data) {
  this->nodes[index].setData(data);
  this->nodesSet.insert(data);
  this->nodesVector[index].setData(data);
}

/**
 * @brief Sets the nodes count of the graph. Deletes the current memory
 * allocated for the nodes array and creates a new one with the updates
 * number of nodes.
 * 
 * @param nodesCount the number of nodes.
 */
template <typename graph_t> void Graph<graph_t>::setNodesCount(const unsigned int nodesCount) {

  this->nodesCount = nodesCount;
  delete [] this->nodes;

  this->nodesSet.clear();
  this->nodesVector.clear();

  this->nodes = new GraphNode<graph_t>[nodesCount];
  for (unsigned int i = 0; i < nodesCount; i++) {
    this->nodes[i].setIndex(i);
    this->nodesVector.push_back(this->nodes[i]);
  }

}

/**
 * @brief Retrieves the data from a specific node by its index.
 * 
 * @param index Index of the node
 * @return Data stored in the node
 */
template <typename graph_t> graph_t Graph<graph_t>::getNodeData(const unsigned int index) const {
  return this->nodes[index].getData();
}

/**
 * @brief Retrieves a pointer to a node at a specified index.
 * 
 * @param index Index of the node
 * @return Pointer to the node if index is valid, otherwise nullptr
 */
template <typename graph_t> GraphNode<graph_t>* Graph<graph_t>::getNode(const unsigned int index) const {

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
template <typename graph_t> GraphNode<graph_t>* Graph<graph_t>::getNodeWithData(const graph_t data) const {

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
template <typename graph_t> std::vector<graph_t>* Graph<graph_t>::getNodeNeighbors(const unsigned int index) const {

  if (index >= this->nodesCount) {
    return nullptr;
  }
  return this->nodes[index].getNeighborsVector();

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
template <typename graph_t> bool Graph<graph_t>::connectNodesByData(const graph_t firstNodeData, const graph_t secondNodeData) {

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
template <typename graph_t> bool Graph<graph_t>::connectNodesByIndex(const unsigned int index1, const unsigned int index2) {

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
template <typename graph_t> bool Graph<graph_t>::disconnectNodesByData(const graph_t firstNodeData, const graph_t secondNodeData) {

  GraphNode<graph_t>* firstNode = getNodeWithData(firstNodeData);
  if (!firstNode) {
    return false;
  }

  firstNode->removeNeighbor(secondNodeData);
  return true;
}

template class Graph<int>;
template class Graph<DataVector<float>>;
template class Graph<BaseDataVector<float>>;
