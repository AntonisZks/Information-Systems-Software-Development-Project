#include <iostream>
#include <vector>
#include "include/DataStructures/DataVector/BQDataVectors.h"
#include "include/read_data.h"


int main(int argc, char* argv[]) {

  std::vector<BaseDataVector<float>> base_vectors = ReadFilteredBaseVectorFile("data/Dummy/dummy-data.bin");
  std::vector<QueryDataVector<float>> query_vectors = ReadFilteredQueryVectorFile("data/Dummy/dummy-queries.bin");

  for (unsigned int i = 0; i < query_vectors.size(); i++) {
    if (i == 9999) {
      std::cout << "For node " << i << " the distances are: " << query_vectors[i] << std::endl;
      std::cout << "The query type value is: " << query_vectors[i].getQueryType() << std::endl;
      std::cout << "The L value is: " << query_vectors[i].getL() << std::endl;
      std::cout << "The R value is: " << query_vectors[i].getR() << std::endl;
      std::cout << "The V value is: " << query_vectors[i].getV() << std::endl;
      std::cout << "Size of the vector is: " << query_vectors[i].getDimension() << std::endl;
    }
    
  }

  return 0;

}
