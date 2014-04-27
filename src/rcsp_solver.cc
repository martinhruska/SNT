#include <stdlib.h>
#include <iostream>

#include "rcsp_parser.hh"

#define UNUSED(x) (void)(x)

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
    RCSPSolver::RCSPInstance instance;
    int res = RCSPSolver::Parser::parse(instance, inputFile);
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
    RCSPSolver::Parser::toString(instance);

    return EXIT_SUCCESS;
}
