#include <iostream>
#include <cstdlib>
#include <vector>
#include <set>
#include <ctime>
#include "include/Graph/graph.h"
#include "include/read_data.h"
#include "include/DataVector/DataVector.h"
#include "include/graphFunctions.h"

// Type Alias
typedef GraphNode<DataVector<float>> GreedySearchNode;
typedef Graph<DataVector<float>> GreedySearchGraph;
typedef std::set<GreedySearchNode> GreedySearchNodeSet;

/**
 * @brief Generates a set of unique random indices, excluding a specific index.
 * 
 * @param max The maximum value for random index generation
 * @param i The index to exclude from the random selection
 * @param length The number of unique random indices to generate
 * 
 * @return A set of unique random indices of the specified length, excluding index i
 */
static std::set<int> generateRandomIndeces(const unsigned int max, const unsigned int i, unsigned int length) {

  // Initialize a set for the indeces
  std::set<int> indeces;

  // Keep asigning random integer values to the set until we reach the given length
  while (indeces.size() < length) {
    unsigned int randInd = rand() % max;
    if (randInd != i) {
        indeces.insert(randInd);
    }
  }

  return indeces;

}

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

  // Fill the graph randomly
  for (unsigned int i = 0; i < graph.getNodesCount(); i++) {
      
    // Generate a random range of indeces and apply those indeces as neighbors for the current node
    std::set<int> indeces = generateRandomIndeces(graph.getNodesCount(), i, max_edges);

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
GreedySearchNode* start_node = graph.getNode(0);

// Execute the Greedy Search Algorithm to the graph
std::pair<GreedySearchNodeSet, GreedySearchNodeSet> result = GreedySearch(graph, *start_node, query_vectors.at(0), 10, 15);

std::cout << "K-nearest points: "; printSet(result.first);
std::cout << "Visited: "; printSet(result.second);

// Execute the Robust Prune Algorithm to the graph

// Convert query_vector to a GraphNode
GraphNode<DataVector<float>> query_node(query_vectors.at(0));
// Create the graph with more edges per node
GreedySearchGraph graph1 = createGraph(base_vectors, 50); // Increase max_edges

// Execute the Robust Prune Algorithm to the graph
std::pair<std::set<GraphNode<DataVector<float>>>, std::set<GraphNode<DataVector<float>>>> pruned_result = robustPrune(graph1, *start_node, 2.0f, 15);

std::cout << "Visited nodes: ";
 printSet(pruned_result.first);
 std::cout << "Pruned neighbors: ";
 printSet(pruned_result.second);


return 0;
}
