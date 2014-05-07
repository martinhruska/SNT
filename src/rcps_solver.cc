#include <stdlib.h>
#include <string.h>
#include <iostream>

#include "rcps_parser.hh"
#include "rcps_graph.hh"
#include "rcps_sat_model.hh"
#include "rcps_optimizer.hh"

#define UNUSED(x) (void)(x)

using namespace RCPSSolver;

/**
 * Get solver from given parameters
 * Returns: 0 for Glucose
            1 for MiniSAT
*/
int getSolver(char* solverOption)
{
    int res = 0;
    if (strcmp(solverOption,"g") == 0)
    {
        res = 0;
    }
    else if (strcmp(solverOption,"m") == 0)
    {
         res = 1;
    }
    else
    {
        res = -1;
    }
    return res;
}

int getTimeout(char *timeOption)
{
    char *pEnd;
    int timeout = strtol(timeOption,&pEnd,10);
    if (pEnd == timeOption)
    {
        return -1;
    }

    return timeout;
}

void printHelp()
{
    std::cout << "./rcps_solver input [solver] [timeout]\n";
    std::cout << "input .... input file from PSPLIB\n";
    std::cout << "solver ... 'm' for minisat (default), 'g' for Glucose\n";
    std::cout << "timeout ... timeout for solution (note, this timeout is only for rcps_solver, so the last SAT solver task will be finished)\n";
    std::cout << "If you enter both, solver and time option, their order is compulsory\n";
}

int main(int argc, char** argv)
{
    const int requestParams = 2;
    const int infilePos = 1;
    const int solverPos = 2;
    const int timeoutPos = 3;
    if (argc < requestParams && argc > requestParams+2)
    { // 2-4 parameters allowed
        std::cerr << "Wrong number of parameters \n";
        printHelp();
        return EXIT_FAILURE;
    }

    int timeout = -1;
    int solver = 1; // 0 glucose, 1 minisat

    if (strcmp(argv[infilePos], "-h") ||
        strcmp(argv[infilePos], "-help"))
    {
        printHelp();
        return EXIT_SUCCESS;
    }

    if (argc == requestParams+2)
    { // timeout and solver parameters have been entered
        if ((timeout=getTimeout(argv[timeoutPos])) < 0)
        {
            std::cerr << "Timeout has to be integer\n";
            return EXIT_FAILURE;
        }
        if ((solver=getSolver(argv[solverPos])) < 0)
        {
            std::cerr << "Options for solver:\n";
            std::cerr << "g - glucose:\n";
            std::cerr << "m - miniSAT:\n";
            return EXIT_FAILURE;
        }
    }
    else if (argc == requestParams + 1)
    { // timeout or 
        if ((timeout=getTimeout(argv[infilePos+1])) > 0)
        { // solver is default, timeout has been set
            solver = 0;
        }
        else if ((solver=getSolver(argv[solverPos])) >= 0)
        { // timeout is default, solver is added
            timeout = -1;
        }
        else
        { // wrong parameter
            std::cerr << "The second parameter is wrong\n";
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
          
    RCPSOptimizer::optimize(instance, solver, timeout);
    return EXIT_SUCCESS;
}
