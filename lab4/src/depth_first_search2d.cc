// Author: Tucker Haydon
#include <cstdlib>
#include <iostream>
#include <stack>

#include <memory>

#include "graph.h"
#include "occupancy_grid2d.h"

#include "depth_first_search2d.h"

namespace mediation_layer {
  // Hiding extraneous information
  // Do not need to modify this
  using Node2DPtr = std::shared_ptr<Node2D>;

  // Anonymous namespace. Put any file-local functions or variables in here
  namespace {
    // Helper struct. Functions as a linked list with data. The linked list
    // represents a path. Data contained includes a node and a cost to reach
    // that node. 
    struct NodeVisitor {
      std::shared_ptr<struct NodeVisitor> parent;
      Node2DPtr node_ptr;
      double cost;

      // Equality operator
      bool operator==(const NodeVisitor& other) const {
        return *(this->node_ptr) == *(other.node_ptr);
      }
    };
  }

  PathInfo DepthFirstSearch2D::Run(
      const Graph2D& graph, 
      const Node2DPtr start_ptr, 
      const Node2DPtr end_ptr) {
    using NodeVisitorPtr = std::shared_ptr<NodeVisitor>;

    ///////////////////////////////////////////////////////////////////
    // SETUP
    // DO NOT MODIFY THIS
    ///////////////////////////////////////////////////////////////////
    Timer timer;
    timer.Start();

    // Use these data structures
    std::stack<NodeVisitorPtr> to_explore;
    std::vector<NodeVisitorPtr> explored;

    ///////////////////////////////////////////////////////////////////
    // YOUR WORK GOES HERE
    // SOME EXAMPLE CODE INCLUDED BELOW
    ///////////////////////////////////////////////////////////////////   
    
    // Create a NodeVisitorPtr
    NodeVisitorPtr nv_ptr = std::make_shared<NodeVisitor>();
    nv_ptr->parent = nullptr;
    nv_ptr->node_ptr = start_ptr;
    nv_ptr->cost = 0;
    to_explore.push(nv_ptr);

    bool check = false;
    int count = 0;
    
    while(true) {
      check = false;
      nv_ptr = to_explore.top();
      to_explore.pop();
      for(const NodeVisitorPtr exp: explored) {
        if (*exp == *nv_ptr) {
          check = true;
          break;
        }
      }

      if (check == true){
        continue;
      }      

      if (*(nv_ptr->node_ptr) == *end_ptr) {
        break;
      }
      else {  
        explored.push_back(nv_ptr);      
        const std::vector<DirectedEdge2D> edges = graph.Edges(nv_ptr->node_ptr);
  
        // Iterate through the list of edges
        for(const DirectedEdge2D& edge: edges) {
          NodeVisitorPtr neighbors = std::make_shared<NodeVisitor>();
          neighbors->parent = nv_ptr;
          neighbors->node_ptr = edge.Sink();
          neighbors->cost = nv_ptr->cost + edge.Cost();
          to_explore.push(neighbors);
        }

      }
      count++;    
    }

    std::vector<Node2DPtr> path_track;
    double final_cost = 0;
    
    while (true){
      if (nv_ptr == nullptr) {
        break;
      }
      path_track.push_back(nv_ptr->node_ptr);
      final_cost += nv_ptr->cost;
      nv_ptr = nv_ptr->parent; 
    }
    
    // Create a PathInfo
    PathInfo path_info;
    path_info.path = path_track;
    path_info.details.num_nodes_explored = count;
    path_info.details.path_cost = final_cost;
    path_info.details.path_length = path_track.size();
    path_info.details.run_time = timer.Stop();
    
    return path_info;
  }
}
