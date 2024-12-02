#include <iostream>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include "../include/BQDataVectors.h"
#include "../include/read_data.h"
#include "../include/distance.h"
#include "../include/FilteredVamanaIndex.h"
#include "../include/Filter.h"
#include "../include/recall.h"
#include "../include/groundtruth.h"


/**
 * @brief Parse the command line arguments and store them in a map for easy access.
 * 
 * @param argc Number of arguments
 * @param argv Array of arguments
 * 
 * @return A map containing the parsed arguments
 */
std::unordered_map<std::string, std::string> parseArguments(int argc, char* argv[]) {
  
  std::unordered_map<std::string, std::string> args;

  // Iterate over the arguments and store them in a map for easy access
  for (unsigned int i = 2; i < (unsigned int)argc; i += 2) {
    std::string key = argv[i];

    // Check if the key is valid (starts with '-')
    if (key[0] != '-') {
      throw std::invalid_argument("Invalid argument: " + key);
    }

    // Check if the key has a value associated with it
    if (i + 1 >= (unsigned int)argc) {
      throw std::invalid_argument("Missing value for argument: " + key);
    }
    std::string value = argv[i + 1];
    args[key] = value;
  }

  return args;
}

/**
 * @brief Compute the groundtruth for a set of base and query vectors. This function handles the case of 
 * computing the groundtruth execution mode of the application. It reads the base and query vectors from 
 * the binary files, computes the groundtruth distances, and saves the computed distances to a file.
 * 
 * @param args A map containing the parsed arguments
 * 
 */
void ComputeGroundtruth(std::unordered_map<std::string, std::string> args) {

  std::string baseFile, queryFile, groundtruthFile;
  unsigned int maxDistances = 1000;

  // Check if the required arguments are provided
  if (args.find("-base-file") == args.end()) {
    throw std::invalid_argument("Missing required argument: -base-file");
  } else {
    baseFile = args["-base-file"];
  }

  if (args.find("-query-file") == args.end()) {
    throw std::invalid_argument("Missing required argument: -query-file");
  } else {
    queryFile = args["-query-file"];
  }

  if (args.find("-gt-file") == args.end()) {
    throw std::invalid_argument("Missing required argument: -gt-file");
  } else {
    groundtruthFile = args["-gt-file"];
  }

  if (args.find("-max-distances") != args.end()) {
    maxDistances = std::stoi(args["-max-distances"]);
  }

  // TODO: Reject any other argument by terminating the program


  // Create some aliases for the vector types
  using BaseVectorVector = std::vector<BaseDataVector<float>>;
  using QueryVectorVector = std::vector<QueryDataVector<float>>;
  
  // Read the base and query vectors from the binary files
  BaseVectorVector base_vectors = ReadFilteredBaseVectorFile(baseFile);
  QueryVectorVector query_vectors = ReadFilteredQueryVectorFile(queryFile);

  // Compute the distance vector, and save the computed distances to a file
  std::vector<std::vector<int>> base_indexes = computeGroundtruth(base_vectors, query_vectors, maxDistances);
  saveGroundtruthToFile(base_indexes, groundtruthFile);

}


int main(int argc, char* argv[]) {

  // Receive the execution mode and arguments, that is the first argument of the command line
  std::string executeMode = argv[1];
  std::unordered_map<std::string, std::string> args;

  // Trye to parse the arguments and catch any invalid argument exceptions
  try {

    args = parseArguments(argc, argv);

    // Check the execution mode and call the appropriate function
    if (executeMode == "--compute-gt") {
      ComputeGroundtruth(args);
    } 
    else {
      std::cerr << "Invalid execution mode: " << executeMode << std::endl;
      return 1;
    }

  }
  catch (std::invalid_argument& e) {
    std::cerr << "Error: " << e.what() << std::endl;
    return 1;
  }
  

  // // Example usage of readGroundtruthFromFile
  // std::vector<std::vector<int>> groundtruth = readGroundtruthFromFile("data/Dummy/dummy-groundtruth.bin");

  // // Initialize all the filters
  // // IMPORTANT: This version of the application only supports CategoricalAttributeFilter
  // std::set<CategoricalAttributeFilter> filters;
  // for (auto vector : base_vectors) {
  //   CategoricalAttributeFilter filter(vector.getC());
  //   filters.insert(filter);
  // }

  // // Initialize and create the filtered Vamana index
  // FilteredVamanaIndex<BaseDataVector<float>> index(filters);
  // index.createGraph(base_vectors, 1.1, 150, 12);

  // // Store the start nodes for each filter inside a vector
  // std::vector<GraphNode<BaseDataVector<float>>> start_nodes;
  // for (auto filter : filters) {
  //   start_nodes.push_back(index.getNodesWithCategoricalValueFilter(filter)[0]); // TODO: Use filtered Medoid instead of the first node
  // }

  // // Initialize the query vector and its filters vector
  // for (unsigned int query_number = 0; query_number < 100; query_number++) {

  //   QueryDataVector<float> xq = query_vectors[query_number];
  //   if (xq.getQueryType() > 1) {
  //     continue;
  //   }
    
  //   std::vector<CategoricalAttributeFilter> Fx;
  //   if (xq.getQueryType() == 1) {
  //     Fx.push_back(CategoricalAttributeFilter(xq.getV()));
  //   }

  //   std::cout << "Query type: " << xq.getQueryType() << " ";
  //   unsigned int k = 100;
    
  //   // Initialize the arguments for the recall function
  //   std::set<BaseDataVector<float>> exactNeighbors;
  //   for (auto index : groundtruth[query_number]) {
  //     exactNeighbors.insert(base_vectors[index]);
  //     if (exactNeighbors.size() >= k) {
  //       break;
  //     }
  //   }

  //   GreedyResult greedyResult = FilteredGreedySearch(index.getGraph(), start_nodes, xq, k, 150, Fx);
  //   std::set<BaseDataVector<float>> approximateNeighbors = greedyResult.first;

  //   double recall = calculateRecallEvaluation(approximateNeighbors, exactNeighbors);
  //   std::cout << "Recall: " << recall << std::endl;
  // }

  return 0;

}
