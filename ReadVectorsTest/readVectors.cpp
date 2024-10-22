#include <iostream>
#include <fstream>
#include <vector>
#include "../include/DataVector/DataVectorNew.h"  // Include your DataVector class

using namespace std;

//Function to save the vector data to a text file
void saveVector(const vector<DataVector<float>>& dataVectors, const string& outputFilename) {
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

//Function to read the file, assign data to DataVector objects, and return a vector of DataVector objects
vector<DataVector<float>> ReadVectorFile(const string& filename) {
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

        // Add the DataVector object to the vector
        dataVectors.push_back(dataVector);
        nb_vectors++;
    }

    cout << "Total number of vectors in: " << filename << "is "<< nb_vectors << endl;

    file.close();
    return dataVectors; // Return the array of DataVector objects
}
