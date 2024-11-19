#include <iostream>
#include <vector>
#include "include/DataStructures/DataVector/BQDataVectors.h"
#include "include/read_data.h"


int main(int argc, char* argv[]) {

  std::vector<BaseDataVector<float>> base_vectors = ReadFilteredBaseVectorFile("data/Dummy/dummy-data.bin");

  for (unsigned int i = 0; i < base_vectors.size(); i++) {
    if (i == 9999) {
      std::cout << "For node " << i << " the distances are: " << base_vectors[i] << std::endl;
      std::cout << "The C value is: " << base_vectors[i].getC() << std::endl;
      std::cout << "The T value is: " << base_vectors[i].getT() << std::endl;
      std::cout << "Size of the vector is: " << base_vectors[i].getDimension() << std::endl;
    }
  }

  return 0;

}
