#include <stdlib.h>
#include <iostream>

#include "rcps_parser.hh"
#include "rcps_graph.hh"
#include "rcps_sat_model.hh"
//#include "rcps_model_to_glucose.hh"
#include "rcps_optimizer.hh"

#define UNUSED(x) (void)(x)

using namespace RCPSSolver;

int main(int argc, char** argv)
{
    const int requestParams = 2;
    const int infilePos = 1;
    if (argc != requestParams)
    {
        std::cerr << "Not enough parameters \n";
        return EXIT_FAILURE;
    }

    // parse input file
    const char *inputFile = argv[infilePos];
    RCPSInstance instance;
    int res = Parser::parse(instance, inputFile);
    std::cout << "parsed\n";
    if (res)
    {
        if (res == 2)
        {
            std::cerr << "Not a single mode task \n";
        }
        else
        {
            std::cerr << "Cannot parse input file \n";
        }
        return EXIT_FAILURE;
    }
    PrecedenceGraph precGraph(instance.getJobsNumber());
    if (parseGraphFromRCPSInstance(precGraph, instance) != 0)
    {
        std::cerr << "Cannot parser to precedence graph\n";
    }
    std::cout << "parsed graph\n";
    rcpsFloydWarshall(precGraph);
    std::cout << "warshall\n";
    storeGraphValuesToInstance(precGraph,instance);
    std::cout << "graph stored\n";
    //Parser::toString(instance);
    RCPSSATModel model;
    createSATmodelFromRCPS(model, instance);
    std::cout << "model created\n";
       
    RCPSOptimizer::optimize(model, instance);
    //printModel(model);
    //Parser::toString(instance);
    /*
    Glucose::Solver solver;
    RCPSModel2Glucose transformer;
    transformer.transformModel2Solver(model, solver);
    if (!solver.solve())
    {
        std::cerr << "Unable to solve the model\n";
        return EXIT_SUCCESS;
    }
    auto revVar = transformer.getReverseVar();
    auto dbs = model.getStartVars();
    auto dbp = model.getProcessVars();
    for (int i=0; i<solver.nVars(); ++i)
    {
        int v = revVar[i];
        Variable vp;
        if (dbs.count(v))
        {
            vp = dbs[v];
            std::cout << "Activity " << vp.job << " Time " << vp.time << " is " << (solver.model[i] == l_True) << std::endl;
        }
        else if (dbp.count(v))
        {
            vp = dbp[v];
        }
    }
    */
    return EXIT_SUCCESS;
}
