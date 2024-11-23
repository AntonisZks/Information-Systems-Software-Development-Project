#include <iostream>
#include <vector>
#include <algorithm>
#include "include/BQDataVectors.h"
#include "include/read_data.h"
#include "include/distance.h"
#include <fstream>

/**
 * @brief Compute the groundtruth for a set of base and query vectors.
 * 
 * This function computes the groundtruth for a set of base and query vectors by calculating the Euclidean distance
 * between each query vector and all base vectors. The function supports only two query types: 0 and 1. For query type 0,
 * the function computes the distances between the query vector and all base vectors. For query type 1, the function computes
 * the distances between the query vector and the base vectors with the same C value.
 * 
 * @param base_vectors A vector of BaseDataVector objects representing the base vectors
 * @param query_vectors A vector of QueryDataVector objects representing the query vectors
 * @param maxDistances The maximum number of distances to compute for each query vector
 * 
 * @return A 2D vector containing the computed distances for each query vector
 */
std::vector<std::vector<float>> computeGroundtruth(
  const std::vector<BaseDataVector<float>> base_vectors, const std::vector<QueryDataVector<float>> query_vectors, const unsigned int maxDistances) {

  // Allocate memory for the distance vector
  std::vector<std::vector<float>> distances(query_vectors.size());

  // Compute the distances between the query vectors and the base vectors (with the same filter)
  // If no filter provided then compute to the whole graph
  for (auto query : query_vectors) {

    // Compute the distances between the query vector and all base vectors
    if (query.getQueryType() == 0) {
      for (auto base : base_vectors) {
        distances[query.getIndex()].push_back(euclideanDistance(base, query));
      }
    }

    // If the filter type is C_EQUALS_v then compute the distances between the query vector 
    // and the base vectors with the same C value
    else if (query.getQueryType() == 1) {
      for (auto base : base_vectors) {
        if (base.getC() == query.getV()) {
          distances[query.getIndex()].push_back(euclideanDistance(base, query));
        }
      }
    }
    // IMPORTANT: Queries with filter type 2 and 3 are not supported in this version of the application
  }

  // Sort the distances for each query vector
  for (unsigned int i = 0; i < query_vectors.size(); i++) {
    std::sort(distances[i].begin(), distances[i].end());
  }

  // Return the first `maxDistances` distances for each query vector
  for (auto& query_distances : distances) {
    query_distances.resize(maxDistances);
  }

  return distances;

}

/**
 * @brief Save the computed groundtruth distances to a binary file.
 * 
 * This function saves the computed groundtruth distances to a binary file. The file format consists of the following:
 * - The number of query vectors (4 bytes)
 * - For each query vector:
 *   - The number of distances (4 bytes)
 *   - The computed distances (4 bytes each)
 * 
 * @param distances A 2D vector containing the computed distances for each query vector
 * @param filename The name of the output file to save the distances
 */
void saveGroundtruthToFile(const std::vector<std::vector<float>>& distances, const std::string& filename) {
  std::ofstream file(filename, std::ios::binary);

  if (!file.is_open()) {
    std::cerr << "Error opening file: " << filename << std::endl;
    return;
  }

  // Write the number of query vectors to the file
  uint32_t num_queries = distances.size();
  file.write(reinterpret_cast<const char*>(&num_queries), sizeof(num_queries));

  // Write the distances for each query vector
  for (const auto& query_distances : distances) {
    uint32_t num_distances = query_distances.size();
    file.write(reinterpret_cast<const char*>(&num_distances), sizeof(num_distances));
    file.write(reinterpret_cast<const char*>(query_distances.data()), num_distances * sizeof(float));
  }

  file.close();
}

/**
 * @brief Read the groundtruth distances from a binary file.
 * 
 * This function reads the groundtruth distances from a binary file. The file format consists of the following:
 * - The number of query vectors (4 bytes)
 * - For each query vector:
 *   - The number of distances (4 bytes)
 *   - The computed distances (4 bytes each)
 * 
 * @param filename The name of the input file to read the distances from
 * 
 * @return A 2D vector containing the read distances for each query vector
 */
std::vector<std::vector<float>> readGroundtruthFromFile(const std::string& filename) {
  std::ifstream file(filename, std::ios::binary);

  if (!file.is_open()) {
    std::cerr << "Error opening file: " << filename << std::endl;
    return {};
  }

  // Read the number of query vectors from the file
  uint32_t num_queries;
  file.read(reinterpret_cast<char*>(&num_queries), sizeof(num_queries));

  std::vector<std::vector<float>> distances(num_queries);

  // Read the distances for each query vector
  for (auto& query_distances : distances) {
    uint32_t num_distances;
    file.read(reinterpret_cast<char*>(&num_distances), sizeof(num_distances));
    query_distances.resize(num_distances);
    file.read(reinterpret_cast<char*>(query_distances.data()), num_distances * sizeof(float));
  }

  file.close();
  return distances;
}

int main(int argc, char* argv[]) {

  std::vector<BaseDataVector<float>> base_vectors = ReadFilteredBaseVectorFile("data/Dummy/dummy-data.bin");
  std::vector<QueryDataVector<float>> query_vectors = ReadFilteredQueryVectorFile("data/Dummy/dummy-queries.bin");

  // Compute the distance vector, and save the computed distances to a file
  std::vector<std::vector<float>> distances = computeGroundtruth(base_vectors, query_vectors, 1000);
  saveGroundtruthToFile(distances, "data/Dummy/dummy-groundtruth.bin");

  // Example usage of readGroundtruthFromFile
  std::vector<std::vector<float>> read_distances = readGroundtruthFromFile("data/Dummy/dummy-groundtruth.bin");

  return 0;

}
