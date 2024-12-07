#include <algorithm>
#include "../../../include/groundtruth.h"

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
std::vector<std::vector<int>> computeGroundtruth(
  const std::vector<BaseDataVector<float>> base_vectors, const std::vector<QueryDataVector<float>> query_vectors, const unsigned int maxBaseVectors) {

  // Allocate memory for the distance vector and the indexes of the base vectors
  std::vector<std::vector<float>> distances(query_vectors.size());
  std::vector<std::vector<int>> base_vectors_indexes(query_vectors.size());

  // Compute the distances between the query vectors and the base vectors (with the same filter)
  // If no filter provided then compute to the whole graph
  withProgress(0, query_vectors.size(), "Computing Groundtruth", [&](int i) {

    auto query = query_vectors[i];

    // Compute the distances between the query vector and all base vectors
    if (query.getQueryType() == NO_FILTER) {
      for (auto base : base_vectors) {
        distances[query.getIndex()].push_back(euclideanDistance(base, query));
        base_vectors_indexes[query.getIndex()].push_back(base.getIndex());
      }
    }

    // If the filter type is C_EQUALS_v then compute the distances between the query vector 
    // and the base vectors with the same C value
    else if (query.getQueryType() == C_EQUALS_v) {
      for (auto base : base_vectors) {
        if (base.getC() == query.getV()) {
          distances[query.getIndex()].push_back(euclideanDistance(base, query));
          base_vectors_indexes[query.getIndex()].push_back(base.getIndex());
        }
      }
    }

    // IMPORTANT: Queries with filter type 2 and 3 are not supported in this version of the application

  });

  // Sort the distances for each query vector and keep the base vector indexes for each distance
  withProgress(0, query_vectors.size(), "Sorting Distances", [&](int i) {
    
    // Pack the distances and the base vector indexes into a vector of pairs
    std::vector<std::pair<float, int>> paired_vec;
    for (unsigned int j = 0; j < distances[i].size(); j++) {
      paired_vec.emplace_back(distances[i][j], base_vectors_indexes[i][j]);
    }

    std::sort(paired_vec.begin(), paired_vec.end());

    // Unpack the sorted distances and base vector indexes
    for (unsigned int j = 0; j < paired_vec.size(); j++) {
      distances[i][j] = paired_vec[j].first;
      base_vectors_indexes[i][j] = paired_vec[j].second;
    }

  });

  // Return the first `maxBaseVectors` distances for each query vector
  for (auto& base_vector_indexes : base_vectors_indexes) {
    base_vector_indexes.resize(std::min((int)maxBaseVectors, (int)base_vector_indexes.size()));
  }

  return base_vectors_indexes;

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
void saveGroundtruthToFile(const std::vector<std::vector<int>>& base_vectors_indexes, const std::string& filename) {

  std::ofstream file(filename, std::ios::binary);

  if (!file.is_open()) {
    std::cerr << "Error opening file: " << filename << std::endl;
    return;
  }

  // Write the number of query vectors to the file
  unsigned int num_queries = base_vectors_indexes.size();
  file.write(reinterpret_cast<const char*>(&num_queries), sizeof(num_queries));

  // Write the distances for each query vector
  withProgress(0, base_vectors_indexes.size(), "Saving Groundtruth", [&](int i) {
  
    const std::vector<int>& base_vector_indexes = base_vectors_indexes[i];

    unsigned int num_vectors = base_vector_indexes.size();
    file.write(reinterpret_cast<const char*>(&num_vectors), sizeof(num_vectors));
    file.write(reinterpret_cast<const char*>(base_vector_indexes.data()), num_vectors * sizeof(float));
  
  });

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
std::vector<std::vector<int>> readGroundtruthFromFile(const std::string& filename) {
  std::ifstream file(filename, std::ios::binary);

  if (!file.is_open()) {
    std::cerr << "Error opening file: " << filename << std::endl;
    return {};
  }

  // Read the number of query vectors from the file
  unsigned int num_queries;
  file.read(reinterpret_cast<char*>(&num_queries), sizeof(num_queries));

  std::vector<std::vector<int>> base_vectors_indexes(num_queries);

  // Read the distances for each query vector
  withProgress(0, base_vectors_indexes.size(), "Loading Groundtruth", [&](int i) {
    auto& base_vector_indexes = base_vectors_indexes[i];

    unsigned int num_vectors;
    file.read(reinterpret_cast<char*>(&num_vectors), sizeof(num_vectors));
    base_vector_indexes.resize(num_vectors);
    file.read(reinterpret_cast<char*>(base_vector_indexes.data()), num_vectors * sizeof(float));
  });

  file.close();
  return base_vectors_indexes;
}