//
// C++ Implementation by Ajay Bhaga
//
// Modified Genetic Algorithm
// Based on design of Samuel Arzt (March 2017)
//

#pragma once

#include <AlgebraKart/network/NetworkActor.h>
#include "genetic_algorithm.h"
#include "agent_controller.h"
#include "../util/event.h"

// Forward declarations
class GeneticAlgorithm;
class NetworkActor;

//-- Singleton class for managing the evolutionary processes.

// Normal class for managing the evolutionary processes.

class EvolutionManager {
public:

    EvolutionManager();
    //   static EvolutionManager *getInstance();
    ~EvolutionManager();

    static void instantiate();
    static void clean(); // exit
    static EvolutionManager* getInstance();
    static int getGenerationCount();
    static void startEvolution(); // Use existing instance of evolution manager
    static void restartAlgorithm(float wait);
    static GeneticAlgorithm *getGeneticAlgorithm();

    static NeuralNetwork* buildNeuralNetwork();
    static void writeStatisticsFileStart();
    static void writeStatisticsToFile();
    static void checkForTrackFinished();
    static bool checkGenerationTermination();
    static void onGATermination();
    static void startEvaluation(std::vector<Genotype*> currentPopulation);
    static void onAgentDied();
    static std::vector<Genotype*> *remainderStochasticSampling(std::vector<Genotype*> currentPopulation);
    static std::vector<Genotype*> *randomRecombination(std::vector<Genotype*> intermediatePopulation, int newPopulationSize);
    static void mutateAllButBestTwo(std::vector<Genotype*> newPopulation);
    static void mutateAll(std::vector<Genotype*> newPopulation);
    static void evalFinished();

    // The amount of agents that are currently alive.
    static int agentsAliveCount;

    // Event for when all agents have died.
    static SimpleEvent::Event allAgentsDied;

    const std::vector<Agent*> &getAgents() const;
    const std::vector<AgentController*> &getAgentControllers() const;
    static std::vector<NetworkActor *> &getNetworkActors();


        // Whether or not the results of each generation shall be written to file.
    static bool saveStatistics;
    static std::string statisticsFileName;
    static std::ofstream statisticsFile;

    // How many of the first to finish the course should be saved to file
    static int saveFirstNGenotype;
    static int genotypesSaved;

    // Population size
    static int populationSize;

    // After how many generations should the genetic algorithm be restarted (0 for never)
    static int restartAfter;

    // Whether to use elitist selection or remainder stochastic sampling
    static bool elitistSelection;

    // Topology of the agent's FNN
    static int* ffnTopology;

    // The current population agents.
    static std::vector<Agent*> agents;

    // The current population agents.
    static std::vector<AgentController*> agentControllers;

    // The current population NetworkActors.
    static std::vector<NetworkActor*> networkActors;

    static GeneticAlgorithm *geneticAlgorithm;

private:
    static EvolutionManager *instance;

    bool firstRun_;
};

#define POPULATION_SIZE 8 // Population
#define NO_WHEEL_CONTACT_DIE_TIME 60.0f // How long for agent to die, when no wheel contact