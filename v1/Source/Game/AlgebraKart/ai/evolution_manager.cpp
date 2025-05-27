//
// C++ Implementation by Ajay Bhaga
//
// Modified Genetic Algorithm
// Based on design of Samuel Arzt (March 2017)
// https://arztsamuel.github.io/en/projects/unity/deepCars/deepCars.html
//

#include "evolution_manager.h"
#include "../shared_libs.h"
#include <AlgebraKart/Constants.h>
#include <AlgebraKart/util/file_utility.h>

#include <Urho3D/Math/MathDefs.h>
//#include <unistd.h>

// Forward declarations
int EvolutionManager::agentsAliveCount;
bool EvolutionManager::saveStatistics;
std::string EvolutionManager::statisticsFileName;
std::ofstream EvolutionManager::statisticsFile;

int EvolutionManager::saveFirstNGenotype;
int EvolutionManager::genotypesSaved;
int EvolutionManager::populationSize;
int EvolutionManager::restartAfter;
bool EvolutionManager::elitistSelection;
EvolutionManager *EvolutionManager::instance = NULL;

// Topology of the agent's FNN
int *EvolutionManager::ffnTopology;

// The current population agents.
std::vector<std::shared_ptr<Agent>> EvolutionManager::agents;
// The current population agents.
std::vector<std::shared_ptr<AgentController>> EvolutionManager::agentControllers;
// The current population NetworkActors.
std::vector<std::shared_ptr<NetworkActor>> EvolutionManager::networkActors;



GeneticAlgorithm *EvolutionManager::geneticAlgorithm;

// Event for when all agents have died.
SimpleEvent::Event EvolutionManager::allAgentsDied;


bool directoryExists(const char *dname) {
    DIR *di = opendir(dname); // open the directory
    if (di) return true; // can open=>return true
    else return false; // otherwise return false
    closedir(di);
}

EvolutionManager::EvolutionManager() {
    saveStatistics = true;

    agentsAliveCount = 0;

    // How many of the first to finish the course should be saved to file
    saveFirstNGenotype = 100;
    genotypesSaved = 0;

    // Population size
    populationSize = POPULATION_SIZE;

    // After how many generations should the genetic algorithm be restarted (0 for never)
    restartAfter = RestartAfter;

    // Whether to use elitist selection or remainder stochastic sampling
    elitistSelection = false;

}

void EvolutionManager::instantiate() {
    instance = new EvolutionManager();
    instance->firstRun_ = true;
}

void EvolutionManager::clean() {
    if (instance)
        delete instance;
}

EvolutionManager::~EvolutionManager() {
    agents.clear();
    agentControllers.clear();

    if (ffnTopology)
        delete ffnTopology;


    if (geneticAlgorithm)
        delete geneticAlgorithm;
}

// The age of the current generation.
int EvolutionManager::getGenerationCount() {
    return getGeneticAlgorithm()->generationCount_;
}

void EvolutionManager::evalFinished() {
    getGeneticAlgorithm()->evaluationFinished();
}

NeuralNetwork *EvolutionManager::buildNeuralNetwork() {
    // Build Neural Network

    // Create neural layer array (NUM_NEURAL_LAYERS = 4)
    ffnTopology = new int[NUM_NEURAL_LAYERS + 1];

    // It comprises 4 layers: an input layer with 9 neurons, two hidden layers with 10 and 8 neurons respectively
    // and an output layer with 4 neurons.

    /*
     * Input Map:
     *
     *  1: sensor 1 (left)
     *  2: sensor 2 (up)
     *  3: sensor 3 (front)
     *  4: sensor 4 (right)
     *  5: distance meter
     *  6: on-track -> flag
     *  7: unflipped -> flag
     *  8: velocity consistent -> flag
     */

    // Input layer
    ffnTopology[0] = 8;

    // Hidden layers
    ffnTopology[1] = 7;
    ffnTopology[2] = 5;

    /*
     * Output Map:
     *
     *  1: steering
     *  2: acceleration
     *  3: action state
     */

    // Output layer
    ffnTopology[3] = 3;
    ffnTopology[4] = 3;

    // Create neural network to determine parameter count (only used for config setting sized same as agent ffn)
    NeuralNetwork *nn = new NeuralNetwork(ffnTopology, NUM_NEURAL_LAYERS);
    return nn;
}


