#include "rcps_model_to_glucose.hh"

#include "mtl/Vec.h"

#include <iostream>

using namespace RCPSSolver;

/**
 * Transforms created sat model to a SAT model in SAT solver
 * @param[in] model Internal SAT model of RCSP sovler
 * @param[out] solver SAT solver
 */
int RCPSModel2Glucose::transformModel2Solver(
        RCPSSATModel& model, Glucose::Solver& solver)
{
    int res1 = transformClauseDb2Solver(model.getConsistency(), solver);
    int res2 = transformClauseDb2Solver(model.getPrecedence(), solver);
    int res3 = transformClauseDb2Solver(model.getStart(), solver);
    int res4 = transformClauseDb2Solver(model.getTimeCons(), solver);
    int res5 = transformClauseDb2Solver(model.getOpt(), solver);
    int res6 = addPriority(model.getProcessVars(), solver);

    if (res1 != 0 || res2 != 0 || res3 != 0 || res4 != 0 || res5 != 0 || res6 != 0)
    {
        return 1;
    }

    return 0;
}


int RCPSModel2Glucose::addPriority(const VariableDb& vars, Glucose::Solver& solver)
{
    for (auto i : vars)
    {
        if (createdVars_.count(i.first))
        {
            solver.addPriority(createdVars_.at(i.first), 30);
        }
    }

    return 0;
}
    
int RCPSModel2Glucose::transformClauseDb2Solver(ClauseDb& clauseDb,
        Glucose::Solver& solver)
{
    for (Clause& cl : clauseDb)
    {
        Glucose::vec<Glucose::Lit> gClause;
        for (Literal& l : cl)
        {
            Glucose::Var var;
            if (createdVars_.count(l.var->id))
            {
                var = createdVars_[l.var->id];
            }
            else
            {
                var = solver.newVar(true,true);
                createdVars_.insert(std::make_pair(l.var->id, var));
                createdVarsReverse_.insert(std::make_pair(var,l.var->id));
            }
            gClause.push(Glucose::mkLit(var, !l.sign));
        }
        solver.addClause(gClause);
    }

    return 0;
}
