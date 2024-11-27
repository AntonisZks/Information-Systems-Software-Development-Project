#ifndef FILTERED_VAMANA_INDEX_H
#define FILTERED_VAMANA_INDEX_H

#include <iostream>
#include "VamanaIndex.h"
#include "Filter.h"

template <typename vamana_t> class FilteredVamanaIndex : public VamanaIndex<vamana_t> {

private:

  std::set<CategoricalAttributeFilter> F;

public:
  
  /**
   * @brief Default constructor for FilteredVamanaIndex.
   */
  FilteredVamanaIndex(void);

  /**
   * @brief Constructor for FilteredVamanaIndex with filters.
   * 
   * @param filters A set of CategoricalAttributeFilter to initialize the index with.
   */
  FilteredVamanaIndex(std::set<CategoricalAttributeFilter> filters);

  /**
   * @brief Get the set of filters.
   * 
   * @return A set of CategoricalAttributeFilter.
   */
  std::set<CategoricalAttributeFilter> getFilters(void) const { return this->F; }

  /**
   * @brief Set the filters.
   * 
   * @param filters A set of CategoricalAttributeFilter to set.
   */
  void setFilters(std::set<CategoricalAttributeFilter> filters) { this->F = filters; }

  /**
   * @brief Get nodes that match a specific categorical value filter.
   * 
   * @param filter The CategoricalAttributeFilter to match nodes against.
   * @return A vector of GraphNode that match the filter.
   */
  std::vector<GraphNode<vamana_t>> getNodesWithCategoricalValueFilter(const CategoricalAttributeFilter& filter);

  /**
   * @brief Create the graph with the given parameters.
   * 
   * @param P A vector of vamana_t elements.
   * @param alpha A float parameter.
   * @param L An unsigned int parameter.
   * @param R An unsigned int parameter.
   */
  void createGraph(const std::vector<vamana_t>& P, const float& alpha, const unsigned int L, const unsigned int R);

};

#endif /* FILTERED_VAMANA_INDEX_H */
