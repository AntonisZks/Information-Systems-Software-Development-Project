#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <string>
#include <cstdlib>
#include <ctime>
#include "include/DataStructures/DataVector/DataVector.h"
#include "include/Algorithms/VamanaIndex.h"
#include "include/read_data.h"
#include "include/DataStructures/DataVector/BQDataVectors.h"

using ParametersMap = std::map<std::string, std::string>;
using BaseVectors = std::vector<DataVector<float>>;
using GroundTruthValues = std::vector<DataVector<int>>;

/**
 * @brief Retrieves the value of a specified parameter from a map.
 * 
 * @param parameters A map containing parameter key-value pairs.
 * @param key The key of the parameter to retrieve.
 * @param value A reference to a string where the value of the parameter will be stored if found.
 * @return true if the parameter is found and the value is retrieved, false otherwise.
 */
static bool getParameterValue(const ParametersMap& parameters, const std::string& key, std::string& value) {
  if (parameters.find(key) != parameters.end()) {
    value = parameters.at(key);
    return true;
  } else {
    std::cout << key << " is required" << std::endl;
    return false;
  }
}

/**
 * @brief Retrieves the exact nearest neighbors for a query vector.
 * 
 * @param base_vectors A vector of DataVector<float> objects representing the dataset.
 * @param groundtruth_values A vector of DataVector<int> objects containing the true nearest neighbor indices for each query.
 * @param query_number The index of the query vector.
 * 
 * @return A set containing the exact nearest neighbors for a query vector.
 */
static std::set<DataVector<float>> getExactNearestNeighbors(
  BaseVectors base_vectors, GroundTruthValues& groundtruth_values, const unsigned int query_number) 
{
  std::set<DataVector<float>> realNeighbors;

  DataVector<int> realNearestIndeces = groundtruth_values.at(query_number);
  for (unsigned int i = 0; i < realNearestIndeces.getDimension(); i++) {
    int currentIndex = realNearestIndeces.getDataAtIndex(i);
    DataVector<float> currentData = base_vectors.at(currentIndex);
    realNeighbors.insert(currentData);
  }

  return realNeighbors;
}

/**
 * @brief Creates a configuration based on command-line arguments. This function parses the command-line arguments to extract 
 * the necessary parameters for creating a Vamana index.
 * 
 * @param argc The number of command-line arguments.
 * @param argv The array of command-line arguments.
 * 
 * @return true if the configuration is successfully created, false otherwise.
 */
bool create(unsigned int argc, char* argv[]) {

  ParametersMap parameters;
  
  std::string base_file, L, R, alpha;

  // Insert all the arguments into the parameters map
  for (unsigned int i = 2; i < argc; i+=2) {
    if (i+1 >= argc) {
      std::cerr << "Missing value for " << argv[i] << std::endl;
      return false;
    }
    parameters[argv[i]] = argv[i+1];
  }

  // Get the values of all the arguments in the parameters map
  if (!getParameterValue(parameters, "-base-file", base_file)) return false;
  if (!getParameterValue(parameters, "-L", L)) return false;
  if (!getParameterValue(parameters, "-R", R)) return false;
  if (!getParameterValue(parameters, "-alpha", alpha)) return false;

  // Read the base vectors from the file
  std::vector<DataVector<float>> base_vectors = ReadVectorFile(base_file);
  if (base_vectors.empty()) {
    std::cerr << "Error reading base file" << std::endl;
    return false;
  }

  VamanaIndex<DataVector<float>> vamanaIndex = VamanaIndex<DataVector<float>>();
  vamanaIndex.createGraph(base_vectors, std::stof(alpha), std::stoi(L), std::stoi(R));

  // Save the graph if the save parameter is present
  if (parameters.find("-save") != parameters.end()) {
    if (!vamanaIndex.saveGraph(parameters["-save"])) {
      std::cerr << "Error opening file for writing." << std::endl;
      return false;
    }
    std::cout << "Vamana Index was saved successfully to `" << parameters["-save"] << "`" << std::endl;
  }

  return true;
}

