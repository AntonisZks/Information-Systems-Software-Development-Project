#ifndef GREEDY_SEARCH_H
#define GREEDY_SEARCH_H

#include <iostream>
#include <vector>
#include <set>
#include <algorithm>
#include <iterator>
#include <stdexcept>
#include "graph.h"
#include "DataVector.h"
#include "distance.h"
#include <queue>
#include <cmath>

// Enum to define different filter types based on the query type
enum FilterType {
    NO_FILTER,          // For query_type = 0, no filter is applied (only the vector is used).
    C_EQUALS_v,         // For query_type = 1, filter for C = v (categorical attribute).
    l_LEQ_T_LEQ_r,      // For query_type = 2, filter for l ≤ T ≤ r (timestamp constraint).
    C_EQUALS_v_AND_l_LEQ_T_LEQ_r // For query_type = 3, filter for both C = v and l ≤ T ≤ r (combined constraints).
};

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
GreedySearch(const Graph<graph_t>& G, const GraphNode<graph_t>& s, const graph_t& xq, unsigned int k, unsigned int L);

template <typename graph_t>
std::pair<std::set<graph_t>, std::set<graph_t>> 
FilteredGreedySearch(const Graph<graph_t>& G, const std::vector<GraphNode<graph_t>>& S,  const graph_t& xq,  
                     const unsigned int k,  const unsigned int L,  const std::vector<FilterType>& queryFilters);
                     

#endif // GREEDY_SEARCH_H
