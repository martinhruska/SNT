/**
 * RCPSSolver
 * author: Martin Hruska
 * e-mail: xhrusk16@stud.fit.vutbr.cz
 */
 
#ifndef RCPS_GRAPH_HH
#define RCPS_GRAPH_HH

#include <vector>
#include "rcps_instance.hh"

namespace RCPSSolver
{
    class PrecedenceGraph;
    int parseGraphFromRCPSInstance(PrecedenceGraph& graph,
            const RCPSInstance& instance);
    int rcpsFloydWarshall(PrecedenceGraph& graph);
    int storeGraphValuesToInstance(const PrecedenceGraph& graph,
            RCPSInstance& instance);
}

/**
 * Class for representation of a graph base on precedence relation
 * for a RCPS instance.
 */
class RCPSSolver::PrecedenceGraph
{
private: // private data type
    typedef std::vector<int> Row;
    typedef std::vector<std::vector<int>> Graph;
private: // private data types
    Graph graph_;
    int size_;
public: // public constructors
    PrecedenceGraph(int size);

    friend int parseGraphFromRCPSInstance(PrecedenceGraph& graph,
            const RCPSInstance& instance);
    friend int rcpsFloydWarshall(PrecedenceGraph& graph);
    friend int storeGraphValuesToInstance(const PrecedenceGraph& graph,
            RCPSInstance& instance);
};

#endif
