#include <map>
#include "../../../include/StichedVamanaIndex.h"
#include "../../../include/Filter.h"
#include "../../../include/VamanaIndex.h"
#include "../../../include/graph.h"


template <typename vamana_t>
void StichedVamanaIndex<vamana_t>::createGraph(
  const std::vector<vamana_t>& P, const float& alpha, const unsigned int L_small, const unsigned int R_small, const unsigned int R_stiched, bool visualized) {

  using Filter = CategoricalAttributeFilter;

  // Initialize graph memory
  unsigned int n = P.size();
  this->P = P;
  this->G.setNodesCount(n);

  // Initialize G = (V, E) to an empty graph
  this->fillGraphNodes();

  // Let Fx proper subset of F be the label-set for every x in P
  std::map<vamana_t, Filter> Fx;
  for (auto node : P) {
    Fx[node] = CategoricalAttributeFilter(node.getC());
  }

  // Let Pf proper subset of P be the set of points with label f in F
  std::map<Filter, std::vector<vamana_t>> Pf;
  for (auto filter : this->F) {
    std::vector<vamana_t> points;
    for (auto node : P) {
      if (node.getC() == filter.getC()) {
        points.push_back(node);
      }
    }

    Pf[filter] = points;
  }

  // Foreach f in F do
  for (auto filter : this->F) {
    
    std::vector<vamana_t> currentVector = Pf[filter];
    
    // Keep the indexes of the current filter points in P, to connect them later. The idea here is to create a new
    // sub-index of type VamanaIndex that will contain the points of the current filter, which are going to have a different
    // index inside the new sub-graph. So we need to keep track of the indexes of the points in the original graph.
    std::map<unsigned int, unsigned int> indexes;
    for (unsigned int i = 0; i < currentVector.size(); i++) {
      vamana_t currentData = currentVector[i];
      indexes[i] = currentData.getIndex();
    }

    // Initialize the sub-index for the current filter and create its graph
    VamanaIndex<vamana_t> subIndex;
    subIndex.createGraph(Pf[filter], alpha, R_small, L_small, false);

    for (unsigned int i = 0; i < subIndex.getGraph().getNodesCount(); i++) {
      
      // Get the current node from the sub-index and its index in the sub-graph
      GraphNode<vamana_t>* node = subIndex.getGraph().getNode(i);
      unsigned int nodeIndex = node->getData().getIndex();

      // Receive all the neighbors of the current node and connect them in the main graph, using the indexes map above
      std::vector<vamana_t>* neighbors = node->getNeighborsVector();
      for (auto neighbor : *neighbors) {
        unsigned int currentNeighborIndex = neighbor.getIndex();
        this->G.connectNodesByIndex(indexes[nodeIndex], indexes[currentNeighborIndex]);
      }
      
    }

  }

  // Filtered Robust Prune for every v in V (the nodes of the graph)
  for (unsigned int i = 0; i < this->G.getNodesCount(); i++) {

    GraphNode<vamana_t>* currentNode = this->G.getNode(i);
    std::vector<vamana_t>* neighbors = currentNode->getNeighborsVector();

    // TODO: Implement Filtered Vamana call

  }

  // for (unsigned int i = 0; i < this->G.getNodesCount(); i++) {
  //   std::cout << "Node " << i << " with filter " << this->G.getNode(i)->getData().getC() << " has number of neighbors: ";
  //   std::cout << this->G.getNode(i)->getNeighborsVector()->size() << std::endl;
  // }

}

template class StichedVamanaIndex<BaseDataVector<float>>;
