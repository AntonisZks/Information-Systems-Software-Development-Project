#include <iostream>
#include <vector>
#include "include/Graph/graph.h"

int main(int argc, char* argv[]) {

  GraphNode<int> n1(1);
  GraphNode<int> n2(2);

  n1.addNeighbor(n2.getData());

  std::vector<int> neighbors = n1.getNeighbors();

  Graph<int> g1(3);

  g1.setNodeData(0, 1);
  g1.setNodeData(1, 2);
  g1.setNodeData(2, 3);
  

  g1.connectNodes(1, 2);
  g1.connectNodes(2, 1);
  g1.connectNodes(2, 3);
  

  std::cout << g1 << std::endl;

  return 0;

}
