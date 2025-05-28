//
// C++ Implementation by Ajay Bhaga
//
// Modified Genetic Algorithm
// Based on Deep Learning Cars design of Samuel Arzt (March 2017)
// https://arztsamuel.github.io/en/projects/unity/deepCars/deepCars.html
//

#include <algorithm>
#include "../shared_libs.h"
#include <Urho3D/Math/Vector3.h>
#include <Urho3D/Math/Quaternion.h>
#include <Urho3D/IO/Log.h>

SimpleEvent::Event GeneticAlgorithm::terminationCriterion;
SimpleEvent::Event GeneticAlgorithm::algorithmTerminated;
SimpleEvent::Event GeneticAlgorithm::fitnessCalculationFinished;

GeneticAlgorithm::GeneticAlgorithm(int genotypeParamCount, int populationSize) {

    genotypeParamCount_ = genotypeParamCount;
    populationSize_ = populationSize;
    generationCount_ = 1;
    sortPopulation_ = true;
    running_ = false;
    refreshAgents_ = false;
}

// Build population of genotypes
void GeneticAlgorithm::buildPopulation() {
    // Reset generation count
    generationCount_ = 1;
    // Build population, one genotype at a time
    for (int i = 0; i < populationSize_; i++) {
        std::string name = "agent_";
        name += String(i).CString();
        // Each genotype represents a behaviour model
        currentPopulation_.push_back(std::make_unique<Genotype>(name, genotypeParamCount_));
    }
}

GeneticAlgorithm::~GeneticAlgorithm() {
}

void GeneticAlgorithm::start() {
    // Init
    generationCount_ = 1;
    running_ = true;
    // Mark for refresh
    refreshAgents_ = true;

    // Build population
    buildPopulation();
    // Bound to defaultPopulationInitialization()
    initializePopulation(currentPopulation_);
    prevPopulation_ = std::vector(currentPopulation_);
    evaluation(currentPopulation_);
}

// Sort by genotype
bool sortByGenotype(const std::shared_ptr<Genotype> lhs, const std::shared_ptr<Genotype> rhs) { if ((!lhs) && (rhs)) { return rhs->fitness > 0; } if ((!rhs) && (lhs)) { return lhs->fitness > 0; } return lhs->fitness > rhs->fitness; }

void GeneticAlgorithm::evaluationFinished() {
    bool badPop = false;
    // Iterate through agent controllers and apply update
    std::vector<std::shared_ptr<AgentController>> controllers = EvolutionManager::getInstance()->getAgentControllers();

    // Calculate fitness from evaluation
    fitnessCalculationMethod(currentPopulation_);

    // Sort population if flag was set
    if (sortPopulation_) {
        // Sort by genotype -> highest fitness is first element
        std::sort(currentPopulation_.begin(), currentPopulation_.end(), sortByGenotype);
    }

    // Fire fitness calculation finished event
    if (&fitnessCalculationFinished)
        fitnessCalculationFinished();

    // Check termination criterion
    if (checkTermination(currentPopulation_)) {
        terminate();
        return;
    }

    std::vector<std::shared_ptr<Genotype>> intermediatePopulation;
    std::vector<std::shared_ptr<Genotype>> newPopulation;

    // [AB]: These are seg-faulting
    // To avoid -> stop evolving when no population growth

    // Apply selection
    intermediatePopulation = remainderStochasticSampling(currentPopulation_);

    // If less than 2 genotypes exist in the intermediate population, the random recombination will fail
//    assert(intermediatePopulation && intermediatePopulation->size() >= 2);
    if (intermediatePopulation.size() < 2) {
        // Bad population
        badPop = true;
    }

    if (!badPop) {
        // Apply recombination
        newPopulation = randomRecombination(intermediatePopulation, populationSize_);
    }

    //assert(newPopulation);
    // If new population is null, the mutate below will fail
    if (newPopulation.empty()) {
        // Bad population
        badPop = true;
    }

    if (!badPop) {
        // Apply mutation
        mutateAllButBestTwo(newPopulation);
    }

    // Store current as previous
    prevPopulation_ = currentPopulation_;

    // Stop when bad population
    if (badPop) {
        // Do not evolve

    } else {
        // Continue evolution cycle

        // Set current population to newly generated one and start evaluation again
        currentPopulation_ = newPopulation;
        generationCount_++;
    }

    // Calls startEvaluation() -> regenerates agents
    evaluation(currentPopulation_);
}

