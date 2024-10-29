#include <iostream>
#include "../include/acutest.h"
#include "../include/distance.h"  //Includes function prototypes and DataVector class
#include <cmath>
#include <utility>
#include <vector>

/**
 * @brief Test case for the Euclidean Distance between two DataVector objects. Ensures
 * the euclidean distance between two vectors is successfully computed.
*/
void testEuclideanDistance(){

    // Initialize one DataVector object
    DataVector<float> vec1(3);
    vec1.setDataAtIndex(1.0f, 0);
    vec1.setDataAtIndex(2.0f, 1);
    vec1.setDataAtIndex(3.0f, 2);

    // Initialize another DataVector object
    DataVector<float> vec2(3);
    vec2.setDataAtIndex(4.0f, 0);
    vec2.setDataAtIndex(5.0f, 1);
    vec2.setDataAtIndex(6.0f, 2);

    // Define the expected and calculated distances
    double expectedDistance = sqrt(27.0); // Distance between vec1 and vec2
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
    {"Test Euclidean Distance", testEuclideanDistance},
    {"Test Euclidean Distance (Different Dimensions)", testEuclideanDistanceDifferentDimensions},
    {nullptr, nullptr} // Termination
};
