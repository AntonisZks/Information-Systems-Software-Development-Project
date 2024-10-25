//######################################################################################
// This file includes the functions applied to the graph:
// ~> Greedy_Algorithm
// ~> Robust_Prune
//######################################################################################

// CPP libraries
// this contains most of the STL headers files (data structures) amd iostream
// WARNING individual #inlcludes to ensure portability
// temporary placeholder for clean code


// Custom header files
#include "graphFunctions.h"
#include "distance.h"
#include "vanama.h"

#include <iostream>
#include <map>
#include <vector>
#include <queue>
#include <set>
#include <cmath>
#include <functional>

//######################################################################################

// DEFINITIONS

/**
 * @brief Greedy Search Algorithm
 * 
 * @param graph the graph to implement the greedy search
 * @param s the starting node
 * @param query the current query
 * @param k the result size
 * @param L the search list size
*/

struct NodeData {
    double x;  
    double y;  

     
    double distanceTo(const NodeData &other) const {
        return sqrt((x - other.x) * (x - other.x) + (y - other.y) * (y - other.y));
    }
};

/*struct Pair {
    int first;
    int second;
};
using pair = std::pair<int, int>*/

template <typename T>
void GreedySearch(Graph<T> &graph, NodeData query, int k) {
    using Pair = std::pair<double, T>;
    std::priority_queue<Pair, std::vector<Pair>, std::greater<Pair> > search_queue;
    std::set<T> visited;
    using Pair = std::pair<double, T>;

     
    for (const auto& node : graph.nodes) {
        double dist = node.second.distanceTo(query);
        search_queue.push(Pair(dist, node.first));
    }

    std::vector<Pair> result;

    
    while (!search_queue.empty() && result.size() < k) {
        Pair closest = search_queue.top();
        search_queue.pop();
        
         
        if (visited.count(closest.second) == 0) {
            visited.insert(closest.second);
            result.push_back(closest);
        }
    }

    std::sort(result.begin(), result.end(), [](const Pair &a, const Pair &b) {
        return a.first < b.first; 
    });


}

//NOT void, will take arguement a graph and return a pruned graph
void Robust_Prune(void){

}
