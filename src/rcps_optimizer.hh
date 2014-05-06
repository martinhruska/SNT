#ifndef RCPS_MODEL_OPTIMIZER
#define RCPS_MODEL_OPTIMIZER

#include "rcps_sat_model.hh"
#include "rcps_instance.hh"

namespace RCPSSolver
{
    class RCPSOptimizer;
}

class RCPSSolver::RCPSOptimizer
{
public:
    static int optimize(RCPSSATModel& model, const RCPSInstance& instance,
    	int timeout);
};

#endif
