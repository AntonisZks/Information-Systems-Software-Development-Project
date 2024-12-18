#include "../../../include/VamanaIndex.h"
#include "../../../include/DataVector.h"
#include "../../../include/BQDataVectors.h"

#include <thread>
#include <chrono>
#include <random>
#include <algorithm>
#include <numeric>
#include <fstream>
#include <iostream>

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

  std::vector<int> permutation(end - start + 1);
  std::iota(permutation.begin(), permutation.end(), start);
  std::shuffle(permutation.begin(), permutation.end(), std::mt19937{std::random_device{}()});

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

  std::set<int> indices;
  std::mt19937 generator(std::random_device{}());
  std::uniform_int_distribution<unsigned int> distribution(0, max - 1);

  // Generate random indices until the set reaches the desired length
  while (indices.size() < length) {
    unsigned int randInd = distribution(generator);
    if (randInd != i) {
      indices.insert(randInd);
    }
  }

  return indices;

}

/**
 * @brief Fills the graph nodes with the given dataset points. 
 */
template <typename vamana_t> void VamanaIndex<vamana_t>::fillGraphNodes(void) {

  // Fill the nodes with the dataset points and set the index of each point
  for (unsigned int i = 0; i < this->P.size(); i++) {
    vamana_t point = this->P.at(i);
    point.setIndex(i);
    this->G.setNodeData(i, point);
  }

}

/**
 * @brief Populates the graph of the vamana index. Specifically it fills every graph node with edges pointing
 * to random neighbors inside the graph, with these edges being at most maxEdges.
 * 
 * @param maxEdges the maximum number of edges a node can have.
 */
template <typename vamana_t> void VamanaIndex<vamana_t>::createRandomEdges(const unsigned int maxEdges) {

  // Create random edges for each node in the graph by connecting them to random neighbors
  for (unsigned int i = 0; i < this->G.getNodesCount(); i++) {
    std::set<int> indices = generateRandomIndices(this->G.getNodesCount(), i, std::min(maxEdges, this->G.getNodesCount() - 1));
    for (int index : indices) {
      this->G.connectNodesByIndex(i, index);
    }
  }

}

/**
 * @brief Computes the distances between every node in the dataset and stores them in the distance matrix.
 */
template <typename vamana_t> void VamanaIndex<vamana_t>::computeDistances(const bool visualize) {

  // Define a lambda function to compute the distances between two points
  auto compute = [&](int i) {
    for (unsigned int j = i; j < this->P.size(); j++) {
      double dist = euclideanDistance(this->P.at(i), this->P.at(j));
      this->distanceMatrix[i][j] = dist;
      this->distanceMatrix[j][i] = dist;
    }
  };

  // Compute distances with or without visualization, depending on the visualize flag value
  if (visualize) {
    withProgress(0, this->P.size(), "Computing Distances", compute);
  } else {
    for (unsigned int i = 0; i < this->P.size(); i++) {
      compute(i);
    }
  }

}

/**
 * @brief Creates a Vamana Index Graph according to the provided dataset points and the given parameters.
 * Specifically this method follows the Vamana algorithm found on the paper:
 * 
 * https://proceedings.neurips.cc/paper_files/paper/2019/file/09853c7fb1d3f8ee67a61b6bf4a7f8e6-Paper.pdf
 * 
 * and creates a directed, unweighted, and random connected graph data structure, populated with the data points,
 * using the GreedySearch and RobustPrune algorithms.
 * 
 * @param P the vector containing the data points
 * @param alpha the parameter alpha
 * @param L the parameter L
 * @param R the parameter R
 * @param visualize whether to visualize the progress
 * @param distanceMatrix optional precomputed distance matrix
 */
