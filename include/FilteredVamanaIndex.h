#ifndef FILTERED_VAMANA_INDEX_H
#define FILTERED_VAMANA_INDEX_H


#include <iostream>
#include "VamanaIndex.h"

template <typename vamana_t> class FilteredVamanaIndex : public VamanaIndex<vamana_t> {

public:
  FilteredVamanaIndex(void);

  void createGraph(const std::vector<vamana_t>& P, const float& alpha, const unsigned int L, const unsigned int R);

};


#endif /* FILTERED_VAMANA_INDEX_H */
