//
// C++ Implementation by Ajay Bhaga
//
// Modified Genetic Algorithm
// Based on design of Samuel Arzt (March 2017)
//

#pragma once

#include "agent_controller.h"
#include <Urho3D/Math/Vector3.h>
#include <Urho3D/Math/Quaternion.h>
#include <AlgebraKart/network/NetworkActor.h>

// Forward declaration
class AgentController;

class AgentMovement {
public:

    AgentMovement(AgentController *agentController);
    ~AgentMovement();

    void update(float deltaTime);
    void checkInput();
    void applyInput(float deltaTime);
    void setInputs(double *input, std::shared_ptr<NetworkActor> actor);
    void applyVelocity(float deltaTime);
    void applyFriction(float deltaTime);
    void setTarget(Vector3 target);
    Vector3 getTarget();
    void stop();
    double getSteerInput() const {
        return steerInputNorm_;
    }

    double getAccelInput() const {
        return accelInputNorm_;
    }

    double getAction() const {
        return actionNorm_;
    }

    AgentController *agentController;

private:

    // Movement constants
    const float MAX_VEL = 20.0f;
    const float ACCELERATION = 8.0f;
    const float VEL_FRICT = 2.0f;
    const float TURN_SPEED = 100.0f;

    // The current velocity of the agent.
    Urho3D::Vector3 velocity;
    // The current rotation of the agent.
    Urho3D::Quaternion rotation;

    // Vector to target
    Urho3D::Vector3 toTarget;

    // Min/max for normalization
    double steerInputMin_;
    double steerInputMax_;
    // Horizontal = steer force
    double steerInput_;
    double steerInputNorm_;


    // Min/max for normalization
    double accelInputMin_;
    double accelInputMax_;
    // Vertical = engine force
    double accelInput_;
    double accelInputNorm_;


    // Min/max for normalization
    double actionMin_;
    double actionMax_;
    // Action
    double action_;
    double actionNorm_;

    std::shared_ptr<NetworkActor> networkActor_;
};

