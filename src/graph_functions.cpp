#include "../include/graphFunctions.h"
#include "../include/DataVector/DataVector.h"
#include "../include/Graph/graph_node.h" // Ensure this path is correct and the file exists
#include <vector>
#include <queue>
#include <limits>
#include <cmath>
#include <algorithm>

using namespace std;

// Global graph structure (for simplicity)
std::vector<GraphNode<DataVector<int>>> graph;

// Function to get a node by its index
template <typename graph_t>
GraphNode<graph_t>* getNode(unsigned int index) {
    return &graph[index];
}

// Function to calculate Euclidean distance between two DataVector objects
template <typename dvector_t>
double distance(const DataVector<dvector_t>& a, const DataVector<dvector_t>& b) {
    return a.euclideanDistance(b);
}

// GreedySearch algorithm to find the k-nearest neighbors
template <typename dvector_t>
std::vector<DataVector<dvector_t>> GreedySearch(
    const std::vector<DataVector<dvector_t>>& dataset,
    const DataVector<dvector_t>& query,
    int k
) 
{
    // Priority queue to store the nearest neighbors
    std::priority_queue<std::pair<double, DataVector<dvector_t>>> nearest_neighbors;

    // Iterate through the dataset to find the k-nearest neighbors
    for (const auto& data_vector : dataset) {
        double distance = query.euclideanDistance(data_vector); 

        if (nearest_neighbors.size() < k) {
            nearest_neighbors.push({distance, data_vector});
        } else if (distance < nearest_neighbors.top().first) {
            nearest_neighbors.pop();
            nearest_neighbors.push({distance, data_vector});
        }
    }

    // Extract the k-nearest neighbors from the priority queue
    std::vector<DataVector<dvector_t>> result;
    while (!nearest_neighbors.empty()) {
        result.push_back(nearest_neighbors.top().second);
        nearest_neighbors.pop();
    }

    // Reverse the result to have the nearest neighbors in ascending order of distance
    std::reverse(result.begin(), result.end());

    return result;
}

// RobustPrune function
template <typename graph_t>
std::vector<graph_t> robustPrune(unsigned int p_index, std::vector<unsigned int> candidate_indices, float alpha, unsigned int R) {
    std::vector<graph_t> prunedNeighbors;

    // Get a reference to the point p
    GraphNode<graph_t>* p = getNode<graph_t>(p_index);

    // Repeat until we have R neighbors or candidate indices are exhausted
    while (!candidate_indices.empty() && prunedNeighbors.size() < R) {
        // Find the closest neighbor to p within candidate_indices
        unsigned int closest_index = candidate_indices[0];
        float min_distance = distance(p->getData(), getNode<graph_t>(closest_index)->getData());

        for (unsigned int i = 1; i < candidate_indices.size(); ++i) {
            unsigned int current_index = candidate_indices[i];
            float current_distance = distance(p->getData(), getNode<graph_t>(current_index)->getData());

            if (current_distance < min_distance) {
                closest_index = current_index;
                min_distance = current_distance;
            }
        }

        // Get the closest node
        GraphNode<graph_t>* closestNode = getNode<graph_t>(closest_index);

        // Add the closest neighbor to the set of final neighbors
        prunedNeighbors.push_back(closestNode->getData());

        // Remove the closest neighbor from candidate indices
        candidate_indices.erase(std::remove(candidate_indices.begin(), candidate_indices.end(), closest_index), candidate_indices.end());

        // Remove neighbors that do not meet the distance threshold a * d(p, closestNode)
        float threshold_distance = alpha * min_distance;
        for (auto it = candidate_indices.begin(); it != candidate_indices.end();) {
            if (distance(p->getData(), getNode<graph_t>(*it)->getData()) > threshold_distance) {
                // Remove candidate if the distance exceeds the threshold
                it = candidate_indices.erase(it);
            } else {
                ++it;
            }
        }

        // Stop if we have reached the limit R
        if (prunedNeighbors.size() == R) {
            break;
        }
    }

    return prunedNeighbors;
}
