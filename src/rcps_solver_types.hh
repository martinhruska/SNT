#ifndef RCPS_SOLVER_TYPE_HH
#define RCPS_SOLVER_TYPE_HH

#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>

namespace RCPSSolver
{
    typedef int ActivityType;
    typedef int ResourceType;
    typedef std::vector<int> Vector;
    typedef std::vector<Vector> Matrix;
    typedef std::unordered_map<std::string, int> Translator;

    struct Literal
    {
        int time;
        int activity;
        int var;
        bool sign;

        friend Literal createLiteral(int time, int activity,
                int var, bool sign=false);
    };

    inline Literal createLiteral(int time, int activity, int var, bool sign)
    {
        Literal l;
        l.time = time;
        l.activity = activity;
        l.var = var;
        l.sign = sign;

        return l;
    }

    typedef std::unordered_map<int,Literal> LiteralDatabase;
    typedef std::unordered_set<int> Clause;
}

#endif
