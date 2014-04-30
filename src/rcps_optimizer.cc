#include "rcps_optimizer.hh"

#include <iostream>

#include "rcps_model_to_glucose.hh"


using namespace RCPSSolver;

int RCPSOptimizer::optimize(RCPSSATModel& model, const RCPSInstance& instance)
{
    int lastMax = instance.getUpperBound();
    int max = lastMax;
    const int lb =instance.getLowerBound();
    bool solved = true;

    while (solved && max >= lb)
    {
        RCPSModel2Glucose transformer;
        Glucose::Solver solver;
        transformer.transformModel2Solver(model, solver);
        if (modelTimeConstraint(instance.getLowerBound(),instance.getUpperBound(),
            model, instance))
        {
            break;
        }
        solved = solver.solve();
        std::cout << "Max: " << max << " [" << (solved==true) << "]\n";
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

