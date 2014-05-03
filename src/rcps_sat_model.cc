#include "rcps_sat_model.hh"

#include <iostream>

/**
 * Create SAT model from given RCPS instance.
 * @param[in] instance RCPS instance which will be used for creating SAT model
 * @param[out] model SAT model to be filled with data
 */
int RCPSSolver::createSATmodelFromRCPS(RCPSSATModel &model,
        const RCPSInstance& instance)
{
    // creates database of variable that will be futher used
    createVarDb(model, instance);
    const int jobsNumber = instance.getJobsNumber();
    for (int job=0; job < jobsNumber; ++job)
    { // create consistency clauses
        const int eStart = instance.getEStart(job);
        const int lStart = instance.getLStart(job);
        for (int time = eStart; time < lStart; ++time)
        {
            const int consistTime = time + instance.getDuration(job)-1;
            for (int l = time; l < consistTime; ++l)
            {
                Literal litS = createLiteral(
                        model.startVarMap_[job][time],false);
                Literal litU = createLiteral(
                        model.processVarMap_[job][l], true);
                model.clausesConsistency_.push_back(Clause());
                model.clausesConsistency_.back().push_back(litS);
                model.clausesConsistency_.back().push_back(litU);
            }
        }
    }

    for (int jobi=0; jobi < jobsNumber; ++jobi)
    { // create consistency clauses
        const int eStarti = instance.getEStart(jobi);
        const int lStarti = instance.getLStart(jobi);
        for (int jobj=0; jobj < jobsNumber; ++jobj)
        {
            if (!instance.getPrecedence(jobj,jobi))
            { // not in precedence relation
                continue;
            }

            const int eStartj = instance.getEStart(jobj);
            const int precTime = eStarti - instance.getDuration(jobj);
            for (int time = eStarti; time <= lStarti; ++time)
            {
                Literal liti = createLiteral(
                    model.startVarMap_[jobi][time],false);
                model.clausesPrecedence_.push_back(Clause());
                model.clausesPrecedence_.back().push_back(liti);

                for (int l = eStartj; l <= precTime; ++l)
                {
                    Literal litj = createLiteral(
                            model.startVarMap_[jobj][l], true);
                    model.clausesPrecedence_.back().push_back(litj);
                }
            }
        }
    }

    for (int job=0; job < jobsNumber; ++job)
    { // every activity has to start
        const int eStart = instance.getEStart(job);
        const int lStart = instance.getLStart(job);
        model.clausesStart_.push_back(Clause());
        for (int time=eStart; time <= lStart; ++time)
        {
            Literal lit = createLiteral(
                model.startVarMap_[job][time], true);
            model.clausesStart_.back().push_back(lit);
        }
    }

    model.clausesTimeCons_ = ClauseDb();

    return 0;

}

/**
 * Function limits maximal makespan to newMaxTime.
 * @param newMaxTime new limit of maximal makespan
 * @param lastMaxTime old limit of maximal makspan
 * @param instance RCPS instance
 * @param model SAT model where constrains are added
 */
int RCPSSolver::modelTimeConstraint(const int newMaxTime, const int lastMaxTime,
        RCPSSATModel& model, const RCPSInstance& instance)
{
    if (newMaxTime < instance.getLowerBound())
    {
        return 1;
    }
    const int lastJob = instance.getJobsNumber()-1;
    for (int time = newMaxTime; time < lastMaxTime; ++time)
    {
        if (!model.startVarMap_.count(lastJob) ||
                !model.startVarMap_.at(lastJob).count(time))
        {
            return 2;
        }
        model.clausesTimeCons_.push_back(Clause());
        Literal lit = createLiteral(
            model.startVarMap_.at(lastJob).at(time), false);
        model.clausesTimeCons_.back().push_back(lit);
    }

    return 0;
}

/**
 * Create variable databases.
 * The instance parameter should be filled with usefull data.
 * @param[in] instance RCPS instance which will be used for creating SAT model
 * @param[out] model SAT model to be filled with data
 */
int RCPSSolver::createVarDb(RCPSSATModel& model,
        const RCPSInstance& instance)
{
    const int jobsNumber = instance.getJobsNumber();
    for (int job = 0; job<jobsNumber; ++job)
    { // create variable databases
        const int eStart = instance.getEStart(job);
        const int lStart = instance.getLStart(job);
        model.startVarMap_.insert(std::make_pair(job, TimeToVar()));
        for (int time = eStart; time <= lStart; ++time)
        { // adding start variables
            const int id = model.getVarCounter();
            model.incrementVarCounter();
            model.startVarDb_.insert(std::make_pair(id,createVariable(id, job, time)));
            model.startVarMap_[job].insert(std::make_pair(time, &model.startVarDb_[id]));
        }

        const int lFinish = instance.getLFinish(job);
        model.processVarMap_.insert(std::make_pair(job, TimeToVar()));
        for (int time = eStart; time <= lFinish; ++time)
        { // adding process variables
            const int id = model.getVarCounter();
            model.incrementVarCounter();
            model.processVarDb_.insert(std::make_pair(id,createVariable(id, job, time)));
            model.processVarMap_[job].insert(std::make_pair(time, &model.processVarDb_[id]));
        }
    }
    return 0;
}

void RCPSSolver::printModel(RCPSSATModel& model)
{
    std::cout << "Print start values: \n";
    printVarDb(model.startVarDb_);
    std::cout << "Print process values: \n";
    printVarDb(model.processVarDb_);
    std::cout << "Print consistency: ";
    printClauseDb(model.clausesConsistency_);
    std::cout << "Print precedence: ";
    printClauseDb(model.clausesPrecedence_);
    std::cout << "Print time cons: ";
    printClauseDb(model.clausesTimeCons_);
    std::cout << "Print start: ";
    printClauseDb(model.clausesStart_);
}

void RCPSSolver::printVarDb(VariableDb& vars)
{
    for (auto i : vars)
    {
        std::cout << i.first << " [" <<i.second.job << "," << i.second.time << "], ";
    }
    std::cout << '\n';
}

void RCPSSolver::printClauseDb(ClauseDb& clauseDb)
{
    for (auto clause : clauseDb)
    {
        for (auto lit : clause)
        {
            int v = lit.var->id;
            int jobres = lit.var->job;
            int t = lit.var->time;
            if (!lit.sign)
                std::cout << "-";
            std::cout << v << " [" << jobres << ',' << t << "]" << " \\/ ";
            //std::cout << lit.var->id << " \\/ ";
        }
        std::cout << " && \n";
    }
    std::cout << '\n';
}
