#include <iostream>
#include <cstdlib>
#include <vector>
#include <set>
#include <ctime>
#include "include/Graph/graph.h"
#include "include/read_data.h"
#include "include/DataVector/DataVector.h"
#include "include/GreedySearch.h"
#include "include/vanama.h"

// Type Alias
typedef GraphNode<DataVector<float>> GreedySearchNode;
typedef Graph<DataVector<float>> GreedySearchGraph;
typedef std::set<GreedySearchNode> GreedySearchNodeSet;

/**
 * @brief Prints the contents of a set, where each element is a node with data.
 * 
 * @param set_t The type of elements within the set
 * @param set The set to print
 */
template <typename set_t> static void printSet(const std::set<set_t> set) {

  // Print the given set
  std::cout << "{";
  for (unsigned int i = 0; i < set.size(); i++) {
    auto it = set.begin();
    std::advance(it, i);
    set_t currentItem = *it;
    std::cout << currentItem.getData() << ", ";
  }
  std::cout << "}" << std::endl;

}

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

  std::string base_path;
  std::string query_path;

  // Check if the program call was correct
  if (argc != 3) {
    std::cout << "Usage: " << argv[0] << " [base_path] " << "[query_path]" << std::endl;
    return 1;
  }

  // Assign the path of the dataset and read its contents
  base_path = argv[1];
  query_path = argv[2];
  
  // Store the dataset vectors
  std::vector<DataVector<float>> base_vectors = ReadVectorFile(base_path);
  std::vector<DataVector<float>> query_vectors = ReadVectorFile(query_path);

  // Create the graph and get its first node
  GreedySearchGraph graph = createGraph(base_vectors, 10);

  Create_Vamana_Index(graph, 2, 5, 10);

  return 0;

}
