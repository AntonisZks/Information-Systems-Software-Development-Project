#ifndef DISTANCE_H
#define DISTANCE_H

#include <iostream>
#include <cmath>
#include <utility>
#include <vector>
#include "DataStructures/DataVector/DataVector.h"

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
