
#include "../include/graphFunctions.h"
#include "../include/Graph/graph.h"

#include <iostream>
#include <map>
#include <algorithm>
#include <vector>
#include <queue>
#include <set>
#include <cmath>
#include <functional>
#include <cmath>
#include <utility>
#include <limits>
#include <string>
#include <map>



using namespace std;

// DEFINITIONS

/**
 * @brief Greedy Search Algorithm
 * 
 * @param graph the graph to implement the greedy search
 * @param s the starting node
 * @param query the current query
 * @param k the result size
 * @param L the search list size
*/


// GreedySearch function to find the k closest nodes in a Graph to a given query node's data
template <typename T>
std::pair<std::vector<std::pair<double, T>>, std::set<T>> GreedySearch(Graph<T>& graph, const T& query, int k, int L) {
    using Pair = std::pair<double, T>;
    std::priority_queue<Pair, std::vector<Pair>, std::greater<Pair>> search_queue;
    std::set<T> visited;
    std::vector<Pair> result;

    // Initialize search with the starting node (assume first node as start)
    T start_node = graph.getNodeData(0); // Modify this if a different starting point 
    search_queue.emplace(start_node.distanceTo(query), start_node);
    
    // Main search loop
    while (!search_queue.empty() && result.size() < k) {
        // Get the closest unvisited node
        Pair closest = search_queue.top();
        search_queue.pop();

        // If this node hasn't been visited, process it
        if (visited.insert(closest.second).second) { // insert returns false if already visited
            result.push_back(closest);

            // Add neighbors to the search queue
            auto neighbors = graph.getNodeNeighbors(closest.second);
            for (const auto& neighbor : *neighbors) {
                if (visited.find(neighbor) == visited.end()) {
                    double dist = neighbor.distanceTo(query);
                    search_queue.emplace(dist, neighbor);
                }
            }

            // Maintain only the closest L points in the search queue if it exceeds size L
            while (search_queue.size() > L) {
                search_queue.pop();
            }
        }
    }

    return {result, visited};
}



template <typename graph_t>
std::vector<graph_t> robustPrune(unsigned int p_index, std::vector<unsigned int> candidate_indices, float alpha, unsigned int R) {
    
}


