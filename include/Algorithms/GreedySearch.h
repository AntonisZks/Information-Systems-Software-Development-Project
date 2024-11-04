#ifndef GRAPH_FUNCTIONS_H
#define GRAPH_FUNCTIONS_H

#include <iostream>
#include <vector>
#include <set>
#include <algorithm>
#include <iterator>
#include <stdexcept>
#include "../DataStructures/Graph/graph.h"
#include "../DataStructures/DataVector/DataVector.h"
#include "../distance.h"
#include <queue>
#include <cmath>



// Type Alias
template <typename graph_t> using GraphNodeSet = std::set<GraphNode<graph_t>>;
template <typename graph_t> using GreedySearchResult = std::pair<GraphNodeSet<graph_t>, GraphNodeSet<graph_t>>;

typedef DataVector<float> GreedySearchVector;
typedef unsigned int result_size_t;
typedef unsigned int search_list_size_t;

/**
 * @brief Computes the difference between two sets.
 * 
 * @param set_t Type of elements in the set
 * @param set1 The first set
 * @param set2 The second set
 * 
 * @return A set containing elements that are in set1 but not in set2
 */
template <typename set_t> 
static std::set<set_t> getSetDifference(const std::set<set_t>& set1, const std::set<set_t>& set2) {

  // Create a result set indicating the result of the difference operation
  std::set<set_t> result;

  // Compute the difference of the two sets
  std::set_difference(
    set1.begin(), set1.end(), 
    set2.begin(), set2.end(), 
    std::inserter(result, result.begin())
  );

  return result;

}

/**
 * @brief Retrieves the element at a specific index from a set.
 * 
 * @param set_t Type of elements in the set
 * @param index The zero-based index of the element
 * @param set The set to access
 * 
 * @return The element at the specified index
 * @throws std::invalid_argument if the index is out of range
 */
template <typename set_t>
static set_t getSetItemAtIndex(const unsigned int& index, const std::set<set_t>& set) {

  // Check if the given index is valid
  if (index < 0 || index >= set.size()) {
    throw std::invalid_argument("Index is not valid");
  }

  // Find the item at the given index and return it
  auto it = set.begin();
  std::advance(it, index);

  return *it;

}

/**
 * @brief Creates a sorted array of graph nodes based on Euclidean distance to a query vector.
 * 
 * @param graph_t Data type stored in the graph nodes
 * @param candidates Set of candidate nodes to sort
 * @param xq Query vector for distance computation
 * 
 * @return Pointer to a dynamically allocated array of sorted candidate nodes
 */
template <typename graph_t> 
static GraphNode<graph_t>* getCandidatesArray(const GraphNodeSet<graph_t>& candidates, const GreedySearchVector& xq) {

  double distance1, distance2;

  // Create an array to store the items of the candidates set
  GraphNode<graph_t>* candidates_items = new GraphNode<graph_t>[candidates.size()];

  // Move all the items to the array
  for (unsigned int i = 0; i < candidates.size(); i++) {
    GraphNode<graph_t> currentNode = getSetItemAtIndex(i, candidates);
    candidates_items[i] = currentNode;
  }

  // Sort the array using Bubble Sort according to the Euclidean Distance of the inner vectors
  for (unsigned int i = 0; i < candidates.size() - 1; i++) {
    
    bool swapped = false;
    for (unsigned int j = 0; j < candidates.size() - i - 1; j++) {
      
      try {
        distance1 = euclideanDistance(candidates_items[j].getData(), xq);
        distance2 = euclideanDistance(candidates_items[j + 1].getData(), xq);
      }
      catch (const std::invalid_argument& e) {
        std::cerr << "Invalid argument while computing distances for sorting candidates" << std::endl;
        delete [] candidates_items;
        exit(1);
      }
      
      if (distance1 < distance2) {
        std::swap(candidates_items[j], candidates_items[j + 1]);
        swapped = true;
      }

    }
    if (!swapped) { break; }
  }

  return candidates_items;

}

/**
 * @brief Limits a set of graph nodes to the N nearest nodes relative to a query vector.
 * 
 * @param graph_t Data type stored in the graph nodes
 * @param candidates Set of candidate nodes to filter
 * @param xq Query vector for distance computation
 * @param N Maximum number of nearest nodes to retain
 */
template <typename graph_t>
static void getNNearestPointsTo(GraphNodeSet<graph_t>& candidates, const GreedySearchVector xq, unsigned int N) {

  // Create a candidates array to store all the vectors temporary
  GraphNode<graph_t>* candidates_items = getCandidatesArray(candidates, xq);

  // Clear the candidates set and insert the sorted vectors
  candidates.clear();
  for (unsigned int i = 0; i < N; i++) {
    candidates.insert(candidates_items[i]);
  }

  // Delete the array
  delete [] candidates_items;

}

