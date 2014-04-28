#include <stdlib.h>
#include <iostream>

#include "rcsp_parser.hh"
#include "rcsp_graph.hh"

#define UNUSED(x) (void)(x)

using namespace RCSPSolver;

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
    RCSPInstance instance;
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
    if (parseGraphFromRCSPInstance(precGraph, instance) != 0)
    {
        std::cerr << "Cannot parser to precedence graph\n";
    }
    rcspFloydWarshall(precGraph);
    storeGraphValuesToInstance(precGraph,instance);
    Parser::toString(instance);

    return EXIT_SUCCESS;
}
