#ifndef READ_DATA_H
#define READ_DATA_H

#include <vector>           // Required for std::vector
#include <string>           // Required for std::string
#include "../include/DataVector/DataVector.h"  // Adjust the path if necessary

using namespace std;        // Optional: can avoid repeating std::

/**
 * @brief Function to read a file and convert its data into a vector of DataVector<float> objects.
 * The file is assumed to be in a binary format where each vector starts with its dimensionality (int),
 * followed by the float values representing the vector data.
 * 
 * @param filename The name of the input file to read vector data from.
 * 
 * @return A vector of DataVector<float> objects containing the read data.
 */
vector<DataVector<float>> ReadVectorFile(const string& filename);

/**
 * @brief Function to save the vector data to a text file. It iterates through each DataVector
 * object in the provided vector and writes the index, followed by the vector values, to the file.
 * 
 * @param dataVectors A vector of DataVector<float> objects containing the vector data to save.
 * @param outputFilename The name of the output text file to save the vector data.
 */
void saveVectors(const vector<DataVector<float>>& dataVectors, const string& outputFilename);

#endif // READ_DATA_H
