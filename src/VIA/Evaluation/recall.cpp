#include "../../../include/recall.h"
#include "../../../include/DataVector.h"

/**
 * @brief Calculates the recall between two sets of items. Recall is a measure of how many relevant items 
 * were retrieved out of the total relevant items.
 * 
 * @tparam set_t The type of elements within the sets, usually a custom data type that has an ordering operator.
 * @param X The set of retrieved items.
 * @param G The set of ground truth items (relevant items).
 * 
 * @return A double representing the recall, ranging from 0.0 (no relevant items retrieved)
 * to 1.0 (all relevant items retrieved).
 */
template <typename set_t> double calculateRecallEvaluation(const std::set<set_t>& X, const std::set<set_t>& G) {

  // Create a set to store the intersection of X and G, representing the correctly retrieved items.
  std::set<set_t> result;
  std::set_intersection(

    X.begin(), X.end(), 
    G.begin(), G.end(), 
    std::inserter(result, result.begin())

  );

  // Compute the intersection of sets X and G, and insert the result into 'result'
  std::set_intersection(X.begin(), X.end(), G.begin(), G.end(), std::inserter(result, result.begin()));

  // Get the size of set G
  unsigned int k = G.size();

  // Return the ratio of the size of the intersection to the size of G
  return (double)result.size() / (double)k;

}

// Explicit template instantiation for int and float types
template double calculateRecallEvaluation(const std::set<int>& X, const std::set<int>& G);
template double calculateRecallEvaluation(const std::set<float>& X, const std::set<float>& G);
template double calculateRecallEvaluation(const std::set<DataVector<float>>& X, const std::set<DataVector<float>>& G);
