#ifndef FILTERED_VAMANA_INDEX_H
#define FILTERED_VAMANA_INDEX_H

#include <iostream>
#include <map>
#include "VamanaIndex.h"
#include "Filter.h"

using Filter = CategoricalAttributeFilter;

template <typename vamana_t> class VamanaIndex;

template <typename vamana_t> class FilteredVamanaIndex : public VamanaIndex<vamana_t> {

protected:

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
  void createGraph(const std::vector<vamana_t>& P, const float& alpha, const unsigned int L, const unsigned int R, unsigned int distance_threads = 1, bool visualized = true, bool empty = true);

  /**
   * @brief Load a graph from a file. Specifically this method is used to receive the contents of a Vamana Index Graph
   * stored inside a file and create the Vamana Index object based on those contents. It is used to save time of the 
   * production making it easy to use an index with specific parameters just by loading it instead of creating it again.
   * 
   * @param filename the full path of the file containing the graph
   * 
   * @return true if the graph was loaded successfully, false otherwise
  */
  bool loadGraph(const std::string& filename);

  /**
   * @brief Finds the set of medoid nodes in the graph using a sample of nodes.
   *
   * The medoid is the node with the minimum average distance to all other nodes in the sample.
   * This function uses Euclidean distance to calculate the distances between nodes.
   *
   * @param tau The graph from which to find the medoid.
   * @return A map containing the medoid node for each filter.
   */
  std::map<Filter, GraphNode<vamana_t>> findFilteredMedoid(const unsigned int tau);

};

#endif /* FILTERED_VAMANA_INDEX_H */
