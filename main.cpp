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
#include "include/Evaluation/recall.h"

// // Type Alias
// typedef GraphNode<DataVector<float>> GreedySearchNode;
// typedef Graph<DataVector<float>> GreedySearchGraph;
// typedef std::set<GreedySearchNode> GreedySearchNodeSet;

/**
 * @brief Generates a set of unique random indices, excluding a specific index.
 * 
 * @param max The maximum value for random index generation
 * @param i The index to exclude from the random selection
 * @param length The number of unique random indices to generate
 * 
 * @return A set of unique random indices of the specified length, excluding index i
 */
static std::set<int> generateRandomIndices(const unsigned int max, const unsigned int i, unsigned int length) {
    // Initialize a set for the indices
    std::set<int> indices;

    // Static random number generator with a fixed seed
    static std::mt19937 generator(42);  // Seed with a fixed value for consistent randomness
    std::uniform_int_distribution<unsigned int> distribution(0, max - 1);

    // Keep assigning random integer values to the set until we reach the given length
    while (indices.size() < length) {
        unsigned int randInd = distribution(generator);
        if (randInd != i) {
            indices.insert(randInd);
        }
    }

    return indices;
}

// /**
//  * @brief Prints the contents of a set, where each element is a node with data.
//  * 
//  * @param set_t The type of elements within the set
//  * @param set The set to print
//  */
// template <typename set_t> static void printSet(const std::set<set_t> set) {

//   // Print the given set
//   std::cout << "{";
//   for (unsigned int i = 0; i < set.size(); i++) {
//     auto it = set.begin();
//     std::advance(it, i);
//     set_t currentItem = *it;
//     std::cout << currentItem.getData() << ", ";
//   }
//   std::cout << "}" << std::endl;

// }

/**
 * @brief Creates a graph from a dataset of vectors, adding edges between nodes based on random selection.
 * 
 * @param base_vectors A vector of data vectors to populate the graph nodes
 * @param max_edges The maximum number of edges per node
 * 
 * @return A graph with nodes connected by randomly assigned edges
 */
static Graph<DataVector<float>> createGraph(const std::vector<DataVector<float>> base_vectors, unsigned int max_edges) {

  // Initialize a graph and insert all the vectors from the base vectors dataset 
  Graph<DataVector<float>> graph(base_vectors.size());
  for (unsigned int i = 0; i < graph.getNodesCount(); i++) {
    graph.setNodeData(i, base_vectors.at(i));
  }

  // Fill the graph randomly
  for (unsigned int i = 0; i < graph.getNodesCount(); i++) {
      
    // Generate a random range of indeces and apply those indeces as neighbors for the current node
    std::set<int> indeces = generateRandomIndices(graph.getNodesCount(), i, max_edges);

    for (unsigned int j = 0; j < indeces.size(); j++) {
      std::set<int>::iterator it = indeces.begin();
      std::advance(it, j);
      auto currentIndex = *it;

      graph.connectNodesByIndex(i, currentIndex);
    }
      
  }

  return graph;

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
  
  


  DataVector<float> v1(2); v1.setDataAtIndex(1, 0); v1.setDataAtIndex(1, 1); v1.setIndex(0);
  DataVector<float> v2(2); v2.setDataAtIndex(1, 0); v2.setDataAtIndex(3, 1); v2.setIndex(1);
  DataVector<float> v3(2); v3.setDataAtIndex(2, 0); v3.setDataAtIndex(4, 1); v3.setIndex(2);
  DataVector<float> v4(2); v4.setDataAtIndex(4, 0); v4.setDataAtIndex(2, 1); v4.setIndex(3);
  DataVector<float> v5(2); v5.setDataAtIndex(4, 0); v5.setDataAtIndex(6, 1); v5.setIndex(4);
  DataVector<float> v6(2); v6.setDataAtIndex(5, 0); v6.setDataAtIndex(3, 1); v6.setIndex(5);
  DataVector<float> v7(2); v7.setDataAtIndex(6, 0); v7.setDataAtIndex(5, 1); v7.setIndex(6);
  DataVector<float> v8(2); v8.setDataAtIndex(7, 0); v8.setDataAtIndex(1, 1); v8.setIndex(7);

  DataVector<float> q(2); q.setDataAtIndex(4, 0); q.setDataAtIndex(3, 1);
  
  std::vector<DataVector<float>> baseVectors;
  baseVectors.push_back(v1);
  baseVectors.push_back(v2);
  baseVectors.push_back(v3);
  baseVectors.push_back(v4);
  baseVectors.push_back(v5);
  baseVectors.push_back(v6);
  baseVectors.push_back(v7);
  baseVectors.push_back(v8);
  

  Graph<DataVector<float>> G = createGraph(baseVectors, 2);
  GraphNode<DataVector<float>>* s = G.getNode(0);

  std::cout << G << std::endl;

  // std::pair<std::set<DataVector<float>>, std::set<DataVector<float>>> greedyResult;
  // greedyResult = GreedySearch2(G, *s, q, 3, 5);

  // std::cout << "Candidates: ";
  // for (auto value : greedyResult.first) {
  //   std::cout << value;
  // }
  // std::cout << std::endl;

  // std::cout << "Visited: ";
  // for (auto value : greedyResult.second) {
  //   std::cout << value;
  // }
  // std::cout << std::endl;
  

  return 0;
}
