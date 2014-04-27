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
    Vector eFinishs_;
    Vector lStarts_;
    Vector lFinishs_;

    Matrix resourceDemands_;
    Matrix precedenceRel_;

    int upperBound_;

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

    inline int getPrecedence(ActivityType act1, ActivityType act2)
        {return precedenceRel_[act1][act2];}
    inline void setPrecedence(ActivityType act1, ActivityType act2, int val)
        {precedenceRel_[act1][act2] = val;}

    void initializeMatrix(Matrix& matrix, const int x, const int y)
    {
        for(int i = 0; i < x; ++i)
        {
            matrix.push_back(Vector());
            for(int j = 0; j < y; ++j)
            {
                matrix[i].push_back(0);
            }
        }
    }

    void initializeVector(Vector& vec, const int x)
    {
        for(int i = 0; i < x; ++i)
        {
            vec.push_back(0);
        }
    }

public: // friend functions
    //friend int parse(RCSPInstance& instance, const char* source);
    friend class Parser;
};

#endif