void GeneticAlgorithm::terminate() {

    running_ = false;
    if (&algorithmTerminated)
        algorithmTerminated();
}

// Bound to initializePopulation for GA
void GeneticAlgorithm::defaultPopulationInitialization(std::vector<std::shared_ptr<Genotype>> population) {

    int popCount = 0;
    // Set parameters to random values in set range
    for (int i = 0; i < population.size(); i++) {
        population[i]->setRandomParameters(DefInitParamMin, DefInitParamMax);
        popCount++;
    }
}

void GeneticAlgorithm::asyncEvaluation(std::vector<std::shared_ptr<Genotype>> currentPopulation) {
    // At this point the async evaluation should be started and after it is finished EvaluationFinished should be called
    std::cout << "Reached async evaluation." << std::endl;
}

void GeneticAlgorithm::defaultFitnessCalculation(std::vector<std::shared_ptr<Genotype>> currentPopulation) {

    // First calculate average evaluation of whole population
    int populationSize = 0;
    float overallEvaluation = 0;

    for (int i = 0; i < currentPopulation.size(); i++) {
        overallEvaluation += currentPopulation[i]->evaluation;
        populationSize++;
    }

    float averageEvaluation = overallEvaluation / populationSize;


    // Now assign fitness with formula fitness = evaluation / averageEvaluation
    for (int i = 0; i < currentPopulation.size(); i++) {
            currentPopulation[i]->fitness = currentPopulation[i]->evaluation / averageEvaluation;
        }
}

std::vector<std::shared_ptr<Genotype>> GeneticAlgorithm::remainderStochasticSampling(std::vector<std::shared_ptr<Genotype>> currentPopulation) {

    // AB:
    // TODO: this is not producing intermediate population of atleast 2 sometimes, this causes downstream error

    std::vector<std::shared_ptr<Genotype>> intermediatePopulation = std::vector<std::shared_ptr<Genotype>>();
    // Put integer portion of genotypes into intermediatePopulation
    // Assumes that currentPopulation is already sorted

    //std::cout << "selection -> remainderStochasticSampling(): " << currentPopulation.size() << std::endl;



    for (int i = 0; i < currentPopulation.size(); i++) {

        URHO3D_LOGDEBUGF("remainderStochasticSampling -> currentPopulation[i]->fitness: %f", currentPopulation[i]->fitness);
        // This is the potential error -> fitness could likely be less than 1 in bad performing population?
        if (currentPopulation[i]->fitness < 1) {
            URHO3D_LOGDEBUGF("remainderStochasticSampling -> BREAK");
            break;
        } else {
            for (int i = 0; i < (int) currentPopulation[i]->fitness; i++) {
                Genotype *g = new Genotype(std::string("rss"), currentPopulation[i]->getParameterCopy());
                intermediatePopulation.emplace_back(g);
                URHO3D_LOGDEBUGF("remainderStochasticSampling -> intermediatePopulation -> load rss genotype: %s", g->getAgentName().c_str());
            }
        }
    }

    // Put remainder portion of genotypes into intermediatePopulation
    for (int i = 0; i < currentPopulation.size(); i++) {

        float remainder = currentPopulation[i]->fitness - (int) currentPopulation[i]->fitness;
        if (Urho3D::Random(0.0f, 1.0f) < remainder) {
            Genotype *g = new Genotype(std::string("rem"), currentPopulation[i]->getParameterCopy());
            intermediatePopulation.emplace_back(g);
            URHO3D_LOGDEBUGF("remainderStochasticSampling -> intermediatePopulation -> load rem genotype: %s", g->getAgentName().c_str());
        }
    }

    std::cout << "selection -> remainderStochasticSampling(): " << intermediatePopulation.size() << std::endl;

    return intermediatePopulation;
}

// Mutates all members of the new population with the default probability, while leaving the first 2 genotypes in the list.
void GeneticAlgorithm::mutateAllButBestTwo(std::vector<std::shared_ptr<Genotype>> newPopulation) {
    std::cout << "Mutating all population but best two." << std::endl;

    // Start at 2
    for (int i = 2; i < newPopulation.size(); i++) {
        if (Urho3D::Random(0.0f, 1.0f) < DefMutationProb) {
            mutateGenotype(newPopulation[i], DefMutationProb, DefMutationAmount);
        }
    }
}

