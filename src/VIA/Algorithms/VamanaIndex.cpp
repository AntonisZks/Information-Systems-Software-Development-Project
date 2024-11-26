#include "../../../include/VamanaIndex.h"
#include "../../../include/DataVector.h"

#include <thread>
#include <chrono>

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
 * @brief Generates a set of unique random indices, excluding a specific index.
 * 
 * @param max The maximum value for random index generation
 * @param i The index to exclude from the random selection
 * @param length The number of unique random indices to generate
 * 
 * @return A set of unique random indices of the specified length, excluding index i
 */
static std::set<int> generateRandomIndices(const unsigned int max, const unsigned int i, unsigned int length) {

  // Initialize a set for the indices
  std::set<int> indices;

  static std::mt19937 generator(std::random_device{}());
  std::uniform_int_distribution<unsigned int> distribution(0, max - 1);

  // Keep assigning random integer values to the set until we reach the given length
  while (indices.size() < length) {
    unsigned int randInd = distribution(generator);
    if (randInd != i) {
      indices.insert(randInd);
    }
  }

  return indices;

}

/**
 * @brief Fills the graph nodes with the given dataset points. 
*/
template <typename vamana_t> void VamanaIndex<vamana_t>::fillGraphNodes(void) {
  for (unsigned int i = 0; i < this->P.size(); i++) {
    this->G.setNodeData(i, this->P.at(i));
  }
}

/**
 * @brief Populates the graph of the vamana index. Speicifcally it fills every graph node with edges pointing
 * to random neighbors inside the graph, with these edges being at most maxEdges.
 * 
 * @param maxEdges the maximum number of edges a node can have.
*/
template <typename vamana_t> 
void VamanaIndex<vamana_t>::createRandomEdges(const unsigned int maxEdges) {

  // Fill the graph randomly
  for (unsigned int i = 0; i < this->G.getNodesCount(); i++) {
      
    // Generate a random range of indeces and apply those indeces as neighbors for the current node
    std::set<int> indeces = generateRandomIndices(this->G.getNodesCount(), i, maxEdges);
    for (unsigned int j = 0; j < indeces.size(); j++) {
      std::set<int>::iterator it = indeces.begin();
      std::advance(it, j);
      auto currentIndex = *it;

      this->G.connectNodesByIndex(i, currentIndex);
    }   
  }
}

/**
 * @brief Creates a Vamana Index Graph according to the provided dataset points and the given parameters.
 * Specifically this method follows the Vamana algorithm found on the paper:
 * 
 * https://proceedings.neurips.cc/paper_files/paper/2019/file/09853c7fb1d3f8ee67a61b6bf4a7f8e6-Paper.pdf
 * 
 * and creates a directed, unweighted, and random connected graph data structure, populated with the data points,
 * using the GreedySearch and RobustPrune algorithms.
 * 
 * @param P the vector containing the data points
 * @param alpha the parameter alpha
 * @param L the parameter L
 * @param R the parameter R
 * 
*/
template <typename vamana_t> 
void VamanaIndex<vamana_t>::createGraph(
  const std::vector<vamana_t>& P, const float& alpha, const unsigned int L, const unsigned int& R) {

  using GreedyResult = std::pair<std::set<vamana_t>, std::set<vamana_t>>;

  GreedyResult greedyResult;
  this->P = P;
  unsigned int n = P.size();
  this->G.setNodesCount(n);

  // Initialize the graph to a random R-regular directed graph with n vertices and R edges per vertex
  this->fillGraphNodes();
  this->createRandomEdges(R);
  GraphNode<vamana_t> s = findMedoid(this->G, 1000);
  std::vector<int> sigma = generateRandomPermutation(0, n-1);

  withProgress(0, n, "Creating Vamana", [&](int i) {
    
    GraphNode<vamana_t>* sigma_i_node = this->G.getNode(sigma.at(i));
    vamana_t sigma_i = sigma_i_node->getData();

    // Run Greedy Search and Robust Prune for the current sigma[i] node and its neighbors
    greedyResult = GreedySearch(this->G, s, this->P.at(sigma.at(i)), 1, L);
    RobustPrune(this->G, *sigma_i_node, greedyResult.second, alpha, R);

    // Get the neighbors of sigma[i] node and iterate over them to run Robust Prune for each one of them as well
    std::vector<vamana_t>* sigma_i_neighbors = sigma_i_node->getNeighbors();
    for (auto j : *sigma_i_neighbors) {
      std::set<vamana_t> outgoing;
      GraphNode<vamana_t>* j_node = this->G.getNode(j.getIndex());

      // The outgoing set has to consist of the neighbors of j and the sigma[i] node itself
      for (auto neighbor : *j_node->getNeighbors()) {
        outgoing.insert(neighbor);
      }
      outgoing.insert(sigma_i);

      // Check if the |N_out(j) union {sigma[i]}| > R and run Robust Prune accordingly
      if (outgoing.size() > (long unsigned int)R) {
        RobustPrune(this->G, *j_node, outgoing, alpha, R);
      } 
      else {
        j_node->addNeighbor(sigma_i);
      }
    }

  });

}

