#ifndef RCSP_SOLVER_INSTANCE_HH
#define RCSP_SOLVER_INSTANCE_HH

#include "rcsp_solver_types.hh"

namespace RCSPSolver
{
    class RCSPInstance;
}

class RCSPSolver::RCSPInstance
{
public: // public constructor
    RCSPInstance(){}; 
private: // private data members
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

public: // public member functions
    inline int getDemand(ActivityType act, ResourceType res)
        {return resourceDemands_[act][res];}
    inline void setDemand(ActivityType act, ResourceType res, int val)
        {resourceDemands_[act][res] = val;}

    inline int getPrecedence(ActivityType act1, ActivityType act2) const
        {return precedenceRel_[act1][act2];}
    inline void setPrecedence(ActivityType act1, ActivityType act2, int val)
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
    void setEStart(int job, int val) {eStarts_[job] = val;}
    void setEFinish(int job, int val) {eFinishes_[job] = val;}
    void setLStart(int job, int val) {lStarts_[job] = val;}
    void setLFinish(int job, int val) {lFinishes_[job] = val;}


    int getUpperBound() {return upperBound_;}
    int getDuration(int pos) const
    {
        return durations_[pos];
    }
    int getJobsNumber() const {return jobs_;}

public: // friend functions
    //friend int parse(RCSPInstance& instance, const char* source);
    friend class Parser;
};

#endif
