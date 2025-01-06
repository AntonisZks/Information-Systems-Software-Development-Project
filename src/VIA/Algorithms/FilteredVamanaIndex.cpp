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
  const std::vector<vamana_t>& P, const float& alpha, const unsigned int L, const unsigned int R, const DISTANCE_SAVE_METHOD distanceSaveMethod,
  unsigned int distance_threads, bool visualized, bool empty) {

  using Filter = CategoricalAttributeFilter;
  using GreedyResult = std::pair<std::set<vamana_t>, std::set<vamana_t>>;
  
  // Initialize graph memory
  unsigned int n = P.size();
  this->P = P;

  // Compute the distances between the points if it is specified to save the distances in a matrix
  if (distanceSaveMethod == MATRIX) {
    this->distanceMatrix = new double*[n];
    for (unsigned int i = 0; i < n; i++) {
      this->distanceMatrix[i] = new double[n];
    }
    this->computeDistances(true, distance_threads);
  }

  // Initialize G to an empty graph and get the medoid node
  this->G.setNodesCount(n);
  this->fillGraphNodes();

  // Fill graph with random edges if required
  if (!empty) {
    this->createRandomEdges(R);
  }
  GraphNode<vamana_t> s = this->findMedoid(this->G, 1000);

  // Let st(f) be the start node for filter label f for every f in F.
  std::map<Filter, GraphNode<vamana_t>> st = this->findFilteredMedoid(1000);

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

    GreedyResult greedyResult = FilteredGreedySearch(*this, S_F_x_sigma_i, this->P[sigma[i]], 0, L, queryFilters, distanceSaveMethod);

    // Construct the V_F_x_sigma[i] based on the second greedy result item
    std::set<vamana_t> V_F_x_sigma_i = greedyResult.second;

    // NOTE: In the command V <- V union V_F_x_sigma[i], set V is missing in the pseudocode

    // Run Filtered Robust Prune to update out-neighbors of sigma[i]
    GraphNode<vamana_t>* sigma_i = this->G.getNode(this->P[sigma[i]].getIndex());
    FilteredRobustPrune(*this, *sigma_i, V_F_x_sigma_i, alpha, R, distanceSaveMethod);

    // Receive neighbors of sigma_i
    std::vector<vamana_t>* neighbors = sigma_i->getNeighborsVector();
    for (auto j : *neighbors) {
    
      // Add sigma_i to the neighbors of the current j
      GraphNode<vamana_t>* j_node = this->G.getNode(j.getIndex());
      j_node->addNeighbor(this->P[sigma[i]]);

      // Checking if the neighbors of j is greater than R. If so run Filtered Robust Prune
      std::set<vamana_t> j_neighbors = j_node->getNeighborsSet();
      if (j_neighbors.size() > R) {
        FilteredRobustPrune(*this, *j_node, j_neighbors, alpha, R, distanceSaveMethod);
      }

    }

  });

  // Free up the memory allocated for the distance matrix
  if (distanceSaveMethod == MATRIX) {
    for (unsigned int i = 0; i < n; i++) {
      delete[] this->distanceMatrix[i];
    }
    delete[] this->distanceMatrix;
  }

}

/**
 * @brief Load a graph from a file. Specifically this method is used to receive the contents of a Vamana Index Graph
 * stored inside a file and create the Vamana Index object based on those contents. It is used to save time of the 
 * production making it easy to use an index with specific parameters just by loading it instead of creating it again.
 * 
 * @param filename the full path of the file containing the graph
 * 
 * @return true if the graph was loaded successfully, false otherwise
*/
template <typename vamana_t> bool FilteredVamanaIndex<vamana_t>::loadGraph(const std::string& filename) {

  // Load the graph from the file using the VamanaIndex loadGraph method
  if (!VamanaIndex<vamana_t>::loadGraph(filename)) {
    return false;
  }

  // Initialize the filters from the graph nodes
  // IMPORTANT: This version of the application only supports CategoricalAttributeFilter
  std::set<CategoricalAttributeFilter> filters;
  for (auto vector : this->P) {
    CategoricalAttributeFilter filter(vector.getC());
    filters.insert(filter);
  }
  this->setFilters(filters);

  return true;

}

/**
 * @brief Finds the set of medoid nodes in the graph using a sample of nodes.
 *
 * The medoid is the node with the minimum average distance to all other nodes in the sample.
 * This function uses Euclidean distance to calculate the distances between nodes.
 *
 * @param tau The graph from which to find the medoid.
 * @return A map containing the medoid node for each filter.
 */
template <typename vamana_t>
std::map<Filter, GraphNode<vamana_t>> FilteredVamanaIndex<vamana_t>::findFilteredMedoid(const unsigned int tau) {

  // Initialize M to be an empty map, and T to a zero map
  std::map<Filter, GraphNode<vamana_t>> M;
  std::map<GraphNode<vamana_t>, unsigned int> T;
  for (unsigned int i = 0; i < this->G.getNodesCount(); i++) {
    T[*this->G.getNode(i)] = 0;
  }

  // Foreach f in F, the set of all filters do
  withProgress(0, this->F.size(), "Finding Filtered Medoid", [&](int i) {

    // Get the filter
    Filter filter = *std::next(this->F.begin(), i);

    // Let Pf be the set of points with label f in F
    std::vector<GraphNode<vamana_t>> Pf = this->getNodesWithCategoricalValueFilter(filter);

    // Let Rf be a random sample of tau points from Pf
    std::vector<int> Rf_indexes = generateRandomPermutation(0, std::min(tau, (unsigned int)Pf.size()) - 1);
    std::vector<GraphNode<vamana_t>> Rf;
    for (auto i : Rf_indexes) {
      Rf.push_back(Pf[i]);
    }

    // p* <- argmin_{p in Rf} T[p]
    GraphNode<vamana_t> p_star = Rf[0];
    for (auto p : Rf) {
      if (T[p] < T[p_star]) p_star = p;
    }

    // Update M[f] <- p* and T[p*] <- T[p*] + 1
    M[filter] = p_star;
    T[p_star]++;

  });

  return M;

}

template class FilteredVamanaIndex<BaseDataVector<float>>;
