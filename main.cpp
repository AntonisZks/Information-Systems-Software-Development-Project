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
#include "include/Algorithms/RobustPrune.h"
#include "include/Algorithms/Vanama.h"
#include "include/Evaluation/recall.h"

// // Type Alias
// typedef GraphNode<DataVector<float>> GreedySearchNode;
// typedef Graph<DataVector<float>> GreedySearchGraph;
// typedef std::set<GreedySearchNode> GreedySearchNodeSet;


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
  if (argc != 4) {
    std::cout << "Usage: " << argv[0] << " [base_vectors_file]";
    std::cout << " [query_vectors_file]" << " [groundtruth_file]" << std::endl;

    return 1;
  }

  // Store the dataset files
  std::string base_file = argv[1];
  std::string query_file = argv[2];
  std::string groundtruth_file = argv[3];
  
  // Receive the data vectors from the dataset files
  std::vector<DataVector<float>> base_vectors = ReadVectorFile(base_file);
  std::vector<DataVector<float>> query_vectors = ReadVectorFile(query_file);  
  std::vector<DataVector<int>> groundtruth_values = ReadGroundTruth(groundtruth_file);

  

  DataVector<float> v1(2, 15); v1.setDataAtIndex(1, 0); v1.setDataAtIndex(1, 1); v1.setIndex(0);
  DataVector<float> v2(2, 15); v2.setDataAtIndex(1, 0); v2.setDataAtIndex(3, 1); v2.setIndex(1);
  DataVector<float> v3(2, 15); v3.setDataAtIndex(2, 0); v3.setDataAtIndex(4, 1); v3.setIndex(2);
  DataVector<float> v4(2, 15); v4.setDataAtIndex(4, 0); v4.setDataAtIndex(2, 1); v4.setIndex(3);
  DataVector<float> v5(2, 15); v5.setDataAtIndex(4, 0); v5.setDataAtIndex(6, 1); v5.setIndex(4);
  DataVector<float> v6(2, 15); v6.setDataAtIndex(5, 0); v6.setDataAtIndex(3, 1); v6.setIndex(5);
  DataVector<float> v7(2, 15); v7.setDataAtIndex(6, 0); v7.setDataAtIndex(5, 1); v7.setIndex(6);
  DataVector<float> v8(2, 15); v8.setDataAtIndex(7, 0); v8.setDataAtIndex(1, 1); v8.setIndex(7);

  DataVector<float> q(2, 0); q.setDataAtIndex(4, 0); q.setDataAtIndex(3, 1);
  
  std::vector<DataVector<float>> baseVectors;
  baseVectors.push_back(v1);
  baseVectors.push_back(v2);
  baseVectors.push_back(v3);
  baseVectors.push_back(v4);
  baseVectors.push_back(v5);
  baseVectors.push_back(v6);
  baseVectors.push_back(v7);
  baseVectors.push_back(v8);
  

  std::set<DataVector<float>> realNeighbors; // { v4, v6, v3 };

  Graph<DataVector<float>> G = Vamana(base_vectors, 1.0, 120, 14);
  GraphNode<DataVector<float>>* s = G.getNode(0);

  DataVector<int> realNearestIndeces = groundtruth_values.at(0);
  for (unsigned int i = 0; i < realNearestIndeces.getDimension(); i++) {
    int currentIndex = realNearestIndeces.getDataAtIndex(i);
    GraphNode<DataVector<float>>* currentNode = G.getNode(currentIndex);
    realNeighbors.insert(currentNode->getData());
  }

  std::pair<std::set<DataVector<float>>, std::set<DataVector<float>>> greedyResult;
  greedyResult = GreedySearch2(G, *s, query_vectors.at(0), 100, 120);

  std::cout << calculateRecallEvaluation(greedyResult.first, realNeighbors) << std::endl;

  return 0;

}
