#include "rcps_optimizer.hh"

#ifdef MINISAT_OPT
    #include "minisat/core/Solver.h"
    #include "rcps_model_to_minisat.hh"
#else
    #include "rcps_model_to_glucose.hh"
    #include "core/Solver.h"
#endif

#include <iostream>
#include <time.h>


using namespace RCPSSolver;
//using namespace Glucose;
//using namespace Minisat;

int RCPSOptimizer::optimize(RCPSSATModel& model, const RCPSInstance& instance, int timeout)
{
    int max = instance.getUpperBound();
    int lastMax = max+1;
    const int lb =instance.getLowerBound();
    bool solved = true;
    clock_t start = clock();

    while (solved || max >= lb)
    {
        clock_t startCycle = clock();
        #ifdef MINISAT_OPT
            RCPSModel2Minisat transformer;
            Minisat::Solver solver(&model, &instance, &transformer);
        #else
            RCPSModel2Glucose transformer;
            Glucose::Solver solver(&model, &instance, &transformer);
        #endif
           
        std::cerr << "transforming\n";
        if (modelTimeConstraint(max, lastMax,
            model, instance))
        {
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
        --max;

        if (timeout>=0 && (clock()-start)/CLOCKS_PER_SEC > timeout)
        {
            std::cout << "Timeout\n";
            break;
        }
    }

    std::cout << "Done " << (clock()-start)/CLOCKS_PER_SEC << std::endl;

    int res;
    if (lastMax == instance.getLowerBound())
    { // reached lower bound
        std::cout << "Successufully optimized" << std::endl;
        res = max;
    }
    else
    {
        res = lastMax;
    }

    return res;
}