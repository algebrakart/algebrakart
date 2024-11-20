//
// C++ Implementation by Ajay Bhaga
//

#include <cstring>
#include <Urho3D/Math/Ray.h>
#include <Urho3D/IO/Log.h>
#include <Urho3D/Scene/Scene.h>
#include <Urho3D/Graphics/Light.h>
#include <Urho3D/Input/Input.h>
#include <Urho3D/UI/Font.h>
#include <Urho3D/Graphics/DebugRenderer.h>
#include <Urho3D/Core/Variant.h>

#include "sensor.h"
#include "evolution_manager.h"

// TODO: Add sensor types (multiple types can diversify the mutation process).

Sensor::Sensor(int index) {
    agentIndex = index;
}

Sensor::~Sensor() {

}

void Sensor::start() {
    show();
}

void Sensor::update(float timeStep) {

    // Update timers
    lastRaycast += timeStep;

    NetworkActor *actor = EvolutionManager::getInstance()->getNetworkActors()[agentIndex];
    Scene *scene_ = EvolutionManager::getInstance()->getNetworkActors()[agentIndex]->GetScene();
    Vehicle *vehicle = EvolutionManager::getInstance()->getNetworkActors()[agentIndex]->GetVehicle();
    Node *node = EvolutionManager::getInstance()->getNetworkActors()[agentIndex]->GetVehicle()->GetNode();

    if (!actor || !vehicle || !scene_ || !node)
        return;


    // Includes offset
    this->center = EvolutionManager::getInstance()->getNetworkActors()[agentIndex]->GetVehicle()->GetNode()->GetPosition() + this->offset;
    String name = EvolutionManager::getInstance()->getNetworkActors()[agentIndex]->GetUserName();

    Quaternion rotation = EvolutionManager::getInstance()->getNetworkActors()[agentIndex]->GetVehicle()->GetNode()->GetRotation();

    // Skip node during replication (with invalid origin values) -> this can cause fuzzy zero in Bullet Collisions
    if (this->center.Length() > 1999999.0f)
        return;


    // Calculate hit distance
    float hitDistance = 0.0;

    // Vehicle sensor ray test
    // Generate ray from sensor center to sensor direction by magnitude based on parameter called sensorStretch

    float sensorStretch = 180.0f;

    float hoodLength = 90.0f;
    Vector3 hoodOffset = EvolutionManager::getInstance()->getNetworkActors()[agentIndex]->GetVehicle()->GetNode()->GetRotation()*Vector3(0.0f, 0,hoodLength);
    // Start at vehicle position with offset
    Vector3 rayStart = this->center + hoodOffset;
    // Apply rotation of sensor direction against rotation of vehicle
    // No rotation
//    Quaternion rayDir = ((this->direction * sensorStretch)).Normalized();

    //rotation = Quaternion(rotation.x_, 0, 0);
    //tation = Quaternion(0, rotation.y_, 0);
    //rotation = Quaternion(0, 0 ,0);

    // No rotation
    //Quaternion rayDir = rotation*((this->direction * sensorStretch)).Normalized();

    Vector3 rayDir = rotation*(this->direction * sensorStretch);

    float a = rayDir.Normalized().Length();

    // Avoid fuzzy zero raycast
    if (a > 0) {

        Ray sensorRay(rayStart, rayDir.Normalized());
        float sensorRayLength = sensorStretch;
        PhysicsRaycastResult result;

        DebugRenderer *dbgRenderer = scene_->GetComponent<DebugRenderer>();

/*
        if (dbgRenderer) {

            Vector3 axisRot = rotation.Axis();
//            dbgRenderer->AddLine(sensorRay.origin_, sensorRay.origin_+(Vector3(0, axisRot.y_, 0)*150.0f), Color(Urho3D::Color::CYAN));
            //dbgRenderer->AddLine(sensorRay.origin_, sensorRay.origin_+(rotation.EulerAngles()*50.0f), Color(Urho3D::Color::YELLOW));

            // Sensor line
            dbgRenderer->AddLine(sensorRay.origin_, sensorRay.origin_+(sensorRay.direction_*sensorRayLength), Color(colour));


            //dbgRenderer->AddLine(posWS, posWS + node_->GetRight(), Color(1.0f, 0.0, 0.0));
            //dbgRenderer->AddLine(posWS, posWS + this->vehicle_->GetNode()->GetDirection() * 40.0f, Color::CYAN);
            //  dbgRenderer->AddLine(posWS, toTarget_, Color::YELLOW);


            btTransform trans;
            //vehicle_->GetBody()->getWorldTransform(trans);
            Vector3 posWS = ToVector3(trans.getOrigin());
            //Vector3 centerWS = ToQuaternion(trans.getRotation()) * localCenter;
            //posWS += centerWS;
            Matrix3x4 mat34(posWS, ToQuaternion(trans.getRotation()), 1.0f);


            //dbgRenderer->AddBoundingBox(bbox, mat34, color);

            //dbgRenderer->AddLine(posWS, posWS + (node_->GetUp()*100.0f), Color(1.0f, 0.0, 0.0));
            //dbgRenderer->AddLine(posWS, posWS + (node_->GetRight()*100.0f), Color(1.0f, 0.0, 0.0));


        }*/



        // Wait for ray cast time
        if (lastRaycast > RAYCAST_TIME_WAIT) {
            if (scene_) {
                if (scene_->GetComponent<PhysicsWorld>()) {
                    if (sensorRayLength == 0) sensorRayLength = sensorStretch;

                    if (sensorRayLength < 10) {
                        scene_->GetComponent<PhysicsWorld>()->RaycastSingle(result, sensorRay, sensorRayLength, 2);
                    } else {
                        // Faster performance on more than 10 metres raycast
                        scene_->GetComponent<PhysicsWorld>()->RaycastSingleSegmented(result, sensorRay, sensorRayLength,
                                                                                     1.0f);
                    }
                    // Reset timer for new ray cast
                    lastRaycast = 0;
                    if (result.body_) {
                        // Set hit sensor
                        setHit(true);
                        /* URHO3D_LOGINFOF("[Agent %d] %s -> Sensor::update: -> direction [%f,%f,%f] -> Hit Distance -> %f",
                                         agentIndex, name.CString(),
                                         this->direction.x_, this->direction.y_, this->direction.z_, result.distance_);
     */

                        if (dbgRenderer) {
                            // SHOW HIT
                            //     dbgRenderer->AddLine(result.position_, result.position_ + (result.normal_ * result.distance_),
                            //                          Color(1.0f, 1.0, 1.0));

                            hitDistance = result.distance_;
                        }


                    } else {

                        // NO HIT
                        /* URHO3D_LOGINFOF("%s -> Sensor::update: -> direction [%f,%f,%f] -> Hit Distance -> NO HIT.",
                                         name.CString(),
                                         this->direction.x_, this->direction.y_, this->direction.z_);*/
                    }

                    // SENSOR SYSTEM
                    // FORWARD
                    // LEFT [
                    // DOWN [0, 1, 0]




                    if (hitDistance < MIN_DIST) {
                        hitDistance = MIN_DIST;
                    }

                    // Transform to percent of max distance
                    this->output = hitDistance;

                    // Set position of sensor target (adjusted based on hit)
//    this->target = this->center + (this->direction * hitDistance);
                    // Set position of sensor target (fixed)
                    //this->target = this->center + (this->direction * 1.0f);
                }
            }
        }
    }

}

// Hides the visual representation of the sensor
void Sensor::hide() {
    visibility = false;
}

// Shows the visual representation of the sensor
void Sensor::show() {
    visibility = true;
}

const Urho3D::Vector3 &Sensor::getTarget() const {
    return target;
}

const Urho3D::Vector3 &Sensor::getOffset() const {
    return offset;
}

void Sensor::setOffset(const Urho3D::Vector3 &offset) {
    Sensor::offset = offset;
}

const Urho3D::Vector3 &Sensor::getCenter() const {
    return center;
}

void Sensor::setCenter(const Urho3D::Vector3 &center) {
    Sensor::center = center;
}

bool Sensor::isHit() const {
    return hit;
}

void Sensor::setHit(bool hit) {
    this->hit = hit;
}

long Sensor::getLastHit() const {
    return lastHit;
}

void Sensor::setLastHit(long lastHit) {
    this->lastHit = lastHit;
}

const Vector3 &Sensor::getColour() const {
    return colour;
}

void Sensor::setColour(const Vector3 &colour) {
    Sensor::colour = colour;
}

void Sensor::setDirection(const Vector3 &direction) {
    Sensor::direction = direction;
}

const Vector3 &Sensor::getDirection() const {
    return direction;
}
