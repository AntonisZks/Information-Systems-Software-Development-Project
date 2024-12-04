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
#include "../include/StichedVamanaIndex.h"


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

  // Create some aliases for the vector types
  using BaseVectorVector = std::vector<BaseDataVector<float>>;
  using QueryVectorVector = std::vector<QueryDataVector<float>>;

  std::string baseFile, queryFile, groundtruthFile;
  unsigned int maxDistances = 1000;

  // Check if the command line arguments of the execution mode are valid
  std::vector<std::string> validArguments = {"-base-file", "-query-file", "-gt-file", "-max-distances"};
  for (auto arg : args) {
    if (std::find(validArguments.begin(), validArguments.end(), arg.first) == validArguments.end()) {
      throw std::invalid_argument("Invalid argument: " + arg.first);
    }
  }

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
  
  // Read the base and query vectors from the binary files
  BaseVectorVector base_vectors = ReadFilteredBaseVectorFile(baseFile);
  QueryVectorVector query_vectors = ReadFilteredQueryVectorFile(queryFile);

  // Compute the distance vector, and save the computed distances to a file
  std::vector<std::vector<int>> base_indexes = computeGroundtruth(base_vectors, query_vectors, maxDistances);
  saveGroundtruthToFile(base_indexes, groundtruthFile);

}


void Create(std::unordered_map<std::string, std::string> args) {

  // Create some aliases for the vector types
  using BaseVectorVector = std::vector<BaseDataVector<float>>;
  
  std::string indexType, baseFile, L, R, alpha, outputFile;
  std::string L_small, R_small, R_stiched;
  bool save = false;

  // Check if the command line arguments of the execution mode are valid
  std::vector<std::string> validArguments = {"-index-type", "-base-file", "-L", "-L-small", "-R", "-R-small", "-R-stiched", "-alpha", "-save"};
  for (auto arg : args) {
    if (std::find(validArguments.begin(), validArguments.end(), arg.first) == validArguments.end()) {
      throw std::invalid_argument("Invalid argument: " + arg.first);
    }
  }

  // Check if the required arguments are provided
  if (args.find("-index-type") == args.end()) {
    throw std::invalid_argument("Missing required argument: -index-type");
  } else {
    indexType = args["-index-type"];
    if (indexType == "stiched") {
    }
  }

  if (indexType == "filtered") {

    if (args.find("-L") == args.end()) {
      throw std::invalid_argument("Missing required argument: -L");
    } else {
      L = args["-L"];
    }

    if (args.find("-R") == args.end()) {
      throw std::invalid_argument("Missing required argument: -R");
    } else {
      R = args["-R"];
    }

  }
  else if (indexType == "stiched") {

    if (args.find("-L-small") == args.end()) {
      throw std::invalid_argument("Missing required argument: -L-small");
    } else {
      L_small = args["-L-small"];
    }

    if (args.find("-R-small") == args.end()) {
      throw std::invalid_argument("Missing required argument: -R-small");
    } else {
      R_small = args["-R-small"];
    }

    if (args.find("-R-stiched") == args.end()) {
      throw std::invalid_argument("Missing required argument: -R-stiched");
    } else {
      R_stiched = args["-R-stiched"];
    }

  }
  else {
    throw std::invalid_argument("Invalid index type: " + indexType + ". Supported index types are: filtered, stiched");
  }

  if (args.find("-base-file") == args.end()) {
    throw std::invalid_argument("Missing required argument: -base-file");
  } else {
    baseFile = args["-base-file"];
  }

  if (args.find("-alpha") == args.end()) {
    throw std::invalid_argument("Missing required argument: -alpha");
  } else {
    alpha = args["-alpha"];
  }

  if (args.find("-save") != args.end()) {
    if (args["-save"] == "") {
      throw std::invalid_argument("Missing value for argument: -save");
    }
    outputFile = args["-save"];
    save = true;
  }

  // Read the base vectors from the binary file
  BaseVectorVector base_vectors = ReadFilteredBaseVectorFile(baseFile);

  // Initialize all the filters
  // IMPORTANT: This version of the application only supports CategoricalAttributeFilter
  std::set<CategoricalAttributeFilter> filters;
  for (auto vector : base_vectors) {
    CategoricalAttributeFilter filter(vector.getC());
    filters.insert(filter);
  }

  if (indexType == "filtered") {
    // Initialize and create the filtered Vamana index
    FilteredVamanaIndex<BaseDataVector<float>> index(filters);
    index.createGraph(base_vectors, std::stoi(alpha), std::stoi(L), std::stoi(R));

    // Save the graph to a file if the save flag is set
    if (save) {
      index.saveGraph(outputFile);
    }
  }
  else if (indexType == "stiched") {
    
    // Initialize and create the stiched Vamana index
    StichedVamanaIndex<BaseDataVector<float>> index(filters);
    index.createGraph(base_vectors, std::stof(alpha), std::stoi(L_small), std::stoi(R_small), std::stoi(R_stiched));

    // Save the graph to a file if the save flag is set
    if (save) {
      index.saveGraph(outputFile);
    }

  }

}


