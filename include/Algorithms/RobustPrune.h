#ifndef ROBUST_PRUNE_H
#define ROBUST_PRUNE_H

#include <iostream>
#include <set>
#include "../DataStructures/Graph/graph.h"

// RobustPrune function
template<typename T>
std::pair<std::set<GraphNode<T>>, std::set<GraphNode<T>>> 
robustPrune(Graph<T>& graph, const GraphNode<T>& start_node, float alpha, int R) {
  std::vector<T> prunedNeighbors;
  std::set<GraphNode<T>> visited_nodes;
  std::set<GraphNode<T>> pruned_neighbors;
  std::vector<unsigned int> candidate_indices;

  // Initialize candidate_indices with all node indices except the start node
  for (unsigned int i = 0; i < graph.getNodesCount(); ++i) {
    if (i != (unsigned int)start_node.getIndex()) {
      candidate_indices.push_back(i);
    }
  }


  // Get a reference to the point p
  GraphNode<T>* p = graph.getNode(start_node.getIndex());

  // Repeat until we have R neighbors or candidate indices are exhausted
  while (!candidate_indices.empty() && (int)prunedNeighbors.size() < R) {
    // Find the closest neighbor to p within candidate_indices
    unsigned int closest_index = candidate_indices[0];
    float min_distance = euclideanDistance(p->getData(), graph.getNode(closest_index)->getData());

    for (unsigned int i = 1; i < candidate_indices.size(); ++i) {
      unsigned int current_index = candidate_indices[i];
      float current_distance = euclideanDistance(p->getData(), graph.getNode(current_index)->getData());

      if (current_distance < min_distance) {
        closest_index = current_index;
        min_distance = current_distance;
      }
    }

    // Get the closest node
    GraphNode<T>* closestNode = graph.getNode(closest_index);

    // Add the closest neighbor to the set of final neighbors
    prunedNeighbors.push_back(closestNode->getData());
    pruned_neighbors.insert(*closestNode);
    visited_nodes.insert(*closestNode); // Add to visited nodes

    // Remove the closest neighbor from candidate indices
    candidate_indices.erase(
      std::remove(candidate_indices.begin(), candidate_indices.end(), closest_index), candidate_indices.end()
    );

    // Remove neighbors that do not meet the distance threshold alpha * d(p, closestNode)
    float threshold_distance = alpha * min_distance;
    for (auto it = candidate_indices.begin(); it != candidate_indices.end();) {
      GraphNode<T>* candidateNode = graph.getNode(*it);
      if (euclideanDistance(p->getData(), candidateNode->getData()) > threshold_distance) {
        // Remove candidate if the distance exceeds the threshold
        it = candidate_indices.erase(it);
      } else {
        visited_nodes.insert(*candidateNode); // Add to visited nodes
        ++it;
      }
    }

    // Stop if we have reached the limit R
    if ((int)prunedNeighbors.size() == R) {
      break;
    }
  }

  return {visited_nodes, pruned_neighbors};
}

#endif