void EvolutionManager::startEvolution() {

    // Build neural network for agent
    NeuralNetwork *nn = buildNeuralNetwork();

    // Setup genetic algorithm
    geneticAlgorithm = new GeneticAlgorithm(nn->weightCount, populationSize);
    genotypesSaved = 0;

    // Set genetic algorithm function bindings

    // Assign evaluation function to GA
    geneticAlgorithm->evaluation = startEvaluation;

    char buffer[80];
    time_t rawtime;
    struct tm *timeinfo;
    time(&rawtime);
    timeinfo = localtime(&rawtime);


    // Attach function to Event for all agents died
    allAgentsDied += evalFinished;

    // Attach functions to Genetic Algorithm Events
    geneticAlgorithm->fitnessCalculationFinished += writeStatisticsToFile;
    geneticAlgorithm->fitnessCalculationFinished += checkForTrackFinished;
    geneticAlgorithm->terminationCriterion += checkGenerationTermination;
    geneticAlgorithm->algorithmTerminated += onGATermination;

    // Statistics
    strftime(buffer, sizeof(buffer), "%d-%m-%Y_%H:%M:%S", timeinfo);
    std::string str(buffer);
    statisticsFileName = std::string("evol-mgr_eval-data-") + buffer;
    writeStatisticsFileStart();
    std::cout << "[" << currentDateTime()
              << "] Evolution Manager - saveStatistics enabled -> adding handler to fitnessCalculationFinished."
              << std::endl << std::flush;

    // Start GA Algorithm
    geneticAlgorithm->start();
}

void EvolutionManager::writeStatisticsFileStart() {

    std::cout << "[" << currentDateTime() << "] Evolution Manager - writeStatisticsFileStart()..." << std::endl
              << std::flush;

    // Statistics File Init
}

void EvolutionManager::writeStatisticsToFile() {

    /*URHO3D_LOGDEBUGF("EvolutionManager::writeStatisticsToFile");
    std::cout << "[" << currentDateTime() << "] Evolution Manager - writeStatisticsToFile()..." << std::endl
              << std::flush;*/


    std::string outText;
    std::string lineText = "=========================================================================================";

    std::vector<std::shared_ptr<Genotype>> currentPopulation = getGeneticAlgorithm()->getCurrentPopulation();

    outText += lineText;
    outText += "\n";
    outText += "POPULATION TIME: ";
    outText += currentDateTime().c_str();
    outText += "\n";

    outText += "-----------------";
    outText += "\n";
    outText += "POPULATION LISTING";
    outText += "\n";
    outText += "-----------------";
    outText += "\n";

    outText += "Generation Count -> ";
    outText += String(getGeneticAlgorithm()->generationCount_).CString();
    outText += "\n";
    outText += "Population Size -> ";
    outText += String(getGeneticAlgorithm()->populationSize_).CString();
    outText += "\n";
    outText += "\n";
    outText += "-----------------";
    outText += "\n";
    outText += "GENOTYPE LISTING";
    outText += "\n";
    outText += "-----------------";
    outText += "\n";

    for (int i = 0; i < currentPopulation.size(); i++) {

        float eval = currentPopulation[i]->evaluation;
        float fit = currentPopulation[i]->fitness;

        outText += "Genotype ID -> ";
        outText += String(i+1).CString();
        outText += "\n";
        outText += "Genotype Evaluation: ";
        outText += String(eval).CString();
        outText += "\n";
        outText += "Genotype Fitness: ";
        outText += String(fit).CString();
        outText += "\n";

        std::cout << "[Evolution Manager] - Generation Count -> "
                  << String(getGeneticAlgorithm()->generationCount_).CString() << "\n"
                  << "Genotype Evaluation: " << String(currentPopulation[i]->evaluation).CString() << std::endl << outText << std::endl
                  << std::flush;
        std::cout << lineText << std::endl << std::flush;
    }

    outText += lineText;
    outText += "\n";

    std::string fileExt = ".txt";
        std::string dirPath = TRAINING_DATA_DIR;
        std::string fullPath = dirPath + statisticsFileName + fileExt;

        statisticsFile.open(fullPath, std::ios_base::app);
        if (statisticsFile.is_open()) {
            statisticsFile << outText;
            statisticsFile.close();
        }

}

// Checks the current population and saves genotypes to a file if their evaluation is greater than or equal to 1.
void EvolutionManager::checkForTrackFinished() {
    std::cout << "[" << currentDateTime() << "] Evolution Manager - checkForTrackFinished()..." << std::endl
              << std::flush;

    if (instance->firstRun_) {
        instance->firstRun_ = false;
        return;
    }

    //if (genotypesSaved >= saveFirstNGenotype) return;

    std::string saveFolder = TRAINING_DATA_DIR + statisticsFileName + "/";
    std::vector<std::shared_ptr<Genotype>> currentPopulation = getGeneticAlgorithm()->getCurrentPopulation();

    for (int i = 0; i < currentPopulation.size(); i++) {

        if (currentPopulation[i]->evaluation >= 1) {

            /*
            const std::string cwd = std::filesystem::current_path().c_str();
            //const std::string path = cwd + "/bin/" + saveFolder.data();
            const std::string path = cwd + "/" + saveFolder.data();

            // Create directory
            if (FileUtility::WriteDirectory(path) == 0) {
                // Success

                // Store genotype data file
                std::string genotypeFile = path + currentPopulation[i]->getAgentName() + "_gtype.dat";
                currentPopulation[i]->saveToFile(genotypeFile.data());

            } else {
                // Failed
                int b = 0;
            }*/

        } else
            return; // List should be sorted, so we can exit here.
    }
}