/**
 * @brief Saves a specific graph into a file. Specifically this method is used to save the contents of a Vamana 
 * Index Graph, inside a file in order to be loaded later for further usage. The main point of this method is to 
 * reduce the time of the production.
 * 
 * @param filename the full path of the file in which the graph is going to be saved
 * 
 * @return true if the graph was saved successfully, false otherwise
*/
template <typename vamana_t> bool VamanaIndex<vamana_t>::saveGraph(const std::string& filename) {

  // Open the file for writing in binary mode and check for errors in the process
  std::ofstream outFile(filename, std::ios::binary);
  if (!outFile) {
    std::cerr << "Error opening file for writing." << std::endl;
    return false;
  }

  // Save the nodes count of the graph, and its data
  outFile << this->G.getNodesCount() << std::endl;
  withProgress(0, this->G.getNodesCount(), "Saving Nodes", [&](int i) {
  
    // Store the current node index and get its neighbors
    GraphNode<vamana_t>* currentNode = this->G.getNode(i);
    outFile << *currentNode;
    
    outFile << std::endl; // Newline to seperate each node's neighbors
  
  });

  withProgress(0, this->G.getNodesCount(), "Saving Edges", [&](int i) {

    GraphNode<vamana_t>* currentNode = this->G.getNode(i);
    
    std::vector<vamana_t>* neighbors = currentNode->getNeighbors();
    unsigned int neighborsCount = neighbors->size();

    outFile << neighborsCount;
    for (unsigned int j = 0; j < neighborsCount; j++) {
      outFile << " " << neighbors->at(j);
    }

    outFile << std::endl; // Newline to seperate each node's neighbors

  });

  return true;

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
template <typename vamana_t> bool VamanaIndex<vamana_t>::loadGraph(const std::string& filename) {

  // Open the file for reading and check for errors in the process
  std::ifstream inFile(filename);
  if (!inFile) {
    std::cerr << "Error opening file for reading.\n";
    return false;
  }


  unsigned int nodesCount;
  inFile >> nodesCount;
  
  this->G.setNodesCount(nodesCount); // Set the nodes count of the graph

  // Load the nodes of the graph and their data from the file and store them in the graph
  withProgress(0, nodesCount, "Nodes Loading", [&](int i) {
    
    vamana_t currentData;
    inFile >> currentData;
    currentData.setIndex(i);

    this->G.setNodeData(i, currentData);
    this->P.push_back(currentData);
  
  });

  // Load the edges of the graph from the file and connect the nodes accordingly
  withProgress(0, nodesCount, "Edges Loading", [&](int i) {

    unsigned int neighborsCount;
    inFile >> neighborsCount;

    // Connect the current node with its neighbors by their indices in the graph nodes array
    for (unsigned int j = 0; j < neighborsCount; j++) {
      vamana_t currentData;
      inFile >> currentData;
      
      unsigned int neighborIndex = this->G.getNode(currentData.getIndex())->getIndex();
      this->G.connectNodesByIndex(i, neighborIndex);
    }

  });

  return true;

}

// template <typename vamana_t>
// map<int, BaseDataVector<float>> VamanaIndex<vamana_t>::FilteredMedoid(const Graph<vamana_t>& Graph, int threshold) {

//   //________________________________________________________
//   //
//   //                          DATA
//   //________________________________________________________

//   //Set of all nodes (with there data).
//   set<BaseDataVector<float>> P = Graph.getNodesSet;

//   //the given threshold
//   //int τ = threshold;

//   //IMPLEMENTATION
//   // # set<int> GetGraphFilters(Graph<vamana_t>& Graph)
//   //We need a function that returns a set of all the categorical attributes
//   //that are in the graph.
//   //--------------------------------------------------------------------------------------
//   //Set of all categorical attributes.
//   set<int> Filters; //= GetGraphFilters(Graph)

//   //The map that the function returns
//   //The key is the categorical attribute (int) the filter,
//   //and it correspods to a the value, which is a node (BaseDataVector<float>)
//   // f=1 -> starting node: BaseDataVector<float> p1
//   // f=2 -> starting node: BaseDataVector<float> p2
//   // ...
//   map<int, BaseDataVector<float>> M;

//   //The map that takes the key (a node), and connects it
//   //to an integer (number of times this node was considered
//   //to be the filtered medoid point)
//   map<BaseDataVector<float>,int> T;
//   //Initialize T to an zero map
//   for (BaseDataVector<float> nodes : P){
//     T[nodes] = 0;
//   }

//   //True medoid point of Pf, p*
//   BaseDataVector<float> pointAsterisk;

//   //__________________________________________________________
//   //
//   //                    ALGORITHM LOOP
//   //__________________________________________________________

//   for(int f : Filters){
    
//     //IMPLEMENTATION
//     //# set<BaseDataVector<float>> GetNodesWithFilters(Graph<vamana_t>& Graph, int filter){}
//     //We need a function that returns a subset of nodes of the set P
//     //that correspond to the arguement f (nodes that have the same categorical attribute).
//     //--------------------------------------------------------------------------------------
//     //Subset of P. Nodes with the same categorical attribute.
//     set<BaseDataVector<float>> Pf; //= GetNodesWithFilters(P, f)

//     //IMPLEMENTATION
//     //# set<BaseDataVector<float>> GetRandomNodesWithFilters(set<BaseDataVector<float>> Set, int threshold){}
//     //We need a function that returns a subset of τ random nodes of the set P
//     //that correspond to the arguement f (nodes that have the same categorical attribute).
//     //---------------------------------------------------------------------------------------
//     //Subset of Pf. Set of τ randomly selected nodes of Pf.
//     set<BaseDataVector<float>> Rf; //= GetRandomNodesWithFilters(Pf, τ)

//     //This is the arg_min method.
//     //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//     pointAsterisk = *Rf.begin();
//     int min_value = T[pointAsterisk];

//     //this is gonna run τ times
//     for(BaseDataVector<float> p : Rf){
//       if(T[p] < min_value){
//         pointAsterisk = p;
//         min_value = T[p];
//       }
//     }
//     //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

//     //Update...
//     M[f] = pointAsterisk;
//     T[pointAsterisk]++; 
//   }

//   return M;

// }

/**
 * @brief Finds the medoid node in the graph using a sample of nodes.
 *
 * The medoid is the node with the minimum average distance to all other nodes in the sample.
 * This function uses Euclidean distance to calculate the distances between nodes.
 *
 * @tparam T The type of data stored in the graph nodes.
 * @param graph The graph from which to find the medoid.
 * @param sample_size The number of nodes to sample from the graph for medoid calculation. Default is 100.
 * @return The medoid node of the sampled nodes.
 */
template <typename vamana_t> GraphNode<vamana_t> VamanaIndex<vamana_t>::findMedoid(const Graph<vamana_t>& graph, int sample_size) {

  const int node_count = graph.getNodesCount();  // Get the total number of nodes in the graph
  sample_size = std::min(sample_size, node_count);  // Ensure the sample size doesn’t exceed the total node count

  // Create a list of all node indices and shuffle it to select a random subset
  std::vector<int> sampled_indices(node_count);  // Vector to hold indices from 0 to node_count - 1
  std::iota(sampled_indices.begin(), sampled_indices.end(), 0);  // Fill the vector with sequential numbers (0, 1, ..., node_count - 1)
  
  // Randomly shuffle the indices to select a random subset of nodes
  std::shuffle(sampled_indices.begin(), sampled_indices.end(), std::mt19937{std::random_device{}()});
  
  // Resize the vector to contain only `sample_size` elements, representing the sampled subset of nodes
  sampled_indices.resize(sample_size);

  // Initialize a distance matrix for the sampled nodes
  // This matrix will store the pairwise distances between each pair of sampled nodes
  std::vector<std::vector<float>> distance_matrix(sample_size, std::vector<float>(sample_size, 0.0));

  // Compute pairwise distances between each pair of sampled nodes
  withProgress(0, sample_size, "Finding Medoid", [&](int i) {

    for (int j = i + 1; j < sample_size; ++j) {
      // Calculate the Euclidean distance between nodes `i` and `j` in the sampled subset
      float dist = euclideanDistance(graph.getNode(sampled_indices[i])->getData(), graph.getNode(sampled_indices[j])->getData());
      
      // Store the computed distance in both `distance_matrix[i][j]` and `distance_matrix[j][i]`
      // This leverages the symmetry of the matrix, as distance from i to j equals distance from j to i
      distance_matrix[i][j] = dist;
      distance_matrix[j][i] = dist;
    }

  });

  // Find the medoid node among the sampled nodes by calculating the average distance for each one
  float min_average_distance = std::numeric_limits<float>::max();  // Initialize with a large value to find minimum
  GraphNode<vamana_t>* medoid_node = nullptr;  // Pointer to the node with the minimum average distance (the medoid)

  // Loop through each sampled node to calculate its average distance to other sampled nodes
  for (int i = 0; i < sample_size; ++i) {
    // Calculate the total distance from node `i` to all other nodes in the sample
    float total_distance = std::accumulate(distance_matrix[i].begin(), distance_matrix[i].end(), 0.0f);
    
    // Compute the average distance by dividing the total distance by the number of other nodes (sample_size - 1)
    float average_distance = total_distance / (sample_size - 1);

    // Check if this node has a smaller average distance than the smallest found so far
    if (average_distance < min_average_distance) {
      // Update the minimum average distance and set this node as the current medoid candidate
      min_average_distance = average_distance;
      medoid_node = graph.getNode(sampled_indices[i]);
    }
  }

  return *medoid_node;

}

 /**
 * @brief tests a specific Vamana index and prints its accuracy. Specifically this method is used to evaluate
 * a Vamana Index Graph, by searching inside the graph for the nearest neighbors of a given query point, and
 * determining how many of them were found. This method is using the RECALL evaluation function that calculates
 * the success rate of the search process. For the search process GreedySearch is being used.
 * 
 * @param k the number of nearest neighbors to be found
 * @param L the parameter L
 * @param query_vectors the vector containing all the query points
 * @param query_number the number of the query point we are interested in
 * @param realNeighbors the exact solutions
 *  
*/
template <typename vamana_t> void VamanaIndex<vamana_t>::test(const unsigned int k, const unsigned int L, const std::vector<vamana_t>& query_vectors, 
  const unsigned int& query_number, const std::set<vamana_t>& realNeighbors) 
{

  using GreedyResult = std::pair<std::set<DataVector<float>>, std::set<DataVector<float>>>;

  // Find the medoid node of the graph and run Greedy Search to find the k nearest neighbors
  GraphNode<DataVector<float>> s = findMedoid(this->G, 1000);
  GreedyResult greedyResult = GreedySearch(this->G, s, query_vectors.at(query_number), k, L);

  // Calculate the recall evaluation of the search process and print the results to the console
  float recall = calculateRecallEvaluation(greedyResult.first, realNeighbors);

  std::cout << std::endl << "[================= RESULTS =================]" << std::endl; 
  std::cout << "Recall Evaluation: " << recall*100 << "%" << std::endl;

}

template class VamanaIndex<DataVector<float>>;
