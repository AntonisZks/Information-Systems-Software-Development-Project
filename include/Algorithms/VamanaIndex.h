#ifndef VAMANA_INDEX_H
#define VAMANA_INDEX_H

#include <iostream>
#include <vector>
#include <set>
#include <fstream>
#include <sstream>
#include "../DataStructures/Graph/graph.h"
#include "../Evaluation/recall.h"
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
static std::vector<int> generateRandomPermutation(const unsigned int start, const unsigned int end) 
{
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
static std::set<int> generateRandomIndices(const unsigned int max, const unsigned int i, unsigned int length) 
{
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

/**
 * @brief Class that represents the Vamana Index entity of the application. It provides methods for creating
 * an index according to the provided algorithm from the paper, saving a particular graph index into a file, and
 * loading and graph from a file for later usage.
*/
template <typename vamana_t> class VamanaIndex {

private:
  
  Graph<vamana_t> G;
  std::vector<vamana_t> P;

  /**
   * @brief Fills the graph nodes with the given dataset points. 
  */
  void fillGraphNodes(void) {
    for (unsigned int i = 0; i < this->P.size(); i++) {
      this->G.setNodeData(i, this->P.at(i));
    }
  }

  /**
   * @brief Populates the graph of the vamana index. Speicifcally it fills every graph node with edges pointing
   * to random neighbors inside the graph, with these edges being at most maxEdges.
   * 
   * @param maxEdges the maximum number of edges a node can have.
  */
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

  /**
   * @brief Returns the graph of the Vamana Index entity as a constant reference.
   * 
   * @return the graph index
  */
  const Graph<vamana_t>& getGraph(void) const {
    return this->G;
  }

  /**
   * @brief Returns the dataset points of the Vamana Index entity as a vector.
   * 
   * @return the points vector
  */
  std::vector<vamana_t> getPoints(void) const {
    return this->P;
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
   * 
  */
  void createGraph(const std::vector<vamana_t>& P, const float& alpha, const unsigned int L, const unsigned int& R) {

    using GreedyResult = std::pair<std::set<vamana_t>, std::set<vamana_t>>;

    GreedyResult greedyResult;
    this->P = P;
    unsigned int n = P.size();
    this->G.setNodesCount(n);

    // Initialize the graph to a random R-regular directed graph with n vertices and R edges per vertex
    this->fillGraphNodes();
    this->createRandomEdges(R);
    GraphNode<vamana_t> s = findMedoid(this->G, 1000);
    std::vector<int> sigma = generateRandomPermutation(0, n-1);

    for (unsigned int i = 0; i < n; i++) {

      double percentage = (double)(100*i)/n;
      printProgressBar(percentage, "Vamana Index Creation Progress: ");

      GraphNode<vamana_t>* sigma_i_node = this->G.getNode(sigma.at(i));
      vamana_t sigma_i = sigma_i_node->getData();

      // Run Greedy Search and Robust Prune for the current sigma[i] node and its neighbors
      greedyResult = GreedySearch(this->G, s, this->P.at(sigma.at(i)), 1, L);
      RobustPrune(this->G, *sigma_i_node, greedyResult.second, alpha, R);

      // Get the neighbors of sigma[i] node and iterate over them to run Robust Prune for each one of them as well
      std::vector<vamana_t>* sigma_i_neighbors = sigma_i_node->getNeighbors();
      for (auto j : *sigma_i_neighbors) {
        std::set<vamana_t> outgoing;
        GraphNode<vamana_t>* j_node = this->G.getNode(j.getIndex());

        // The outgoing set has to consist of the neighbors of j and the sigma[i] node itself
        for (auto neighbor : *j_node->getNeighbors()) {
          outgoing.insert(neighbor);
        }
        outgoing.insert(sigma_i);

        // Check if the |N_out(j) union {sigma[i]}| > R and run Robust Prune accordingly
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

  /**
   * @brief Saves a specific graph into a file. Specifically this method is used to save the contents of a Vamana 
   * Index Graph, inside a file in order to be loaded later for further usage. The main point of this method is to 
   * reduce the time of the production.
   * 
   * @param filename the full path of the file in which the graph is going to be saved
   * 
   * @return true if the graph was saved successfully, false otherwise
  */
  bool saveGraph(const std::string& filename) {

    // Open the file for writing in binary mode and check for errors in the process
    std::ofstream outFile(filename, std::ios::binary);
    if (!outFile) {
      std::cerr << "Error opening file for writing." << std::endl;
      return false;
    }

    // Save the nodes count of the graph, and its data
    outFile << this->G.getNodesCount() << std::endl;
    for (unsigned int i = 0; i < this->G.getNodesCount(); i++) {
      
      // Store the current node index and get its neighbors
      GraphNode<vamana_t>* currentNode = this->G.getNode(i);
      outFile << *currentNode;
      
      outFile << std::endl; // Newline to seperate each node's neighbors
    
    }

    // Save every edge of the graph by storing the neighbors of each node in the graph
    for (unsigned int i = 0; i < this->G.getNodesCount(); i++) {
    
      GraphNode<vamana_t>* currentNode = this->G.getNode(i);
      
      std::vector<vamana_t>* neighbors = currentNode->getNeighbors();
      unsigned int neighborsCount = neighbors->size();

      outFile << neighborsCount;
      for (unsigned int j = 0; j < neighborsCount; j++) {
        outFile << " " << neighbors->at(j);
      }

      outFile << std::endl; // Newline to seperate each node's neighbors
    
    }

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
  bool loadGraph(const std::string& filename) {

    // Open the file for reading and check for errors in the process
    std::ifstream inFile(filename);
    if (!inFile) {
      std::cerr << "Error opening file for reading.\n";
      return false;
    }


    unsigned int nodesCount;
    inFile >> nodesCount;
    
    this->G.setNodesCount(nodesCount); // Set the nodes count of the graph

    // Load the nodes of the graph and their data from the file and store them in the graph
    for (unsigned int i = 0; i < nodesCount; i++) {

      double percentage = (double)(100*i)/nodesCount;
      printProgressBar(percentage, "Loading vertices:               ");

      vamana_t currentData;
      inFile >> currentData;
      currentData.setIndex(i);

      this->G.setNodeData(i, currentData);
      this->P.push_back(currentData);

    }
    printProgressBar(100, "Loading vertices:               ");

    // Load the edges of the graph from the file and connect the nodes accordingly
    for (unsigned int i = 0; i < nodesCount; i++) {

      double percentage = (double)(100*i)/nodesCount;
      printProgressBar(percentage, "Loading edges:                  ");

      unsigned int neighborsCount;
      inFile >> neighborsCount;

      // Connect the current node with its neighbors by their indices in the graph nodes array
      for (unsigned int j = 0; j < neighborsCount; j++) {
        vamana_t currentData;
        inFile >> currentData;
        
        unsigned int neighborIndex = this->G.getNode(currentData.getIndex())->getIndex();
        this->G.connectNodesByIndex(i, neighborIndex);
      }

    }
    printProgressBar(100, "Loading edges:                  ");

    return true;

  }

  GraphNode<vamana_t> findMedoid(const Graph<vamana_t>& graph, const unsigned int maxIterations) {
    // Get the total number of nodes in the graph
    const int node_count = graph.getNodesCount();

    // Initialize a variable to track the minimum average distance found so far
    float min_average_distance = std::numeric_limits<float>::max();

    // Pointer to store the node with the minimum average distance (medoid candidate)
    GraphNode<vamana_t>* medoid_node = nullptr;

    // Iterate over all nodes to find the medoid
    // A medoid is defined as the node with the minimum sum of distances to all other nodes
    for (int i = 0; i < node_count; ++i) {
        // Get the current node as a candidate for the medoid
        GraphNode<vamana_t>* candidate_node = graph.getNode(i);

        // Variable to accumulate the total distance from this candidate node to all other nodes
        float total_distance = 0.0;

        // Calculate the sum of distances from this candidate node to every other node
        for (int j = 0; j < node_count; ++j) {
            // Skip distance calculation if the nodes are the same (distance to itself is 0)
            if (i != j) {
              total_distance += euclideanDistance(candidate_node->getData(), graph.getNode(j)->getData());
            }
        }

        // Compute the average distance for this candidate node
        // This is done by dividing the total distance by the number of other nodes
        float average_distance = total_distance / (node_count - 1);

        // Check if this candidate node has a smaller average distance than the current minimum
        // If so, update the minimum distance and set this node as the current medoid candidate
        if (average_distance < min_average_distance) {
          min_average_distance = average_distance;
          medoid_node = candidate_node;
        }
      }

    // Return the node with the smallest average distance, which is the medoid
    // The medoid represents the node that minimizes the total distance to all other nodes
    return *medoid_node;
  }

  using namespace std;
  
  map<string, int> FilteredMedoid(const Graph<T>& P, int threshold){

    /*

    method of the Graph class that returns the DataVectors as a sets
    
    */

    //Variables
    set<int> Filters;//this is the set of all the filters (i.e. categorical attributes)
    map<string,int> M; //this will be a map of Medoid for every filter's set of nodes

    //Algorithm Loop
    for(int f : Filters){
      //P()

    }

    return M;
  }

  /**
   * @brief tests a specific Vamana index and prints its accuracy. Specifically this method is used to evaluate
   * a Vamana Index Graph, by searching inside the graph for the nearest neighbors of a given query point, and
   * determining how many of them were found. This method is using the RECALL evaluation function that calculates
   * the success rate of the search process. For the search process GreedySearch is being used.
   * 
   * @param k the number of nearest neighbors to be found
   * @param L the parameter L
   * @param query_vectors the vector containing all the query points
   * @param query_number the number of the query point we are interested in
   * @param realNeighbors the exact solutions
   *  
  */
  void test(
    const unsigned int k, const unsigned int L, const std::vector<vamana_t>& query_vectors, 
    const unsigned int& query_number, const std::set<vamana_t>& realNeighbors) 
  {

    using GreedyResult = std::pair<std::set<DataVector<float>>, std::set<DataVector<float>>>;

    // Find the medoid node of the graph and run Greedy Search to find the k nearest neighbors
    GraphNode<DataVector<float>> s = findMedoid(this->G, 1000);
    GreedyResult greedyResult = GreedySearch(this->G, s, query_vectors.at(query_number), k, L);

    // Calculate the recall evaluation of the search process and print the results to the console
    float recall = calculateRecallEvaluation(greedyResult.first, realNeighbors);

    std::cout << std::endl << "[================= RESULTS =================]" << std::endl; 
    std::cout << "Recall Evaluation: " << recall*100 << "%" << std::endl;
  
  }

};

/**
 * @brief Overloading the operator << for the Vamana Index.
 * 
 * @param out the output stream
 * @param index the Vamana Index to be printed
 * 
 * @return the output stream
*/
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