void GeneticAlgorithm::mutateAll(std::vector<std::shared_ptr<Genotype>> newPopulation) {

    for (int i = 0; i < newPopulation.size(); i++) {
        if (Urho3D::Random(0.0f, 1.0f) < DefMutationProb) {
            mutateGenotype(newPopulation[i], DefMutationProb, DefMutationAmount);
        }
    }
}

std::vector<std::shared_ptr<Genotype>> GeneticAlgorithm::randomRecombination(std::vector<std::shared_ptr<Genotype>> intermediatePopulation, int newPopulationSize) {

    if (intermediatePopulation.size() < 2) {

        std::cout << "The intermediate population has to be at least of size 2 for this operator.";
    }

    std::vector<std::shared_ptr<Genotype>> newPopulation = std::vector<std::shared_ptr<Genotype>>();

    if (newPopulation.size() < newPopulationSize) {

        std::shared_ptr<Genotype> offspring1;
        std::shared_ptr<Genotype> offspring2;

        // Get first 2 list items (top 2)
        size_t n = 2;
        auto end = std::next(intermediatePopulation.begin(), std::min(n, intermediatePopulation.size()));
//        std::vector<std::shared_ptr<Genotype>> b = std::make_shared<Genotype>(intermediatePopulation.begin(), end);
        std::vector<std::shared_ptr<Genotype>> b = std::vector(intermediatePopulation.begin(), end);

        std::shared_ptr<Genotype> intermediatePopulation0;
        std::shared_ptr<Genotype> intermediatePopulation1;

        int count = 0;
        for (int i = 0; i < b.size(); i++) {

            switch (i) {
                case 0:
                    intermediatePopulation0 = b[i];
                    break;
                case 1:
                    intermediatePopulation1 = b[i];
                    break;

            }
        }

        // Always add best two (unmodified)
        newPopulation.emplace_back(intermediatePopulation0);
        newPopulation.emplace_back(intermediatePopulation1);

        while (newPopulation.size() < newPopulationSize) {

            // Get two random indices that are not the same.
            int randomIndex1 = (int) Urho3D::Random(0.0, (float) std::round(intermediatePopulation.size()));
            int randomIndex2;

            do {
                randomIndex2 = (int) Urho3D::Random(0.0, (float) std::round(intermediatePopulation.size()));
            } while (randomIndex2 == randomIndex1);

            completeCrossover(intermediatePopulation[randomIndex1],
                                                     intermediatePopulation[randomIndex2],
                                                     DefCrossSwapProb, offspring1, offspring2);

            newPopulation.emplace_back(offspring1);
            if (newPopulation.size() < newPopulationSize) {
                newPopulation.emplace_back(offspring2);
            }
        }
    }

    return newPopulation;
}
/*
std::vector<Genotype*> *GeneticAlgorithm::defaultSelectionOperator(std::vector<Genotype*> currentPopulation) {

    std::vector<Genotype*>* intermediatePopulation = new std::vector<Genotype*>();

    // Get first 3 list items (top 3)
    size_t n = 3;
    auto end = std::next(currentPopulation.begin(), std::min(n, currentPopulation.size()));
    std::vector<Genotype*> b(currentPopulation.begin(), end);

    std::cout << "defaultSelectionOperator: " << currentPopulation.size() << std::endl;

    // Selects best three genotypes of the current population and copies them to the intermediate population.
    for (std::vector<Genotype*>::iterator it = b.begin(); it != b.end(); ++it) {
        intermediatePopulation->push_back(*it);
    }

    return intermediatePopulation;
}*/

