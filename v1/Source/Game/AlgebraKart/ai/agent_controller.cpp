//
// C++ Implementation by Ajay Bhaga
//
// Modified Genetic Algorithm
// Based on design of Samuel Arzt (March 2017)
//

#include "../shared_libs.h"
#include "agent_fsm.h"
#include "../util/math_helper.h"
#include "agent_controller.h"


class EvolutionManager;

AgentController::AgentController(int index) {
    this->agentIndex = index;

    this->movement = std::shared_ptr<AgentMovement>(new AgentMovement(this));
    this->fsm = std::shared_ptr<AgentFSM>(new AgentFSM());

    int numSensors = 4;
    for (int i = 0; i < numSensors; i++) {
        Sensor *s = new Sensor(agentIndex);
        sensors.emplace_back(*s);
    }

    float agentHeight = 16.0f;
    float agentHood = 70.0f;
    // SENSOR #1: (GREEN) LEFT
    sensors[0].setOffset(Urho3D::Vector3(0, agentHeight, 0));
    //sensors[0].setDirection(Urho3D::Vector3(Urho3D::Vector3(1.0f, -0.2f, -0.5f)));
    sensors[0].setDirection(Vector3(1,0,0));
    sensors[0].setColour(Vector3(0.0, 1.0, 0.0));

    // SENSOR #2: (BLUE) UP
    sensors[1].setOffset(Urho3D::Vector3(0, agentHeight, 0));
    sensors[1].setDirection(Vector3(0,1,0));
    sensors[1].setColour(Vector3(0.0, 0.0, 1.0));
    // ** DETECTS

    // SENSOR #2: (RED) FRONT
    sensors[2].setOffset(Urho3D::Vector3(0, agentHeight, 0));
    sensors[2].setDirection(Vector3(0,0,1));
    sensors[2].setColour(Vector3(1.0, 0.0, 0.0));

    // SENSOR #3: (PURPLE) RIGHT
    sensors[3].setOffset(Urho3D::Vector3(0, agentHeight, 0));
    sensors[3].setDirection(Vector3(-1,0,0));
    sensors[3].setColour(Vector3(1.0, 0.0, 1.0));

    // Boolean flag detectors
    int numFlags = 3;
    for (int i = 0; i < numFlags; i++) {
        Flag *f = new Flag(agentIndex);
        flags.emplace_back(*f);
    }

    /*
     *  BOOLEAN FLAGS
        *  6: on-track -> flag
        *  7: unflipped -> flag
        *  8: velocity consistent -> flag
    */

    // FLAG #1: on-track flag
    flags[0].setFlag(false);
    // FLAG #2: un-flipped flag
    flags[1].setFlag(false);
    // FLAG #3: velocity consistent flag
    flags[2].setFlag(false);

    lastCurrSpdKm_ = 0;
    lastAvgSpdKm_ = 0;

    start();
 }

AgentController::~AgentController() {
}

void AgentController::awake() {

    // Cache components
    // movement -> CarMovement
    // spriteRender
    // sensors
}

void AgentController::start() {

    // When agent controller hits wall -> die action
//    this->movement->hitWall += die;

    auto start = std::chrono::high_resolution_clock::now();
    restart();
}

void AgentController::restart() {

    timeSinceLastCheckpoint = 0.0f;
    timeSinceNoWheelContact = 0.0f;
    distanceCovered_ = 0.0f;
    startTime = std::chrono::high_resolution_clock::now().time_since_epoch().count();

    alive_ = true;

    for (int i = 0; i < this->sensors.size(); i++) {
        sensors[i].show();
    }

    if (EvolutionManager::getInstance()->getAgents().size() > 0) {
        EvolutionManager::getInstance()->getAgents()[agentIndex]->reset();
        setCurrentCompletionReward(0);
    }
}

