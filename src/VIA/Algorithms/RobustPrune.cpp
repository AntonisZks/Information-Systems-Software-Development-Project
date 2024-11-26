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
void RobustPrune(Graph<graph_t>& G, GraphNode<graph_t>& p_node, std::set<graph_t>& V, float alpha, int R) {
    
  // Get the data of the node p_node
  graph_t p = p_node.getData();

  // Retrieve all neighbors of p_node and insert them into set V
  std::vector<graph_t>* neighbors = p_node.getNeighbors();
  for (auto neighbor : *neighbors) {
    V.insert(neighbor);
  }
  // Remove p_node itself from V
  V.erase(p);
  // Clear the neighbors of p_node
  p_node.clearNeighbors();

  // Continue pruning until V is empty or the desired number of neighbors is reached
  while (!V.empty()) {

    // Find the closest neighbor to p_node in V
    graph_t p_star = getSetItemAtIndex(0, V);
    float p_star_distance = euclideanDistance(p, p_star);

    // Update p_star if a closer neighbor is found
    for (auto p_tone : V) {
      float currentDistance = euclideanDistance(p, p_tone);
      
      if (currentDistance < p_star_distance) {
        p_star_distance = currentDistance;
        p_star = p_tone;
      }
    }

    // Add the closest neighbor to p_node
    p_node.addNeighbor(p_star);

    // Check if the desired number of neighbors has been reached
    if (p_node.getNeighbors()->size() == (long unsigned int)R) {
      break;
    }

    // Create a copy of V to avoid modifying the original set during iteration
    std::set<graph_t> V_copy = V;
    for (auto p_tone : V_copy) {
      // Remove neighbors that are too far from p_star based on alpha and euclideanDistance
      if ((alpha * euclideanDistance(p_star, p_tone)) < euclideanDistance(p, p_tone)) {
        V.erase(p_tone);
      }
    }
  }
}

template void RobustPrune<DataVector<float>>(
  Graph<DataVector<float>>& G, GraphNode<DataVector<float>>& p_node, std::set<DataVector<float>>& V, float alpha, int R);
