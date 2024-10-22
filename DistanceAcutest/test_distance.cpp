#include <iostream>
#include "../include/acutest.h"
#include "../include/DataVector/DataVectorNew.h"  // Include your DataVector class
#include <cmath>
#include <utility>
#include <vector>
#include "../include/readVectors.h"  // Include your DataVector class

// Function to calculate Euclidean distance between two DataVector objects
double euclideanDistance(const DataVector<float>& a, const DataVector<float>& b) {
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

// Test case for Euclidean distance
void testEuclideanDistance() {
    DataVector<float> vec1(3);
    vec1.setDataAtIndex(1.0f, 0);
    vec1.setDataAtIndex(2.0f, 1);
    vec1.setDataAtIndex(3.0f, 2);

    DataVector<float> vec2(3);
    vec2.setDataAtIndex(4.0f, 0);
    vec2.setDataAtIndex(5.0f, 1);
    vec2.setDataAtIndex(6.0f, 2);

    double expectedDistance = sqrt(27.0); // Distance between vec1 and vec2
    double calculatedDistance = euclideanDistance(vec1, vec2);

    TEST_CHECK(fabs(expectedDistance - calculatedDistance) < 1e-6);
}

// Test case for handling vectors of different dimensions
void testEuclideanDistanceDifferentDimensions() {
    DataVector<float> vec1(3);
    vec1.setDataAtIndex(1.0f, 0);
    vec1.setDataAtIndex(2.0f, 1);
    vec1.setDataAtIndex(3.0f, 2);

    DataVector<float> vec2(4);
    vec2.setDataAtIndex(4.0f, 0);
    vec2.setDataAtIndex(5.0f, 1);
    vec2.setDataAtIndex(6.0f, 2);
    vec2.setDataAtIndex(7.0f, 3);

    bool exceptionThrown = false;
    try {
        euclideanDistance(vec1, vec2);
    } catch (const std::invalid_argument&) {
        exceptionThrown = true;
    }

    TEST_CHECK(exceptionThrown);
}

TEST_LIST = {
    {"Test Euclidean Distance", testEuclideanDistance},
    {"Test Euclidean Distance (Different Dimensions)", testEuclideanDistanceDifferentDimensions},
    {nullptr, nullptr} // Termination
};
