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

// Register the test cases in the TEST_LIST defined by Acutest
TEST_LIST = {
    {"Test Read Vector File", testReadVectorFile},
    {nullptr, nullptr} // Termination
};
