#include <iostream>
#include "graph.h"
#include "DataVector.h"
#include "BQDataVectors.h"
#include "VamanaIndex.h"
#include "distance.h"

template <typename graph_t> class VamanaIndex;
template <typename graph_t> class FilteredVamanaIndex;

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
template <typename graph_t> void RobustPrune(
  VamanaIndex<graph_t>& index, 
  GraphNode<graph_t>& p_node, 
  std::set<graph_t>& V, 
  float alpha, 
  int R,
  const DISTANCE_SAVE_METHOD distanceSaveMethod
);

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
void FilteredRobustPrune(FilteredVamanaIndex<graph_t>& index, GraphNode<graph_t>& p_node,std::set<graph_t>& V, float alpha,int R);
