#ifndef RCSP_SOLVER_PARSER_HH
#define RCSP_SOLVER_PARSER_HH

#include "rcsp_instance.hh"

#include <string>

namespace RCSPSolver
{
    class Parser;
}

class RCSPSolver::Parser
{
public:
    static int parse(RCSPInstance& instance, const char* source);
    static void toString(RCSPInstance& instance);
private:
    static int getValue(std::string& line);
    static int parsePSPInfo(RCSPInstance& instance, std::string& line);
    static int parsePrecedenceLine(RCSPInstance& instance, std::string& line);
    static int parseDemandLine(RCSPInstance& instance, std::string& line);
    static int parseResourcesLine(RCSPInstance& instance, std::string& line);
};
#endif
