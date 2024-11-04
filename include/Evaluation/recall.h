#include <iostream>
#include <set>
#include <algorithm>
#include "../DataStructures/DataVector/DataVector.h"


template <typename graph_t>
double calculateRecallEvaluation(const std::set<graph_t>& X, const std::set<graph_t>& G) {

  std::set<graph_t> result;
  std::set_intersection(X.begin(), X.end(), G.begin(), G.end(), std::inserter(result, result.begin()));

  unsigned int k = G.size();

  return (double)result.size() / (double)k;

}