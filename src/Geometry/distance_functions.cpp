#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include "../../include/distance.h"  //Includes function prototypes and DataVector class

using namespace std;

/**
 * @brief Comparison operator for ordering elements by Euclidean distance.
 * 
 * @param a The first element to compare
 * @param b The second element to compare
 * 
 * @return True if the first element is closer to the target vector than the second element.
 */
template <typename point_t> bool EuclideanDistanceOrder<point_t>::operator()(const point_t& a, const point_t& b) {

    double distanceA = euclideanDistance(a, xq);
    double distanceB = euclideanDistance(b, xq);

    // Primary comparison by distance
    if (distanceA != distanceB) {
        return distanceA < distanceB;
    }

    // Secondary comparison if distances are the same
    return a < b;

}

/**
 * @brief Function to calculate Euclidean distance between two DataVector objects. It uses
 * the Euclidean Distance formula for vectors of dimension n and calculates their distance.
 * 
 * @param a the first vector
 * @param b the second vector
 * 
 * @return the Euclidean distance between those two vector
 * @throws invalid_argument error in case of different dimensions
*/
double euclideanDistance(const DataVector<float>& a, const DataVector<float>& b){
    if (a.getDimension() != b.getDimension()) {
        throw std::invalid_argument("Vectors must have the same dimension");
    }

    double sum = 0.0;
    unsigned int dimension = a.getDimension();
    for (unsigned int i = 0; i < dimension; ++i) {
        double diff = a.getDataAtIndex(i) - b.getDataAtIndex(i);
        sum += diff * diff;
    }
    return sqrt(sum);
}


/**
 * @brief Function to calculate Manhattan distance between two DataVector objects. It uses
 * the Manhattan Distance formula for vectors of dimension n and calculates their distance.
 * 
 * @param a the first vector
 * @param b the second vector
 * 
 * @return the Manhattan distance between those two vector
 * @throws invalid_argument error in case of different dimensions
*/
double manhattanDistance(const DataVector<float>& a, const DataVector<float>& b){
    if (a.getDimension() != b.getDimension()) {
        throw std::invalid_argument("Vectors must have the same dimension");
    }

    double sum = 0.0;
    unsigned int dimension = a.getDimension();
    for (unsigned int i = 0; i < dimension; ++i) {
        sum += abs(a.getDataAtIndex(i) - b.getDataAtIndex(i));
    }
    return sum;
}

template struct EuclideanDistanceOrder<DataVector<float>>;