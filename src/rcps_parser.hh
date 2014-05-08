/**
 * RCPSSolver
 * author: Martin Hruska
 * e-mail: xhrusk16@stud.fit.vutbr.cz
 */
 
#ifndef RCPS_SOLVER_PARSER_HH
#define RCPS_SOLVER_PARSER_HH

#include "rcps_instance.hh"

#include <string>

namespace RCPSSolver
{
    class Parser;
}

class RCPSSolver::Parser
{
public:
    static int parse(RCPSInstance& instance, const char* source);
    static void toString(RCPSInstance& instance);
private:
    static int getValue(std::string& line);
    static int parsePSPInfo(RCPSInstance& instance, std::string& line);
    static int parsePrecedenceLine(RCPSInstance& instance, std::string& line);
    static int parseDemandLine(RCPSInstance& instance, std::string& line);
    static int parseResourcesLine(RCPSInstance& instance, std::string& line);
};
#endif
