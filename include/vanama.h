#ifndef VAMANA_H
#define VAMANA_H

#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <map>
#include <set>
#include <algorithm>
#include <random>
#include "Graph/graph.h"
#include "GreedySearch.h"

// Type Alias
typedef GraphNode<DataVector<float>> GreedySearchNode;
typedef Graph<DataVector<float>> GreedySearchGraph;
typedef std::set<GreedySearchNode> GreedySearchNodeSet;
#include <algorithm>
#include <random>
#include "Graph/graph.h"
#include "GreedySearch.h"

// Type Alias
typedef GraphNode<DataVector<float>> GreedySearchNode;
typedef Graph<DataVector<float>> GreedySearchGraph;
typedef std::set<GreedySearchNode> GreedySearchNodeSet;

// DECLARATIONS

/**
 * @brief Prints the contents of a set, where each element is a node with data.
 * 
 * @param set_t The type of elements within the set
 * @param set The set to print
 */
template <typename set_t> static void print_set(const std::set<set_t> set) {

  // Print the given set
  std::cout << "{";
  for (unsigned int i = 0; i < set.size(); i++) {
    auto it = set.begin();
    std::advance(it, i);
    set_t currentItem = *it;
    std::cout << currentItem.getData() << ", ";
  }
  std::cout << "}" << std::endl;

}

/**
 * @brief Generates a set of unique random indices, excluding a specific index.
 * 
 * @param max The maximum value for random index generation
 * @param i The index to exclude from the random selection
 * @param length The number of unique random indices to generate
 * 
 * @return A set of unique random indices of the specified length, excluding index i
 */
static std::set<int> generateRandomIndeces(const unsigned int max, const unsigned int i, unsigned int length) {

  // Initialize a set for the indeces
  std::set<int> indeces;

  // Keep asigning random integer values to the set until we reach the given length
  while (indeces.size() < length) {
    unsigned int randInd = rand() % max;
    if (randInd != i) {
        indeces.insert(randInd);
    }
  }

  return indeces;

}

/**
 * @brief Fills a graph by adding edges between nodes based on random selection.
 * 
 * @param graph the graph to fill
 * @param max_edges The maximum number of edges per node
 */
template <typename graph_t>
static void fillGraphRandomly(Graph<graph_t>& graph, const unsigned int max_edges) {

  // Fill the graph randomly
  for (unsigned int i = 0; i < graph.getNodesCount(); i++) {
      
    // Generate a random range of indeces and apply those indeces as neighbors for the current node
    std::set<int> indeces = generateRandomIndeces(graph.getNodesCount(), i, max_edges);

    for (unsigned int j = 0; j < indeces.size(); j++) {
      std::set<int>::iterator it = indeces.begin();
      std::advance(it, j);
      auto currentIndex = *it;

      graph.connectNodesByIndex(i, currentIndex);
    }
      
  }

}

static std::vector<int> generateRandomPermutation(const unsigned int start, const unsigned int end) {

  std::vector<int> permutation;
  for (unsigned int i = start; i <= end; i++) {
    permutation.push_back(i);
  }

  std::random_device rd;
  std::mt19937 gen(rd());
  std::shuffle(permutation.begin(), permutation.end(), gen);

  return permutation;

}

template<typename T>
GraphNode<T> findMedoid(const Graph<T>& graph) {
    // Get the total number of nodes in the graph
    const int node_count = graph.getNodesCount();
    
    // Initialize a variable to track the minimum average distance found so far
    float min_average_distance = std::numeric_limits<float>::max();
    
    // Pointer to store the node with the minimum average distance (medoid candidate)
    GraphNode<T>* medoid_node = nullptr;

    // Iterate over all nodes to find the medoid
    // A medoid is defined as the node with the minimum sum of distances to all other nodes
    for (int i = 0; i < node_count; ++i) {
        // Get the current node as a candidate for the medoid
        GraphNode<T>* candidate_node = graph.getNode(i);
        
        // Variable to accumulate the total distance from this candidate node to all other nodes
        float total_distance = 0.0;

        // Calculate the sum of distances from this candidate node to every other node
        for (int j = 0; j < node_count; ++j) {
            // Skip distance calculation if the nodes are the same (distance to itself is 0)
            if (i != j) {
                total_distance += euclideanDistance(candidate_node->getData(), graph.getNode(j)->getData());
            }
        }

        // Compute the average distance for this candidate node
        // This is done by dividing the total distance by the number of other nodes
        float average_distance = total_distance / (node_count - 1);

        // Check if this candidate node has a smaller average distance than the current minimum
        // If so, update the minimum distance and set this node as the current medoid candidate
        if (average_distance < min_average_distance) {
            min_average_distance = average_distance;
            medoid_node = candidate_node;
        }
    }

    // Return the node with the smallest average distance, which is the medoid
    // The medoid represents the node that minimizes the total distance to all other nodes
    return *medoid_node;
}


