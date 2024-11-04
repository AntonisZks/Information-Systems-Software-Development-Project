#include <iostream>
#include <fstream>
#include <vector>
#include "../include/DataStructures/DataVector/DataVector.h"

using namespace std;

/**
 * @brief Function to read a file and convert its data into a vector of DataVector<float> objects.
 * The file is assumed to be in a binary format where each vector starts with its dimensionality (int),
 * followed by the float values representing the vector data.
 * 
 * @param filename The name of the input file to read vector data from.
 * 
 * @return A vector of DataVector<float> objects containing the read data.
 */
vector<DataVector<float>> ReadVectorFile(const string& filename){
    ifstream file(filename, ios::binary);

    if (!file.is_open()) {
        cerr << "Error opening file: " << filename << endl;
        return {};
    }

    vector<DataVector<float>> dataVectors; // Vector to store DataVector objects
    int nb_vectors = 0;

    while (file) {
        int d;  // Dimensionality of the vector (first 4 bytes)

        // Read dimensionality (4 bytes)
        file.read(reinterpret_cast<char*>(&d), sizeof(d));

        if (file.eof())
            break; // Exit loop if end of file is reached

        // Create a DataVector object with the given dimension
        DataVector<float> dataVector(d);

        // Read the actual vector data (d * 4 bytes for float data)
        for (int i = 0; i < d; ++i) {
            float value;
            file.read(reinterpret_cast<char*>(&value), sizeof(value));
            dataVector.setDataAtIndex(value, i);  // Set the data at the respective index
        }

        dataVector.setIndex(nb_vectors); // Set the index of the data vector for easy and fast accessing

        // Add the DataVector object to the vector
        dataVectors.push_back(dataVector);
        nb_vectors++;
    }

    file.close();

    return dataVectors; // Return the array of DataVector objects
}

vector<DataVector<int>> ReadGroundTruth(const string& filename) {
    ifstream file(filename, ios::binary);

    if (!file.is_open()) {
        cerr << "Error opening file: " << filename << endl;
        return {};
    }

    vector<DataVector<int>> dataVectors; // Vector to store DataVector objects
    int nb_vectors = 0;

    while (file) {
        int d;  // Dimensionality of the vector (first 4 bytes)

        // Read dimensionality (4 bytes)
        file.read(reinterpret_cast<char*>(&d), sizeof(d));

        if (file.eof())
            break; // Exit loop if end of file is reached

        // Create a DataVector object with the given dimension
        DataVector<int> dataVector(d);

        // Read the actual vector data (d * 4 bytes for float data)
        for (int i = 0; i < d; ++i) {
            int value;
            file.read(reinterpret_cast<char*>(&value), sizeof(value));
            dataVector.setDataAtIndex(value, i);  // Set the data at the respective index
        }

        // Add the DataVector object to the vector
        dataVectors.push_back(dataVector);
        nb_vectors++;
    }

    file.close();

    return dataVectors; // Return the array of DataVector objects
}

/**
 * @brief Function to save the vector data to a text file. It iterates through each DataVector
 * object in the provided vector and writes the index, followed by the vector values, to the file.
 * 
 * @param dataVectors A vector of DataVector<float> objects containing the vector data to save.
 * @param outputFilename The name of the output text file to save the vector data.
 */

void saveVectors(const vector<DataVector<float>>& dataVectors, const string& outputFilename){
    ofstream outFile(outputFilename);
    
    if (!outFile.is_open()) {
        cerr << "Error opening file for writing: " << outputFilename << endl;
        return;
    }

    for (size_t idx = 0; idx < dataVectors.size(); ++idx) {
        const auto& dataVector = dataVectors[idx];
        outFile << "Vector " << idx << ": ";  // Add index label
        int dimension = dataVector.getDimension(); // Assuming you add a getDimension method to DataVector
        for (int i = 0; i < dimension; ++i) {
            outFile << dataVector.getDataAtIndex(i);
            if (i < dimension - 1) outFile << " "; // Separate values with spaces
        }
        outFile << endl; // New line for the next vector
    }

    outFile.close();
    cout << "Data saved to " << outputFilename << endl;
}

