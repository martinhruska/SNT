/**
 * RCPSSolver
 * author: Martin Hruska
 * e-mail: xhrusk16@stud.fit.vutbr.cz
 */

#include "rcps_parser.hh"

#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <stdexcept>

using namespace RCPSSolver;

/**
 * Functions parse input PSPLIB file
 * \param[in] source - name of input file
 * \param[out] instance - instance to be filled with data
 * \return error code
 */
int Parser::parse(RCPSInstance& instance, const char* source)
{
    std::ifstream ifile(source);

    if (!ifile.is_open())
    { // not open correctly
        return 1;
    }
    
    std::string line;
    int state = 0;
    const int sink=25;
    const int end=26;
    while(std::getline(ifile,line))
    {
        switch(state)
        { // parsing file by line
            case(0):
                if (line[0] != '*')
                {
                    state = sink;
                }
                else
                {
                    state = 1;
                }
                break;
            case(1):
                if (line[0] == '*')
                {
                    state = 2;
                }
                break;
            case(2): // projects is always, so skip it
                state = 3;
                break;
            case(3):
            { // read number of jobs
                int temp = getValue(line);
                if (temp < 0)
                {
                    state = sink;
                }
                instance.jobs_ = temp;
                instance.initializeMatrix(instance.precedenceRel_,
                        instance.jobs_, instance.jobs_);
                instance.initializeVector(instance.durations_,
                        instance.jobs_);
                instance.initializeTimeWindows(instance.jobs_);
                state = 4;
                break;
            }
            case(4):
            { // parse upper bound info
                int temp = getValue(line);
                if (temp < 0)
                {
                    state = sink;
                }
                instance.upperBound_ = temp;
                instance.horizont_ = temp;
                state = 5;
                break;

            }
            case(5):
                state = 6;
                break;
            case(6):
            { // precedence part
                int temp = getValue(line);
                if (temp < 0)
                {
                    state = sink;
                }
                instance.resources_ = temp;
                instance.initializeMatrix(instance.resourceDemands_,
                        instance.jobs_, instance.resources_);
                state = 7;
                break;
            }
            case(7):
                if (line[0] == '*')
                {
                    state = 8;
                }
                break;
            case(8): // skip over PROJECT INFORMATION
            case(9): // skip over project information header
                state += 1;
                break;
            case(10):
                if (parsePSPInfo(instance, line) != 0)
                { // parser precedence part info
                    return -1;
                }
                state = 11;
                break;
            case(11):
            case(12):
            case(13): // skip to precedence relation
                ++state;
                break;
            case(14):
                if (line[0] == '*')
                {
                    state = 15;
                }
                else
                { // precedence relation
                    int res = parsePrecedenceLine(instance,line);
                    if (res != 0)
                    {
                        return res;
                    }
                }
                break;
            case(15):
            case(16):
            case(17): // skip over request header
                ++state;
                break;
            case(18): // resources demands
                if (line[0] == '*')
                {
                    state = 19;
                }
                else
                {
                    int res = parseDemandLine(instance,line);
                    if (res != 0)
                    {
                        return res;
                    }
                }
                break;
            case(19):
            case(20): // skip over to resources
                ++state;
                break;
            case(21): // resources are parsed
                if (parseResourcesLine(instance,line) != 0)
                {
                    return -1;
                }
                ++state;
                break;
            case(22):
                if (line[0] == '*')
                {
                    state = end;
                }
                else
                {
                    state = sink;
                }
                break;
            case(sink):
                break;
            case(end):
                break;
            default:
                state = sink;
        }

        ///std::cout << line << std::endl;
    }

    ifile.close();

    int res;
    if (state == sink)
    {
        res = 1;
    }
    else
    {
        res = 0;
    }
    
    return res;
}

/**
 * Get number of activities from given line
 */
int Parser::getValue(std::string& line)
{
    unsigned int i = 0;
    while (i < line.size() && line[i] != ':') i++;
    if (i >= line.size())
    {
        return -1;
    }
    while (i < line.size() && !isdigit(line[i])) i++;

    if (i >= line.size())
    {
        return -1;
    }

    int res = -1;
    try
    {
        res = std::stoi(line.substr(i));
    }
    catch (const std::invalid_argument& ia) 
    {
        return -1;
    }

    return res;
}

/**
 * Line with general info about RCPS
 */
int Parser::parsePSPInfo(RCPSInstance& instance, std::string& line)
{
    char* cLine;
    char * pEnd;
    instance.projNum_ = strtol(line.c_str(),&pEnd,10);
    if (pEnd == line.c_str())
    {
        return -1;
    }

    cLine = pEnd;
    instance.realJobs_ = strtol(pEnd,&pEnd,10);
    if (pEnd == cLine)
    {
        return -1;
    }
    
    cLine = pEnd;
    instance.relDate_ = strtol(pEnd,&pEnd,10);
    if (pEnd == cLine)
    {
        return -1;
    }

    cLine = pEnd;
    instance.dueDate_ = strtol(pEnd,&pEnd,10);
    if (pEnd == cLine)
    {
        return -1;
    }

    cLine = pEnd;
    instance.tardCost_ = strtol(pEnd,&pEnd,10);
    if (pEnd == cLine)
    {
        return -1;
    }

    cLine = pEnd;
    instance.mpmTime_ = strtol(pEnd,&pEnd,10);
    if (pEnd == cLine)
    {
        return -1;
    }

    return 0;
}

