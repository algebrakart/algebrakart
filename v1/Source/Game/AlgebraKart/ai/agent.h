//
// C++ Implementation by Ajay Bhaga
//
// Modified Genetic Algorithm
// Based on design of Samuel Arzt (March 2017)
//

#pragma once

#include "../shared_libs.h"
#include "../util/event.h"
#include <Urho3D/Math/MathDefs.h>
#include <Urho3D/Math/Vector3.h>
#include <Urho3D/Math/Quaternion.h>
#include <AlgebraKart/network/NetworkActor.h>

#define MAX_AGENTS 1024 // Set max limit for agents (used for storage)

class Agent {
public:

    //Agent(unsigned int id, Genotype *genotype, NeuralLayer::ActivationFunction defaultActivation, int *topology);
    Agent(unsigned int id, std::shared_ptr<Genotype> genotype, NeuralLayer::ActivationFunction defaultActivation, int *topology, std::shared_ptr<NetworkActor> actor);
    ~Agent();
    void reset();
    void kill();
    int compareTo(Agent &other);

    // Agent id
    unsigned int id;
    std::string name;

    // Underlying genotype of this agent.
    std::shared_ptr<Genotype> genotype;

    // The feed-forward neural network which was constructed from the genotype of this agent.
    NeuralNetwork *ffn;

    // Flag when the agent died (stopped participating in the simulation).
    SimpleEvent::Event agentDied;

    // Whether this agent is currently alive (actively participating in the simulation).
    bool isAlive();

    const Urho3D::Vector3 &getColour() const;
    void setColour(const Urho3D::Vector3 &colour);
    const char* getName();
    unsigned generateId();
    bool isHit() const;
    void setHit(bool hit);
    long getLastHit() const;
    void setLastHit(long lastHit);

    std::shared_ptr<NetworkActor> getActor() { return networkActor_; };
    void setActor(std::shared_ptr<NetworkActor> actor) { networkActor_ = actor; };

    float getRespawnTime() const {
        return respawnTime_;
    }

    void setRespawnTime(float respawnTime) {
        respawnTime_ = respawnTime;
    }

private:
    bool alive = false;
    bool hit = false;
    long lastHit = 0;
    float respawnTime_ = 0;

private:

    Urho3D::Vector3 colour;
    std::shared_ptr<NetworkActor> networkActor_;
};