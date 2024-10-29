#include <iostream>
#include <fstream>
#include <vector>
#include "../include/DataVector/DataVector.h"  // Include your DataVector class

using namespace std;

void saveVectors(const vector<DataVector<float>>& dataVectors, const string& outputFilename);
vector<DataVector<float>> ReadVectorFile(const string& filename);