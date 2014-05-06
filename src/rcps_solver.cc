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
    const int timeoutPos = 2;
    if (argc != requestParams && argc != requestParams+1)
    {
        std::cerr << "Not enough parameters \n";
        return EXIT_FAILURE;
    }

    int timeout = -1;

    if (argc == requestParams+1)
    { // timeout parameter has been entered
        char *pEnd;
        timeout = strtol(argv[timeoutPos],&pEnd,10);
        if (pEnd == argv[timeoutPos])
        {
            std::cerr << "Timeout has to be integer\n";
            return EXIT_FAILURE;
        }
    }   

    // parse input file
    const char *inputFile = argv[infilePos];
    RCPSInstance instance;
    int res = Parser::parse(instance, inputFile);
    std::cerr << "parsed\n";
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
    std::cerr << "parsed graph\n";
    rcpsFloydWarshall(precGraph);
    std::cerr << "warshall\n";
    storeGraphValuesToInstance(precGraph,instance);
    std::cerr << "graph stored\n";
    //Parser::toString(instance);
    RCPSSATModel model;
    createSATmodelFromRCPS(model, instance);
    std::cerr << "model created\n";
       
    RCPSOptimizer::optimize(model, instance, timeout);
    return EXIT_SUCCESS;
}
