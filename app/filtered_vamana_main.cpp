#include <iostream>
#include <vector>
#include <algorithm>
#include "../include/BQDataVectors.h"
#include "../include/read_data.h"
#include "../include/distance.h"
#include "../include/FilteredVamanaIndex.h"
#include "../include/Filter.h"
#include "../include/recall.h"
#include "../include/groundtruth.h"


int main(int argc, char* argv[]) {

  using GreedyResult = std::pair<std::set<BaseDataVector<float>>, std::set<BaseDataVector<float>>>;

  std::vector<BaseDataVector<float>> base_vectors = ReadFilteredBaseVectorFile("data/Dummy/dummy-data.bin");
  std::vector<QueryDataVector<float>> query_vectors = ReadFilteredQueryVectorFile("data/Dummy/dummy-queries.bin");

  // Compute the distance vector, and save the computed distances to a file
  std::vector<std::vector<int>> base_indexes = computeGroundtruth(base_vectors, query_vectors, 1000);
  saveGroundtruthToFile(base_indexes, "data/Dummy/dummy-groundtruth.bin");

  // Example usage of readGroundtruthFromFile
  std::vector<std::vector<int>> groundtruth = readGroundtruthFromFile("data/Dummy/dummy-groundtruth.bin");

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
