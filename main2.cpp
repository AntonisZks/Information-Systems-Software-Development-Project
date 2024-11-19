#include <iostream>
#include <vector>
#include <algorithm>
#include "include/DataStructures/DataVector/BQDataVectors.h"
#include "include/read_data.h"
#include "include/distance.h"


int main(int argc, char* argv[]) {

  std::vector<BaseDataVector<float>> base_vectors = ReadFilteredBaseVectorFile("data/Dummy/dummy-data.bin");
  std::vector<QueryDataVector<float>> query_vectors = ReadFilteredQueryVectorFile("data/Dummy/dummy-queries.bin");

  // Allocate memory for the distances between the query and base vectors
  double** distances = new double*[query_vectors.size()];
  unsigned int** baseVectorIndeces = new unsigned int*[query_vectors.size()];
  for (unsigned int i = 0; i < query_vectors.size(); i++) {
    distances[i] = new double[base_vectors.size()];
    baseVectorIndeces[i] = new unsigned int[base_vectors.size()];
  }

  for (unsigned int i = 0; i < query_vectors.size(); i++) {
    for (unsigned int j = 0; j < base_vectors.size(); j++) {
      float distance = euclideanDistance(query_vectors[i], base_vectors[j]);
      distances[i][j] = distance;
      baseVectorIndeces[i][j] = j;
    }
  }

  // Sort the distances and keep the indeces of the base vectors
  for (unsigned int i = 0; i < query_vectors.size(); i++) {
    std::sort(baseVectorIndeces[i], baseVectorIndeces[i] + base_vectors.size(), 
      [&distances, i](unsigned int a, unsigned int b) {
        return distances[i][a] < distances[i][b];
      }
    );
  }

  // Print the indeces of the base vectors with the smallest distances to the query vectors
  for (unsigned int i = 0; i < query_vectors.size(); i++) {
    std::cout << "Query " << i << ":\n";
    for (unsigned int j = 0; j < 5; j++) {
      std::cout << "Base vector index: " << baseVectorIndeces[i][j] << ", distance: " << distances[i][baseVectorIndeces[i][j]] << std::endl;
    }
  }

  // Deallocate memory for the distances
  for (unsigned int i = 0; i < query_vectors.size(); i++) {
    delete[] distances[i];
    delete[] baseVectorIndeces[i];
  }
  delete[] distances;
  delete[] baseVectorIndeces;

  return 0;

}
