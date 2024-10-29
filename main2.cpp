#include <iostream>
#include <vector>
#include <set>
#include "include/Graph/graph.h"
#include "include/Graph/graph_node.h"
#include <algorithm>
#include <iterator>


void GreedySearch(Graph<float>& graph, GraphNode<float>& s, float xq, unsigned int k, unsigned int L);


int main(int arc, char* argv[]) {

    Graph<float> graph(10);
    for (unsigned int i = 0; i < graph.getNodesCount(); i++) {
        graph.setNodeData(i, i+1);
    }

    for (unsigned int i = 0; i < graph.getNodesCount()-1; i++) {
        graph.connectNodesByData(i, i+1);
    }

    GraphNode<float>* start_node = graph.getNode(0);

    GreedySearch(graph, *start_node, 3, 10, 20);
    

    return 0;

}

void GreedySearch(
    Graph<float>& graph, GraphNode<float>& s, float xq, unsigned int k, unsigned int L) 
{

    std::set<GraphNode<float>> resultSet = { s };
    std::set<GraphNode<float>> visited;

    std::set<GraphNode<float>> result;
    std::set_difference(
        resultSet.begin(), resultSet.end(),
        visited.begin(), visited.end(),
        std::inserter(result, result.begin())
    );

    while (!result.empty()) {
        
        // Get p*
        GraphNode<float> minNode;
        float minItem;

        auto it = result.begin();
        std::advance(it, 0);
        minNode = *it;
        minItem = minNode.getData();
        float minDistance = std::abs(minItem - xq);

        for (unsigned int i = 1; i < result.size(); i++) {
            it = result.begin();
            std::advance(it, i);
            GraphNode<float> currentNode = *it;
            float currentItem = currentNode.getData();
            if (currentItem - xq < minDistance) {
                minNode = currentNode;
                minItem = minNode.getData();
                minDistance = currentItem - xq;
            }
        }
        
        std::vector<float>* neighbors = minNode.getNeighbors();
        std::vector<GraphNode<float>> neighborNodes;
        for (unsigned int i = 0; i < neighbors->size(); i++) {
            GraphNode<float>* currentNode = graph.getNodeWithData(neighbors->at(i));
            neighborNodes.push_back(*currentNode);
        }

        // Update L
        std::set<GraphNode<float>> union_result;
        std::set_union(
            resultSet.begin(), resultSet.end(),
            neighborNodes.begin(), neighborNodes.end(),
            std::inserter(union_result, union_result.begin())
        );
        resultSet = std::move(union_result);

        // Update V
        visited.insert(minNode);


        break;

    }

}
