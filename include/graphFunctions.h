#ifndef GRAPH_FUNCTIONS_H
#define GRAPH_FUNCTIONS_H

#include <iostream>
#include <vector>
#include <set>
#include <algorithm>
#include <iterator>
#include <stdexcept>
#include "Graph/graph.h"
#include "DataVector/DataVector.h"
#include "distance.h"


template <typename set_t> 
static std::set<set_t> getSetDifference(std::set<set_t> set1, std::set<set_t> set2) {

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

template <typename set_t>
static set_t getSetItemAtIndex(const unsigned int index, const std::set<set_t> set) {

  // Check if the given index is valid
  if (index < 0 || index >= set.size()) {
    throw std::invalid_argument("Index is not valid");
  }

  auto it = set.begin();
  std::advance(it, index);

  return *it;

}

/**
 * @brief Greedy Search Algorithm
 * 
 * @param graph the graph to implement the greedy search
 * @param s the starting node
 * @param query the current query
 * @param k the result size
 * @param L the search list size
*/
template <typename graph_t> 
std::pair<std::set<GraphNode<graph_t>>, std::set<GraphNode<graph_t>>> GreedySearch(Graph<graph_t> &graph, GraphNode<graph_t>& s, DataVector<float> xq, unsigned int k, unsigned int L) {

  std::set<GraphNode<graph_t>> candidates_minus_visited;
  double p_star_distance;
  double distance;

  // Initialize sets L <-- {s} and V <-- {}
  std::set<GraphNode<graph_t>> candidates; candidates.insert(s);
  std::set<GraphNode<graph_t>> visited;

  candidates_minus_visited = getSetDifference(candidates, visited);
  while (!candidates_minus_visited.empty()) {
    
    // Calculate p* <-- argmin_{p in L \ V}(||xp - xq||)
    GraphNode<graph_t> p_star = getSetItemAtIndex(0, candidates_minus_visited);
    graph_t p_star_xp = p_star.getData();
    
    try {
      p_star_distance = euclideanDistance(p_star_xp, xq);
    } 
    catch (const std::invalid_argument& e) {
      std::cout << "Invalid argument while computing p_star_distance" << e.what() << std::endl;
      exit(1);
    }

    for (unsigned int i = 1; i < candidates_minus_visited.size(); i++) {

      GraphNode<graph_t> currentNode = getSetItemAtIndex(i, candidates_minus_visited);
      graph_t xp = currentNode.getData();

      try {
        distance = euclideanDistance(xp, xq);
      }
      catch (const std::invalid_argument& e) {
        std::cout << "Invalid argument while computing distance" << e.what() << std::endl;
        exit(1);
      }

      if (distance < p_star_distance) {
        p_star = currentNode;
        p_star_xp = xp;
        p_star_distance = distance;
      }

    }

    // Update L <-- L union N_out(p*) and V <-- V union {p*}
    std::set<GraphNode<graph_t>> n_out;
    std::vector<graph_t>* neighbors = p_star.getNeighbors();

    for (unsigned int i = 0; i < neighbors->size(); i++) {
      GraphNode<graph_t> newNode(neighbors->at(i));
      n_out.insert(newNode);
    }

    candidates.insert(n_out.begin(), n_out.end());
    visited.insert(p_star);

    // Check if |canidates| > L
    if (candidates.size() > L) {

      GraphNode<graph_t>* candidates_items = new GraphNode<graph_t>[candidates.size()];

      for (unsigned int i = 0; i < candidates.size(); i++) {
        GraphNode<graph_t> currentNode = getSetItemAtIndex(i, candidates);
        candidates_items[i] = currentNode;
      }

      for (unsigned int i = 0; i < candidates.size() - 1; i++) {
        
        bool swapped = false;
        for (unsigned int j = 0; j < candidates.size() - i - 1; j++) {

          double distance1 = euclideanDistance(candidates_items[j].getData(), xq);
          double distance2 = euclideanDistance(candidates_items[j + 1].getData(), xq);
          
          if (distance1 < distance2) {
            std::swap(candidates_items[j], candidates_items[j + 1]);
            swapped = true;
          }

        }
        if (!swapped) { break; }
      }

      candidates.clear();
      for (unsigned int i = 0; i < L; i++) {
        candidates.insert(candidates_items[i]);
      }

      delete [] candidates_items;

    }

    candidates_minus_visited = getSetDifference(candidates, visited);

  }

  GraphNode<graph_t>* candidates_items = new GraphNode<graph_t>[candidates.size()];

  for (unsigned int i = 0; i < candidates.size(); i++) {
    GraphNode<graph_t> currentNode = getSetItemAtIndex(i, candidates);
    candidates_items[i] = currentNode;
  }

  for (unsigned int i = 0; i < candidates.size() - 1; i++) {
    
    bool swapped = false;
    for (unsigned int j = 0; j < candidates.size() - i - 1; j++) {

      double distance1 = euclideanDistance(candidates_items[j].getData(), xq);
      double distance2 = euclideanDistance(candidates_items[j + 1].getData(), xq);
      
      if (distance1 < distance2) {
        std::swap(candidates_items[j], candidates_items[j + 1]);
        swapped = true;
      }

    }
    if (!swapped) { break; }
  }

  std::set<GraphNode<graph_t>> k_closest_items;
  for (unsigned int i = 0; i < k; i++) {
    k_closest_items.insert(candidates_items[i]);
  }

  delete [] candidates_items;

  std::pair<std::set<GraphNode<graph_t>>, std::set<GraphNode<graph_t>>> result;
  result.first = k_closest_items;
  result.second = visited;

  return result;

}

void Robust_Prune(void);

#endif /* GRAPH_FUNCTIONS_H */