/**
 * @brief Retrieves all neighboring nodes of a given graph node.
 * 
 * @param graph_t Data type stored in the graph nodes
 * @param p_star The node whose neighbors are to be retrieved
 * 
 * @return A set containing all neighboring nodes of p_star
 */
template <typename graph_t>
static GraphNodeSet<graph_t> getPstarNeighbors(GraphNode<graph_t>& p_star) {

  // Create a set for the neighbors and a vector for the neighbor nodes
  GraphNodeSet<graph_t> n_out;
  std::vector<graph_t>* neighbors = p_star.getNeighbors();

  // Iterate through the neighbor vectors and insert each one in the neighbors set
  for (unsigned int i = 0; i < neighbors->size(); i++) {
    GraphNode<graph_t> newNode(neighbors->at(i));
    n_out.insert(newNode);
  }

  return n_out;

}

/**
 * @brief Finds the node in a candidate set with the minimum Euclidean distance to a query vector.
 * 
 * @param graph_t Data type stored in the graph nodes
 * @param candidates_minus_visited Set of candidate nodes not yet visited
 * @param xq Query vector for distance computation
 * 
 * @return The node in candidates_minus_visited closest to xq
 */
template <typename graph_t>
static GraphNode<graph_t> getPstar(const GraphNodeSet<graph_t>& candidates_minus_visited, const GreedySearchVector& xq) {

  double p_star_distance;
  double distance;

  // Set the first vector of the candidates as p*
  GraphNode<graph_t> p_star = getSetItemAtIndex(0, candidates_minus_visited);
  graph_t p_star_xp = p_star.getData();
  
  // Receive its euclidian distance from the query vector
  try {
    p_star_distance = euclideanDistance(p_star_xp, xq);
  } 
  catch (const std::invalid_argument& e) {
    std::cout << "Invalid argument while computing p_star_distance" << e.what() << std::endl;
    exit(1);
  }

  // Find the minimum euclidean distance from the query vector, and store the corresponding base vector as p*
  for (unsigned int i = 1; i < candidates_minus_visited.size(); i++) {

    // Get the current vector
    GraphNode<graph_t> currentNode = getSetItemAtIndex(i, candidates_minus_visited);
    graph_t xp = currentNode.getData();

    // Compute its euclidean distance from the query vector
    try {
      distance = euclideanDistance(xp, xq);
    }
    catch (const std::invalid_argument& e) {
      std::cout << "Invalid argument while computing distance" << e.what() << std::endl;
      exit(1);
    }

    // Update p* if neede
    if (distance < p_star_distance) {
      p_star = currentNode;
      p_star_xp = xp;
      p_star_distance = distance;
    }

  }

  return p_star;

}

/**
 * @brief Greedy search algorithm for finding the k closest nodes in a graph relative to a query vector.
 * 
 * @param graph_t Data type stored in the graph nodes
 * @param graph The graph to search
 * @param s Starting node for the search
 * @param xq Query vector for distance computation
 * @param k Number of nearest nodes to return
 * @param L Maximum number of nodes in the search list
 * 
 * @return Pair of sets: first contains the k closest nodes, and second contains the visited nodes
 */
template <typename graph_t> 
GreedySearchResult<graph_t> 
GreedySearch(const GraphNode<graph_t>& s, const GreedySearchVector& xq, const result_size_t k, const search_list_size_t L) {

  std::set<GraphNode<graph_t>> candidates_minus_visited;

  // Initialize sets L <-- {s} and V <-- {}
  std::set<GraphNode<graph_t>> candidates; candidates.insert(s);
  std::set<GraphNode<graph_t>> visited;

  candidates_minus_visited = getSetDifference(candidates, visited);
  while (!candidates_minus_visited.empty()) {
    
    // Calculate p* <-- argmin_{p in L \ V}(||xp - xq||)
    GraphNode<graph_t> p_star = getPstar(candidates_minus_visited, xq);

    // Update L <-- L union N_out(p*) and V <-- V union {p*}
    std::set<GraphNode<graph_t>> n_out = getPstarNeighbors(p_star);
    candidates.insert(n_out.begin(), n_out.end());
    visited.insert(p_star);

    // Check if |canidates| > L
    if (candidates.size() > L) {
      getNNearestPointsTo(candidates, xq, L);
    }

    candidates_minus_visited = getSetDifference(candidates, visited);

  }

  // Receive k closest points from candidates
  getNNearestPointsTo(candidates, xq, k);

  // Create the greedy search result pair of values
  GreedySearchResult<graph_t> result;
  result.first = candidates;
  result.second = visited;

  return result;

}


#endif /* GRAPH_FUNCTIONS_H */