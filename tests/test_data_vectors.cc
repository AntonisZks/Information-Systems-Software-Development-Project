#include <iostream>
#include <fstream>
#include <vector>
#include "../include/acutest.h"
#include "../include/read_data.h"
#include "../include/DataVector/DataVector.h"
#include <cmath>  // Ensure cmath is included for fabs

/**
 * @brief Function to create a sample binary file for testing.
 * This function writes a predefined set of vectors to a binary file.
 *
 * @param filename The name of the binary file to create.
 */
void createSampleBinaryFile(const std::string& filename) {
    std::ofstream file(filename, std::ios::binary);
    int dim1 = 3; // Example dimension
    file.write(reinterpret_cast<char*>(&dim1), sizeof(dim1));
    float values[3] = {1.0f, 2.0f, 3.0f}; // Example values
    file.write(reinterpret_cast<char*>(values), sizeof(values));
    file.close();
}

/**
 * @brief Test case for reading vector data from a binary file.
 * This test verifies that the ReadVectorFile function correctly reads
 * the vector data and returns the expected result.
 */
void testReadVectorFile() {
    const std::string testFilename = "sample_vectors.bin";
    createSampleBinaryFile(testFilename); // Create a sample binary file for testing

    std::vector<DataVector<float>> dataVectors = ReadVectorFile(testFilename);
    
    // Check that the dataVectors contains the expected number of vectors
    TEST_CHECK(dataVectors.size() == 1); // Expecting 1 vector

    // Check that the vector has the correct dimension and values
    TEST_CHECK(dataVectors[0].getDimension() == 3); // Expected dimension

    // Check values
    TEST_CHECK(fabs(dataVectors[0].getDataAtIndex(0) - 1.0f) < 1e-6);
    TEST_CHECK(fabs(dataVectors[0].getDataAtIndex(1) - 2.0f) < 1e-6);
    TEST_CHECK(fabs(dataVectors[0].getDataAtIndex(2) - 3.0f) < 1e-6);
}

/**
 * @brief Tests the comparison operators for DataVector objects.
 *
 * This function creates two DataVector<float> objects (vec1 and vec2) with 3 elements 
 * each and assigns values to their indices. It then verifies the comparison 
 * operators (< and >) to ensure that they function correctly based on the Euclidean 
 * magnitudes of vec1 and vec2.
 *
 * @note This test assumes that the < and > operators are overloaded for DataVector 
 * to compare magnitudes.
 */
void test_data_vectors_comparison(void) {

    DataVector<float> vec1(3);
    vec1.setDataAtIndex(1.0f, 0);
    vec1.setDataAtIndex(2.0f, 1);
    vec1.setDataAtIndex(3.0f, 2);
    
    DataVector<float> vec2(3);
    vec2.setDataAtIndex(4.0, 0);
    vec2.setDataAtIndex(5.0, 1);
    vec2.setDataAtIndex(6.0, 2);
    
    // Test comparison operators
    TEST_CHECK(vec1 < vec2);
    TEST_CHECK(vec2 > vec1);
}

/**
 * @brief Tests the equality operator for DataVector objects and checks edge cases 
 * for comparison operators.
 *
 * This function creates two DataVector<float> objects (vec1 and vec2) with 3 elements 
 * each and assigns values to their indices. It then verifies:
 *   - `!(vec1 == vec2)` to ensure that vec1 and vec2 are recognized as unequal based 
 *     on their element values.
 *   - `!(vec1 < vec2) && !(vec1 > vec2)` to check that vec1 and vec2, having equal 
 *     magnitudes but different elements, are neither greater nor less than each other.
 *
 * @note This test assumes that the ==, <, and > operators are overloaded for DataVector 
 * to compare based on element values and Euclidean magnitudes.
 */
void test_data_vectors_equality(void) {

    DataVector<float> vec1(3);
    vec1.setDataAtIndex(1.0f, 0);
    vec1.setDataAtIndex(2.0f, 1);
    vec1.setDataAtIndex(3.0f, 2);
    
    DataVector<float> vec2(3);
    vec2.setDataAtIndex(2.0, 0);
    vec2.setDataAtIndex(3.0, 1);
    vec2.setDataAtIndex(1.0, 2);
    
    // Checking according to their coordinate values
    TEST_CHECK(!(vec1 == vec2));
    
    // The two vectors have the same magnitude, so neither of them is greater 
    // or less than the other one
    TEST_CHECK(!(vec1 < vec2) && !(vec1 > vec2));
}


// Register the test cases in the TEST_LIST defined by Acutest
TEST_LIST = {
    {"Test Read Vector File", testReadVectorFile},
    {"test Data Vector comparison", test_data_vectors_comparison},
    {"test Data Vector equality", test_data_vectors_equality},
    {nullptr, nullptr} // Termination
};
