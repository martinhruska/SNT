#ifndef RCSP_GRAPH_HH
#define RCSP_GRAPH_HH

#include <vector>
#include "rcsp_instance.hh"

namespace RCSPSolver
{
    class PrecedenceGraph;
    int parseGraphFromRCSPInstance(PrecedenceGraph& graph,
            const RCSPInstance& instance);
    int rcspFloydWarshall(PrecedenceGraph& graph);
    int storeGraphValuesToInstance(const PrecedenceGraph& graph,
            RCSPInstance& instance);
}

/**
 * Class for representation of a graph base on precedence relation
 * for a RCSP instance.
 */
class RCSPSolver::PrecedenceGraph
{
private: // private data type
    typedef std::vector<int> Row;
    typedef std::vector<std::vector<int>> Graph;
private: // private data types
    Graph graph_;
    int size_;
public: // public constructors
    PrecedenceGraph(int size);

    friend int parseGraphFromRCSPInstance(PrecedenceGraph& graph,
            const RCSPInstance& instance);
    friend int rcspFloydWarshall(PrecedenceGraph& graph);
    friend int storeGraphValuesToInstance(const PrecedenceGraph& graph,
            RCSPInstance& instance);
};

#endif
