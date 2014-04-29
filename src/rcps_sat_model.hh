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
    void printClauseDb(ClauseDb& clause);
}

class RCPSSolver::RCPSSATModel
{
private: // private data members
    int varCounter_;
    VariableDb startVarDb_;
    VariableDb processVarDb_;
    JobTimeVarMap startVarMap_;
    JobTimeVarMap processVarMap_;
    ClauseDb clausesConsistency_;
    ClauseDb clausesPrecedence_;
    ClauseDb clausesStart_;
    ClauseDb clausesCover_;

public: // public constructor
    RCPSSATModel() : varCounter_(0)
    {};

private: // private functions
    void incrementVarCounter() {++varCounter_;}

public: // public functions
    int getVarCounter() {return varCounter_;}
    ClauseDb& getConsistency() {return clausesConsistency_;}

public: // public functions
    friend int createSATmodelFromRCPS(RCPSSATModel& model,
            const RCPSInstance& instance);
    friend void printModel(RCPSSATModel& model);
    friend void printClauseDb(ClauseDb& clause);
private:
    friend int createVarDb(RCPSSATModel& model,
            const RCPSInstance& instance);
};

#endif
