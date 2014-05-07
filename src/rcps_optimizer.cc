#include "rcps_optimizer.hh"

//#ifdef MINISAT_OPT
    #include "minisat/core/Solver.h"
    #include "rcps_model_to_minisat.hh"
//#else
    #include "rcps_model_to_glucose.hh"
    #include "core/Solver.h"
//#endif

#include <iostream>
#include <time.h>


using namespace RCPSSolver;

/**
 * Interface for optimization function.
 * @param solver 0 .. use glucose, 1 .. use minisat
 */
int RCPSOptimizer::optimize(RCPSSATModel& model, const RCPSInstance& instance,
  int solver, int timeout)
{
    if (solver == 1)
    {
        return optimize_<Minisat::Solver, RCPSModel2Minisat>(
            model, instance, timeout);
    }
    else if (solver == 0)
    {
        return optimize_<Glucose::Solver, RCPSModel2Glucose>(
            model, instance, timeout);
    }
    else
    {
        return -1;
    }
}

template<class Solver, class Transformer>
int RCPSOptimizer::optimize_(RCPSSATModel& model, const RCPSInstance& instance,
  int timeout)
{
    int max = instance.getDueDate(); //instance.getLowerBound()-1;
    int lastMax = max+1;
    const int ub =instance.getUpperBound();
    bool solved = false;
    clock_t start = clock();

    while (!solved && max <= ub)
    {
        clock_t startCycle = clock();
        Transformer transformer;
        Solver solver(&model, &instance, &transformer);
           
        std::cerr << "transforming\n";
        int res;
        if ((res = modelTimeConstraint(max, ub+1,
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
        std::cout << "\nMax: " << max-1 << " [" << (solved==true) << "]" 
        << " " << diffSolve << " " << diffCycle << std::endl;
        lastMax = max;
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
        std::cout << "Successufully optimized" << std::endl;
        res = max;
    }
    else
    {
        res = max; //lastMax;
    }

    return res;
}