/**
 * RCPSSolver
 * author: Martin Hruska
 * e-mail: xhrusk16@stud.fit.vutbr.cz
 */

#ifndef RCPS_SOLVER_INSTANCE_HH
#define RCPS_SOLVER_INSTANCE_HH

#include "rcps_solver_types.hh"

namespace RCPSSolver
{
    class RCPSInstance;
}

/**
 * Class for representation of RCPS instance.
 * It keeps information about number of jobs,
 * precendence, resources, etc..
 */
class RCPSSolver::RCPSInstance
{
public: // public constructor
    RCPSInstance(){}; 
private: // private data members
    // info according to RCPS definition
    int jobs_;
    int resources_;
    Vector durations_;
    Vector resourcesAval_;
    Vector eStarts_;
    Vector eFinishes_;
    Vector lStarts_;
    Vector lFinishes_;

    Matrix resourceDemands_;
    Matrix precedenceRel_;

    int upperBound_;
    int lowerBound_;

    // additional psp lib info
    int projNum_;
    int realJobs_;
    int relDate_;
    int dueDate_;
    int tardCost_;
    int mpmTime_;
    int horizont_;

public: // public member functions
    int getDemand(JobType act, ResourceType res) const
        {return resourceDemands_[act][res];}
    inline void setDemand(JobType act, ResourceType res, int val)
        {resourceDemands_[act][res] = val;}

    inline int getPrecedence(JobType act1, JobType act2) const
        {return precedenceRel_[act1][act2];}
    inline void setPrecedence(JobType act1, JobType act2, int val)
        {precedenceRel_[act1][act2] = val;}

    void initializeMatrix(Matrix& matrix, const int x, const int y)
    {
        matrix = std::vector<Vector>(x);
        for(int i = 0; i < x; ++i)
        {
            matrix[i] = Vector(y);
        }
    }

    void initializeVector(Vector& vec, const int x)
    {
        vec = Vector(x);
    }

    void initializeTimeWindows(const int x)
    {
        initializeVector(eStarts_,x);
        initializeVector(lStarts_,x);
        initializeVector(eFinishes_,x);
        initializeVector(lFinishes_,x);
    }

    void setLowerBound(int val) {lowerBound_ = val;}
    void setUpperBound(int val) {upperBound_ = val;}
    void setEStart(int job, int val) {eStarts_[job] = val;}
    void setEFinish(int job, int val) {eFinishes_[job] = val;}
    void setLStart(int job, int val) {lStarts_[job] = val;}
    void setLFinish(int job, int val) {lFinishes_[job] = val;}

    int getEStart(int job) const {return eStarts_[job];}
    int getLStart(int job) const {return lStarts_[job];}
    int getEFinish(int job) const {return eFinishes_[job];}
    int getLFinish(int job) const {return lFinishes_[job];}


    int getUpperBound() const {return upperBound_;}
    int getDueDate() const {return dueDate_;}
    int getHorizont() const {return horizont_;}
    int getLowerBound() const {return lowerBound_;}
    int getDuration(int pos) const
    {
        return durations_[pos];
    }
    int getJobsNumber() const {return jobs_;}
    int getResourcesNumber() const {return resources_;}
    const Vector& getResourcesAval() const {return resourcesAval_;}

public: // friend functions
    friend class Parser;
};

#endif
