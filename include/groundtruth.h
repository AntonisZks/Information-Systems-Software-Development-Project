#ifndef GROUNDTRUTH_H
#define GROUNDTRUTH_H

#include <iostream>
#include <fstream>
#include <vector>
#include "BQDataVectors.h"
#include "graphics.h"
#include "Filter.h"
#include "distance.h"


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
  const std::vector<BaseDataVector<float>> base_vectors, 
  const std::vector<QueryDataVector<float>> query_vectors, 
  const unsigned int maxBaseVectors
);

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
void saveGroundtruthToFile(const std::vector<std::vector<int>>& base_vectors_indexes, const std::string& filename);

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
std::vector<std::vector<int>> readGroundtruthFromFile(const std::string& filename);


#endif /* GROUNDTRUTH_H */