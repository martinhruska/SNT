/**
 * RCPSSolver
 * author: Martin Hruska
 * e-mail: xhrusk16@stud.fit.vutbr.cz
 */

#ifndef RCPS_SAT_MODEL_HH
#define RCPS_SAT_MODEL_HH

#include "rcps_solver_types.hh"
#include "rcps_instance.hh"

namespace RCPSSolver
{
    class RCPSSATModel;
    int createSATmodelFromRCPS(RCPSSATModel &model,
            const RCPSInstance& instance);
    int createVarDb(RCPSSATModel& model,
            const RCPSInstance& instance);
    void printModel(RCPSSATModel& model);
    void printVarDb(VariableDb& vars);
    void printClauseDb(ClauseDb& clause);
    int modelTimeConstraint(const int newMaxTime, const int lastMaxTime,
            RCPSSATModel& model, const RCPSInstance& instance);
}

/**
 * Class for representation of SAT model
 */
class RCPSSolver::RCPSSATModel
{
private: // private data members
    int varCounter_;
    VariableDb startVarDb_;
    VariableDb processVarDb_;
    JobTimeVarMap startVarMap_;
    JobTimeVarMap processVarMap_;
    // created clauses
    ClauseDb clausesConsistency_;
    ClauseDb clausesPrecedence_;
    ClauseDb clausesStart_;
    ClauseDb clausesCover_;
    ClauseDb clausesTimeCons_;
    ClauseDb clausesOpt_;

public: // public constructor
    RCPSSATModel() : varCounter_(0)
    {};

private: // private functions
    void incrementVarCounter() {++varCounter_;}

public: // public functions
    int getVarCounter() {return varCounter_;}
    ClauseDb& getConsistency() {return clausesConsistency_;}
    ClauseDb& getPrecedence() {return clausesPrecedence_;}
    ClauseDb& getStart() {return clausesStart_;}
    ClauseDb& getTimeCons() {return clausesTimeCons_;}
    ClauseDb& getOpt() {return clausesOpt_;}

    const VariableDb& getStartVars() {return startVarDb_;}
    const VariableDb& getProcessVars() {return processVarDb_;}


public: // public functions
    friend int createSATmodelFromRCPS(RCPSSATModel& model,
            const RCPSInstance& instance);
    friend void printModel(RCPSSATModel& model);
    friend void printVarDb(VariableDb& vars);
    friend void printClauseDb(ClauseDb& clause);
    friend int modelTimeConstraint(const int newMaxTime, const int lastMaxTime,
            RCPSSATModel& model, const RCPSInstance& instance);
private:
    friend int createVarDb(RCPSSATModel& model,
             const RCPSInstance& instance);
};

#endif
