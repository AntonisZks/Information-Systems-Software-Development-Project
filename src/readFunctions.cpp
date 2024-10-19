#include <iostream>
#include <fstream>
#include <vector>
#include "../include/DataVector/DataVectorNew.h"

using namespace std; // Use namespace std for simplicity

// Function to read the file, assign data to DataVector objects, and return a vector of DataVector objects
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

    cout << "Total number of base vectors: " << nb_vectors << endl;

    file.close();
    return dataVectors; // Return the array of DataVector objects
}

int main(void){
    // Call the function with the path to your .fvecs file
    vector<DataVector<float>> vectors = ReadVectorFile("../data/siftsmall/siftsmall_base.fvecs");

    // Access the first vector's data as an example
    if (!vectors.empty()) {
        DataVector<float>& firstVector = vectors[0]; //From the first vector
        cout << "First vector, first value: " << firstVector.getDataAtIndex(0) << endl; //Get the second float
        cout << "First vector, second value: " << firstVector.getDataAtIndex(1) << endl; //Get the second float
        cout << "First vector, third value: " << firstVector.getDataAtIndex(2) << endl; //Get the second float
    }

    return 0;
}

