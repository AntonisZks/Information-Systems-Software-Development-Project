#include "../../../include/RobustPrune.h"
#include "../../../include/DataVector.h"
#include "../../../include/distance.h"

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
 * @brief Prunes the neighbors of a given node in a graph based on a robust pruning algorithm.
 *
 * This function modifies the neighbors of the given node `p_node` in the graph `G` by selecting
 * a subset of neighbors that are within a certain distance threshold defined by `alpha` and `R`.
 *
 * @tparam graph_t The type of the graph nodes.
 * @param G The graph containing the node to be pruned.
 * @param p_node The node whose neighbors are to be pruned.
 * @param V A set of graph nodes to be considered for pruning.
 * @param alpha A float value used as a multiplier for the distance threshold.
 * @param R An integer specifying the maximum number of neighbors to retain.
 *
 * The function performs the following steps:
 * 1. Retrieves the data of the node `p_node` and its neighbors.
 * 2. Inserts all neighbors of `p_node` into the set `V` and clears the neighbors of `p_node`.
 * 3. Iteratively selects the closest neighbor `p_star` from `V` to `p_node` and adds it to the neighbors of `p_node`.
 * 4. Removes nodes from `V` that do not satisfy the distance threshold defined by `alpha`.
 * 5. Stops when the number of neighbors of `p_node` reaches `R` or `V` is empty.
 */
template <typename graph_t>
void RobustPrune(VamanaIndex<graph_t>& index, GraphNode<graph_t>& p_node, std::set<graph_t>& V, float alpha, int R, const DISTANCE_SAVE_METHOD distanceSaveMethod) {
    
  float p_star_distance = 0, currentDistance = 0;
  float distance1 = 0, distance2 = 0;

  // Get the data of the node p_node
  graph_t p = p_node.getData();

  // Retrieve all neighbors of p_node and insert them into set V
  std::vector<graph_t>* neighbors = p_node.getNeighborsVector();
  for (auto neighbor : *neighbors) {
    V.insert(neighbor);
  }

  // Remove p_node itself from V, and clear the neighbors of p_node
  V.erase(p);
  p_node.clearNeighbors();

  // Continue pruning until V is empty or the desired number of neighbors is reached
  while (!V.empty()) {

    // Find the closest neighbor to p_node in V, and initialize the distance to p_star
    graph_t p_star = getSetItemAtIndex(0, V);

    if (distanceSaveMethod == NONE) {
      p_star_distance = euclideanDistance(p, p_star);
    }
    else if (distanceSaveMethod == MATRIX) {
      p_star_distance = index.getDistanceMatrix()[p.getIndex()][p_star.getIndex()];
    }
    

    // Update p_star if a closer neighbor is found
    for (auto p_tone : V) {
      
      if (distanceSaveMethod == NONE) {
        currentDistance = euclideanDistance(p, p_tone);
      }
      else if (distanceSaveMethod == MATRIX) {
        currentDistance = index.getDistanceMatrix()[p.getIndex()][p_tone.getIndex()];
      }
      
      if (currentDistance < p_star_distance) {
        p_star_distance = currentDistance;
        p_star = p_tone;
      }
    }

    // Add the closest neighbor to p_node
    p_node.addNeighbor(p_star);

    // Check if the desired number of neighbors has been reached
    if (p_node.getNeighborsVector()->size() == (long unsigned int)R) {
      break;
    }

    // Create a copy of V to avoid modifying the original set during iteration
    std::set<graph_t> V_copy = V;
    for (auto p_tone : V_copy) {

      // Remove neighbors that are too far from p_star based on alpha and euclideanDistance
      if (distanceSaveMethod == NONE) {
        distance1 = euclideanDistance(p_star, p_tone);
        distance2 = euclideanDistance(p, p_tone);
      }
      else if (distanceSaveMethod == MATRIX) {
        distance1 = index.getDistanceMatrix()[p_star.getIndex()][p_tone.getIndex()];
        distance2 = index.getDistanceMatrix()[p.getIndex()][p_tone.getIndex()];
      }
      
      if ((alpha * distance1) <= distance2) {
        V.erase(p_tone);
      }

    }
  }

}

/**
 * @brief Prunes the neighbors of a given node in a graph based on a robust pruning algorithm with filtering.
 *
 * This function modifies the neighbors of the given node `p_node` in the graph `G` by selecting
 * a subset of neighbors that are within a certain distance threshold defined by `alpha` and `R`,
 * while also applying additional filtering criteria.
 *
 * @tparam graph_t The type of the graph nodes.
 * @param G The graph containing the node to be pruned.
 * @param p_node The node whose neighbors are to be pruned.
 * @param V A set of graph nodes to be considered for pruning.
 * @param alpha A float value used as a multiplier for the distance threshold.
 * @param R An integer specifying the maximum number of neighbors to retain.
 */
