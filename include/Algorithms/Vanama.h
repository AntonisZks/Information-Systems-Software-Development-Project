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

    // Static random number generator with a fixed seed
    static std::mt19937 generator(42);  // Seed with a fixed value for consistent randomness
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

static std::vector<int> generateRandomPermutation(const unsigned int start, const unsigned int end) {

  std::vector<int> permutation;
  for (unsigned int i = start; i <= end; i++) {
    permutation.push_back(i);
  }

  std::mt19937 gen(42);
  std::shuffle(permutation.begin(), permutation.end(), gen);

  return permutation;

}

template <typename graph_t>
Graph<graph_t> Vamana(std::vector<graph_t>& P, float alpha, int L, int R) {

  std::pair<std::set<graph_t>, std::set<graph_t>> greedySearchResult;
  unsigned int n = P.size();

  Graph<graph_t> G = createGraph(P, R);
  GraphNode<graph_t>* s = G.getNode(0);
  std::vector<int> sigma = generateRandomPermutation(0, n-1);

  for (unsigned int i = 0; i < n; i++) {
    
    std::cout << "Node " << i << "..." << std::endl;

    GraphNode<graph_t>* sigma_i_node = G.getNode(sigma.at(i));
    graph_t sigma_i = sigma_i_node->getData();

    greedySearchResult = GreedySearch2(G, *s, P.at(sigma.at(i)), 1, L);
    RobustPrune(G, *sigma_i_node, greedySearchResult.second, alpha, R);

    std::vector<graph_t>* sigma_i_neighbors = sigma_i_node->getNeighbors();
    for (auto j : *sigma_i_neighbors) {
      
      std::set<graph_t> outgoing;
      GraphNode<graph_t>* j_node = G.getNode(j.getIndex());

      for (auto neighbor : *j_node->getNeighbors()) {
        outgoing.insert(neighbor);
      }
      outgoing.insert(sigma_i);

      if (outgoing.size() > (long unsigned int)R) {
        RobustPrune(G, *j_node, outgoing, alpha, R);
      }
      else {
        j_node->addNeighbor(sigma_i);
      }

    }

  }

  return G;

}

#endif /* VAMANA_H */
