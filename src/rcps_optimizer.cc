#include "rcps_optimizer.hh"

//#include "rcps_model_to_minisat.hh"

#include <iostream>

#include "rcps_model_to_glucose.hh"

using namespace RCPSSolver;
using namespace Glucose;
//using namespace Minisat;

int RCPSOptimizer::optimize(RCPSSATModel& model, const RCPSInstance& instance)
{
    int max = instance.getUpperBound();
    int lastMax = max+1;
    const int lb =instance.getLowerBound();
    bool solved = true;

    while (solved && max >= lb)
    {
        RCPSModel2Glucose transformer;
        //RCPSModel2Minisat transformer;
        Solver solver(&model, &instance, &transformer);
        std::cout << "transforming\n";
        if (modelTimeConstraint(max, lastMax,
            model, instance))
        {
            break;
        }
        transformer.transformModel2Solver(model, solver);
        //printModel(model);
        std::cout << "transformed\n";
        std::cout << "START SOLVING\n";
        solved = solver.solve();
        //solver.printModel();
        std::cout << "Max: " << max-1 << " [" << (solved==true) << "]" << std::endl;
        lastMax = max;
        --max;
        /*
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
    }

    std::cout << "Optimization done" << std::endl;

    int res;
    if (solved)
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

