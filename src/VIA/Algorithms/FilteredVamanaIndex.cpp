#include "../../../include/FilteredVamanaIndex.h"


template <typename vamana_t> 
FilteredVamanaIndex<vamana_t>::FilteredVamanaIndex(void) : VamanaIndex<vamana_t>() {
  std::cout << "Filtered Vamana Index created" << std::endl;
}

template <typename vamana_t>
void FilteredVamanaIndex<vamana_t>::createGraph(
  const std::vector<vamana_t>& P, const float& alpha, const unsigned int L, const unsigned int R) {
  
  this->fillGraphNodes();
  std::cout << "Creating Filtered Vamana Index Graph" << std::endl;

  

}

template class FilteredVamanaIndex<BaseDataVector<float>>;
