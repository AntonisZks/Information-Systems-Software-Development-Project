#include "../include/graphFunctions.h"
#include "../include/DataVector/DataVector.h"
#include <vector>
#include <queue>
#include <limits>
#include <cmath>

using namespace std;

// GreedySearch algorithm to find the k-nearest neighbors
template <typename dvector_t>
std::vector<DataVector<dvector_t>> GreedySearch(
    const std::vector<DataVector<dvector_t>>& dataset,
    const DataVector<dvector_t>& query,
    int k
) 
{
    // Priority queue to store the nearest neighbors
    std::priority_queue<std::pair<double, DataVector<dvector_t>>> nearest_neighbors;

    // Iterate through the dataset to find the k-nearest neighbors
    for (const auto& data_vector : dataset) {
        double distance = query.euclideanDistance(data_vector); 

        if (nearest_neighbors.size() < k) {
            nearest_neighbors.push({distance, data_vector});
        } else if (distance < nearest_neighbors.top().first) {
            nearest_neighbors.pop();
            nearest_neighbors.push({distance, data_vector});
        }
    }

    // Extract the k-nearest neighbors from the priority queue
    std::vector<DataVector<dvector_t>> result;
    while (!nearest_neighbors.empty()) {
        result.push_back(nearest_neighbors.top().second);
        nearest_neighbors.pop();
    }

    // Reverse the result to have the nearest neighbors in ascending order of distance
    std::reverse(result.begin(), result.end());

    return result;
}

// Declaration only
template <typename graph_t>
std::vector<graph_t> robustPrune(unsigned int p_index, std::vector<unsigned int> candidate_indices, float alpha, unsigned int R);



int main() {
    // Example usage of GreedySearch function
    std::vector<DataVector<int>> dataset; // Assuming DataVector<int> is a valid type
    DataVector<int> query; // Assuming DataVector<int> is a valid type
    int k = 3;

    // Call the GreedySearch function
    std::vector<DataVector<int>> result = GreedySearch(dataset, query, k);

    // Print the results

    std::cout << "K-Nearest Neighbors:" << std::endl;
    for (const auto& neighbor : result) {
        // Assuming DataVector has a method to print its contents
        std::cout << neighbor << std::endl; // Assuming operator<< is overloaded for DataVector
    }

    return 0;
}