template <typename graph_t>
void Create_Vamana_Index(Graph<graph_t>& graph, const float alpha, const unsigned int L, const unsigned int R) {

  // Initialize G to a random R-regular directed graph
  // Let s denote the medoid of the dataset P (the graph)
  // Let sigma denote a random permutation of 1, 2, ..., n
  fillGraphRandomly(graph, R);
  GraphNode<graph_t>* s = graph.getNode(0); // FIXME: s should be the mediod 
  std::vector<int> sigma = generateRandomPermutation(0, graph.getNodesCount()-1);

  for (unsigned int i = 0; i < graph.getNodesCount(); i++) {

    std::cout << "Node " << i << std::endl;

    // Denote currentNode to be the node at index sigma[i]
    GraphNode<graph_t>* currentNode = graph.getNode(sigma.at(i));
    std::pair<GreedySearchNodeSet, GreedySearchNodeSet> greedySearchResult;

    // Let [L;V] <-- GreedySearch(s, x_sigma[i], 1, L). // Here L is the candidates, and V is visited
    greedySearchResult = GreedySearch(*s, currentNode->getData(), 1, L);
    GreedySearchNodeSet candidates = greedySearchResult.first;
    GreedySearchNodeSet visited = greedySearchResult.second;

    // Run RobustPrune(sigma(i), V, alpha, R) to update out-neighbors of sigma(i)
    std::pair<std::set<GraphNode<graph_t>>, std::set<GraphNode<graph_t>>> robustResult;
    robustResult = robustPrune(graph, *currentNode, alpha, R);

    // For all point j in N_out(sigma[i]) do...
    // Here we iterate through all the neighbors of the node located at the index sigma[i]
    std::vector<graph_t>* neighbors = currentNode->getNeighbors();
    for (unsigned int j = 0; j < neighbors->size(); j++) {

      // Initialize a set to keep the neighbors of the current neighbor and the graph node at index sigma[i]
      std::set<GraphNode<graph_t>> outgoing;

      graph_t currentNeighborData = neighbors->at(j);
      GraphNode<graph_t>* currentNeighbor = graph.getNodeWithData(currentNeighborData);
      std::vector<graph_t>* neighborNeighbors = currentNeighbor->getNeighbors();

      // Fill the set with the neighbors of the current neighbor and add the graph node at index sigma[i]
      for (unsigned int k = 0; k < neighborNeighbors->size(); k++) {

        graph_t neighborNeighborData = neighborNeighbors->at(k);
        GraphNode<graph_t>* neighborNeighborNode = graph.getNodeWithData(neighborNeighborData);
        outgoing.insert(*neighborNeighborNode);
      
      }
      outgoing.insert(*currentNode);
      
      // If |N_out(j) union {sigma(i)}| > R then...
      // Here N_out(j) is the outgoing set
      if (outgoing.size() > R) {

        // Run RobustPrune(j, N_out(j) union {sigma(i)}, alpha, R) to update out-neighbors of j
        std::pair<std::set<GraphNode<graph_t>>, std::set<GraphNode<graph_t>>> robustResult2;
        robustResult2 = robustPrune(graph, *currentNeighbor, alpha, R);

      } else {
        // Update N_out(j) <-- N_out(j) union sigma(i)
        currentNeighbor->addNeighbor(currentNode->getData());
      }

    }

  }

}

#endif /* VAMANA_H */