// Simply crosses the first with the second genotype of the intermediate population until the new population is of desired size.
std::vector<std::shared_ptr<Genotype>> GeneticAlgorithm::defaultRecombinationOperator(std::vector<std::shared_ptr<Genotype>> intermediatePopulation, int newPopulationSize) {

    if (intermediatePopulation.size() < 2) {
        std::cout << "Intermediate population size must be greater than 2 for this operator.";
    }

    std::vector<std::shared_ptr<Genotype>> newPopulation = std::vector<std::shared_ptr<Genotype>>();

    if (newPopulation.size() < newPopulationSize) {

        std::shared_ptr<Genotype> offspring1;
        std::shared_ptr<Genotype> offspring2;

        // Get first 2 list items (top 2)
        size_t n = 2;
        auto end = std::next(intermediatePopulation.begin(), std::min(n, intermediatePopulation.size()));
        std::vector<std::shared_ptr<Genotype>> b(intermediatePopulation.begin(), end);

        completeCrossover(b[0], b[1], DefCrossSwapProb, offspring1, offspring2);

        newPopulation.push_back(offspring1);
        if (newPopulation.size() < newPopulationSize) {
            newPopulation.push_back(offspring2);
        }
    }

    return newPopulation;
}

void GeneticAlgorithm::defaultMutationOperator(std::vector<std::shared_ptr<Genotype>> newPopulation) {

    for (std::vector<std::shared_ptr<Genotype>>::iterator it = newPopulation.begin(); it != newPopulation.end(); ++it) {
        for (int i = 0; i < newPopulation.size(); i++) {
            if (Urho3D::Random(0.0f,1.0f) < DefMutationPerc) {
                mutateGenotype(newPopulation[i], DefMutationProb, DefMutationAmount);
            }
        }
    }
}

void GeneticAlgorithm::completeCrossover(std::shared_ptr<Genotype> parent1, std::shared_ptr<Genotype> parent2, float swapChance, std::shared_ptr<Genotype> offspring1,
                                         std::shared_ptr<Genotype> offspring2) {

    // Initialize new parameter vectors
    int parameterCount = parent1->getParameterCopy().size();
    std::vector<std::shared_ptr<float>> off1Parameters = std::vector<std::shared_ptr<float>>();
    off1Parameters.resize(parameterCount);
    std::vector<std::shared_ptr<float>> off2Parameters = std::vector<std::shared_ptr<float>>();
    off2Parameters.resize(parameterCount);

    // Iterate over all parameters randomly swapping
    for (int i = 0; i < parameterCount; i++) {

        if (Urho3D::Random(0.0f,1.0f) < swapChance) {
            // Swap parameters
            off1Parameters[i] = std::make_shared<float>(parent2->getParameter(i));
            off2Parameters[i] = std::make_shared<float>(parent1->getParameter(i));
        } else {
            // Dont swap parameters
            off1Parameters[i] = std::make_shared<float>(parent1->getParameter(i));
            off2Parameters[i] = std::make_shared<float>(parent2->getParameter(i));
        }
    }

    std::string babyName1 = "baby1_" + parent1->getAgentName() + "-" + parent2->getAgentName();
    std::string babyName2 = "baby2_" + parent1->getAgentName() + "-" + parent2->getAgentName();

    offspring1 = std::make_shared<Genotype>(babyName1, parameterCount, off1Parameters);
    offspring2 = std::make_shared<Genotype>(babyName2, parameterCount, off2Parameters);

}

void GeneticAlgorithm::mutateGenotype(std::shared_ptr<Genotype> genotype, float mutationProb, float mutationAmount) {

    for (int i = 0; i < genotype->getParameterCount(); i++) {

        if (Urho3D::Random(0.0f,1.0f) < mutationProb) {
            // Mutate by random amount in range [-mutationAmount, mutationAmount]
            genotype->setParameter(i, genotype->getParameter(i) + (float)Urho3D::Random(0.0f,1.0f) * (mutationAmount * 2) - mutationAmount);
        }
    }
}

bool GeneticAlgorithm::defaultTermination(std::vector<std::shared_ptr<Genotype>> currentPopulation) {

    std::cout << "GeneticAlgorithm::defaultTermination -> Generation count: " << String(EvolutionManager::getInstance()->getGenerationCount()).CString() << std::endl;

    return (EvolutionManager::getInstance()->getGenerationCount() >= RestartAfter);
}

const std::vector<std::shared_ptr<Genotype>> GeneticAlgorithm::getCurrentPopulation() const {
    return currentPopulation_;
}

const std::vector<std::shared_ptr<Genotype>> GeneticAlgorithm::getPrevPopulation() const {
    return prevPopulation_;
}
