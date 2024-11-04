#include <iostream>
#include <set>
#include <algorithm>
#include "../DataStructures/DataVector/DataVector.h"


/**
 * @brief Calculates the recall between two sets of items. Recall is a measure of how many relevant items 
 * were retrieved out of the total relevant items.
 * 
 * @tparam graph_t The type of elements within the sets, usually a custom data type that has an ordering operator.
 * @param X The set of retrieved items.
 * @param G The set of ground truth items (relevant items).
 * 
 * @return A double representing the recall, ranging from 0.0 (no relevant items retrieved)
 * to 1.0 (all relevant items retrieved).
 */
template <typename graph_t>
double calculateRecallEvaluation(const std::set<graph_t>& X, const std::set<graph_t>& G) {

  // Create a set to store the intersection of X and G, representing the correctly retrieved items.
  std::set<graph_t> result;
  std::set_intersection(

    X.begin(), X.end(), 
    G.begin(), G.end(), 
    std::inserter(result, result.begin())

  );

  unsigned int k = G.size();

  return static_cast<double>(result.size()) / static_cast<double>(k);

}
