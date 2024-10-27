#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include "../include/DataVector/DataVectorNew.h"  // Include your DataVector header

using namespace std;

// Function to calculate Euclidean distance between two DataVector objects
double euclideanDistance(const DataVector<float>& a, const DataVector<float>& b) {
    if (a.getDimension() != b.getDimension()) {
        throw invalid_argument("Vectors must have the same dimension");
    }

    double sum = 0.0;
    unsigned int dimension = a.getDimension();
    for (unsigned int i = 0; i < dimension; ++i) {
        double diff = a.getDataAtIndex(i) - b.getDataAtIndex(i);
        sum += diff * diff;
    }
    return sqrt(sum);
}

// Function to calculate Manhattan distance between two DataVector objects
double manhattanDistance(const DataVector<float>& a, const DataVector<float>& b) {
    if (a.getDimension() != b.getDimension()) {
        throw invalid_argument("Vectors must have the same dimension");
    }

    double sum = 0.0;
    unsigned int dimension = a.getDimension();
    for (unsigned int i = 0; i < dimension; ++i) {
        sum += abs(a.getDataAtIndex(i) - b.getDataAtIndex(i));
    }
    return sum;
}