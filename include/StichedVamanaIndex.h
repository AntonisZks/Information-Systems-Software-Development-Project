#ifndef STICHED_VAMANA_INDEX_H
#define STICHED_VAMANA_INDEX_H


#include <iostream>
#include <vector>
#include "FilteredVamanaIndex.h"

template <typename vamana_t> class StichedVamanaIndex : public FilteredVamanaIndex<vamana_t> {

public:

  /**
   * @brief Default constructor for the StichedVamanaIndex class.
   */
  StichedVamanaIndex() : FilteredVamanaIndex<vamana_t>() {}

  /**
   * @brief Constructor for the StichedVamanaIndex class with filters.
   * 
   * @param filters A set of CategoricalAttributeFilter to initialize the index with.
   */
  StichedVamanaIndex(std::set<CategoricalAttributeFilter> filters) : FilteredVamanaIndex<vamana_t>(filters) {}

  /**
   * @brief Create the graph with the given parameters.
   * 
   * @param P A vector of vamana_t elements.
   * @param alpha A float parameter.
   * @param L An unsigned int parameter.
   * @param R An unsigned int parameter.
   */
  void createGraph(const std::vector<vamana_t>& P, const float& alpha, const unsigned int L_small, 
                   const unsigned int R_small, const unsigned int R_stiched, unsigned int distance_threads, bool visualized = true, bool empty = true);

};


#endif /* STICHED_VAMANA_INDEX_H */