void Test(std::unordered_map<std::string, std::string> args) {

  // Create some aliases for the vector types
  using QueryVectorVector = std::vector<QueryDataVector<float>>;
  using GreedyResult = std::pair<std::set<BaseDataVector<float>>, std::set<BaseDataVector<float>>>;

  std::string indexFile, k, L, groundtruthFile, queryFile, queryNumber;

  // Check if the command line arguments of the execution mode are valid
  std::vector<std::string> validArguments = {"-load", "-k", "-L", "-gt-file", "-query-file", "-query"};
  for (auto arg : args) {
    if (std::find(validArguments.begin(), validArguments.end(), arg.first) == validArguments.end()) {
      throw std::invalid_argument("Invalid argument: " + arg.first);
    }
  }

  // Check if the required arguments are provided
  if (args.find("-load") == args.end()) {
    throw std::invalid_argument("Missing required argument: -load");
  } else {
    indexFile = args["-load"];
  }

  if (args.find("-k") == args.end()) {
    throw std::invalid_argument("Missing required argument: -k");
  } else {
    k = args["-k"];
  }

  if (args.find("-L") == args.end()) {
    throw std::invalid_argument("Missing required argument: -L");
  } else {
    L = args["-L"];
  }

  if (args.find("-gt-file") == args.end()) {
    throw std::invalid_argument("Missing required argument: -gt-file");
  } else {
    groundtruthFile = args["-gt-file"];
  }

  if (args.find("-query-file") == args.end()) {
    throw std::invalid_argument("Missing required argument: -query-file");
  } else {
    queryFile = args["-query-file"];
  }

  if (args.find("-query") == args.end()) {
    throw std::invalid_argument("Missing required argument: -query");
  } else {
    queryNumber = args["-query"];
  }

  // Read the query vectors from the binary file, and check if the query type is supported by the application
  QueryVectorVector query_vectors = ReadFilteredQueryVectorFile(queryFile);
  QueryDataVector<float> xq = query_vectors[std::stoi(queryNumber)];
  if (xq.getQueryType() > 1) {
    return;
  }

  FilteredVamanaIndex<BaseDataVector<float>> index;
  index.loadGraph(indexFile);
  std::vector<std::vector<int>> groundtruth = readGroundtruthFromFile(groundtruthFile);


  // Store the start nodes for each filter inside a vector
  std::vector<GraphNode<BaseDataVector<float>>> start_nodes;
  for (auto filter : index.getFilters()) {
    start_nodes.push_back(index.getNodesWithCategoricalValueFilter(filter)[0]); // TODO: Use filtered Medoid instead of the first node
  }

  std::vector<CategoricalAttributeFilter> Fx;
  if (xq.getQueryType() == 1) {
    Fx.push_back(CategoricalAttributeFilter(xq.getV()));
  }

  std::vector<GraphNode<BaseDataVector<float>>> P = index.getNodes();
  std::set<BaseDataVector<float>> exactNeighbors;

  for (auto index : groundtruth[std::stoi(queryNumber)]) {
    exactNeighbors.insert(P[index].getData());
    if ((int)exactNeighbors.size() >= std::stoi(k)) {
      break;
    }
  }

  GreedyResult greedyResult = FilteredGreedySearch(index.getGraph(), start_nodes, xq, std::stoi(k), std::stoi(L), Fx);
  std::set<BaseDataVector<float>> approximateNeighbors = greedyResult.first;
  
  double recall = calculateRecallEvaluation(approximateNeighbors, exactNeighbors);
  std::cout << "Recall: " << recall*100 << "%" << std::endl;

}


int main(int argc, char* argv[]) {

  // Check if an execution mode was provided in the command line
  if (argc < 2) {
    std::cerr << "Usage: " << argv[0] << " <execution-mode> [arguments]" << std::endl;
    return 1;
  }

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
    else if (executeMode == "--create") {
      Create(args);
    }
    else if (executeMode == "--test") {
      Test(args);  
    }
    else {
      std::cerr << "Invalid execution mode: " << executeMode << ". Available execution modes are:" << std::endl;
      std::cerr << "1)  --compute-gt" << std::endl;
      std::cerr << "2)  --create" << std::endl;
      std::cerr << "3)  --test" << std::endl;
      
      return 1;
    }

  }
  catch (std::invalid_argument& e) {
    std::cerr << "Error: " << e.what() << std::endl;
    return 1;
  }

  return 0;

}
