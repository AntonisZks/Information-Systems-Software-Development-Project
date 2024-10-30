#include "../include/graphFunctions.h"
#include "distance.h"
#include "vanama.h"

#include <iostream>
#include <map>
#include <algorithm>
#include <vector>
#include <queue>
#include <set>
#include <cmath>
#include <functional>

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

// template <typename T>
// pair<vector<pair<double, T>>, set<T>> GreedySearch(Graph<T>& graph, const NodeData& query, int k) {
//     using Pair = pair<double, T>;
//     priority_queue<Pair, vector<Pair>, greater<Pair>> search_queue;
//     set<T> visited;

//     // Populate the priority queue with distances
//     for (const auto& node : graph.nodes) {
//         double dist = node.second.distanceTo(query);
//         search_queue.emplace(dist, node.first);
//     }

//     vector<Pair> result;

//     // Select the k closest neighbors
//     while (!search_queue.empty() && result.size() < k) {
//         Pair closest = search_queue.top();
//         search_queue.pop();

//         // Only add unvisited nodes
//         if (visited.insert(closest.second).second) {
//             result.push_back(closest);
//         }
//     }

//     return { result, visited };
// }

// template<typename graph_t> void GreedySearch(Graph<graph_t> &graph, GraphNode<graph_t>& s, DataVector<float> xq, int k, int L) {

//     std::cout << "Hello, World!" << std::endl;

// }

//NOT void, will take arguement a graph and return a pruned graph
void Robust_Prune(void){

}
