#include <iostream>
#include <set>
#include <algorithm>
#include "DataVector.h"

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
template <typename set_t> double calculateRecallEvaluation(const std::set<set_t>& X, const std::set<set_t>& G);
