#include <map>
#include "../../../include/StichedVamanaIndex.h"
#include "../../../include/Filter.h"
#include "../../../include/VamanaIndex.h"
#include "../../../include/graph.h"
#include "../../../include/RobustPrune.h"
#include "../../../include/graphics.h"


/**
 * @brief Create the graph with the given parameters.
 * 
 * @param P A vector of vamana_t elements.
 * @param alpha A float parameter.
 * @param L An unsigned int parameter.
 * @param R An unsigned int parameter.
 */
template <typename vamana_t>
void StichedVamanaIndex<vamana_t>::createGraph(
  const std::vector<vamana_t>& P, const float& alpha, const unsigned int L_small, const unsigned int R_small, const unsigned int R_stiched, bool visualized) {

  using Filter = CategoricalAttributeFilter;

  // Initialize graph memory
  unsigned int n = P.size();
  this->P = P;
  this->distanceMatrix = new double*[n];
  for (unsigned int i = 0; i < n; i++) {
    this->distanceMatrix[i] = new double[n];
  }
  this->computeDistances();

  // Initialize G = (V, E) to an empty graph
  this->G.setNodesCount(n);
  this->fillGraphNodes();

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
  withProgress(0, this->F.size(), "Creating Stiched Vamana", [&](int i) {

    // Get the current filter
    std::set<Filter>::iterator it = this->F.begin();
    std::advance(it, i);
    Filter filter = *it;

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
    subIndex.createGraph(Pf[filter], alpha, R_small, L_small, false, this->distanceMatrix);

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

  });

  // Filtered Robust Prune for every v in V (the nodes of the graph)
  for (unsigned int i = 0; i < this->G.getNodesCount(); i++) {

    GraphNode<vamana_t>* currentNode = this->G.getNode(i);
    std::set<vamana_t> neighbors = currentNode->getNeighborsSet();

    // Run Filtered Robust Prune for the current node and its neighbors
    FilteredRobustPrune(*this, *currentNode, neighbors, alpha, R_stiched);

  }

  // Free up the memory allocated for the distance matrix
  for (unsigned int i = 0; i < n; i++) {
    delete[] this->distanceMatrix[i];
  }
  delete[] this->distanceMatrix;

}

template class StichedVamanaIndex<BaseDataVector<float>>;
