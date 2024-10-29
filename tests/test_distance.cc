#include <iostream>
#include "../include/acutest.h"
#include "../include/distance.h"  //Includes function prototypes and DataVector class
#include <cmath>
#include <utility>
#include <vector>

/**
 * @brief Function to calculate Euclidean distance between two DataVector objects. It uses
 * the Euclidean Distance formula for vectors of dimension n and calculates their distance.
 * 
 * @param a the first vector
 * @param b the second vector.
 * 
 * @return the Euclidean distance between those two vector.
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
 * @brief Test case for the Euclidean Distance between two DataVector objects. Ensures
 * the euclidean distance between two vectors is successfully computed.
*/
void testEuclideanDistance(){
    // Initialize one DataVector object with 128 dimensions
    DataVector<float> vec1(128);
    for (int i = 0; i < 128; ++i) {
        vec1.setDataAtIndex(static_cast<float>(i + 1), i); // Fill with values 1, 2, 3, ..., 128
    }

    // Initialize another DataVector object with 128 dimensions
    DataVector<float> vec2(128);
    for (int i = 0; i < 128; ++i) {
        vec2.setDataAtIndex(static_cast<float>(i + 4), i); // Fill with values 4, 5, 6, ..., 131
    }

    // Define the expected and calculated distances
    double expectedDistance = 0.0;
    for (int i = 0; i < 128; ++i) {
        expectedDistance += pow(vec1.getDataAtIndex(i) - vec2.getDataAtIndex(i), 2);
    }
    expectedDistance = sqrt(expectedDistance); // Calculated Euclidean distance between vec1 and vec2

    double calculatedDistance = euclideanDistance(vec1, vec2);

    // Check the results
    TEST_CHECK(fabs(expectedDistance - calculatedDistance) < 1e-6);
}


/**
 * @brief Test case for the Euclidean Distance between two DataVector objects. It handles the 
 * case where two vectors don't have the same dimension.
*/
void testEuclideanDistanceDifferentDimensions() {

    // Initialize one DataVector object
    DataVector<float> vec1(3);
    vec1.setDataAtIndex(1.0f, 0);
    vec1.setDataAtIndex(2.0f, 1);
    vec1.setDataAtIndex(3.0f, 2);

    // Initialize another DataVector object
    DataVector<float> vec2(4);
    vec2.setDataAtIndex(4.0f, 0);
    vec2.setDataAtIndex(5.0f, 1);
    vec2.setDataAtIndex(6.0f, 2);
    vec2.setDataAtIndex(7.0f, 3);

    // Try to calculate euclidean distance on these vectors and expect error
    bool exceptionThrown = false;
    try {
        euclideanDistance(vec1, vec2);
    } catch (const std::invalid_argument&) {
        exceptionThrown = true;
    }

    TEST_CHECK(exceptionThrown);
}

TEST_LIST = {
    {"Euclidean Distance 128 dimenstions", testEuclideanDistance},
    {"Test Euclidean Distance (Different Dimensions)", testEuclideanDistanceDifferentDimensions},
    {nullptr, nullptr} // Termination
};
