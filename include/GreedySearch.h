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
#include "BQDataVectors.h"
#include "Filter.h"
#include "distance.h"
#include <queue>
#include <cmath>
#include "VamanaIndex.h"
#include "FilteredVamanaIndex.h"

/**
 * @brief Enum to define the execution mode of the algorithm.
 * 
 * The algorithm can be executed in two modes: CREATE and TEST. CREATE mode is used to create the index, 
 * while TEST mode is used to test the index.
 */
enum EXEC_MODE {
    CREATE = 0,
    TEST = 1
};

template <typename vamana_t> class VamanaIndex;
template <typename vamana_t> class FilteredVamanaIndex;

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
template <typename graph_t, typename query_t> std::pair<std::set<graph_t>, std::set<graph_t>> GreedySearch(
    const VamanaIndex<graph_t>& index, 
    const GraphNode<graph_t>& s, 
    const query_t& xq, 
    unsigned int k, 
    unsigned int L,
    const EXEC_MODE execMode = CREATE
);

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
template <typename graph_t, typename query_t> std::pair<std::set<graph_t>, std::set<graph_t>> FilteredGreedySearch(
    const FilteredVamanaIndex<graph_t>& index, 
    const std::vector<GraphNode<graph_t>>& S, 
    const query_t& xq,  
    const unsigned int k, 
    const unsigned int L,  
    const std::vector<CategoricalAttributeFilter>& queryFilters,
    const EXEC_MODE execMode = CREATE
);
                     

#endif // GREEDY_SEARCH_H
