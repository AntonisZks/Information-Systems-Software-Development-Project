#include <iostream>
#include <vector>
#include "include/DataStructures/DataVector/BQDataVectors.h"
#include "include/read_data.h"
#include "include/distance.h"


int main(int argc, char* argv[]) {

  std::vector<BaseDataVector<float>> base_vectors = ReadFilteredBaseVectorFile("data/Dummy/dummy-data.bin");
  std::vector<QueryDataVector<float>> query_vectors = ReadFilteredQueryVectorFile("data/Dummy/dummy-queries.bin");

  // Allocate memory for the distances between the query and base vectors
  double** distances = new double*[query_vectors.size()];
  for (unsigned int i = 0; i < query_vectors.size(); i++) {
    distances[i] = new double[base_vectors.size()];
  }

  for (unsigned int i = 0; i < query_vectors.size(); i++) {
    for (unsigned int j = 0; j < base_vectors.size(); j++) {
      float distance = euclideanDistance(query_vectors[i], base_vectors[j]);
      // std::cout << "Distance between query " << i << " and base vector " << j << ": " << distance << std::endl;
    }
  }

  return 0;

}