template <typename vamana_t> 
void VamanaIndex<vamana_t>::createGraph(
const std::vector<vamana_t>& P, const float& alpha, const unsigned int L, const unsigned int& R, bool visualize, double** distanceMatrix) {

  using GreedyResult = std::pair<std::set<vamana_t>, std::set<vamana_t>>;
  GreedyResult greedyResult;

  // Check if the dataset is empty or it has only one point
  if (P.size() <= 1) return;

  // Initialize graph memory
  unsigned int n = P.size();
  this->P = P;

  // If the distance matrix is provided, use it, otherwise compute the distances
  if (distanceMatrix != nullptr) {
    this->distanceMatrix = distanceMatrix;
  } else {
    this->distanceMatrix = new double*[n];
    for (unsigned int i = 0; i < n; i++) {
      this->distanceMatrix[i] = new double[n];
    }
    this->computeDistances(visualize);
  }

  // Set the number of nodes in the graph, fill the nodes with the dataset points, and create random edges for the nodes
  this->G.setNodesCount(n);
  this->fillGraphNodes();
  this->createRandomEdges(R);

  // Find the medoid node in the graph, and generate a random permutation of node indices
  GraphNode<vamana_t> s = findMedoid(this->G, visualize, 1000);
  std::vector<int> sigma = generateRandomPermutation(0, n-1);

  // Define a lambda function for the main loop process of the Vamana algorithm
  auto loopProcess = [&](int i) {

    GraphNode<vamana_t>* sigma_i_node = this->G.getNode(sigma.at(i));
    vamana_t sigma_i = sigma_i_node->getData();

    // Run Greedy Search and Robust Prune for the current sigma[i] node and its neighbors
    greedyResult = GreedySearch(*this, s, this->P.at(sigma.at(i)), 1, L);
    RobustPrune(*this, *sigma_i_node, greedyResult.second, alpha, R);

    // Get the neighbors of sigma[i] node and iterate over them to run Robust Prune for each one of them as well
    for (auto j : *sigma_i_node->getNeighborsVector()) {
      std::set<vamana_t> outgoing;
      GraphNode<vamana_t>* j_node = this->G.getNode(j.getIndex());

      // The outgoing set has to consist of the neighbors of j and the sigma[i] node itself
      for (auto neighbor : *j_node->getNeighborsVector()) {
        outgoing.insert(neighbor);
      }
      outgoing.insert(sigma_i);

      // Check if the |N_out(j) union {sigma[i]}| > R and run Robust Prune accordingly
      if (outgoing.size() > (unsigned int)R) {
        RobustPrune(*this, *j_node, outgoing, alpha, R);
      } else {
        j_node->addNeighbor(sigma_i);
      }
    }

  };

  // Process each node with or without visualization
  if (visualize) {
    withProgress(0, n, "Creating Vamana", loopProcess);
  } else {
    for (unsigned int i = 0; i < n; i++) {
      loopProcess(i);
    }
  }

  // Free up the memory allocated for the distance matrix, if it was computed
  if (distanceMatrix == nullptr) {
    for (unsigned int i = 0; i < n; i++) {
      delete[] this->distanceMatrix[i];
    }
    delete[] this->distanceMatrix;
  }

}

/**
 * @brief Saves a specific graph into a file. Specifically this method is used to save the contents of a Vamana 
 * Index Graph, inside a file in order to be loaded later for further usage. The main point of this method is to 
 * reduce the time of the production.
 * 
 * @param filename the full path of the file in which the graph is going to be saved
 * 
 * @return true if the graph was saved successfully, false otherwise
 */
template <typename vamana_t> bool VamanaIndex<vamana_t>::saveGraph(const std::string& filename) {

  // Open the file for writing and check if it was opened successfully
  std::ofstream outFile(filename, std::ios::binary);
  if (!outFile) {
    std::cerr << "Error opening file for writing." << std::endl;
    return false;
  }

  // Write the number of nodes in the graph and then write the nodes and their neighbors
  outFile << this->G.getNodesCount() << std::endl;
  withProgress(0, this->G.getNodesCount(), "Saving Nodes", [&](int i) {
    outFile << *this->G.getNode(i) << std::endl;
  });

  // Write the neighbors of each node in the graph to the file as well
  withProgress(0, this->G.getNodesCount(), "Saving Edges", [&](int i) {
    auto neighbors = this->G.getNode(i)->getNeighborsVector();
    outFile << neighbors->size();
    for (const auto& neighbor : *neighbors) {
      outFile << " " << neighbor;
    }
    outFile << std::endl;
  });

  return true;

}

