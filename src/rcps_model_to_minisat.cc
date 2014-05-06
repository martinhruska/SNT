#include "rcps_model_to_minisat.hh"

#include "minisat/mtl/Vec.h"

#include <iostream>

using namespace RCPSSolver;

/**
 * Transforms created sat model to a SAT model in SAT solver
 * @param[in] model Internal SAT model of RCSP sovler
 * @param[out] solver SAT solver
 */
int RCPSModel2Minisat::transformModel2Solver(
        RCPSSATModel& model, Minisat::Solver& solver)
{
    int res1 = transformClauseDb2Solver(model.getConsistency(), solver);
    int res2 = transformClauseDb2Solver(model.getPrecedence(), solver);
    int res3 = transformClauseDb2Solver(model.getStart(), solver);
    int res4 = transformClauseDb2Solver(model.getTimeCons(), solver);
    int res5 = transformClauseDb2Solver(model.getOpt(), solver);

    if (res1 != 0 || res2 != 0 || res3 != 0 || res4 != 0 || res5 != 0)
    {
        return 1;
    }

    return 0;
}
    
int RCPSModel2Minisat::transformClauseDb2Solver(ClauseDb& clauseDb,
        Minisat::Solver& solver)
{
    for (Clause& cl : clauseDb)
    {
        Minisat::vec<Minisat::Lit> gClause;
        for (Literal& l : cl)
        {
            Minisat::Var var;
            if (createdVars_.count(l.var->id))
            {
                var = createdVars_.at(l.var->id);
            }
            else
            {
                var = solver.newVar(Minisat::lbool(true),true);
                createdVars_.insert(std::make_pair(l.var->id, var));
                createdVarsReverse_.insert(std::make_pair(var,l.var->id));
            }
            gClause.push(Minisat::mkLit(var, !l.sign));
        }
        solver.addClause(gClause);
    }

    return 0;
}