// Bound to terminationCriterion for GA
bool EvolutionManager::checkGenerationTermination() {
    // Check termination based on current population
    return getGeneticAlgorithm()->checkTermination(getGeneticAlgorithm()->getCurrentPopulation());
}

// Bound to algorithmTerminated for GA
void EvolutionManager::onGATermination() {

    URHO3D_LOGDEBUG("EvolutionManager::onGATermination");

    // Detach handlers
    allAgentsDied -= evalFinished;
    // Detach functions to Genetic Algorithm Events
    geneticAlgorithm->fitnessCalculationFinished -= checkForTrackFinished;
    geneticAlgorithm->terminationCriterion -= checkGenerationTermination;
    geneticAlgorithm->algorithmTerminated -= onGATermination;
    geneticAlgorithm->fitnessCalculationFinished -= writeStatisticsToFile;

    restartAlgorithm(0.0f);
}

// Restart the algorithm after a specific wait time
void EvolutionManager::restartAlgorithm(float wait) {

    //std::cout << "[" << currentDateTime() << "] Evolution Manager - waiting -> " << wait << std::endl << std::flush;
    //sleep((int) wait);

    std::cout << "[" << currentDateTime() << "] Evolution Manager - restarting algorithm..." << std::endl << std::flush;

    // Instantiates Genetic Algorithm and passes Evolution Manager
    startEvolution();
}

// Starts the evaluation by first creating new agents from the current population and then restarting the track manager.
void EvolutionManager::startEvaluation(std::vector<std::shared_ptr<Genotype>> currentPopulation) {

    URHO3D_LOGDEBUGF("EvolutionManager::startEvaluation");

    // GENERATE AGENT POPULATION

    // Store agent's NetworkActor

    // Create new agents from currentPopulation
    agentsAliveCount = 0;

    std::vector<std::shared_ptr<Agent>> agentsNew;
    std::vector<std::shared_ptr<AgentController>> agentControllersNew;

    for (int i = 0; i < currentPopulation.size(); i++) {

        // Resolve network actor (void AlgebraKart::CreateAgents() creates agents)
        std::shared_ptr<NetworkActor> actor = nullptr;

        if (!getNetworkActors().empty()) {
            if (getNetworkActors()[i]) {
                actor = getNetworkActors()[i];
            }
        }

        unsigned int newId = -1;
        newId = (i+1);

        // Create agent based on Genotype, with activation function and ffn topology
        std::shared_ptr<Agent> agent = std::make_shared<Agent>(newId, currentPopulation[i], MathHelper::softSignFunction, ffnTopology, actor);
        // Assigned an Agent Controller linked by id
        std::shared_ptr<AgentController> agentController = std::make_shared<AgentController>(i);

        agentsNew.emplace_back(agent);
        agentControllersNew.emplace_back(agentController);

        // Attach handler for agent died
        agent->agentDied += onAgentDied;
        agentsAliveCount++;


    }

    // Move to new lists
    agents.clear();
    agents = agentsNew;

    agentControllers.clear();
    agentControllers = agentControllersNew;
}

// Agent Died
void EvolutionManager::onAgentDied() {

    for (int i = 0; i < agents.size(); i++) {
        if (agents[i]) {
            if (!agents[i]->isAlive()) {
                if (agents[i]->getActor()) {
                    agents[i]->getActor()->SetEnabled(false);
                }
            }
        }
    }


    agentsAliveCount--;

    // Check if all agents have died and signal if so
    if (agentsAliveCount == 0) {
        allAgentsDied();
    }

}

GeneticAlgorithm *EvolutionManager::getGeneticAlgorithm() {
    return geneticAlgorithm;
}

const std::vector<std::shared_ptr<Agent>> EvolutionManager::getAgents() const {
    return agents;
}

const std::vector<std::shared_ptr<AgentController>> &EvolutionManager::getAgentControllers() const {
    return agentControllers;
}

EvolutionManager *EvolutionManager::getInstance() {
    if (!instance) {
        instantiate();
    }
    return instance;
}

std::vector<std::shared_ptr<NetworkActor>> &EvolutionManager::getNetworkActors() {
    return networkActors;
}