template <typename graph_t>
void FilteredRobustPrune(FilteredVamanaIndex<graph_t>& index, GraphNode<graph_t>& p_node, std::set<graph_t>& V, float alpha, int R, const DISTANCE_SAVE_METHOD distanceSaveMethod) {
  
  float p_star_distance = 0, currentDistance = 0;
  float distance1 = 0, distance2 = 0;

  // Get the data of the node p_node
  graph_t p = p_node.getData();

  // Retrieve all neighbors of p_node and insert them into set V
  std::vector<graph_t>* neighbors = p_node.getNeighborsVector();
  for (auto neighbor : *neighbors) {
    V.insert(neighbor);
  }

  // Remove p_node itself from V, and clear the neighbors of p_node
  V.erase(p);
  p_node.clearNeighbors();

  // Continue pruning until V is empty or the desired number of neighbors is reached
  while (!V.empty()) {

    // Find the closest neighbor to p_node in V
    graph_t p_star = getSetItemAtIndex(0, V);

    if (distanceSaveMethod == NONE) {
      p_star_distance = euclideanDistance(p, p_star);
    }
    else if (distanceSaveMethod == MATRIX) {
      p_star_distance = index.getDistanceMatrix()[p.getIndex()][p_star.getIndex()];
    }

    // Update p_star if a closer neighbor is found
    for (auto p_tone : V) {
      if (distanceSaveMethod == NONE) {
        currentDistance = euclideanDistance(p, p_tone);
      }
      else if (distanceSaveMethod == MATRIX) {
        currentDistance = index.getDistanceMatrix()[p.getIndex()][p_tone.getIndex()];
      }
      
      if (currentDistance < p_star_distance) {
        p_star_distance = currentDistance;
        p_star = p_tone;
      }
    }

    // Add the closest neighbor to p_node
    p_node.addNeighbor(p_star);

    // Check if the desired number of neighbors has been reached
    if (p_node.getNeighborsVector()->size() == (long unsigned int)R) {
      break;
    }

    // Filtering logic for V
    std::set<graph_t> V_copy = V; // Create a copy of V
    for (auto p_tone : V_copy) {

      // Checking if the operation Fp' interst Fp is not a proper subset of Fp*
      if (p_tone.getC() != p.getC()) {
        if ((int)p_star.getC() == -1) {
          continue;
        }
      } 
      else {
        if (p_star.getC() != p_tone.getC()) {
          continue;
        }
      }

      // Remove neighbors that are too far from p_star based on alpha and euclideanDistance
      if (distanceSaveMethod == NONE) {
        distance1 = euclideanDistance(p_star, p_tone);
        distance2 = euclideanDistance(p, p_tone);
      }
      else if (distanceSaveMethod == MATRIX) {
        distance1 = index.getDistanceMatrix()[p_star.getIndex()][p_tone.getIndex()];
        distance2 = index.getDistanceMatrix()[p.getIndex()][p_tone.getIndex()];
      }
      
      if ((alpha * distance1) <= distance2) {
        V.erase(p_tone);
      }

    }
  }

}


/// Explicit instantiations for RobustPrune and FilteredRobustPrune


// Explicit instantiation for RobustPrune with float data type and DataVector query type
template void RobustPrune<DataVector<float>>(
  VamanaIndex<DataVector<float>>& index, 
  GraphNode<DataVector<float>>& p_node, 
  std::set<DataVector<float>>& V, 
  float alpha, 
  int R,
  const DISTANCE_SAVE_METHOD distanceSaveMethod
);

// Explicit instantiation for FilteredRobustPrune with float data type and DataVector query type
template void RobustPrune<BaseDataVector<float>>(
  VamanaIndex<BaseDataVector<float>>& index, 
  GraphNode<BaseDataVector<float>>& p_node, 
  std::set<BaseDataVector<float>>& V, 
  float alpha, 
  int R,
  const DISTANCE_SAVE_METHOD distanceSaveMethod
);

// Explicit instantiation for FilteredRobustPrune with float data type and DataVector query type
template void FilteredRobustPrune<BaseDataVector<float>>(
  FilteredVamanaIndex<BaseDataVector<float>>& index, 
  GraphNode<BaseDataVector<float>>& p_node, 
  std::set<BaseDataVector<float>>& V, 
  float alpha, 
  int R,
  const DISTANCE_SAVE_METHOD distanceSaveMethod
);
