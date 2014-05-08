/**
 * RCPSSolver
 * author: Martin Hruska
 * e-mail: xhrusk16@stud.fit.vutbr.cz
 */

#ifndef RCPS_MODEL_OPTIMIZER
#define RCPS_MODEL_OPTIMIZER

#include "rcps_sat_model.hh"
#include "rcps_instance.hh"

namespace RCPSSolver
{
    class RCPSOptimizer;
}

/**
 * The class provides main wrapping method for optimizing given RCPS instance.
 */
class RCPSSolver::RCPSOptimizer
{
private:
    template <class Solver, class Transformer>
    static int optimize_(RCPSInstance& instance,
    	int timeout); 
public:
    static int optimize(RCPSInstance& instance,
    	int solver, int timeout);
};

#endif
