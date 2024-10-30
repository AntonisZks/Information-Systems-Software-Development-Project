#include <iostream>
#include <cstdlib>
#include <vector>
#include <set>
#include <ctime>
#include "include/Graph/graph.h"
#include "include/read_data.h"
#include "include/DataVector/DataVector.h"
#include "include/graphFunctions.h"

static std::set<int> getRandomIndeces(const unsigned int max, const unsigned int i) {
  std::set<int> indeces;

  while (indeces.size() < 20) {
    unsigned int randInd = rand() % max;
    if (randInd != i) {
        indeces.insert(randInd);
    }
  }

  return indeces;

}

template <typename set_t>
static void printSet(const std::set<set_t> set) {

  std::cout << "{";
  for (unsigned int i = 0; i < set.size(); i++) {
    auto it = set.begin();
    std::advance(it, i);
    set_t currentItem = *it;
    std::cout << currentItem.getData() << ", ";
  }
  std::cout << "}" << std::endl;

}

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
  
  std::vector<DataVector<float>> base_vectors = ReadVectorFile(base_path);
  std::vector<DataVector<float>> query_vectors = ReadVectorFile(query_path);

  Graph<DataVector<float>> graph(10000);
  for (unsigned int i = 0; i < graph.getNodesCount(); i++) {
    graph.setNodeData(i, base_vectors.at(i));
  }

  for (unsigned int i = 0; i < graph.getNodesCount(); i++) {

      
    std::set<int> indeces = getRandomIndeces(graph.getNodesCount(), i);

    for (unsigned int j = 0; j < indeces.size(); j++) {
      std::set<int>::iterator it = indeces.begin();
      std::advance(it, j);
      auto currentIndex = *it;

      graph.connectNodesByIndex(i, currentIndex);
    }
      
  }

  GraphNode<DataVector<float>>* start_node = graph.getNode(0);
  std::pair<std::set<GraphNode<DataVector<float>>>, std::set<GraphNode<DataVector<float>>>> result = GreedySearch(graph, *start_node, query_vectors.at(0), 10, 15);

  std::cout << "K-nearest points: ";
  printSet(result.first);
  std::cout << "Visited: ";
  printSet(result.second);

  return 0;

}
