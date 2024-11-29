#include "../../../include/FilteredVamanaIndex.h"
#include "../../../include/GreedySearch.h"
#include "../../../include/RobustPrune.h"
#include "../../../include/Filter.h"
#include <map>


/**
 * @brief Generates a random permutation of integers in a specified range. This function creates a vector 
 * containing all integers from `start` to `end` and then shuffles them randomly to produce a random permutation.
 * 
 * @param start The starting integer of the range (inclusive)
 * @param end The ending integer of the range (inclusive)
 * 
 * @return A vector containing a shuffled sequence of integers from `start` to `end`
 */
static std::vector<int> generateRandomPermutation(const unsigned int start, const unsigned int end) {

  // Initialize a vector to hold the sequence of integers from start to end
  std::vector<int> permutation;
  for (unsigned int i = start; i <= end; i++) {
    permutation.push_back(i);
  }

  // Use a more unpredictable seed
  std::mt19937 gen(std::random_device{}());  // Seed with random device for true randomness
  std::shuffle(permutation.begin(), permutation.end(), gen);

  return permutation;

}

/**
 * @brief Default constructor for FilteredVamanaIndex.
 */
template <typename vamana_t> 
FilteredVamanaIndex<vamana_t>::FilteredVamanaIndex(void) : VamanaIndex<vamana_t>() {}

/**
 * @brief Constructor for FilteredVamanaIndex with filters.
 * 
 * @param filters A set of CategoricalAttributeFilter to initialize the index with.
 */
template <typename vamana_t>
FilteredVamanaIndex<vamana_t>::FilteredVamanaIndex(std::set<CategoricalAttributeFilter> filters) : VamanaIndex<vamana_t>() {
  this->F = filters;
}

/**
 * @brief Get nodes that match a specific categorical value filter.
 * 
 * @param filter The CategoricalAttributeFilter to match nodes against.
 * @return A vector of GraphNode that match the filter.
 */
template <typename vamana_t>
std::vector<GraphNode<vamana_t>> 
FilteredVamanaIndex<vamana_t>::getNodesWithCategoricalValueFilter(const CategoricalAttributeFilter& filter) {

  std::vector<GraphNode<vamana_t>> filteredNodes;
  std::vector<GraphNode<vamana_t>> graphNodes = this->getNodes();

  for (auto node : graphNodes) {
    if (node.getData().getC() == filter.getC()) {
      filteredNodes.push_back(node);
    }
  }

  return filteredNodes;

}

/**
 * @brief Create the graph with the given parameters.
 * 
 * @param P A vector of vamana_t elements.
 * @param alpha A float parameter.
 * @param L An unsigned int parameter.
 * @param R An unsigned int parameter.
 */
template <typename vamana_t>
void FilteredVamanaIndex<vamana_t>::createGraph(
  const std::vector<vamana_t>& P, const float& alpha, const unsigned int L, const unsigned int R) {

  using Filter = CategoricalAttributeFilter;
  using GreedyResult = std::pair<std::set<vamana_t>, std::set<vamana_t>>;
  
  // Initialize graph memory
  unsigned int n = P.size();
  this->P = P;
  this->G.setNodesCount(n);

  // Initialize G to an empty graph and get the medoid node
  this->fillGraphNodes();
  GraphNode<vamana_t> s = this->findMedoid(this->G, 1000);

  // Let st(f) be the start node for filter label f for every f in F
  std::map<Filter, GraphNode<vamana_t>> st;
  for (auto filter : this->F) {
    std::vector<GraphNode<vamana_t>> nodes = this->getNodesWithCategoricalValueFilter(filter);
    GraphNode<vamana_t> startNode = nodes[0];

    st[filter] = startNode;
  }

  // Let sigma be a random permutation of the indices of [n]
  std::vector<int> sigma = generateRandomPermutation(0, n-1);

  // Let Fx be the label-set for every x in P
  std::map<vamana_t, Filter> Fx;
  for (auto node : P) {
    Fx[node] = CategoricalAttributeFilter(node.getC());
  }

  // Execute the main for loop execution of the algorithm, but with the addition of a progress bar
  withProgress(0, n, "Creating Filtered Vamana", [&](int i) {

    // Let S_F_x_sigma[i] = { st(f) : f in F_X_sigma[i] }
    std::vector<GraphNode<vamana_t>> S_F_x_sigma_i;
    vamana_t x = P[sigma[i]];
    Filter F_x_sigma_i = Fx[x];
    S_F_x_sigma_i.push_back(st[F_x_sigma_i]);

    // Run Filtered Greedy Search with S = S_F_x_sigma[i], query = x_sigm[i], 
    // and query filters = F_x_sigma[i]
    std::vector<Filter> queryFilters;
    queryFilters.push_back(F_x_sigma_i);

    GreedyResult greedyResult = FilteredGreedySearch(
      this->G, S_F_x_sigma_i, this->P[sigma[i]], 0, L, queryFilters
    );

    // Construct the V_F_x_sigma[i] based on the second greedy result item
    std::set<vamana_t> V_F_x_sigma_i = greedyResult.second;

    // NOTE: In the command V <- V union V_F_x_sigma[i], set V is missing in the pseudocode

    // Run Filtered Robust Prune to update out-neighbors of sigma[i]
    GraphNode<vamana_t>* sigma_i = this->G.getNode(this->P[sigma[i]].getIndex());
    FilteredRobustPrune(this->G, *sigma_i, V_F_x_sigma_i, alpha, R);

    // Receive neighbors of sigma_i
    std::vector<vamana_t>* neighbors = sigma_i->getNeighborsVector();
    for (auto j : *neighbors) {
    
      // Add sigma_i to the neighbors of the current j
      GraphNode<vamana_t>* j_node = this->G.getNode(j.getIndex());
      j_node->addNeighbor(this->P[sigma[i]]);

      // Checking if the neighbors of j is greater than R. If so run Filtered Robust Prune
      std::set<vamana_t> j_neighbors = j_node->getNeighborsSet();
      if (j_neighbors.size() > R) {
        FilteredRobustPrune(this->G, *j_node, j_neighbors, alpha, R);
      }

    }

  });

}

template class FilteredVamanaIndex<BaseDataVector<float>>;
