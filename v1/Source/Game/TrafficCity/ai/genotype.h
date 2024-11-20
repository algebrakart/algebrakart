//
// Genotype represents one member of population
//
//
// C++ Implementation by Ajay Bhaga
//
// Modified Genetic Algorithm
// Based on design of Samuel Arzt (March 2017)
//


#pragma once

#include <string>
#include <iostream>
#include <string>
#include <cassert>
#include <fstream>      // std::ifstream, std::ofstream
#include <vector>


class Genotype {
public:

    Genotype(std::string name, std::vector<float> parameters);
    Genotype(std::string name, int paramCount);
    Genotype(std::string name, int paramCount, float *offParameters);
    ~Genotype();
    void setRandomParameters(float minValue, float maxValue);
    std::vector<float>& getParameterCopy();
    int getParameterCount();
    void saveToFile(const char *filePath);
    Genotype *loadFromFile(const char *filePath);
    float getParameter(int index);
    void setParameter(int index, float value);
    Genotype *generateRandom(int parameterCount, float minValue, float maxValue);
    void outputToConsole();

    float evaluation;
    float fitness; // Fitness is calculated based on evaluation

private:
    std::vector<float> parameters;
    std::string agentName_;
public:
    const std::string &getAgentName() const;

    void setAgentName(const std::string &agentName);
};


struct genotype_record
{
    std::string agentName;
    float evaluation;
    float fitness;
    int parameterCount;
    float* parameters;
};

class GenotypeFile {
public:
    genotype_record record;
};