/**
 * @brief Load a graph from a file. Specifically this method is used to receive the contents of a Vamana Index Graph
 * stored inside a file and create the Vamana Index object based on those contents. It is used to save time of the 
 * production making it easy to use an index with specific parameters just by loading it instead of creating it again.
 * 
 * @param filename the full path of the file containing the graph
 * 
 * @return true if the graph was loaded successfully, false otherwise
 */
template <typename vamana_t> bool VamanaIndex<vamana_t>::loadGraph(const std::string& filename) {

  // Open the file for reading and check if it was opened successfully
  std::ifstream inFile(filename);
  if (!inFile) {
    std::cerr << "Error opening file for reading.\n";
    return false;
  }

  // Read the number of nodes in the graph and initialize the graph with that number
  unsigned int nodesCount;
  inFile >> nodesCount;
  this->G.setNodesCount(nodesCount);

  // Read the nodes and their neighbors from the file and populate the graph
  withProgress(0, nodesCount, "Loading nodes", [&](int i) {
    vamana_t currentData;
    inFile >> currentData;
    currentData.setIndex(i);
    this->G.setNodeData(i, currentData);
    this->P.push_back(currentData);
  });

  // Read the edges of each node from the file and connect the nodes in the graph
  withProgress(0, nodesCount, "Loading edges", [&](int i) {
    unsigned int neighborsCount;
    inFile >> neighborsCount;
    for (unsigned int j = 0; j < neighborsCount; j++) {
      vamana_t currentData;
      inFile >> currentData;
      this->G.connectNodesByIndex(i, this->G.getNode(currentData.getIndex())->getIndex());
    }
  });

  return true;

}

/**
 * @brief Finds the medoid node in the graph using a sample of nodes.
 *
 * The medoid is the node with the minimum average distance to all other nodes in the sample.
 * This function uses Euclidean distance to calculate the distances between nodes.
 *
 * @tparam T The type of data stored in the graph nodes.
 * @param graph The graph from which to find the medoid.
 * @param sample_size The number of nodes to sample from the graph for medoid calculation. Default is 100.
 * @return The medoid node of the sampled nodes.
 */
template <typename vamana_t> GraphNode<vamana_t> VamanaIndex<vamana_t>::findMedoid(const Graph<vamana_t>& graph, bool visualize, int sample_size) {

  const int node_count = graph.getNodesCount();
  sample_size = std::min(sample_size, node_count);

  // Create a list of all node indices and shuffle it to select a random subset
  std::vector<int> sampled_indices(node_count);
  std::iota(sampled_indices.begin(), sampled_indices.end(), 0);
  std::shuffle(sampled_indices.begin(), sampled_indices.end(), std::mt19937{std::random_device{}()});
  sampled_indices.resize(sample_size);

  // Initialize a distance matrix for the sampled nodes
  std::vector<std::vector<float>> distance_matrix(sample_size, std::vector<float>(sample_size, 0.0));

  // Compute pairwise distances between each pair of sampled nodes
  auto computeDistances = [&](int i) {
    for (int j = i + 1; j < sample_size; ++j) {
      float dist = euclideanDistance(graph.getNode(sampled_indices[i])->getData(), graph.getNode(sampled_indices[j])->getData());
      distance_matrix[i][j] = dist;
      distance_matrix[j][i] = dist;
    }
  };

  // Compute distances with or without visualization, depending on the visualize flag value
  if (visualize) {
    withProgress(0, sample_size, "Finding Medoid", computeDistances);
  } else {
    for (unsigned int i = 0; i < (unsigned int)sample_size; i++) {
      computeDistances(i);
    }
  }

  // Find the medoid node among the sampled nodes by calculating the average distance for each one
  float min_average_distance = std::numeric_limits<float>::max();
  GraphNode<vamana_t>* medoid_node = nullptr;

  for (int i = 0; i < sample_size; ++i) {
    float total_distance = std::accumulate(distance_matrix[i].begin(), distance_matrix[i].end(), 0.0f);
    float average_distance = total_distance / (sample_size - 1);
    if (average_distance < min_average_distance) {
      min_average_distance = average_distance;
      medoid_node = graph.getNode(sampled_indices[i]);
    }
  }

  return *medoid_node;

}

template class VamanaIndex<DataVector<float>>;
template class VamanaIndex<BaseDataVector<float>>;
