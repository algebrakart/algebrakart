#include "../shared_libs.h"
#include <Urho3D/Math/Vector3.h>
#include <Urho3D/Math/Quaternion.h>
#include <Urho3D/IO/Log.h>
#include <AlgebraKart/network/NetworkActor.h>
#include <algorithm>

class NetworkActor;

AgentMovement::AgentMovement(AgentController *agentController) {
    this->agentController = std::make_shared<AgentController>(*agentController);

    steerInputMin_ = 999.0f;
    steerInputMax_ = -999.0f;
    steerInput_ = 0.0f;

    accelInputMin_ = 999.0f;
    accelInputMax_ = -999.0f;
    accelInput_ = 0.0f;

    actionMin_  = 999.0f;
    actionMax_ = -999.0f;
    action_ = 0.0f;
}

AgentMovement::~AgentMovement() {

}

void AgentMovement::update(float deltaTime) {
    checkInput();
    applyInput(deltaTime);
}

void AgentMovement::checkInput() {
}

void AgentMovement::applyInput(float deltaTime) {

    // Set control block for AI bot

    Controls controls;

    // set controls and pos
    //controls.yaw_ = yaw_;

    // Apply input by mapping normalized input to actor controls
    // Apply time

    float w1 = 0.0f;
    float w2 = 1.0f;
    float wyPtSteer = networkActor_->vehicle_->getDesiredSteer();

    // Apply more acceleration force when desired steer set
    float accControl = accelInputNorm_ * deltaTime;// * abs(networkActor_->vehicle_->getDesiredSteer());
    // float steerControl = ((steerInputNorm_ * deltaTime)*w1)+(wyPtSteer*deltaTime*w2);
    float steerControl = (wyPtSteer*deltaTime*w2);
    float actControl = actionNorm_ * deltaTime;



    //if (!ui->GetFocusElement()) {}
    if (accControl > 0.0f) {
        if (accControl > 0.00001f) {
           controls.Set(NTWK_CTRL_FORWARD, 1);
           controls.extraData_["accelLevel"] = accControl;
        }
    }

    if (accControl < -0.0f) {
        controls.Set(NTWK_CTRL_BACK, 1);
    }

    if (steerControl > 0.0f) {
        controls.Set(NTWK_CTRL_RIGHT, 1);
        controls.extraData_["steerLevel"] = steerControl;
    }

    if (steerControl < -0.0f) {
        controls.Set(NTWK_CTRL_LEFT, 1);
        controls.extraData_["steerLevel"] = steerControl;
    }

    if (networkActor_)
        networkActor_->SetControls(controls);

    /*
    // Agent can only accelerate further if velocity is lower than engineForce * MAX_VEL
    bool canAccelerate = true;
    if (verticalInput < 0) {
        canAccelerate = velocity.y_ > verticalInput * MAX_VEL;
    } else if (verticalInput > 0) {
        canAccelerate = velocity.y_ < verticalInput * MAX_VEL;
    }

    // Set velocity
    if (canAccelerate) {
        velocity.y_ += (float) verticalInput * ACCELERATION * deltaTime;

        // Clamp velocity
        if (velocity.y_ > MAX_VEL) {
            velocity.y_ = MAX_VEL;
        } else if (velocity.y_ < -MAX_VEL) {
            velocity.y_ = -MAX_VEL;
        }
    }

 */
    // Set rotation
  //  this->rotation = agentController->agent->getRotation();
  //  this->rotation *= (float)-horizontalInput * TURN_SPEED * deltaTime;
}

void AgentMovement::setInputs(double *input, NetworkActor *actor) {

    // 3 outputs (steer input, accel input, action)

    // normalize based on input steering

    // Store raw
    steerInput_ = input[0]; // steering

    // Store limit
    steerInputMin_ = -1.0f;
    steerInputMax_ = 1.0f;

    // Store normalized
    steerInputNorm_ = -0.5f+(steerInput_-steerInputMin_)/(steerInputMax_-steerInputMin_);

    // 0->1
    // We want output spread from -1.0 to 1.0f

    accelInput_ = input[1]; // acceleration

    // Store limit
    accelInputMin_ = 0.0f; // Set to 0, -1 is for reverse but it will normalize to 0.5
    accelInputMax_ = 1.0f;


    // Store normalized
    accelInputNorm_ = (accelInput_-accelInputMin_)/(accelInputMax_-accelInputMin_);

    action_ = input[2]; // action state

    // Store limit
    if (actionMax_ < action_) actionMax_ = action_;
    if (actionMin_ > action_) actionMin_ = action_;

    // Store normalized
    actionNorm_ = (action_-actionMin_)/(actionMax_-actionMin_);

    networkActor_ = actor;

    /*URHO3D_LOGDEBUGF("Agent Movement: Agent [%d] -> FFN Calculated [steerInput=%f, accelInput=%f, action=%f] -> REWARD %f",
                     agentController->agentIndex, steerInput, accelInput, action, agentController->getCurrentCompletionReward());*/
/*
    URHO3D_LOGDEBUGF("Agent Movement: Agent [%d] -> FFN Calculated [steerInputMin_=%f, steerInputMax_=%f,  steerInput=%f, accelInput=%f, action=%f] -> REWARD %f",
                     agentController->agentIndex, steerInputMin_, steerInputMax_, steerInputNorm_, accelInputNorm_, actionNorm_, agentController->getCurrentCompletionReward());
*/
}

void AgentMovement::stop() {
    velocity = Urho3D::Vector3(0, 0, 0);
    rotation = Urho3D::Quaternion(0, Urho3D::Vector3(0, 0, 1));
}