/**
 * @brief Tests the Vamana index using command-line arguments. This function parses the command-line arguments to extract 
 * the necessary parameters for testing a Vamana index.
 * 
 * @param argc The number of command-line arguments.
 * @param argv The array of command-line arguments.
 * 
 * @return true if the test is successful, false otherwise.
 */
bool test(unsigned int argc, char* argv[]) {

  ParametersMap parameters;
  
  std::string load_file, k, L, groundtruth_file, query_file, query_number;

  // Insert all the arguments into the parameters map
  for (unsigned int i = 2; i < argc; i+=2) {
    if (i+1 >= argc) {
      std::cerr << "Missing value for " << argv[i] << std::endl;
      return false;
    }
    parameters[argv[i]] = argv[i+1];
  }

  // Get the values of all the arguments in the parameters map
  if (!getParameterValue(parameters, "-load", load_file)) return false;
  if (!getParameterValue(parameters, "-k", k)) return false;
  if (!getParameterValue(parameters, "-L", L)) return false;
  if (!getParameterValue(parameters, "-gt-file", groundtruth_file)) return false;
  if (!getParameterValue(parameters, "-query-file", query_file)) return false;
  if (!getParameterValue(parameters, "-query", query_number)) return false;

  // Get the groundtruth vectors from the file
  std::vector<DataVector<int>> groundtruth_values = ReadGroundTruth(groundtruth_file);
  if (groundtruth_values.empty()) {
    std::cerr << "Error reading groundtruth file" << std::endl;
    return false;
  }

  // Get the query vectors from the file
  std::vector<DataVector<float>> query_vectors = ReadVectorFile(query_file);
  if (query_vectors.empty()) {
    std::cerr << "Error reading query file" << std::endl;
    return false;
  }

  // Load the Vamana index from the file
  VamanaIndex<DataVector<float>> vamanaIndex = VamanaIndex<DataVector<float>>();
  if (!vamanaIndex.loadGraph(load_file)) {
    std::cerr << "Error loading Vamana index from file" << std::endl;
    return false;
  }

  // Perform the nearest neighbor search
  std::set<DataVector<float>> exactNeighbors = getExactNearestNeighbors(
    vamanaIndex.getPoints(), groundtruth_values, std::stoi(query_number)
  );
  vamanaIndex.test(std::stoi(k), std::stoi(L), query_vectors, std::stoi(query_number), exactNeighbors);

  return true;
}

/**
 * @brief Main function that reads datasets, creates a graph, and performs a greedy search.
 * 
 * @param argc The number of command-line arguments
 * @param argv Array of command-line arguments
 * 
 * @return Returns 0 upon successful execution; otherwise returns 1
 */
int main(int argc, char* argv[]) {

  srand(static_cast<unsigned int>(time(0)));

  // // Get the first argument to determine the operation
  // std::string firstArgument = argv[1];

  // if (firstArgument == "--create") {
  //   if (!create(argc, argv)) return 1;
  //   return 0;
  // }
  // else if (firstArgument == "--test") {
  //   if (!test(argc, argv)) return 1;
  //   return 0;
  // }
  // else {
  //   std::cerr << "Invalid operation" << std::endl;
  //   return 1;
  // }

  std::vector<BaseDataVector<float>> base_vectors = ReadFilteredBaseVectorFile("data/Dummy/dummy-data.bin");

  for (unsigned int i = 0; i < base_vectors.size(); i++) {
    if (i == 9999) {
      std::cout << "For node " << i << " the distances are: " << base_vectors[i] << std::endl;
      std::cout << "The C value is: " << base_vectors[i].getC() << std::endl;
      std::cout << "The T value is: " << base_vectors[i].getT() << std::endl;
      std::cout << "Size of the vector is: " << base_vectors[i].getDimension() << std::endl;
    }
  }

  return 0;
  
}
