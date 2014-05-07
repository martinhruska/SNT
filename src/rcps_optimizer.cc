#include "rcps_optimizer.hh"

//#ifdef MINISAT_OPT
    #include "minisat/core/Solver.h"
    #include "rcps_model_to_minisat.hh"
//#else
    #include "rcps_model_to_glucose.hh"
    #include "core/Solver.h"
//#endif
#include "rcps_graph.hh"
#include "rcps_sat_model.hh"

#include <iostream>
#include <time.h>


using namespace RCPSSolver;

/**
 * Interface for optimization function.
 * @param solver 0 .. use glucose, 1 .. use minisat
 */
int RCPSOptimizer::optimize(RCPSInstance& instance,
  int solver, int timeout)
{
    if (solver == 1)
    {
        return optimize_<Minisat::Solver, RCPSModel2Minisat>(
            instance, timeout);
    }
    else if (solver == 0)
    {
        return optimize_<Glucose::Solver, RCPSModel2Glucose>(
            instance, timeout);
    }
    else
    {
        return -1;
    }
}

template<class Solver, class Transformer>
int RCPSOptimizer::optimize_(RCPSInstance& instance,
  int timeout)
{
    int max = instance.getDueDate(); //instance.getLowerBound()-1;
    bool solved = false;
    clock_t start = clock();

    while (!solved && max <= instance.getHorizont())
    {
        clock_t startCycle = clock();
         
        instance.setUpperBound(max+1); 
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

        Transformer transformer;
        Solver solver(&model, &instance, &transformer);
        std::cerr << "transforming\n";
        int res;
        if ((res = modelTimeConstraint(max, instance.getUpperBound()+1,
            model, instance)))
        {
            std::cerr << "Cannot model time constraints " << res << '\n';
            break;
        }
        transformer.transformModel2Solver(model, solver);
        //printModel(model);
        std::cerr << "transformed\n";
        std::cerr << "START SOLVING\n";
        clock_t startSolve = clock();
        solved = solver.solve();
        double diffSolve =  (clock()-startSolve)/CLOCKS_PER_SEC;
        double diffCycle =  (clock()-startCycle)/CLOCKS_PER_SEC;
        //solver.printModel();
        std::cout << "\nMakespan: " << max-1 << " [" << (solved==true) << "]" 
        << " " << diffSolve << " " << diffCycle << std::endl;
        ++max;

        if (timeout>=0 && (clock()-start)/CLOCKS_PER_SEC > timeout)
        {
            std::cout << "Timeout\n";
            break;
        }
    }

    std::cout << "Done " << (clock()-start)/CLOCKS_PER_SEC << std::endl;

    int res;
    if (solved || max > instance.getUpperBound())
    { // reached lower bound
        std::cout << "Optimal: " << max << std::endl;
        res = max;
    }
    else
    {
        res = max; //lastMax;
    }

    return res;
}