//
// C++ Implementation by Ajay Bhaga
//
// Based on Deep Learning Cars design of Samuel Arzt (March 2017)
// https://arztsamuel.github.io/en/projects/unity/deepCars/deepCars.html
//

#pragma once

#include "genotype.h"
#include "../util/event.h"
#include "evolution_manager.h"

// Default min value of initial population parameters.
static const float DefInitParamMin = -1.0f;

// Default max value of initial population parameters.
static const float DefInitParamMax = 1.0f;

// Default probability of a parameter being swapped during crossover.
static const float DefCrossSwapProb = 0.6f;

// Default probability of a parameter being mutated.
static const float DefMutationProb = 0.3f;

// Default amount by which parameters may be mutated.
static const float DefMutationAmount = 2.0f;

// Default percent of genotypes in a new population that are mutated.
static const float DefMutationPerc = 1.0f;

static const int RestartAfter = 100;

// Forward declarations
class EvolutionManager;

class GeneticAlgorithm {
public:

    GeneticAlgorithm(int genotypeParamCount, int populationSize);
    //GeneticAlgorithm(int genotypeParamCount, int populationSize);
    ~GeneticAlgorithm();

    static SimpleEvent::Event terminationCriterion;
    static SimpleEvent::Event algorithmTerminated;
    static SimpleEvent::Event fitnessCalculationFinished;

    void start();
    void evaluationFinished();
    void terminate();

    // Static methods
    void buildPopulation();

     static void defaultPopulationInitialization(std::vector<std::unique_ptr<Genotype>> population);
     static void asyncEvaluation(std::vector<std::unique_ptr<Genotype>> currentPopulation);
     static void defaultFitnessCalculation(std::vector<std::unique_ptr<Genotype>> currentPopulation);
     //static std::vector<Genotype*> *defaultSelectionOperator(std::vector<Genotype*> currentPopulation);
     static std::vector<std::unique_ptr<Genotype>> *remainderStochasticSampling(std::vector<std::unique_ptr<Genotype>> currentPopulation);
    // Mutates all members of the new population with the default probability, while leaving the first 2 genotypes in the list.
    static void mutateAllButBestTwo(std::vector<std::unique_ptr<Genotype>> newPopulation);
    static void mutateAll(std::vector<std::unique_ptr<Genotype>> newPopulation);
    static std::vector<std::unique_ptr<Genotype>> *randomRecombination(std::vector<std::unique_ptr<Genotype>> intermediatePopulation, int newPopulationSize);
    static std::vector<std::unique_ptr<Genotype>> *defaultRecombinationOperator(std::vector<std::unique_ptr<Genotype>> intermediatePopulation, int newPopulationSize);



     static void defaultMutationOperator(std::vector<std::unique_ptr<Genotype>> newPopulation);
     static void completeCrossover(std::unique_ptr<Genotype> parent1, std::unique_ptr<Genotype> parent2, float swapChance, std::unique_ptr<Genotype> &offspring1, std::unique_ptr<Genotype> &offspring2);
     static void mutateGenotype(std::unique_ptr<Genotype> genotype, float mutationProb, float mutationAmount);
     static bool defaultTermination(std::vector<std::unique_ptr<Genotype>> currentPopulation);

    // Use to initialize the initial population.
    typedef std::function<void (std::vector<std::unique_ptr<Genotype>> initialPopulation)> InitializationOperator;

    // Used to evaluate (or start the evaluation process of) the current population.
    typedef std::function<void (std::vector<std::unique_ptr<Genotype>> currentPopulation)> EvaluationOperator;

    // Used to calculate the fitness value of each genotype of the current population.
    typedef std::function<void (std::vector<std::unique_ptr<Genotype>> currentPopulation)> FitnessCalculation;

    // Used to select genotypes of the current population and create the intermediate population.
    typedef std::function<std::vector<std::unique_ptr<Genotype>> *(std::vector<std::unique_ptr<Genotype>> currentPopulation)> SelectionOperator;

    // Used to recombine the intermediate population to generate a new population.
    typedef std::function<std::vector<std::unique_ptr<Genotype>> *(std::vector<std::unique_ptr<Genotype>> intermediatePopulation, int newPopulationSize)> RecombinationOperator;

    // Used to mutate the new population.
    typedef std::function<void (std::vector<std::unique_ptr<Genotype>> newPopulation)> MutationOperator;

    // Used to check whether any termination criterion has been met.
    typedef std::function<bool (std::vector<std::unique_ptr<Genotype>> currentPopulation)> CheckTerminationCriterion;

    // std::function<void(int)> f1 = [](int x){ return C::f(x); };
    // Operators
    InitializationOperator initializePopulation = defaultPopulationInitialization;
    EvaluationOperator evaluation = asyncEvaluation;
    FitnessCalculation fitnessCalculationMethod = defaultFitnessCalculation;
    SelectionOperator selection = remainderStochasticSampling;
    RecombinationOperator recombination = defaultRecombinationOperator;
    MutationOperator mutation = defaultMutationOperator;
    CheckTerminationCriterion checkTermination = defaultTermination;

    const std::vector<std::unique_ptr<Genotype>> &getCurrentPopulation() const;
    const std::vector<std::unique_ptr<Genotype>> &getPrevPopulation() const;

    // The amount of genotypes in a population.
    int populationSize_;

    // The amount of generations that have already passed.
    int generationCount_ = 1;

    // Whether the current population shall be sorted before calling the termination criterion operator.
    bool sortPopulation_;

    // Whether the genetic algorithm is currently running.
    bool running_;

    int genotypeParamCount_;

    // Recreate agent -> create Network Actor
    bool refreshAgents_;

    bool isRefreshAgents() const {
        return refreshAgents_;
    }

    void setRefreshAgents(bool refreshAgents) {
        refreshAgents_ = refreshAgents;
    }

private:
    std::vector<std::unique_ptr<Genotype>> currentPopulation_;
    std::vector<std::unique_ptr<Genotype>> prevPopulation_;

};
