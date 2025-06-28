#ifndef DISTANCE_H
#define DISTANCE_H

#include <iostream>
#include <cmath>
#include <utility>
#include <vector>
#include "DataVector.h"

enum DISTANCE_SAVE_METHOD {
  NONE = 0,
  MATRIX = 1,
};


/**
 * @brief Comparator structure for ordering elements by Euclidean distance.
 * 
 * This functor orders elements based on their Euclidean distance to a target vector.
 * If two elements have the same distance, they are compared lexicographically.
 */
template <typename base_t, typename query_t>
struct EuclideanDistanceOrder {

  query_t xq; // Target vector for distance comparisons
  double** distances;
  bool useCashe;

  /**
   * @brief Constructs a new EuclideanDistanceOrder object with a target vector.
   * 
   * @param target The target vector for distance comparisons.
   */
  EuclideanDistanceOrder(const query_t& target, double** distanceMatrix, const bool useCashe_) 
    : xq(target), distances(distanceMatrix), useCashe(useCashe_) {}

  /**
   * @brief Comparison operator for ordering elements by Euclidean distance.
   * 
   * @param a The first element to compare
   * @param b The second element to compare
   * 
   * @return True if the first element is closer to the target vector than the second element.
   */
  bool operator()(const base_t& a, const base_t& b) const;

};

/**
 * @brief Function to calculate Euclidean distance between two DataVector objects. It uses
 * the Euclidean Distance formula for vectors of dimension n and calculates their distance.
 * 
 * @param a the first vector
 * @param b the second vector.
 * 
 * @return the Euclidean distance between those two vector.
*/
double euclideanDistance(const DataVector<float>& a, const DataVector<float>& b);

/**
 * @brief Function to calculate Manhattan distance between two DataVector objects. It uses
 * the Manhattan Distance formula for vectors of dimension n and calculates their distance.
 * 
 * @param a the first vector
 * @param b the second vector.
 * 
 * @return the Manhattan distance between those two vector.
*/
double manhattanDistance(const DataVector<float>& a, const DataVector<float>& b);

#endif /* DISTANCE_H */
