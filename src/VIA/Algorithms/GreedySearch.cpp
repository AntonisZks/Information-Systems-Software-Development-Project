#include "../../../include/GreedySearch.h"

/**
 * @brief Computes the difference between two sets, returning elements
 * that are in the first set but not in the second.
 * 
 * @param set_t Type of elements in the sets
 * @param set1 The first set
 * @param set2 The second set
 * 
 * @return A set containing elements that are in set1 but not in set2
 */
template <typename set_t> 
static std::set<set_t> getSetDifference(const std::set<set_t>& set1, const std::set<set_t>& set2) {
  
  std::set<set_t> result; // Resulting set to hold the difference

  // Compute difference and store in result
  std::set_difference(
    set1.begin(), set1.end(), 
    set2.begin(), set2.end(), 
    std::inserter(result, result.begin())
  );

  return result;

}

/**
 * @brief Retrieves the element at a specific index in a set. This function enables index-based access in a set, 
 * even though sets are not indexed containers.
 * 
 * @param set_t Type of elements in the set
 * @param index The zero-based index of the desired element
 * @param set The set from which to retrieve the element
 * 
 * @return The element at the specified index
 * @throws std::invalid_argument if the index is out of range
 */
template <typename set_t>
static set_t getSetItemAtIndex(const unsigned int& index, const std::set<set_t>& set) {

  // Validate index
  if (index >= set.size()) {
    throw std::invalid_argument("Index is not valid");
  }

  // Advance an iterator to the indexed position
  auto it = set.begin();
  std::advance(it, index);

  return *it; // Return the element at the specified index

}

/**
 * @brief Greedy search algorithm for finding the k nearest nodes in a graph relative to a query vector.
 * 
 * This function implements a greedy search that iteratively explores the closest nodes to the query vector.
 * It maintains a candidate set of nodes to visit and a visited set of nodes already processed.
 * 
 * @param graph_t Type of data stored in the graph nodes
 * @param G The graph to search
 * @param s Starting node for the search
 * @param xq Query vector for distance computation
 * @param k Number of nearest nodes to return
 * @param L Maximum number of nodes in the candidate set
 * 
 * @return Pair of sets: the first set contains the k nearest nodes, and the second set contains all visited nodes
 */
template <typename graph_t>
std::pair<std::set<graph_t>, std::set<graph_t>>
GreedySearch(const Graph<graph_t>& G, const GraphNode<graph_t>& s, const graph_t& xq, unsigned int k, unsigned int L) {
  
  std::set<graph_t> candidates = {s.getData()};
  std::set<graph_t> visited = {};

  // Calculate initial difference between candidates and visited sets
  std::set<graph_t> candidates_minus_visited = getSetDifference(candidates, visited);
  unsigned int cnt = 0;

  // Main search loop: continue until there are no unvisited candidates
  while (!candidates_minus_visited.empty()) {

    // Select the closest candidate to the query vector xq
    graph_t p_star = getSetItemAtIndex(0, candidates_minus_visited);
    float p_star_distance = euclideanDistance(p_star, xq);

    // Compare each unvisited candidate's distance to find the nearest
    for (auto xp : candidates_minus_visited) {
      float currentDistance = euclideanDistance(xp, xq);
      if (currentDistance < p_star_distance) {
        p_star_distance = currentDistance;
        p_star = xp;
      }
    }

    // Retrieve neighbors of the closest node, p_star, and add them to candidates
    GraphNode<graph_t>* p_star_node = G.getNode(p_star.getIndex());
    std::vector<graph_t>* p_star_neighbors = p_star_node->getNeighbors();

    for (auto neighbor : *p_star_neighbors) {
      candidates.insert(neighbor);
    }
    visited.insert(p_star); // Mark the closest node as visited

    // Limit the size of candidates to L by keeping the closest L elements to the query
    if (candidates.size() > static_cast<size_t>(L)) {
      std::set<graph_t, EuclideanDistanceOrder<graph_t>> newCandidates{EuclideanDistanceOrder<graph_t>(xq)};
      for (auto candidate : candidates) {
        newCandidates.insert(candidate);
      }
      
      // Reassign only the closest L candidates back to candidates set
      candidates.clear();
      auto it = newCandidates.begin();
      for (unsigned int i = 0; i < L && it != newCandidates.end(); i++, it++) {
        candidates.insert(*it);
      }
    }

    // Update candidates_minus_visited with the new difference after adding p_star to visited
    candidates_minus_visited = getSetDifference(candidates, visited);
    cnt++; // Increment loop counter for diagnostics if needed
  }

  // Final selection of k closest candidates after main loop
  std::set<graph_t, EuclideanDistanceOrder<graph_t>> newCandidates{EuclideanDistanceOrder<graph_t>(xq)};
  for (auto candidate : candidates) {
    newCandidates.insert(candidate);
  }

  // Reassign only the closest k candidates to the candidates set for final result
  candidates.clear();
  auto it = newCandidates.begin();
  for (unsigned int i = 0; i < k && it != newCandidates.end(); i++, it++) {
    candidates.insert(*it);
  }

  return {candidates, visited};

}

template std::pair<std::set<DataVector<float>>, std::set<DataVector<float>>> 
GreedySearch(const Graph<DataVector<float>>& G, const GraphNode<DataVector<float>>& s, const DataVector<float>& xq, unsigned int k, unsigned int L);