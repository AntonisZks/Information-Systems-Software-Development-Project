#include <iostream>
#include <fstream>
#include <vector>
#include "DataVectorNew.h"  // Include your DataVector class
#include "readVectors.h"

int main(void) {
    //read the base vectors
    vector<DataVector<float>> baseDataVector = ReadVectorFile("../data/siftsmall/siftsmall_base.fvecs");
    //save the base vectors
    saveVector(baseDataVector, "base_vectors.txt");

    //read the query vectors
    vector<DataVector<float>> queryDataVector = ReadVectorFile("../data/siftsmall/siftsmall_query.fvecs");
    //save the query vectors
    saveVector(queryDataVector, "query_vectors.txt");


    // Access the first vector's data as an example
    /*if (!baseDataVector.empty()) {
        DataVector<float>& firstVector = baseDataVector[0]; // From the first vector
        cout << "First vector, first value: " << firstVector.getDataAtIndex(0) << endl;
        cout << "First vector, second value: " << firstVector.getDataAtIndex(1) << endl;
        cout << "First vector, third value: " << firstVector.getDataAtIndex(2) << endl;
    }*/

    return 0;
}