#include <iostream>
#include <set>
#include <algorithm>
#include "../DataStructures/DataVector/DataVector.h"

/**
 * @brief Calculates the recall evaluation metric.
 *
 * This function computes the recall, which is the fraction of relevant instances 
 * that have been retrieved over the total amount of relevant instances. It is 
 * defined as the size of the intersection of sets X and G divided by the size of set G.
 *
 * @tparam graph_t The type of the elements in the sets.
 * @param X A set of retrieved instances.
 * @param G A set of relevant instances.
 * @return The recall value as a double.
 */
template <typename graph_t>
double calculateRecallEvaluation(const std::set<graph_t>& X, const std::set<graph_t>& G) {

// Declare a set to store the intersection result
std::set<graph_t> result;

// Compute the intersection of sets X and G, and insert the result into 'result'
std::set_intersection(X.begin(), X.end(), G.begin(), G.end(), std::inserter(result, result.begin()));

// Get the size of set G
unsigned int k = G.size();

// Return the ratio of the size of the intersection to the size of G
return (double)result.size() / (double)k;
}
