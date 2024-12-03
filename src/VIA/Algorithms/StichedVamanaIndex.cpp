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
    std::cout << "Creating graph for filter: " << filter.getC() << std::endl;
    VamanaIndex<vamana_t>::createGraph(Pf[filter], alpha, R_small, L_small, false);
  }

  std::cout << this->G << std::endl;

}

template class StichedVamanaIndex<BaseDataVector<float>>;
