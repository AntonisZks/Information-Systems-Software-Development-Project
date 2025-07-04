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
template <typename graph_t, typename query_t>
std::pair<std::set<graph_t>, std::set<graph_t>>
GreedySearch(const VamanaIndex<graph_t>& index, const GraphNode<graph_t>& s, const query_t& xq, unsigned int k, unsigned int L, const DISTANCE_SAVE_METHOD distanceSaveMethod) {
  
  std::set<graph_t> candidates = {s.getData()};
  std::set<graph_t> visited = {};

  // Calculate initial difference between candidates and visited sets
  std::set<graph_t> candidates_minus_visited = getSetDifference(candidates, visited);
  float p_star_distance = 0, currentDistance = 0;

  // Main search loop: continue until there are no unvisited candidates
  while (!candidates_minus_visited.empty()) {

    // Select the closest candidate to the query vector xq
    graph_t p_star = getSetItemAtIndex(0, candidates_minus_visited);

    if (distanceSaveMethod == NONE) {
      p_star_distance = euclideanDistance(p_star, xq);
    } else if (distanceSaveMethod == MATRIX) {
      p_star_distance = index.getDistanceMatrix()[p_star.getIndex()][xq.getIndex()];
    }

    // Compare each unvisited candidate's distance to find the nearest
    for (auto xp : candidates_minus_visited) {

      if (distanceSaveMethod == NONE) {
        currentDistance = euclideanDistance(xp, xq);
      } else if (distanceSaveMethod == MATRIX) {
        currentDistance = index.getDistanceMatrix()[xp.getIndex()][xq.getIndex()];
      }

      if (currentDistance < p_star_distance) {
        p_star_distance = currentDistance;
        p_star = xp;
      }
    }

    // Retrieve neighbors of the closest node, p_star, and add them to candidates
    GraphNode<graph_t>* p_star_node = index.getGraph().getNode(p_star.getIndex());
    std::vector<graph_t>* p_star_neighbors = p_star_node->getNeighborsVector();

    for (auto neighbor : *p_star_neighbors) {
      candidates.insert(neighbor);
    }
    visited.insert(p_star); // Mark the closest node as visited

    // Limit the size of candidates to L by keeping the closest L elements to the query
    if (candidates.size() > static_cast<size_t>(L)) {
      std::set<graph_t, EuclideanDistanceOrder<graph_t, query_t>> newCandidates{
        EuclideanDistanceOrder<graph_t, query_t>(xq, index.getDistanceMatrix(), distanceSaveMethod==MATRIX)
      };

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
  }

  // Final selection of k closest candidates after main loop
  std::set<graph_t, EuclideanDistanceOrder<graph_t, query_t>> newCandidates{
    EuclideanDistanceOrder<graph_t, query_t>(xq, index.getDistanceMatrix(), distanceSaveMethod==MATRIX)
  };
  
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

/**
 * @brief Greedy search algorithm for finding the k nearest nodes in a graph relative to a query vector.
 * 
 * This function implements a greedy search that iteratively explores the closest nodes to the query vector.
 * It maintains a candidate set of nodes to visit and a visited set of nodes already processed. This version
 * of the function is used with a FilteredVamanaIndex, which applies additional filtering criteria to the search.
 * 
 * @param graph_t Type of data stored in the graph nodes
 * @param query_t Type of the query vector
 * @param index The FilteredVamanaIndex to search
 * @param S Starting nodes for the search
 * @param xq Query vector for distance computation
 * @param k Number of nearest nodes to return
 * @param L Maximum number of nodes in the candidate set
 * @param queryFilters A vector of CategoricalAttributeFilter objects to apply to the search
 * @param mode Execution mode for the algorithm
 * 
 * @return Pair of sets: the first set contains the k nearest nodes, and the second set contains all visited nodes
 * 
 */
template <typename graph_t, typename query_t>
std::pair<std::set<graph_t>, std::set<graph_t>> FilteredGreedySearch(
  const FilteredVamanaIndex<graph_t>& index, const std::vector<GraphNode<graph_t>>& S, const query_t& xq,  
  const unsigned int k, const unsigned int L, const std::vector<CategoricalAttributeFilter>& queryFilters, const DISTANCE_SAVE_METHOD distanceSaveMethod) {

  float p_star_distance = 0, currentDistance = 0;
  
  std::set<graph_t> candidates = {};
  std::set<graph_t> visited = {};

  // Insert starting nodes from S into candidates if they match the query filters
  for (auto s : S) {

    bool passFilter = true;

    // Check if the starting node passes all query filters
    for (auto filter : queryFilters) {
      if (s.getData().getC() != filter.getC()) { // IMPORTANT: In this app version, only C filter is supported
        passFilter = false;
        break;
      }
    }

    // Only add the node to candidates if it passes the filters
    if (passFilter) {
      candidates.insert(s.getData());
    }

  }

  // Calculate initial difference between candidates and visited sets
  std::set<graph_t> candidates_minus_visited = getSetDifference(candidates, visited);

  // Main search loop: continue until there are no unvisited candidates
  while (!candidates_minus_visited.empty()) {

    // Select the closest candidate to the query vector xq
    graph_t p_star = getSetItemAtIndex(0, candidates_minus_visited);

    if (distanceSaveMethod == NONE) {
      p_star_distance = euclideanDistance(p_star, xq);
    } else if (distanceSaveMethod == MATRIX) {
      p_star_distance = index.getDistanceMatrix()[p_star.getIndex()][xq.getIndex()];
    }

    // Compare each unvisited candidate's distance to find the nearest
    for (auto xp : candidates_minus_visited) {
      
      if (distanceSaveMethod == NONE) {
        currentDistance = euclideanDistance(xp, xq);
      } else {
        currentDistance = index.getDistanceMatrix()[xp.getIndex()][xq.getIndex()];
      }

      if (currentDistance < p_star_distance) {
        p_star_distance = currentDistance;
        p_star = xp;
      }
    }

    visited.insert(p_star); // Mark the closest node as visited

    // Retrieve neighbors of the closest node, p_star
    GraphNode<graph_t>* p_star_node = index.getGraph().getNode(p_star.getIndex());
    std::vector<graph_t>* p_star_neighbors = p_star_node->getNeighborsVector();

    // Filter neighbors based on query filters and their existence in the visited set
    for (auto p_tone : *p_star_neighbors) {

      // Check if the current neighbor passes all query filters
      bool passFilter = true;
      for (auto filter : queryFilters) {
        if (p_tone.getC() != filter.getC()) { // IMPORTANT: In this app version, only C filter is supported
          passFilter = false;
          break;
        }
      }

      // Only add the neighbor to candidates if it passes the filters and is not visited
      if (passFilter && visited.find(p_tone) == visited.end()) {
        candidates.insert(p_tone);
      }

    }

    // Limit the size of candidates to L by keeping the closest L elements to the query
    if (candidates.size() > static_cast<size_t>(L)) {

      std::set<graph_t, EuclideanDistanceOrder<graph_t, query_t>> newCandidates{
        EuclideanDistanceOrder<graph_t, query_t>(xq, index.getDistanceMatrix(), distanceSaveMethod==MATRIX)
      };

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

  }

  // Final selection of k closest candidates after main loop
  std::set<graph_t, EuclideanDistanceOrder<graph_t, query_t>> newCandidates{
    EuclideanDistanceOrder<graph_t, query_t>(xq, index.getDistanceMatrix(), distanceSaveMethod==MATRIX)
  };

  for (auto candidate : candidates) {
    newCandidates.insert(candidate);
  }

  // Reassign only the closest k candidates to the candidates set for final result
  candidates.clear();
  auto it = newCandidates.begin();
  for (unsigned int i = 0; i < k && it != newCandidates.end(); i++, it++) {
    candidates.insert(*it);
  }

  return {candidates, visited}; // Return the set of candidates and visited nodes

}

template std::pair<std::set<DataVector<float>>, std::set<DataVector<float>>> GreedySearch(
  const VamanaIndex<DataVector<float>>& index, 
  const GraphNode<DataVector<float>>& s, 
  const DataVector<float>& xq, 
  unsigned int k, 
  unsigned int L,
  const DISTANCE_SAVE_METHOD distanceSaveMethod
);

template std::pair<std::set<BaseDataVector<float>>, std::set<BaseDataVector<float>>> GreedySearch(
  const VamanaIndex<BaseDataVector<float>>& index, 
  const GraphNode<BaseDataVector<float>>& s, 
  const BaseDataVector<float>& xq, 
  unsigned int k, 
  unsigned int L,
  const DISTANCE_SAVE_METHOD distanceSaveMethod
);

template std::pair<std::set<BaseDataVector<float>>, std::set<BaseDataVector<float>>> GreedySearch(
  const VamanaIndex<BaseDataVector<float>>& index, 
  const GraphNode<BaseDataVector<float>>& s, 
  const QueryDataVector<float>& xq, 
  unsigned int k, 
  unsigned int L,
  const DISTANCE_SAVE_METHOD distanceSaveMethod
);

// Filtered Greedy Search
template std::pair<std::set<BaseDataVector<float>>, std::set<BaseDataVector<float>>> FilteredGreedySearch(
  const FilteredVamanaIndex<BaseDataVector<float>>& index, 
  const std::vector<GraphNode<BaseDataVector<float>>>& S, 
  const BaseDataVector<float>& xq, 
  const unsigned int k, 
  const unsigned int L, 
  const std::vector<CategoricalAttributeFilter>& queryFilters,
  const DISTANCE_SAVE_METHOD distanceSaveMethod
);

template std::pair<std::set<BaseDataVector<float>>, std::set<BaseDataVector<float>>> FilteredGreedySearch(
  const FilteredVamanaIndex<BaseDataVector<float>>& index, 
  const std::vector<GraphNode<BaseDataVector<float>>>& S, 
  const QueryDataVector<float>& xq, 
  const unsigned int k, 
  const unsigned int L, 
  const std::vector<CategoricalAttributeFilter>& queryFilters,
  const DISTANCE_SAVE_METHOD distanceSaveMethod
);
