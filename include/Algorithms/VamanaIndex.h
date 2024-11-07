#ifndef VAMANA_INDEX_H
#define VAMANA_INDEX_H

#include <iostream>
#include <vector>
#include <set>
#include <fstream>
#include <sstream>
#include "../DataStructures/Graph/graph.h"
#include "GreedySearch.h"
#include "RobustPrune.h"


/**
 * @brief Generates a random permutation of integers in a specified range. This function creates a vector 
 * containing all integers from `start` to `end` and then shuffles them randomly to produce a random permutation.
 * 
 * @param start The starting integer of the range (inclusive)
 * @param end The ending integer of the range (inclusive)
 * 
 * @return A vector containing a shuffled sequence of integers from `start` to `end`
 */
static std::vector<int> generateRandomPermutation(const unsigned int start, const unsigned int end) {
    // Initialize a vector to hold the sequence of integers from start to end
    std::vector<int> permutation;
    for (unsigned int i = start; i <= end; i++) {
        permutation.push_back(i);
    }

    // Use a more unpredictable seed
    std::mt19937 gen(std::random_device{}());  // Seed with random device for true randomness
    std::shuffle(permutation.begin(), permutation.end(), gen);

    return permutation;
}


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

  static std::mt19937 generator(std::random_device{}());
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


template <typename vamana_t>
class VamanaIndex {

private:
  Graph<vamana_t> G;
  std::vector<vamana_t> P;

  void fillGraphNodes(void) {
    for (unsigned int i = 0; i < this->P.size(); i++) {
      this->G.setNodeData(i, this->P.at(i));
    }
  }

  void createRandomEdges(const unsigned int maxEdges) {
    // Fill the graph randomly
    for (unsigned int i = 0; i < this->G.getNodesCount(); i++) {
        
      // Generate a random range of indeces and apply those indeces as neighbors for the current node
      std::set<int> indeces = generateRandomIndices(this->G.getNodesCount(), i, maxEdges);
      for (unsigned int j = 0; j < indeces.size(); j++) {
        std::set<int>::iterator it = indeces.begin();
        std::advance(it, j);
        auto currentIndex = *it;

        this->G.connectNodesByIndex(i, currentIndex);
      }   
    }
  }

public:

  /**
   * @brief Default Constructor for the VamanaIndex. Exists to avoid errors.
   */
  VamanaIndex(void) {}

  const Graph<vamana_t>& getGraph(void) const {
    return this->G;
  }

  void createGraph(const std::vector<vamana_t>& P, const float& alpha, const unsigned int L, const unsigned int& R) {

    using GreedyResult = std::pair<std::set<vamana_t>, std::set<vamana_t>>;

    GreedyResult greedyResult;
    this->P = P;
    unsigned int n = P.size();
    this->G.setNodesCount(n);

    // // Initialize the graph to a random R-regular directed graph
    this->fillGraphNodes();
    this->createRandomEdges(R);
    GraphNode<vamana_t> s = findMedoid(this->G, 1000);
    std::vector<int> sigma = generateRandomPermutation(0, n-1);

    for (unsigned int i = 0; i < n; i++) {

      double percentage = (double)(100*i)/n;
      printProgressBar(percentage, "Vamana Index Creation Progress: ");

      GraphNode<vamana_t>* sigma_i_node = this->G.getNode(sigma.at(i));
      vamana_t sigma_i = sigma_i_node->getData();

      // Run Greedy Search and Robust Prune
      greedyResult = GreedySearch(this->G, s, this->P.at(sigma.at(i)), 1, L);
      RobustPrune(this->G, *sigma_i_node, greedyResult.second, alpha, R);

      // Get the neighbors of sigma[i] node and iterate over them
      std::vector<vamana_t>* sigma_i_neighbors = sigma_i_node->getNeighbors();
      for (auto j : *sigma_i_neighbors) {
        std::set<vamana_t> outgoing;
        GraphNode<vamana_t>* j_node = this->G.getNode(j.getIndex());

        // The outgoing set has to consist of the neighbors of j and the sigma[i]
        for (auto neighbor : *j_node->getNeighbors()) {
          outgoing.insert(neighbor);
        }
        outgoing.insert(sigma_i);

        // Check if the |N_out(j) union {sigma[i]}| > R and run Robust Prune
        if (outgoing.size() > (long unsigned int)R) {
          RobustPrune(this->G, *j_node, outgoing, alpha, R);
        } 
        else {
          j_node->addNeighbor(sigma_i);
        }
      }

    }

    printProgressBar(100, "Vamana Index Creation Progress: ");

  }

  void saveGraph(const std::string& filename) {

    std::ofstream outFile(filename, std::ios::binary);
    if (!outFile) {
      std::cerr << "Error opening file for writing." << std::endl;
      return;
    }

    // Save the nodes count of the graph, and its data
    outFile << this->G.getNodesCount() << std::endl;
    for (unsigned int i = 0; i < this->G.getNodesCount(); i++) {
      
      // Store the current node index and get its neighbors
      outFile << i;
      GraphNode<vamana_t>* currentNode = this->G.getNode(i);
      std::vector<vamana_t>* neighbors = currentNode->getNeighbors();

      // Store every neighbor of the current node
      outFile << " " << neighbors->size();
      for (const auto& neighbor : *neighbors) {
        outFile << " " << neighbor;
      }
     
      outFile << std::endl; // Newline to seperate each node's neighbors
    
    }

  }

  void loadGraph(const std::string& filename) {

    // TODO: implement the loading functionality

  }

  void test(
    const unsigned int k, const unsigned int L, const std::vector<vamana_t>& query_vectors, 
    const unsigned int& query_number, const std::set<vamana_t>& realNeighbors) 
  {

    using GreedyResult = std::pair<std::set<DataVector<float>>, std::set<DataVector<float>>>;

    GraphNode<DataVector<float>> s = findMedoid(this->G, 1000);
    GreedyResult greedyResult = GreedySearch(this->G, s, query_vectors.at(query_number), k, L);
    float recall = calculateRecallEvaluation(greedyResult.first, realNeighbors);

    std::cout << std::endl << "[================= RESULTS =================]" << std::endl; 
    std::cout << "Recall Evaluation: " << recall*100 << "%" << std::endl;
  
  }

};

template <typename vamana_t>
std::ostream& operator<<(std::ostream& out, const VamanaIndex<vamana_t>& index) {
  if (index.getGraph().getNodesCount() == 0) {
    out << "Vamana Index Empty";
    return out;
  }

  out << index.getGraph();
  return out;
}

#endif /* VAMANA_INDEX_H */
