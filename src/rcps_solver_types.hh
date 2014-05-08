/**
 * RCPSSolver
 * author: Martin Hruska
 * e-mail: xhrusk16@stud.fit.vutbr.cz
 */

#ifndef RCPS_SOLVER_TYPE_HH
#define RCPS_SOLVER_TYPE_HH

#include <vector>
#include <string>
#include <unordered_map>

namespace RCPSSolver
{
    typedef int JobType;
    typedef int ResourceType;
    typedef std::vector<int> Vector;
    typedef std::vector<Vector> Matrix;
    typedef std::unordered_map<std::string, int> Translator;

    struct Variable
    {
        int id;
        JobType job;
        int time;

        friend Variable createVariable(int id, JobType job, int time);
    };

    inline Variable createVariable(int id, JobType job, int time)
    {
        Variable v;
        v.id = id;
        v.job = job;
        v.time = time;

        return v;
    }

    struct Literal
    {
        struct Variable* var;
        bool sign;

        friend Literal createLiteral(int var, bool sign=true);
    };

    inline Literal createLiteral(Variable* var, bool sign)
    {
        Literal l;
        l.var = var;
        l.sign = sign;

        return l;
    }

    typedef std::unordered_map<int,Variable> VariableDb;
    typedef std::unordered_map<int,Variable*> TimeToVar;
    typedef std::unordered_map<JobType,TimeToVar> JobTimeVarMap;
    typedef std::vector<Literal> Clause;
    typedef std::vector<Clause> ClauseDb;
}

#endif
