#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <unordered_map>
#include <string>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <algorithm>
#include "../include/DataVector.h"
#include "../include/VamanaIndex.h"
#include "../include/FilteredVamanaIndex.h"
#include "../include/StichedVamanaIndex.h"
#include "../include/read_data.h"
#include "../include/BQDataVectors.h"
#include "../include/recall.h"
#include "../include/graphics.h"
#include "../include/groundtruth.h"
#include "../include/distance.h"
#include "../include/Filter.h"

using ParametersMap = std::unordered_map<std::string, std::string>;
using BaseVectors = std::vector<DataVector<float>>;
using GroundTruthValues = std::vector<DataVector<int>>;
using FilteredGreedyResult = std::pair<std::set<BaseDataVector<float>>, std::set<BaseDataVector<float>>>;
using SimpleGreedyResult = std::pair<std::set<DataVector<float>>, std::set<DataVector<float>>>;

static bool getParameterValue(const ParametersMap& parameters, const std::string& key, std::string& value) {
  if (parameters.find(key) != parameters.end()) {
    value = parameters.at(key);
    return true;
  } else {
    std::cerr << "Error: Missing required parameter: " << key << std::endl;
    return false;
  }
}

static std::set<DataVector<float>> getExactNearestNeighbors(
  BaseVectors base_vectors, const GroundTruthValues& groundtruth_values, const unsigned int query_number) 
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

std::unordered_map<std::string, std::string> parseArguments(int argc, char* argv[]) {
  std::unordered_map<std::string, std::string> args;
  for (unsigned int i = 2; i < (unsigned int)argc; i += 2) {
    std::string key = argv[i];
    if (key[0] != '-') {
      throw std::invalid_argument("Error: Invalid argument format: " + key + ". Arguments should start with '-'");
    }
    if (i + 1 >= (unsigned int)argc) {
      throw std::invalid_argument("Error: Missing value for argument: " + key);
    }
    std::string value = argv[i + 1];
    args[key] = value;
  }
  return args;
}

void ComputeGroundtruth(std::unordered_map<std::string, std::string> args) {
  using BaseVectorVector = std::vector<BaseDataVector<float>>;
  using QueryVectorVector = std::vector<QueryDataVector<float>>;

  std::string baseFile, queryFile, groundtruthFile;
  unsigned int maxDistances = 1000;

  std::vector<std::string> validArguments = {"-base-file", "-query-file", "-gt-file", "-max-distances"};
  for (auto arg : args) {
    if (std::find(validArguments.begin(), validArguments.end(), arg.first) == validArguments.end()) {
      throw std::invalid_argument("Error: Invalid argument: " + arg.first + ". Valid arguments are: -base-file, -query-file, -gt-file, -max-distances");
    }
  }

  if (args.find("-base-file") == args.end()) {
    throw std::invalid_argument("Error: Missing required argument: -base-file");
  } else {
    baseFile = args["-base-file"];
  }

  if (args.find("-query-file") == args.end()) {
    throw std::invalid_argument("Error: Missing required argument: -query-file");
  } else {
    queryFile = args["-query-file"];
  }

  if (args.find("-gt-file") == args.end()) {
    throw std::invalid_argument("Error: Missing required argument: -gt-file");
  } else {
    groundtruthFile = args["-gt-file"];
  }

  if (args.find("-max-distances") != args.end()) {
    maxDistances = std::stoi(args["-max-distances"]);
  }

  BaseVectorVector base_vectors = ReadFilteredBaseVectorFile(baseFile);
  QueryVectorVector query_vectors = ReadFilteredQueryVectorFile(queryFile);

  std::vector<std::vector<int>> base_indexes = computeGroundtruth(base_vectors, query_vectors, maxDistances);
  saveGroundtruthToFile(base_indexes, groundtruthFile);
}

