#include "rcsp_graph.hh"
#include <climits>
#include <iostream>

/**
 * Function computing floyd warshall algorithm over precendence graph
 * for the longest path.
 * Input graph is matrix where two nodes without edge has in their
 * place in matrix zero value.
 */
int RCSPSolver::rcspFloydWarshall(PrecedenceGraph& graph)
{
    //graph.size_ = 14;
    for (int k = 0; k < graph.size_; ++k)
    {
        for (int i = 0; i < graph.size_; ++i)
        {
            for (int j = 0; j < graph.size_; ++j)
            {
                if (graph.graph_[i][k] != INT_MIN && graph.graph_[k][j] != INT_MIN && graph.graph_[i][j] < graph.graph_[i][k]+graph.graph_[k][j])
                {
                    graph.graph_[i][j] = graph.graph_[i][k]+graph.graph_[k][j];
                }
            }
        }
    }
    return 0;
}
