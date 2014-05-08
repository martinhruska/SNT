/**
 * RCPSSolver
 * author: Martin Hruska
 * e-mail: xhrusk16@stud.fit.vutbr.cz
 */

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
            const int consistTime = time + instance.getDuration(job);
            for (int l = time; l < consistTime; ++l)
            {
                Literal litS = createLiteral(
                        model.startVarMap_.at(job).at(time),false);
                Literal litU = createLiteral(
                        model.processVarMap_.at(job).at(l), true);
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
            const int lStartj = instance.getLStart(jobj);
            //const int precTime = eStarti - instance.getDuration(jobj);
            for (int time = eStarti; time <= lStarti; ++time)
            {
                Literal liti = createLiteral(
                    model.startVarMap_.at(jobi).at(time),false);
                model.clausesPrecedence_.push_back(Clause());
                model.clausesPrecedence_.back().push_back(liti);

                const int timeDif = time - instance.getDuration(jobj);
                const int precTime = ( timeDif < lStartj) ? timeDif : lStartj ;
                for (int l = eStartj; l <= precTime; ++l)
                {
                    Literal litj = createLiteral(
                            model.startVarMap_.at(jobj).at(l), true);
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
                model.startVarMap_.at(job).at(time), true);
            model.clausesStart_.back().push_back(lit);
        }
    }

    model.clausesOpt_ = ClauseDb();
    for (int job=0; job < jobsNumber; ++job)
    { // create consistency clauses
        const int eFinish = instance.getEFinish(job);
        const int lFinish = instance.getLFinish(job);
        for (int time = eFinish; time < lFinish-1; ++time)
        {
            Literal litU = createLiteral(
                model.processVarMap_.at(job).at(time), false);
            Literal litU1 = createLiteral(
                model.processVarMap_.at(job).at(time+1), true);
            const int d = instance.getDuration(job);
            Literal litS = createLiteral(
                model.startVarMap_.at(job).at(time-d+1), true);
            model.clausesConsistency_.push_back(Clause());
            model.clausesConsistency_.back().push_back(litU);
            model.clausesConsistency_.back().push_back(litU1);
            model.clausesConsistency_.back().push_back(litS);
        }
    }
    // Init db for time constraints
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
    model.clausesTimeCons_.clear();
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
            model.startVarMap_.at(job).insert(std::make_pair(time, &model.startVarDb_.at(id)));
        }

        const int lFinish = instance.getLFinish(job);
        model.processVarMap_.insert(std::make_pair(job, TimeToVar()));
        for (int time = eStart; time <= lFinish; ++time)
        { // adding process variables
            const int id = model.getVarCounter();
            model.incrementVarCounter();
            model.processVarDb_.insert(std::make_pair(id,createVariable(id, job, time)));
            model.processVarMap_.at(job).insert(std::make_pair(time, &model.processVarDb_.at(id)));
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
    std::cout << "Print optis: ";
    printClauseDb(model.clausesOpt_);
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