void Create(std::unordered_map<std::string, std::string> args) {
  using BaseVectorVector = std::vector<BaseDataVector<float>>;
  using BaseVectors = std::vector<DataVector<float>>;

  std::string indexType, baseFile, L, R, alpha, outputFile, connectionMode;
  std::string L_small, R_small, R_stiched;
  bool save = false;
  bool leaveEmpty = false;

  std::vector<std::string> validArguments = {"-index-type", "-base-file", "-L", "-L-small", "-R", "-R-small", "-R-stiched", "-alpha", "-save", "-random-edges", "-connection-mode"};
  for (auto arg : args) {
    if (std::find(validArguments.begin(), validArguments.end(), arg.first) == validArguments.end()) {
      throw std::invalid_argument("Error: Invalid argument: " + arg.first + ". Valid arguments are: -index-type, -base-file, -L, -L-small, -R, -R-small, -R-stiched, -alpha, -save, -connection-mode");
    }
  }

  if (args.find("-index-type") == args.end()) {
    throw std::invalid_argument("Error: Missing required argument: -index-type");
  } else {
    indexType = args["-index-type"];
  }

  if (indexType == "filtered" || indexType == "simple") {
    if (args.find("-L") == args.end()) {
      throw std::invalid_argument("Error: Missing required argument: -L");
    } else {
      L = args["-L"];
    }

    if (args.find("-R") == args.end()) {
      throw std::invalid_argument("Error: Missing required argument: -R");
    } else {
      R = args["-R"];
    }

  } else if (indexType == "stiched") {
    if (args.find("-L-small") == args.end()) {
      throw std::invalid_argument("Error: Missing required argument: -L-small");
    } else {
      L_small = args["-L-small"];
    }

    if (args.find("-R-small") == args.end()) {
      throw std::invalid_argument("Error: Missing required argument: -R-small");
    } else {
      R_small = args["-R-small"];
    }

    if (args.find("-R-stiched") == args.end()) {
      throw std::invalid_argument("Error: Missing required argument: -R-stiched");
    } else {
      R_stiched = args["-R-stiched"];
    }
  } else {
    throw std::invalid_argument("Error: Invalid index type: " + indexType + ". Supported index types are: simple, filtered, stiched");
  }

  if (args.find("-base-file") == args.end()) {
    throw std::invalid_argument("Error: Missing required argument: -base-file");
  } else {
    baseFile = args["-base-file"];
  }

  if (args.find("-alpha") == args.end()) {
    throw std::invalid_argument("Error: Missing required argument: -alpha");
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

  if (args.find("-connection-mode") != args.end()) {
    connectionMode = args["-connection-mode"];
    if (connectionMode == "empty") {
      leaveEmpty = true;
    } else if (connectionMode != "filled") {
      throw std::invalid_argument("Error: Invalid value for -connection-mode. Valid values are: empty, filled");
    }
  }

  if (indexType == "simple") {
    BaseVectors base_vectors = ReadVectorFile(baseFile);
    if (base_vectors.empty()) {
      std::cerr << "Error reading base file" << std::endl;
      return;
    }

    VamanaIndex<DataVector<float>> vamanaIndex = VamanaIndex<DataVector<float>>();
    vamanaIndex.createGraph(base_vectors, std::stof(alpha), std::stoi(L), std::stoi(R));

    if (save) {
      if (!vamanaIndex.saveGraph(outputFile)) {
        std::cerr << "Error opening file for writing." << std::endl;
        return;
      }
      std::cout << std::endl << green << "Vamana Index was saved successfully to " << brightYellow << "`" << outputFile << "`" << reset << std::endl;
    }
  } else {
    BaseVectorVector base_vectors = ReadFilteredBaseVectorFile(baseFile);

    std::set<CategoricalAttributeFilter> filters;
    for (auto vector : base_vectors) {
      CategoricalAttributeFilter filter(vector.getC());
      filters.insert(filter);
    }

    if (indexType == "filtered") {
      FilteredVamanaIndex<BaseDataVector<float>> index(filters);
      index.createGraph(base_vectors, std::stoi(alpha), std::stoi(L), std::stoi(R), true, leaveEmpty);

      if (save) {
        index.saveGraph(outputFile);
        std::cout << std::endl << green << "Vamana Index was saved successfully to " << brightYellow << "`" << outputFile << "`" << reset << std::endl;
      }
    } else if (indexType == "stiched") {
      StichedVamanaIndex<BaseDataVector<float>> index(filters);
      index.createGraph(base_vectors, std::stof(alpha), std::stoi(L_small), std::stoi(R_small), std::stoi(R_stiched), true, leaveEmpty);

      if (save) {
        index.saveGraph(outputFile);
        std::cout << std::endl << green << "Vamana Index was saved successfully to " << brightYellow << "`" << outputFile << "`" << reset << std::endl;
      }
    }
  }
}

void TestSimple(std::unordered_map<std::string, std::string> args) {
  using BaseVectors = std::vector<DataVector<float>>;

  std::string indexFile, k, L, groundtruthFile, queryFile, queryNumber;

  if (!getParameterValue(args, "-load", indexFile)) return;
  if (!getParameterValue(args, "-k", k)) return;
  if (!getParameterValue(args, "-L", L)) return;
  if (!getParameterValue(args, "-gt-file", groundtruthFile)) return;
  if (!getParameterValue(args, "-query-file", queryFile)) return;
  if (!getParameterValue(args, "-query", queryNumber)) return;

  BaseVectors query_vectors = ReadVectorFile(queryFile);
  if (query_vectors.empty()) {
    std::cerr << "Error reading query file" << std::endl;
    return;
  }

  VamanaIndex<DataVector<float>> vamanaIndex = VamanaIndex<DataVector<float>>();
  if (!vamanaIndex.loadGraph(indexFile)) {
    std::cerr << "Error loading Vamana index from file" << std::endl;
    return;
  }

  std::set<DataVector<float>> exactNeighbors = getExactNearestNeighbors(
    vamanaIndex.getPoints(), ReadGroundTruth(groundtruthFile), std::stoi(queryNumber)
  );

  GraphNode<DataVector<float>> s = vamanaIndex.findMedoid(vamanaIndex.getGraph(), 1000);
  
  auto start = std::chrono::high_resolution_clock::now();
  SimpleGreedyResult greedyResult = GreedySearch(vamanaIndex, s, query_vectors.at(std::stoi(queryNumber)), std::stoi(k), std::stoi(L), TEST);
  auto end = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> elapsed = end - start;

  float recall = calculateRecallEvaluation(greedyResult.first, exactNeighbors);

  std::cout << std::endl << brightMagenta << "Results:" << reset << std::endl;
  std::cout << reset << "Current Query: " << brightCyan << queryNumber << reset << " | ";
  std::cout << reset << "Query Type: " << brightBlack << "Unfiltered" << reset << " | ";
  std::cout << reset << "Recall: ";
  if (recall < 0.2) std::cout << brightRed;
  else if (recall < 0.4) std::cout << brightOrange;
  else if (recall < 0.6) std::cout << brightYellow;
  else if (recall < 0.8) std::cout << brightCyan;
  else std::cout << brightGreen;
  std::cout << recall*100 << "%" << reset << " | ";
  std::cout << "Time: " << cyan << elapsed.count() << " seconds" << std::endl;
}

void TestFilteredOrStiched(std::unordered_map<std::string, std::string> args) {
  using QueryVectorVector = std::vector<QueryDataVector<float>>;

  std::string indexFile, k, L, groundtruthFile, queryFile, queryNumber, testOn;

  if (!getParameterValue(args, "-load", indexFile)) return;
  if (!getParameterValue(args, "-k", k)) return;
  if (!getParameterValue(args, "-L", L)) return;
  if (!getParameterValue(args, "-gt-file", groundtruthFile)) return;
  if (!getParameterValue(args, "-query-file", queryFile)) return;
  if (!getParameterValue(args, "-query", queryNumber)) return;
  if (args.find("-test-on") != args.end()) {
    if (queryNumber != "-1") {
      std::cerr << "Error: The -test-on argument can only be used when -query is set to -1." << std::endl;
      return;
    }
    testOn = args["-test-on"];
  }

  QueryVectorVector query_vectors = ReadFilteredQueryVectorFile(queryFile);
  FilteredVamanaIndex<BaseDataVector<float>> index;
  index.loadGraph(indexFile);
  std::vector<std::vector<int>> groundtruth = readGroundtruthFromFile(groundtruthFile);
  std::map<Filter, GraphNode<BaseDataVector<float>>> medoids = index.findFilteredMedoid(std::stoi(L)); 
  std::vector<GraphNode<BaseDataVector<float>>> start_nodes;
  for (auto filter : index.getFilters()) {
    start_nodes.push_back(medoids[filter]);
  }

  auto processQuery = [&](int queryIdx) {
    QueryDataVector<float> xq = query_vectors[queryIdx];
    if (xq.getQueryType() > 1) {
      return;
    }

    std::vector<CategoricalAttributeFilter> Fx;
    if (xq.getQueryType() == 1) {
      Fx.push_back(CategoricalAttributeFilter(xq.getV()));
    }

    std::vector<GraphNode<BaseDataVector<float>>> P = index.getNodes();
    std::set<BaseDataVector<float>> exactNeighbors;

    for (auto idx : groundtruth[queryIdx]) {
      exactNeighbors.insert(P[idx].getData());
      if ((int)exactNeighbors.size() >= std::stoi(k)) {
        break;
      }
    }

    auto start = std::chrono::high_resolution_clock::now();
    FilteredGreedyResult greedyResult = FilteredGreedySearch(index, start_nodes, xq, std::stoi(k), std::stoi(L), Fx, TEST);
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;

    std::set<BaseDataVector<float>> approximateNeighbors = greedyResult.first;
    double recall = calculateRecallEvaluation(approximateNeighbors, exactNeighbors);

    // std::cout << brightMagenta << std::endl << "Results for query " << queryIdx << ":" << reset << std::endl;
    std::cout << reset << "Current Query: " << brightCyan << queryIdx << reset << " | ";
    std::cout << reset << "Query Type: ";
    if (xq.getQueryType() == 0) std::cout << brightBlack << "Unfiltered" << reset << " | ";
    else std::cout << brightWhite << "Filtered  " << reset << " | ";
    std::cout << reset << "Recall: ";
    if (recall < 0.2) std::cout << brightRed;
    else if (recall < 0.4) std::cout << brightOrange;
    else if (recall < 0.6) std::cout << brightYellow;
    else if (recall < 0.8) std::cout << brightCyan;
    else std::cout << brightGreen;
    std::cout << recall*100 << "%" << reset << " | ";
    std::cout << "Time: " << cyan << elapsed.count() << " seconds" << std::endl;
  };

  if (queryNumber == "-1") {
    for (size_t i = 0; i < query_vectors.size(); ++i) {
      if (testOn == "filtered" && query_vectors[i].getQueryType() != 1) continue;
      if (testOn == "unfiltered" && query_vectors[i].getQueryType() != 0) continue;
      processQuery(i);
    }
  } else {
    processQuery(std::stoi(queryNumber));
  }
}

void Test(std::unordered_map<std::string, std::string> args) {
  std::string indexType;
  if (!getParameterValue(args, "-index-type", indexType)) return;

  if (indexType == "simple") {
    TestSimple(args);
  } else if (indexType == "filtered" || indexType == "stiched") {
    TestFilteredOrStiched(args);
  } else {
    std::cerr << "Error: Invalid index type: " << indexType << ". Supported index types are: simple, filtered, stiched" << std::endl;
  }
}

int main(int argc, char* argv[]) {
  srand(static_cast<unsigned int>(time(0)));

  if (argc < 2) {
    std::cerr << "Error: No execution mode provided. Usage: " << argv[0] << " <execution-mode> [arguments]" << std::endl;
    std::cerr << "Available execution modes are:" << std::endl;
    std::cerr << "1)  --compute-gt" << std::endl;
    std::cerr << "2)  --create" << std::endl;
    std::cerr << "3)  --test" << std::endl;
    return 1;
  }

  std::string executeMode = argv[1];
  std::unordered_map<std::string, std::string> args;

  try {
    args = parseArguments(argc, argv);

    if (executeMode == "--compute-gt") {
      ComputeGroundtruth(args);
    } else if (executeMode == "--create") {
      Create(args);
    } else if (executeMode == "--test") {
      Test(args);  
    } else {
      std::cerr << "Error: Invalid execution mode: " << executeMode << ". Available execution modes are:" << std::endl;
      std::cerr << "1)  --compute-gt" << std::endl;
      std::cerr << "2)  --create" << std::endl;
      std::cerr << "3)  --test" << std::endl;
      return 1;
    }
  } catch (std::invalid_argument& e) {
    std::cerr << e.what() << std::endl;
    return 1;
  }

  return 0;
}