void AgentController::update(float duration) {
    timeSinceLastCheckpoint += duration;

    std::shared_ptr<NetworkActor> actor = EvolutionManager::getInstance()->getNetworkActors()[agentIndex];
    Scene *scene_ = EvolutionManager::getInstance()->getNetworkActors()[agentIndex]->GetScene();
    Vehicle *vehicle = EvolutionManager::getInstance()->getNetworkActors()[agentIndex]->GetVehicle();
    Node *node = EvolutionManager::getInstance()->getNetworkActors()[agentIndex]->GetVehicle()->GetNode();

    if (!actor || !vehicle || !scene_ || !node)
        return;


    // Auto-enter, if not entered vehicle
    if (!actor->entered_)
        actor->EnterVehicle();

    // Load distance covered
    distanceCovered_ = actor->GetVehicle()->GetRaycastVehicle()->GetDistanceOnGround();
    // Get readings from sensors
    double *ffnInput = new double[sensors.size()+flags.size()];

    // Load 4 sensors data into ffn input
    for (int i = 0; i < sensors.size(); i++) {
        sensors[i].update(duration);
        ffnInput[i] = sensors[i].output;
    }

    // Calculate detector flags
    lastAvgSpdKm_ = (lastAvgSpdKm_*0.4f)+(lastCurrSpdKm_*0.1f)+(actor->vehicle_->GetRaycastVehicle()->GetSpeedKm()*0.5f);
    lastCurrSpdKm_ = actor->vehicle_->GetRaycastVehicle()->GetSpeedKm();


    // FLAG #1: on-track flag
    int numWheelsContact = actor->vehicle_->GetRaycastVehicle()->getNumWheelsContact();
    // Calculate time for no wheel contact
    if (numWheelsContact == 0) {
        timeSinceNoWheelContact += duration;
    }


    // Eliminate agent which has had no wheel contact for too long
    if (timeSinceNoWheelContact > NO_WHEEL_CONTACT_DIE_TIME) {
        //die();
    }

    if (numWheelsContact > 0) {
        // Reset contact, no contact
        timeSinceNoWheelContact = 0;
    }

    flags[0].setFlag((numWheelsContact > 0));

    // FLAG #2: un-flipped flag
    float TIME_TO_CLASSIFY_FLIP = 5.0f;

    // 5 seconds no contact is considered flipped
    flags[1].setFlag(!(timeSinceNoWheelContact > TIME_TO_CLASSIFY_FLIP));

    // FLAG #3: velocity consistent flag
    flags[2].setFlag((lastAvgSpdKm_ > 1.0f));



    // Load 3 flag data into ffn input
    for (int i = 0; i < flags.size(); i++) {
        // Use sensors size as offset of start index for flags
        ffnInput[sensors.size()+i] = flags[i].isTrue();
    }

    Vector3 actorPos = actor->GetVehicle()->GetNode()->GetPosition();
    Vector3 actorVel = actor->GetVehicle()->GetBody()->GetLinearVelocity();

    int k = sensors.size();


    float sensorAMin = 90.0f;
    float sensorAMax = 400.0f;
    float sensorARange = sensorAMax-sensorAMin;


    float sensorBMin = 20.0f;
    float sensorBMax = 85.0f;
    float sensorBRange = sensorBMax-sensorBMin;

    // SENSOR A model
    float o1 = (ffnInput[0]-sensorAMin)/sensorARange; // green (left) -> more hit distance > 100.0f
    float o3 = (ffnInput[2]-sensorAMin)/sensorARange; // red (front) -> more hit distance > 100.0f
    float o4 = (ffnInput[3]-sensorAMin)/sensorARange; // purple (right) -> more hit distance > 100.0f

    // SENSOR B model
    float o2 = (ffnInput[1]-sensorBMin)/sensorBRange; // blue (up) -> low hit distance? -> < 85.0f


    float o5 = 0;
    if (distanceCovered_ > 0) o5 = 1-(1/distanceCovered_); // How far has the vehicle travelled?

    // Weights
    float w1 = 0.2f; // green
    float w2 = 0.2f; // blue
    float w3 = 0.2f; // red
    float w4 = 0.2f; // purple
    float w5 = 0.2f; // distance meter
    int numWeights = 5;

    // Update reward based on sensors data

    // UP sensor wants vehicle to have a hit (low hit distance) -> touching ground
    // FRONT, LEFT and RIGHT sensors want to have high hit distance to avoid hitting walls



    float rewardRate = 0.02f;
    float reward = ((o1*w1) + (1.0f-(o2*w2)) + (o3*w3) + (o4*w4) + (o5*w5))*duration*rewardRate;
    setCurrentCompletionReward(reward+getCurrentCompletionReward());


    lastRewardLog_ += duration;
    if (lastRewardLog_ > 4.0f) {
        lastRewardLog_ = 0.0f;
/*        for (int i = 0; i < sensors.size(); i++) {
            URHO3D_LOGDEBUGF("[actor %d] -> sensorOutput[%d]=%f, distanceCovered_=%f, reward=%f", (agentIndex+1), i, ffnInput[i], distanceCovered_, getCurrentCompletionReward());
        }*/
    }

    //float MAX_REWARD = 65000.0f;
    float MAX_REWARD = 2000.0f;

    // Check for reward completion condition
    if (getCurrentCompletionReward() > MAX_REWARD && timeSinceLastCheckpoint > 5.0f) {
//        URHO3D_LOGDEBUGF("WINNER! [actor %d] -> reward=%f", (agentIndex+1), getCurrentCompletionReward());

        // TODO: Disabled win
     //   win();
    }

    // Process sensor inputs through ffn
    double *controlInputsFFN = EvolutionManager::getInstance()->getAgents()[agentIndex]->ffn->processInputs(ffnInput);


    // SENSOR #1: (GREEN) LEFT
    // SENSOR #2: (BLUE) UP
    // SENSOR #2: (RED) FRONT
    // SENSOR #3: (PURPLE) RIGHT



    float sensorSteer = 0;
    float sensorSteerTarget = 0;
    float sensorAccel = 0;

    bool leftClear = true;
    bool rightClear = true;
    bool sideClear = true;

    // Left
    if (sensors[0].isHit()) {
        // Steer right
        sideClear = false;
        leftClear = false;
    }

    // Up
    if (sensors[1].isHit()) {
        // Nothing
    }


    // Right
    if (sensors[3].isHit()) {
        // Steer left
        sideClear = false;
        rightClear = false;
    }


    // Front - can override steering
    if (sensors[2].isHit()) {
        // Slow down
        sensorAccel = 0.0004f;

        // Use steer target if chosen
        if (steerSet_)
            sensorSteerTarget = steerChoice_;

        // Set steer based on clearance
        if ((leftClear) && (!rightClear)) {
            // Turn left and drive
            sensorSteerTarget = -1;
            sensorAccel = 0.001f;
        }

        if ((!leftClear) && (rightClear)) {
            // Turn left and drive
            sensorSteerTarget = 1;
            sensorAccel = 0.001f;
        }

        // For longer than 2 second block
        if ((frontHitTime_ > 2.0f) && (!steerSet_)) {
            // Choose a steer
    //        steerChoice_ = Random(-1.0f, 1.0f);
     //       steerSet_ = true;
        }
        frontHitTime_ += duration;

    } else {

        // ALL CLEAR IN FRONT

        // Reset set flags
        frontHitTime_ = 0;
        steerSet_ = false;
        steerChoice_ = 0;

        // Accel
        sensorAccel = Random(0.0001f, 0.001f);
        //sensorAccel = 1.0f;

        if (sideClear) {
            // Clear side, set center steering
            sensorSteerTarget = 0;
        }

        // Set steer based on clearance
        if ((leftClear) && (!rightClear)) {
            // Turn left and drive
            sensorSteerTarget = -1;
        }

        if ((!leftClear) && (rightClear)) {
            // Turn left and drive
            sensorSteerTarget = 1;
        }

    }

    /// APPLY STEERING ///

    // On side clear, drive with steer gradually reducing
//    sensorSteer = sensorSteer*0.5f + sensorSteerTarget*0.5f;

    //sensorAccel = 1.0f;
    double controlInputsSensors[3];
    float accelMult = 1.00f;
    controlInputsSensors[0] = sensorSteer; // steering
    controlInputsSensors[1] = sensorAccel * accelMult; // acceleration
    //controlInputsSensors[0] = steer; // action state

    // Store raw
 //   steerInput_ = input[0]; // steering
 //   accelInput_ = input[1]; // acceleration

    double controlInputs[3];
    float ffnW = 0.0f;
    float senW = 1.0f;

    // Calculate weight sum control inputs
    controlInputs[0] = controlInputsFFN[0]*ffnW + controlInputsSensors[0]*senW;
    controlInputs[1] = controlInputsFFN[1]*ffnW + controlInputsSensors[1]*senW;
    controlInputs[2] = controlInputsFFN[2]*ffnW + controlInputsSensors[2]*senW;

    controlInputs[1] = 17.0f;


    // Resultant data from sensor processing is used for controlling the agent movement
    // Apply inputs to agent movement (two dimension array)
    this->movement->setInputs(controlInputs, actor);
    this->movement->update(duration);

    // Agent timed out, death by timeout
    if (timeSinceLastCheckpoint > MAX_CHECKPOINT_DELAY) {
        //die();
    }
}

