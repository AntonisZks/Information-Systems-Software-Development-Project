#include <iostream>
#include <cstdlib>
#include <vector>
#include <random>
#include <set>
#include <ctime>
#include "include/DataStructures/Graph/graph.h"
#include "include/read_data.h"
#include "include/DataStructures/DataVector/DataVector.h"
#include "include/Algorithms/GreedySearch.h"
#include "include/Algorithms/GreedySearch.h"
// #include "include/Algorithms/Vanama.h"
#include "include/Evaluation/recall.h"
#include "include/Algorithms/VamanaIndex.h"

/**
 * @brief Retrieves the exact nearest neighbors for a specific query vector
 * based on ground truth values.
 * 
 * 
 * @param base_vectors A vector of DataVector<float> objects representing the dataset.
 * @param groundtruth_values A vector of DataVector<int> objects containing the true
 * nearest neighbor indices for each query.
 * 
 * @return A set containing the exact nearest neighbors for a query vector.
 */
static std::set<DataVector<float>> 
getExactNearestNeighbors(
  std::vector<DataVector<float>>& base_vectors, std::vector<DataVector<int>>& groundtruth_values, const unsigned int query_number) {

  std::set<DataVector<float>> realNeighbors;

  DataVector<int> realNearestIndeces = groundtruth_values.at(query_number);
  for (unsigned int i = 0; i < realNearestIndeces.getDimension(); i++) {
    int currentIndex = realNearestIndeces.getDataAtIndex(i);
    DataVector<float> currentData = base_vectors.at(currentIndex);
    realNeighbors.insert(currentData);
  }

  return realNeighbors;

}

/**
 * @brief Main function that reads datasets, creates a graph, and performs a greedy search.
 * 
 * @param argc The number of command-line arguments
 * @param argv Array of command-line arguments
 * 
 * @return Returns 0 upon successful execution; otherwise returns 1
 */
int main(int argc, char* argv[]) {

  srand(static_cast<unsigned int>(time(0)));

  // Check for valid program execution
  if (argc != 9) {
    std::cout << "Usage: " << argv[0] << " [base_vectors_file]";
    std::cout << " [query_vectors_file]" << " [groundtruth_file]";
    std::cout << " [k]" <<  " [alpha]" << " [L]" << " [R]" << " [query_number]"<< std::endl;

    return 1;
  }

  // Store the dataset files
  std::string base_file = argv[1];
  std::string query_file = argv[2];
  std::string groundtruth_file = argv[3];

  // Store the search parameters
  unsigned int k = std::stoi(argv[4]);
  float alpha = std::stof(argv[5]);
  unsigned int L = std::stoi(argv[6]);
  unsigned int R = std::stoi(argv[7]);
  unsigned int query_number = std::stoi(argv[8]);
  
  // Receive the data vectors from the dataset files
  std::vector<DataVector<float>> base_vectors = ReadVectorFile(base_file);
  std::vector<DataVector<float>> query_vectors = ReadVectorFile(query_file);  
  std::vector<DataVector<int>> groundtruth_values = ReadGroundTruth(groundtruth_file);

  // Get the exact nearest neighbors
  std::set<DataVector<float>> realNeighbors = getExactNearestNeighbors(base_vectors, groundtruth_values, query_number);


  VamanaIndex<DataVector<float>> index;
  index.createGraph(base_vectors, alpha, L, R);

  // GraphNode<DataVector<float>> s = findMedoid(index.getGraph(), 10000);
  // std::pair<std::set<DataVector<float>>, std::set<DataVector<float>>> greedyResult;

  // greedyResult = GreedySearch(index.getGraph(), s, query_vectors.at(query_number), k, L);
  // float recall = calculateRecallEvaluation(greedyResult.first, realNeighbors);

  // std::cout << std::endl << "[================= RESULTS =================]" << std::endl; 
  // std::cout << "Recall Evaluation: " << recall*100 << "%" << std::endl;

  return 0;

}
