//
// C++ Implementation by Ajay Bhaga
//
// Modified Genetic Algorithm
// Based on design of Samuel Arzt (March 2017)
//

#pragma once

#include "genotype.h"
#include "neural_layer.h"
#include "genetic_algorithm.h"
#include "neural_network.h"
#include "sensor.h"
#include "flag.h"
#include "agent_fsm.h"
#include "agent.h"
#include "agent_movement.h"
#include "../shared_libs.h"

// Forward declaration
class Agent;
class Sensor;

class AgentController {
public:

    AgentController(int index);
    ~AgentController();

    void awake();
    void start();
    void restart();
    void update(float duration);
    void die();
    void win();
    void checkpointCaptured();

    float getCurrentCompletionReward();
    void setCurrentCompletionReward(float reward);
    const std::vector<Sensor> &getSensors() const;
    const std::vector<Flag> &getFlags() const;
    float getLastCurrSpdKm() const;
    float getLastAvgSpdKm() const;

    int agentIndex; // Agent index of the evolution manager agents array
    AgentMovement *movement;
    AgentFSM *fsm;

    bool useUserInput = false;
    float getTimeSinceLastCheckpoint() const;


    float getDistanceCovered() { return distanceCovered_; }

    // Event for when all agents have died.
//    Event allAgentsDied;

private:
    // Maximum delay in seconds between the collection of two checkpoints until this agent dies.
    const float MAX_CHECKPOINT_DELAY = 300.0f;
    std::vector<Sensor> sensors; // Vector of sensors
    std::vector<Flag> flags; // Vector of flags

    float lastCurrSpdKm_;
    float lastAvgSpdKm_;

    float timeSinceNoWheelContact = 0.0f;
public:
    float getTimeSinceNoWheelContact() const;
    bool isAlive() const {
        return alive_;
    }

private:
    bool alive_;
    float timeSinceLastCheckpoint = 0.0f;
    float distanceCovered_ = 0.0f;
    long startTime;
    long lastTime = 0;

    float lastRewardLog_ = 0;

    float frontHitTime_ = 0;
    bool steerSet_ = false;
    float steerChoice_ = 0;
    bool dieAllowed_ = false;
};