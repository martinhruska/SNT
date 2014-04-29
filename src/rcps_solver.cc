#include <stdlib.h>
#include <iostream>

#include "rcps_parser.hh"
#include "rcps_graph.hh"
#include "rcps_sat_model.hh"

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
    rcpsFloydWarshall(precGraph);
    storeGraphValuesToInstance(precGraph,instance);
    RCPSSATModel model;
    createSATmodelFromRCPS(model, instance);
    printModel(model);
    //Parser::toString(instance);
    return EXIT_SUCCESS;
}
