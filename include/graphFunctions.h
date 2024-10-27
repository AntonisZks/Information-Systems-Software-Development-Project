// For:
// Greedy_Algorithm
// Robust_Prune

// CPP libraries
// this contains most of the STL headers files (data structures) amd iostream
// WARNING individual #inlcludes to ensure portability
// temporary placeholder for clean code
#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <unordered_map>
#include "Graph/graph.h"
#include "DataVector/DataVector.h"

// DECLARATIONS

/**
 * @brief Greedy Search Algorithm
 * 
 * @param graph the graph to implement the greedy search
 * @param s the starting node
 * @param query the current query
 * @param k the result size
 * @param L the search list size
*/
template<typename graph_t, typename T> std::vector<std::pair<double, T> >, std::set<T> >  GreedySearch(Graph<T> &graph, typename graph_t::NodeData query, int k);
void Robust_Prune(void);
