#ifndef RCPS_MODEL_SOLVER
#define RCPS_MODEL_SOLVER

#include "rcps_sat_model.hh"
#include "core/Solver.h"
#include "mtl/Vec.h"

#include <unordered_map>

namespace RCPSSolver
{
    class RCPSModel2Glucose;
}

class RCPSSolver::RCPSModel2Glucose
{
private: // data types
    typedef std::unordered_map<Glucose::Var,int> ReverseVarMap;
private: //data members
    std::unordered_map<int,Glucose::Var> createdVars_;
    ReverseVarMap createdVarsReverse_;
public: // public constructors
    RCPSModel2Glucose() : createdVars_(), createdVarsReverse_()
    {}
public: // public functions
    int transformModel2Solver(RCPSSATModel& model, Glucose::Solver& solver);
    ReverseVarMap& getReverseVar() {return createdVarsReverse_;}

private: // private function
    int transformClauseDb2Solver(ClauseDb& clauseDb, Glucose::Solver& solver);
};

#endif