void AgentController::win() {

    for (int i = 0; i < EvolutionManager::getInstance()->getAgents().size(); i++) {
        if (i != agentIndex) {
            EvolutionManager::getInstance()->getAgents()[i]->kill();
        }
    }

    // Die to end the epoch
    die();
}

void AgentController::die() {

    // Only kill agents when allowed
    if (dieAllowed_) {
        alive_ = false;
        this->movement->stop();

        for (int i = 0; i < sensors.size(); i++) {
            sensors[i].hide();
        }

        EvolutionManager::getInstance()->getAgents()[agentIndex]->kill();
    }
}

void AgentController::checkpointCaptured() {
    timeSinceLastCheckpoint = 0;
}

float AgentController::getCurrentCompletionReward() {
    if (EvolutionManager::getInstance()->getAgents()[agentIndex])
        return EvolutionManager::getInstance()->getAgents()[agentIndex]->genotype->evaluation;
    else
        return -1.0;
}

void AgentController::setCurrentCompletionReward(float reward) {

    if (EvolutionManager::getInstance()) {
        if (EvolutionManager::getInstance()->getAgents().size() >= agentIndex) {
            if (EvolutionManager::getInstance()->getAgents()[agentIndex])
                EvolutionManager::getInstance()->getAgents()[agentIndex]->genotype->evaluation = reward;
        }
    }
}

float AgentController::getTimeSinceLastCheckpoint() const {
    return timeSinceLastCheckpoint;
}

const std::vector<Sensor> &AgentController::getSensors() const {
    return sensors;
}

const std::vector<Flag> &AgentController::getFlags() const {
    return flags;
}

float AgentController::getLastCurrSpdKm() const {
    return lastCurrSpdKm_;
}

float AgentController::getLastAvgSpdKm() const {
    return lastAvgSpdKm_;
}

float AgentController::getTimeSinceNoWheelContact() const {
    return timeSinceNoWheelContact;
}
