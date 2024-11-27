#include "../../../include/FilteredVamanaIndex.h"


template <typename vamana_t> 
FilteredVamanaIndex<vamana_t>::FilteredVamanaIndex(void) : VamanaIndex<vamana_t>() {}

template <typename vamana_t>
std::vector<GraphNode<vamana_t>> FilteredVamanaIndex<vamana_t>::getNodesWithCategoricalValueFilter(const CategoricalAttributeFilter& filter) {

  std::vector<GraphNode<vamana_t>> filteredNodes;
  std::vector<GraphNode<vamana_t>> graphNodes = this->getNodes();

  for (auto node : graphNodes) {
    if (node.getData().getC() == filter.getC()) {
      filteredNodes.push_back(node);
    }
  }

  return filteredNodes;

}

template <typename vamana_t>
void FilteredVamanaIndex<vamana_t>::createGraph(
  const std::vector<vamana_t>& P, const float& alpha, const unsigned int L, const unsigned int R) {
  
  // Initialize graph memory
  unsigned int n = P.size();
  this->P = P;
  this->G.setNodesCount(n);

  // Initialize G to an empty graph and get the medoid node
  this->fillGraphNodes();
  GraphNode<vamana_t> s = this->findMedoid(this->G, 1000);

}

template class FilteredVamanaIndex<BaseDataVector<float>>;
