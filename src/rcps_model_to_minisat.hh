/**
 * RCPSSolver
 * author: Martin Hruska
 * e-mail: xhrusk16@stud.fit.vutbr.cz
 */

#ifndef RCPS_MODEL_MINISAT
#define RCPS_MODEL_MINISAT

#include "rcps_sat_model.hh"
#include "minisat/core/Solver.h"

#include <unordered_map>

namespace RCPSSolver
{
    class RCPSModel2Minisat;
}

/**
 * Class for transformation of internal model to minisat.
 */
class RCPSSolver::RCPSModel2Minisat
{
private: // data types
    typedef std::unordered_map<Minisat::Var,int> ReverseVarMap;
    typedef std::unordered_map<int,Minisat::Var> VarMap;
private: //data members
    VarMap createdVars_;
    ReverseVarMap createdVarsReverse_;
public: // public constructors
    RCPSModel2Minisat() : createdVars_(), createdVarsReverse_()
    {}
public: // public functions
    int transformModel2Solver(RCPSSATModel& model, Minisat::Solver& solver);
    const ReverseVarMap& getReverseVar() {return createdVarsReverse_;}
    const VarMap& getVarMap() {return createdVars_;}

private: // private function
    int addPriority(const VariableDb& vars, Minisat::Solver& solver);
    int transformClauseDb2Solver(ClauseDb& clauseDb, Minisat::Solver& solver);
};

#endif