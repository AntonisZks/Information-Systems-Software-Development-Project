#ifndef VAMANA_INDEX_H
#define VAMANA_INDEX_H

#include <iostream>
#include <vector>
#include <set>
#include <fstream>
#include <sstream>
#include "graph.h"
#include "recall.h"
#include "GreedySearch.h"
#include "RobustPrune.h"

//for filtered medoid
#include <map>
#include <set>
#include "BQDataVectors.h"

using namespace std;


/**
 * @brief Class that represents the Vamana Index entity of the application. It provides methods for creating
 * an index according to the provided algorithm from the paper, saving a particular graph index into a file, and
 * loading and graph from a file for later usage.
*/
template <typename vamana_t> class VamanaIndex {

protected:
  
  Graph<vamana_t> G;
  std::vector<vamana_t> P;

  /**
   * @brief Fills the graph nodes with the given dataset points. 
  */
  void fillGraphNodes(void);

  /**
   * @brief Populates the graph of the vamana index. Speicifcally it fills every graph node with edges pointing
   * to random neighbors inside the graph, with these edges being at most maxEdges.
   * 
   * @param maxEdges the maximum number of edges a node can have.
  */
  void createRandomEdges(const unsigned int maxEdges);

public:

  /**
   * @brief Default Constructor for the VamanaIndex. Exists to avoid errors.
   */
  VamanaIndex(void) {}

  /**
   * @brief Returns the graph of the Vamana Index entity as a constant reference.
   * 
   * @return the graph index
  */
  const inline Graph<vamana_t>& getGraph(void) const { return this->G; }

  /**
   * @brief Returns the dataset points of the Vamana Index entity as a vector.
   * 
   * @return the points vector
  */
  inline std::vector<vamana_t> getPoints(void) const { return this->P; }

  /**
   * @brief Returns the nodes of the Vamana Index entity as a vector.
   * 
   * @return the nodes vector
   */
  inline std::vector<GraphNode<vamana_t>> getNodes(void) const { return this->G.getNodesVector(); }

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
  void createGraph(const std::vector<vamana_t>& P, const float& alpha, const unsigned int L, const unsigned int& R, bool visualize = true);

  /**
   * @brief Saves a specific graph into a file. Specifically this method is used to save the contents of a Vamana 
   * Index Graph, inside a file in order to be loaded later for further usage. The main point of this method is to 
   * reduce the time of the production.
   * 
   * @param filename the full path of the file in which the graph is going to be saved
   * 
   * @return true if the graph was saved successfully, false otherwise
  */
  bool saveGraph(const std::string& filename);

  /**
   * @brief Load a graph from a file. Specifically this method is used to receive the contents of a Vamana Index Graph
   * stored inside a file and create the Vamana Index object based on those contents. It is used to save time of the 
   * production making it easy to use an index with specific parameters just by loading it instead of creating it again.
   * 
   * @param filename the full path of the file containing the graph
   * 
   * @return true if the graph was loaded successfully, false otherwise
  */
  bool loadGraph(const std::string& filename);

  //####################################################################
  //this is the new filtered medoid, i will brief in later time
  //####################################################################
  map<int, BaseDataVector<float>> FilteredMedoid(const Graph<vamana_t>& Graph, int threshold);

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
  GraphNode<vamana_t> findMedoid(const Graph<vamana_t>& graph, bool visualize = true, int sample_size = 100);

};

/**
 * @brief Overloading the operator << for the Vamana Index.
 * 
 * @param out the output stream
 * @param index the Vamana Index to be printed
 * 
 * @return the output stream
*/
template <typename vamana_t> std::ostream& operator<<(std::ostream& out, const VamanaIndex<vamana_t>& index) {

  if (index.getGraph().getNodesCount() == 0) {
    out << "Vamana Index Empty";
    return out;
  }

  out << index.getGraph();
  return out;

}

#endif /* VAMANA_INDEX_H */
