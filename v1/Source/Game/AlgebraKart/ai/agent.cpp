//
// C++ Implementation by Ajay Bhaga
//
// Modified Genetic Algorithm
// Based on design of Samuel Arzt (March 2017)
//

#include "../shared_libs.h"
#include <AlgebraKart/Constants.h>
#include <Urho3D/IO/Log.h>

using namespace Urho3D;

// Initializes a new agent from given genotype, constructing a new feed-forward neural network from
// the parameters of the genotype.
Agent::Agent(unsigned int id, Genotype *genotype, NeuralLayer::ActivationFunction defaultActivation, int *topology, NetworkActor* actor) {

    if (id == -1)
        id = this->generateId();
    this->id = id;

    // Store network actor
    this->networkActor_ = actor;

    // Random position spread magnitude
    float mag = 3.0f;
    this->colour = Urho3D::Vector3(Urho3D::Random(0.0f,1.0f), Urho3D::Random(0.0f,1.0f), Urho3D::Random(0.0f,1.0f));
    //std::cout << "Instantiated agent = " << id << std::endl;

    alive = false;
    this->genotype = genotype;
    ffn = new NeuralNetwork(topology, NUM_NEURAL_LAYERS);
    ffn->setRandomWeights(-1.0f, 1.0f);

    //URHO3D_LOGDEBUGF("[AGENT INITIALIZED] -> [AGENT %l] -> %s", id, ffn->toString().c_str());

    for (int i = 0; i < NUM_NEURAL_LAYERS; i++) {
        ffn->layers[i]->neuronActivationFunction = defaultActivation;
    }

    // Check if topology is valid
    if (ffn->weightCount != genotype->getParameterCount()) {
        std::cout << "Error: the given genotype's parameter count must match the neural network topology's weight count." << std::endl;
    } else {
        //std::cout << "Success: the given genotype's parameter count matches the neural network topology's weight count." << std::endl;
    }

    // Construct FFN from genotype
    for (int k = 0; k < NUM_NEURAL_LAYERS; k++) {

        for (int i = 0; i < ffn->layers[k]->neuronCount; i++) {
            for (int j = 0; j < ffn->layers[k]->outputCount; j++) {

                // Retrieve parameters for genotype
                std::vector<float> parameters = genotype->getParameterCopy();

                // Set weights to parameter values
                for (int p = 0; p < genotype->getParameterCount(); p++) {
                    ffn->layers[k]->weights[i][j] = parameters[p];
                }
            }
        }
    }

    URHO3D_LOGDEBUGF("[Agent %l] Rebuilt FFN with genotype for agent.", (id+1));
}

Agent::~Agent() {

    URHO3D_LOGDEBUGF("[Agent %l] clean up...", (id+1));

    if (ffn) {
        URHO3D_LOGDEBUGF("[Agent %l] Deleting ffn.", (id+1));
        delete ffn;
    }

}

// Reset this agent to be alive again.
void Agent::reset() {
    this->genotype->evaluation = 0;
    genotype->fitness = 0;
    alive = true;
}

void Agent::kill() {
    if (networkActor_)
    // Remove network actor
    networkActor_->Kill();

    agentDied();
    alive = false;
}

int Agent::compareTo(Agent &other) {

    bool match = true;

    // Construct FFN from genotype
    for (int k = 0; k < NUM_NEURAL_LAYERS; k++) {

        for (int i = 0; i < ffn->layers[k]->neuronCount; i++) {
            for (int j = 0; j < ffn->layers[k]->outputCount; j++) {
                // Retrieve parameters for genotype
                std::vector<float> parametersA = this->genotype->getParameterCopy();
                std::vector<float> parametersB = other.genotype->getParameterCopy();

                // Compare genotypes
                for (int p = 0; p < this->genotype->getParameterCount(); p++) {

                    if (parametersA[p] != parametersB[p]) {
                        match = false;
                        URHO3D_LOGDEBUGF("[Agent %d] Agent.compareTo -> Match failed on parameter #: %f", (id+1), p);
                    }
                }
            }
        }
    }

    if (match) {
        return 0;
    } else {
        return -1;
    }
}

bool Agent::isAlive() {
    return alive;
}

const Urho3D::Vector3 &Agent::getColour() const {
    return colour;
}

void Agent::setColour(const Urho3D::Vector3 &colour) {
    Agent::colour = colour;
}

const char* Agent::getName() {
    this->name = std::string("Agent ") + std::to_string(id);
    return name.data();
}

unsigned Agent::generateId() {
    static unsigned int idGenerator = 0;
    return ++idGenerator;
}

bool Agent::isHit() const {
    return hit;
}

void Agent::setHit(bool hit) {
    Agent::hit = hit;
}

long Agent::getLastHit() const {
    return lastHit;
}

void Agent::setLastHit(long lastHit) {
    Agent::lastHit = lastHit;
}