/**
 * Function expects initialized precedence relation
 */
int Parser::parsePrecedenceLine(RCPSInstance& instance, std::string& line)
{
    char* cLine;
    char * pEnd;
    const int job = strtol(line.c_str(),&pEnd,10);
    if (pEnd == line.c_str())
    {
        return -1;
    }

    cLine = pEnd;
    const int mode = strtol(pEnd,&pEnd,10);
    if (pEnd == cLine)
    {
        return -1;
    }
    if (mode != 1)
    {
        return 2;
    }

    cLine = pEnd;
    const int succs = strtol(pEnd,&pEnd,10);
    if (pEnd == cLine)
    {
        return -1;
    }

    for(int i = 0; i < succs; ++i)
    {
        cLine = pEnd;
        const int sucTask = strtol(pEnd,&pEnd,10);
        if (pEnd == cLine)
        {
            return -1;
        }

        instance.setPrecedence(job-1, sucTask-1, 1);
    }

    return 0;
}

/**
 * Function expects initialized request relation and duration vector
 */
int Parser::parseDemandLine(RCPSInstance& instance, std::string& line)
{
    char* cLine;
    char * pEnd;
    const int job = strtol(line.c_str(),&pEnd,10);
    if (pEnd == line.c_str())
    {
        return -1;
    }

    cLine = pEnd;
    const int mode = strtol(pEnd,&pEnd,10);
    if (pEnd == cLine)
    {
        return -1;
    }
    if (mode != 1)
    {
        return 2;
    }

    cLine = pEnd;
    const int duration = strtol(pEnd,&pEnd,10);
    if (pEnd == cLine)
    {
        return -1;
    }
    instance.durations_[job-1] = duration;

    for(int i = 0; i < instance.resources_; ++i)
    {
        cLine = pEnd;
        const int resDemand = strtol(pEnd,&pEnd,10);
        if (pEnd == cLine)
        {
            return -1;
        }

        instance.setDemand(job-1, i, resDemand);
    }

    return 0;
}

/*
 * Parse resources line
 */
int Parser::parseResourcesLine(RCPSInstance& instance, std::string& line)
{
    char* cLine = NULL;
    char* pEnd = NULL;
    for(int i = 0; i < instance.resources_; ++i)
    {
        cLine = pEnd;
        int resAval;
        if (i == 0)
        { // read from start
            resAval = strtol(line.c_str(),&pEnd,10);
        }
        else
        {
            resAval = strtol(pEnd,&pEnd,10);
        }
        if (pEnd == cLine)
        {
            return -1;
        }

        instance.resourcesAval_.push_back(resAval);
    }

    return 0;
}

/**
 * Print a given instance of rcsp problem
 */
void Parser::toString(RCPSInstance& instance)
{
    std::cout << "Jobs" << instance.jobs_ << '\n';

    std::cout << "Durations and resources\n";
    for(int i=0; i < instance.jobs_; i++)
    {
        std::cout << i << " : " << instance.durations_[i] << ' ';
        for (int j : instance.resourceDemands_[i])
        {
            std::cout << j << ' ';
        }
        std::cout << '\n';
    }

    std::cout << "Resources ";
    for(int i : instance.resourcesAval_)
    {
        std::cout << i << " ";
    }
    std::cout << '\n';

    std::cout << "Precedence\n";
    for(int i = 0; i < instance.jobs_; ++i)
    {
        std::cout << i << ": ";
        for(int j = 0; j < instance.jobs_; ++j)
        {
            if (instance.getPrecedence(i,j))
            {
                std::cout << j << " ";
            }
        }
        std::cout << '\n';
    }

    std::cout << "Earliest starts ";
    for(int i : instance.eStarts_)
    {
        std::cout << i << " ";
    }
    std::cout << '\n';

    std::cout << "Latest starts ";
    for(int i : instance.lStarts_)
    {
        std::cout << i << " ";
    }
    std::cout << '\n';

    std::cout << "Additional information \n";
    std::cout << "Upper bound " << instance.upperBound_ << '\n';
    std::cout << "Lower bound " << instance.lowerBound_ << '\n';
    std::cout << "Project number " << instance.projNum_ << '\n';
    std::cout << "Real jobs " << instance.realJobs_ << '\n';
    std::cout << "Rel date " << instance.relDate_ << '\n';
    std::cout << "Due date " << instance.dueDate_ << '\n';
    std::cout << "Tard cost " << instance.tardCost_ << '\n';
    std::cout << "MPM Time " << instance.mpmTime_ << '\n';
}
