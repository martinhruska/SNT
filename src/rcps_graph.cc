/**
 * RCPSSolver
 * author: Martin Hruska
 * e-mail: xhrusk16@stud.fit.vutbr.cz
 */

#include "rcps_graph.hh"
#include <iostream>
#include <climits>

using namespace RCPSSolver;

/**
 * Constructor has to alocate result graph
 */
PrecedenceGraph::PrecedenceGraph(int size) : graph_(size), size_(size)
{
    for (int i=0; i < size; i++)
    {
        graph_[i] = Row(size);
    }
}

/**
 * Transform instance of RCPS to precendece graph
 * @param[in] instance RCPS instance
 * @param[out] graph Graph to be filled
 */
int RCPSSolver::parseGraphFromRCPSInstance(PrecedenceGraph& graph,
        const RCPSInstance& instance)
{
    int jobs = instance.getJobsNumber();
    if (jobs != graph.size_)
    {
        return 1;
    }

    for (int i=0; i < jobs; ++i)
    {
       for (int j=0; j < jobs; ++j)
       { // if there is precendence between functions, duration is set to graph
           int val = 0;
           if (i == j)
           { // diagonal should be zero
               val = 0;
           }
           else if (instance.getPrecedence(i,j))
           { // there is an edge
               val = instance.getDuration(i);
           }
           else
           {
               val = INT_MIN;
           }
           graph.graph_[i][j] = val;
       }
    }

    return 0;
}

/**
 * Store values from graph to RCPS instance.
 * It is supposed that values in graph was computed by
 * Floyd-warshall algorithm
 */
int RCPSSolver::storeGraphValuesToInstance(const PrecedenceGraph& graph,
        RCPSInstance& instance)
{
    int lastJob = graph.size_-1;
    // lower bound is time between first and last activity
    int lb = graph.graph_[0][lastJob];
    // time between the first and the last activity
    instance.setLowerBound(lb);

    int ub = instance.getUpperBound();
    for (int job=0; job <= lastJob-1; ++job)
    {
        int duration = instance.getDuration(job);
        int eStart = graph.graph_[0][job]; // time between start and given job
        instance.setEStart(job, eStart);
        instance.setEFinish(job, eStart+duration);
        int lStart = ub-graph.graph_[job][lastJob]; // time between given job and it finish
        instance.setLStart(job, lStart);
        instance.setLFinish(job, lStart+duration);
    }

    int lastDuration = instance.getDuration(lastJob);
    instance.setEStart(lastJob, lb);
    instance.setEFinish(lastJob, lb+lastDuration);
    instance.setLStart(lastJob, ub);
    instance.setLFinish(lastJob, ub+lastDuration);

    return 0;
}
