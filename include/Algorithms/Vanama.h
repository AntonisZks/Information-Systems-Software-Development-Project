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
#include "../DataStructures/Graph/graph.h"
#include "GreedySearch.h"
#include "RobustPrune.h"

// DECLARATIONS


/**
 * @brief Generates a set of unique random indices, excluding a specific index.
 * 
 * @param max The maximum value for random index generation
 * @param i The index to exclude from the random selection
 * @param length The number of unique random indices to generate
 * 
 * @return A set of unique random indices of the specified length, excluding index i
 */
static std::set<int> generateRandomIndices(const unsigned int max, const unsigned int i, unsigned int length) {

  // Initialize a set for the indices
  std::set<int> indices;

  static std::mt19937 generator(std::random_device{}());
  std::uniform_int_distribution<unsigned int> distribution(0, max - 1);

  // Keep assigning random integer values to the set until we reach the given length
  while (indices.size() < length) {
    unsigned int randInd = distribution(generator);
    if (randInd != i) {
      indices.insert(randInd);
    }
  }

  return indices;

}

/**
 * @brief Creates a graph from a dataset of vectors, adding edges between nodes based on random selection.
 * 
 * @param base_vectors A vector of data vectors to populate the graph nodes
 * @param max_edges The maximum number of edges per node
 * 
 * @return A graph with nodes connected by randomly assigned edges
 */
static Graph<DataVector<float>> createGraph(const std::vector<DataVector<float>>& base_vectors, unsigned int max_edges) {

  // Initialize a graph and insert all the vectors from the base vectors dataset 
  Graph<DataVector<float>> graph(base_vectors.size());
  for (unsigned int i = 0; i < graph.getNodesCount(); i++) {
    graph.setNodeData(i, base_vectors.at(i));
  }
  
  // Fill the graph randomly
  for (unsigned int i = 0; i < graph.getNodesCount(); i++) {
      
    // Generate a random range of indeces and apply those indeces as neighbors for the current node
    std::set<int> indeces = generateRandomIndices(graph.getNodesCount(), i, max_edges);

    for (unsigned int j = 0; j < indeces.size(); j++) {
      std::set<int>::iterator it = indeces.begin();
      std::advance(it, j);
      auto currentIndex = *it;

      graph.connectNodesByIndex(i, currentIndex);
    }
      
  }

  return graph;

}

/**
 * @brief Generates a random permutation of integers in a specified range. This function creates a vector 
 * containing all integers from `start` to `end` and then shuffles them randomly to produce a random permutation.
 * 
 * @param start The starting integer of the range (inclusive)
 * @param end The ending integer of the range (inclusive)
 * 
 * @return A vector containing a shuffled sequence of integers from `start` to `end`
 */
static std::vector<int> generateRandomPermutation(const unsigned int start, const unsigned int end) {
    // Initialize a vector to hold the sequence of integers from start to end
    std::vector<int> permutation;
    for (unsigned int i = start; i <= end; i++) {
        permutation.push_back(i);
    }

    // Use a more unpredictable seed
    std::mt19937 gen(std::random_device{}());  // Seed with random device for true randomness
    std::shuffle(permutation.begin(), permutation.end(), gen);

    return permutation;
}


/**
 * @brief Builds a Vamana graph using a vector dataset. The Vamana algorithm iteratively adds edges 
 * between nodes based on a combination of greedy search and pruning, creating an approximate 
 * k-NN graph structure.
 * 
 * @param P A vector of elements representing the graph nodes' data
 * @param alpha A parameter controlling the expansion of the graph
 * @param L The number of nodes in the search list for greedy search
 * @param R The maximum number of edges per node in the resulting graph
 * 
 * @return A graph with nodes connected by edges following the Vamana algorithm
 */
template <typename graph_t>
Graph<graph_t> Vamana(std::vector<graph_t>& P, float alpha, int L, int R) {

  std::pair<std::set<graph_t>, std::set<graph_t>> greedySearchResult;
  unsigned int n = P.size();

  // Initialize G to a random R-regular directed graph
  Graph<graph_t> G = createGraph(P, R);
  GraphNode<graph_t>* s = G.getNode(0);
  std::vector<int> sigma = generateRandomPermutation(0, n-1);

  for (unsigned int i = 0; i < n; i++) {
    
    std::cout << "Node " << i << "..." << std::endl;
    
    GraphNode<graph_t>* sigma_i_node = G.getNode(sigma.at(i));
    graph_t sigma_i = sigma_i_node->getData();

    // Run Greedy Search and Robust Prune
    greedySearchResult = GreedySearch(G, *s, P.at(sigma.at(i)), 1, L);
    RobustPrune(G, *sigma_i_node, greedySearchResult.second, alpha, R);

    // Get the neighbors of the sigma[i] node and iterate over them
    std::vector<graph_t>* sigma_i_neighbors = sigma_i_node->getNeighbors();
    for (auto j : *sigma_i_neighbors) {
      
      std::set<graph_t> outgoing;
      GraphNode<graph_t>* j_node = G.getNode(j.getIndex());

      // The outgoing set has to consist of the neighbors of j and the sigma[i]
      for (auto neighbor : *j_node->getNeighbors()) {
        outgoing.insert(neighbor);
      }
      outgoing.insert(sigma_i);

      // Check if the |N_out(j) union {sigma[i]}| > R and run Robust Prune
      if (outgoing.size() > (long unsigned int)R) {
        RobustPrune(G, *j_node, outgoing, alpha, R);
      }
      else {
        j_node->addNeighbor(sigma_i);
      }

    }

  }

  // Return the constructed Graph
  return G;

}

#endif /* VAMANA_H */
