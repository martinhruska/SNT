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
private:
    template <class Solver, class Transformer>
    static int optimize_(RCPSSATModel& model, const RCPSInstance& instance,
    	int timeout); 
public:
    static int optimize(RCPSSATModel& model, const RCPSInstance& instance,
    	int solver, int timeout);
};

#endif
