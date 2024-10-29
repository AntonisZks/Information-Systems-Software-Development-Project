#include <iostream>
#include <vector>
#include "include/Graph/graph.h"
#include "include/readVectors.h"
#include "include/DataVector/DataVector.h"

int main(int argc, char* argv[]) {

    std::string dataset_path;

    // Check if the program call was correct
    if (argc != 2) {
        std::cout << "Usage: " << argv[0] << " [dataset_path] " << std::endl;
        return 1;
    }

    // Assign the path of the dataset and read its contents
    dataset_path = argv[1];
    std::vector<DataVector<float>> base_vectors = ReadVectorFile(dataset_path);

    // Print the contents of the dataset
    for (unsigned int i = 0; i < base_vectors.size(); i++) {
        std::cout << "Data Vector " << i+1 << ": " << base_vectors.at(i) << std::endl;
    }

    return 0;
}
