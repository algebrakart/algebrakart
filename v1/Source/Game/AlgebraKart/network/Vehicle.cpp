
#include "Urho3D/Urho3D.h"

#include "Urho3D/Physics/CollisionShape.h"
#include "Urho3D/Physics/Constraint.h"
#include "Urho3D/Core/Context.h"
#include "Urho3D/Graphics/Material.h"
#include "Urho3D/Graphics/Model.h"
#include "Urho3D/Physics/PhysicsEvents.h"
#include "Urho3D/Physics/PhysicsWorld.h"
#include "Urho3D/Physics/PhysicsUtils.h"
#include "Urho3D/Resource/ResourceCache.h"
#include "Urho3D/Physics/RigidBody.h"
#include "Urho3D/Scene/Scene.h"
#include "Urho3D/Graphics/StaticModel.h"
#include "Urho3D/Graphics/DebugRenderer.h"
#include "Urho3D/Graphics/ParticleEmitter.h"
#include "Urho3D/Graphics/ParticleEffect.h"
#include "Urho3D/Audio/SoundSource3D.h"
#include "Urho3D/Audio/Sound.h"
#include "Urho3D/IO/Log.h"


#include "Vehicle.h"
#include <Urho3D/Core/Context.h>
#include <Urho3D/Graphics/DebugRenderer.h>
#include <Urho3D/Graphics/DecalSet.h>
#include <Urho3D/Graphics/Material.h>
#include <Urho3D/Graphics/Model.h>
#include <Urho3D/Graphics/ParticleEffect.h>
#include <Urho3D/Graphics/ParticleEmitter.h>
#include <Urho3D/Graphics/StaticModel.h>
#include <Urho3D/IO/Log.h>
#include <Urho3D/Physics/CollisionShape.h>
#include <Urho3D/Physics/Constraint.h>
#include <Urho3D/Physics/PhysicsEvents.h>
#include <Urho3D/Physics/PhysicsWorld.h>
#include "../../../../Source/Urho3D/Physics/RaycastVehicleBase.h"
#include <Urho3D/Resource/ResourceCache.h>
#include <Urho3D/Scene/Scene.h>
#include <Urho3D/Physics/RigidBody.h>


#include "AlgebraKart/WheelTrackModel.h"
#include "AlgebraKart/SFXModel.h"
#include "AlgebraKart/types.h"
#include "AlgebraKart/Missile.h"

#include "NetworkActor.h"

#include "Urho3D/ThirdParty/SDL/SDL_log.h"
#include "Urho3D/DebugNew.h"
#include "../../../ThirdParty/Bullet/src/BulletDynamics/Vehicle/btWheelInfo.h"

#include <vector>
#include <string>


std::vector<std::string> driveAudioEffect_ = {
        "drive/v1-engine-start.ogg",
        "drive/v1-engine-loop.ogg",
        "drive/v1-engine-rev.ogg",
        "drive/v1-engine-skid1.ogg",
        "drive/v1-engine-skid2.ogg",
        "drive/v1-engine-skid3.ogg",
        "drive/v1-engine-brake.ogg",
        "drive/v1-engine-boost.ogg"
};


//=============================================================================
// VEHICLE
//=============================================================================
#define CUBE_HALF_EXTENTS       1.9f

#define MIN_SLOW_DOWN_VEL       15.0f
#define MIN_STICTION_VEL        5.0f
#define MIN_BRAKE_FORCE         2.0f
#define MIN_IDLE_RPM            1000.0f

#define MIN_DOWN_FORCE          10.0f
#define MAX_DOWN_FORCE          10e4f
//#define MAX_ANGULAR_VEL_LIMIT   80.0f
#define MAX_ANGULAR_VEL_LIMIT   300.0f
//10.0f
//#define LINEAR_VEL_LIMIT_MPH    750.0f
//#define LINEAR_VEL_LIMIT_MPH    1450.0f
#define LINEAR_VEL_LIMIT_MPH    4500.0f
//140.0f -> 140mph->225kmh
#define VEL_TO_MPH              (3.6f/1.60934f)
#define MAX_LINEAR_VEL_LIMIT    (LINEAR_VEL_LIMIT_MPH/VEL_TO_MPH)

#define AUDIO_FIXED_FREQ_44K    44100.0f
#define MIN_SHOCK_IMPACT_VEL    3.0f
#define MAX_SKID_TRACK_SPEED    70.0f
#define MIN_SIDE_SLIP_VEL       4.0f

#define MIN_WHEEL_RPM           0.60f
#define MAX_WHEEL_RPM           0.75f
#define MIN_WHEEL_RPM_AIR       0.89f
#define MAX_WHEEL_RPM_AIR       0.90f

#define MIN_PEELOUT_VAL_AT_ZER0 0.8f
//#define MAX_REAR_SLIP           0.6f

#define MAX_REAR_SLIP           0.04f


//=============================================================================
//=============================================================================
Vehicle::Vehicle(Context* context)
    : ClientObj( context )
    , steering_( 0.0f )
{

    SetUpdateEventMask(USE_FIXEDUPDATE | USE_POSTUPDATE);

    enableControls_ = false; // Disable until the network actor takes control

    engineForce_ = 0.0f;
    brakingForce_ = 50.0f;
    vehicleSteering_ = 0.0f;
    //wheelRadius_ = 3.64f;
//    wheelRadius_ = 0.1456f;
    wheelRadius_ = 1.456f;
    suspensionRestLength_ = 0.6f;
    wheelWidth_ = 0.8f;
    suspensionStiffness_ = 18.0f;
    suspensionDamping_ = 2.0f;
    suspensionCompression_ = 4.0f;
//    wheelFriction_ = 370.0f;
    wheelFriction_ = 480.0f;
    rollInfluence_ = 0.12f;
    emittersCreated = false;

    // Working
    m_fmaxEngineForce = 22000.0f;//950.f;
  //  m_fmaxEngineForce = 4200.0f;//5400.0f;//950.f;

    m_fmaxBreakingForce = 800.f;

    m_fVehicleSteering = 0.0f;
    m_fsteeringIncrement = 0.030f;
    m_fsteeringClamp = 0.5f;
    //m_fwheelRadius = 0.4f;
    //m_fwheelWidth = 0.4f;
    //m_fwheelFriction = 2.2f;

    //m_fsuspensionStiffness = 20.0f;
    //m_fsuspensionDamping = 2.0f;
    //m_fsuspensionCompression = 5.0f;
    //m_frollInfluence = 0.1f;
    //m_fsuspensionRestLength = 0.6f;
    m_fsideFrictionStiffness = 0.5f;

    // skid
    m_fYAngularVelocity = 1.0f;
    m_fMaxSteering = 0.5f;
    numWheelContacts_ = 0;

    currentAcceleration_ = 0.0f;

    // gear
    downShiftRPM_ = 4500.0f;
    upShiftRPM_   = 7500.0f;
    curGearIdx_   = 0;
    curRPM_       = MIN_IDLE_RPM;

    // most vehicle dynamics have complicated gear ratio equations, gear shifting formulas, etc.
    // but it all comes down to at speeds to shift gear
    // speed shown is in mph - has no effect on whether the speed is displayed in KmH or MPH
    gearShiftSpeed_.Push(50.0f);
    gearShiftSpeed_.Push(70.0f);
    gearShiftSpeed_.Push(90.0f);
    gearShiftSpeed_.Push(110.0f);
    gearShiftSpeed_.Push(130.0f);
    numGears_ = gearShiftSpeed_.Size();

    // sound
    playAccelerationSoundInAir_ = true;

    steerIndex_ = 0;

    boosterLevel_ = 1.0f; // No effect
    boosterTime_ = 0;

}

//=============================================================================
//=============================================================================
Vehicle::~Vehicle()
{
    Kill();
}

void Vehicle::Kill() {
    SetUpdateEventMask(USE_NO_EVENT);

    if (raycastVehicle_) {
        raycastVehicle_->SetEnabled(false);
        URHO3D_LOGINFOF("**** DESTROYING CLIENT RAYCAST VEHICLE OBJECT -> %d", this->id_);
        raycastVehicle_->Remove();
    }

    if (hullColShape_) {
        URHO3D_LOGINFOF("**** DESTROYING CLIENT VEHICLE COLLISION SHAPE OBJECT -> %d", this->id_);
        hullColShape_->Remove();
    }

    if (hullObject_) {
        URHO3D_LOGINFOF("**** DESTROYING CLIENT VEHICLE MODEL OBJECT -> %d", this->id_);
        hullObject_->Remove();
    }

    if (!wheels_.Empty()) {
        for (int i : wheels_.Keys()) {
            if (wheels_[i]) {
                wheels_[i]->Remove();
                URHO3D_LOGINFOF("**** DESTROYING CLIENT VEHICLE WHEEL -> %d", i);
            }
        }
    }

    if (node_) {
        node_->Remove();
    }
}


//=============================================================================
//=============================================================================
void Vehicle::RegisterObject(Context* context)
{
    context->RegisterFactory<Vehicle>();

    URHO3D_COPY_BASE_ATTRIBUTES(ClientObj);

    ClientObj::RegisterObject(context);



    //URHO3D_ATTRIBUTE("Vehicle Name", String, name_, String::EMPTY, AM_DEFAULT | AM_NET);
    //URHO3D_ATTRIBUTE("Name", String, name_, String::EMPTY, AM_DEFAULT | AM_NET);
    //URHO3D_ATTRIBUTE("Position", Vector3, position_, Vector3(0,0,0), AM_DEFAULT | AM_NET | AM_LATESTDATA);
    URHO3D_ATTRIBUTE("Steering", float, steering_, 0.0f, AM_DEFAULT | AM_NET);
    URHO3D_ATTRIBUTE("WheelContactTime", float, wheelContactTime_, 0.0f, AM_DEFAULT | AM_NET);
    URHO3D_ATTRIBUTE("WheelContactNum", int, numWheelContacts_, 0, AM_DEFAULT | AM_NET);
}

//=============================================================================
//=============================================================================

void Vehicle::CreateEmitter(Vector3 place)
{
    auto* cache = GetSubsystem<ResourceCache>();
    Node* emitter = GetScene()->CreateChild();
    emitter->SetWorldPosition(node_->GetWorldPosition() + node_->GetWorldRotation() * place + Vector3(0, -wheelRadius_, 0));
    auto* particleEmitter = emitter->CreateComponent<ParticleEmitter>();
    particleEmitter->SetEffect(cache->GetResource<ParticleEffect>("Particle/Dust.xml"));
    particleEmitter->SetEmitting(false);
    particleEmitterNodeList_.Push(emitter);
    emitter->SetTemporary(true);
}

void Vehicle::CreateSfx(Vector3 place) {
    auto *cache = GetSubsystem<ResourceCache>();
    float wheelDim = m_fwheelRadius*2.0f;
    float wheelThickness = 1.0f;
    Model *tireModel = cache->GetResource<Model>("Offroad/Models/tire.mdl");
    BoundingBox tirebbox = tireModel->GetBoundingBox();
    float tireScaleXZ = wheelDim/tirebbox.Size().x_;
    const Color LtBrown(0.972f,0.780f,0.412f );
    Model *trackModel = cache->GetResource<Model>("Offroad/Models/wheelTrack.mdl");

    for ( int i = 0; i < raycastVehicle_->GetNumWheels(); i++ ) {
        // track
        Node * trackNode = GetScene()->CreateChild();;
        wheelTrackList_[i] = trackNode->CreateComponent<WheelTrackModel>();
        wheelTrackList_[i]->SetModel(trackModel->Clone());
        wheelTrackList_[i]->SetMaterial(cache->GetResource<Material>("Offroad/Models/Materials/TireTrack.xml"));

        wheelTrackList_[i]->SetParentNode(node_);
        wheelTrackList_[i]->SetColor(LtBrown);
        wheelTrackList_[i]->SetWidth(tirebbox.Size().y_);
        wheelTrackList_[i]->ValidateBufferElements();
    }
}

void Vehicle::CreateSkidTrack(Vector3 place) {
    auto *cache = GetSubsystem<ResourceCache>();
    float wheelDim = m_fwheelRadius*2.0f;
    float wheelThickness = 1.0f;
    Model *tireModel = cache->GetResource<Model>("Offroad/Models/tire.mdl");
    BoundingBox tirebbox = tireModel->GetBoundingBox();
    float tireScaleXZ = wheelDim/tirebbox.Size().x_;
    const Color LtBrown(0.972f,0.780f,0.412f );
    Model *trackModel = cache->GetResource<Model>("Offroad/Models/wheelTrack.mdl");

    for ( int i = 0; i < raycastVehicle_->GetNumWheels(); i++ ) {
        // track
        Node * trackNode = GetScene()->CreateChild();;
        wheelTrackList_[i] = trackNode->CreateComponent<WheelTrackModel>();
        wheelTrackList_[i]->SetModel(trackModel->Clone());
        wheelTrackList_[i]->SetMaterial(cache->GetResource<Material>("Offroad/Models/Materials/TireTrack.xml"));

        wheelTrackList_[i]->SetParentNode(node_);
        wheelTrackList_[i]->SetColor(LtBrown);
        wheelTrackList_[i]->SetWidth(tirebbox.Size().y_);
        wheelTrackList_[i]->ValidateBufferElements();
    }
}

/// Applying attributes
void Vehicle::ApplyAttributes()
{
    if (node_) {
        raycastVehicle_ = node_->GetOrCreateComponent<RaycastVehicleBase>();
        if (emittersCreated)
            return;
        for (const auto &connectionPoint: connectionPoints_) {
            CreateEmitter(connectionPoint);




        }
        emittersCreated = true;
    }
}

#define PICKUP_TIME_FOR_N2O_DEPLETION 15.0f

void Vehicle::FixedUpdate(float timeStep)
{
    float newSteering = 0.0f;
    float accelerator = 0.0f;
    bool brake = false;
    bool flip = false;
    bool fire = false;

    if (node_) {
        auto *vehicle = node_->GetComponent<RaycastVehicleBase>();
        String vehicleName = node_->GetName().CString();

        if (boosterLevel_ > 1.0f) {
            boosterTime_ += timeStep;
        }

        if (boosterTime_ > PICKUP_TIME_FOR_N2O_DEPLETION) {
            boosterTime_ = 0;
            boosterLevel_ = 1.0f; // Default, no boost
        }

        if (enableControls_) {

            //URHO3D_LOGDEBUGF("**VEHICLE CONTROLS** -> %l", controls_.buttons_);
            float steerLevel = controls_.extraData_["steerLevel"].GetFloat();
            newSteering = steerLevel;

            // Read controls generate vehicle control instruction
            if (controls_.buttons_ & NTWK_CTRL_LEFT) {
//                newSteering = -1.0f;
                //URHO3D_LOGDEBUG(vehicleName + " -> **NTWK_CTRL_LEFT**");
            }
            if (controls_.buttons_ & NTWK_CTRL_RIGHT) {
  //              float steerLevel = controls_.extraData_["steerLevel"].GetFloat();
//                newSteering = steerLevel;
//                newSteering = 1.0f;

                //URHO3D_LOGDEBUG(vehicleName + " -> **NTWK_CTRL_RIGHT**");
            }

            if (controls_.buttons_ & NTWK_CTRL_FORWARD) {
//                accelerator = 1.0f;
                float accelLevel = controls_.extraData_["accelLevel"].GetFloat();
                accelerator = accelLevel;

                //URHO3D_LOGDEBUG(vehicleName + " -> **ACCELERATE**");
            }
            if (controls_.buttons_ & NTWK_CTRL_BACK) {
                accelerator = -0.5f;
            }
            if (controls_.buttons_ & CTRL_SPACE) {
                brake = true;
            }

            if (controls_.buttons_ & NTWK_CTRL_ENTER) {
                // FLIP CAR
                //Flip(timeStep);

                // Do nothing for enter right now
            }

            if (controls_.buttons_ & NTWK_CTRL_FIRE) {
                // FIRE
                fire = true;
                URHO3D_LOGDEBUGF("%s -> FIRE = %l", vehicleName.CString(), controls_.buttons_);
            }
        }

        UpdateGear();

        if (newSteering != 0.0f)
        {
            SetSteering(GetSteering() * 0.95f + newSteering * 0.0081f);
        }
        else
        {
            SetSteering(GetSteering() * 0.8f + newSteering * 0.2f);
        }
        // Set front wheel angles
        vehicleSteering_ = steering_;
        int wheelIndex = 0;
        vehicle->SetSteeringValue(wheelIndex, vehicleSteering_);
        wheelIndex = 1;
        vehicle->SetSteeringValue(wheelIndex, vehicleSteering_);

        ApplyEngineForces(accelerator, brake);

        // do this right after applyEngineForce and before applying other forces
        if ( ApplyStiction(newSteering, accelerator, brake) )
        {
            return;
        }

        int cnt = 0;
        for (int i = 0; i < numWheels_; i++) {
            if (vehicle->WheelIsGrounded(i)) {
                cnt++;
            }
        }

        // Set numbers of wheels in contact (this parameter drives others)
        numWheelContacts_ = cnt; //vehicle->getNumWheelsContact();

        if (cnt > 0) {
            wheelContactTime_ += timeStep;
        }

//        wheelContactBuffer_.Clear();
        // Push contacts into buffer
        wheelContactBuffer_.Push(numWheelContacts_);


        int contactSum = 0;
        int windowSize = Min(4, wheelContactBuffer_.Size());
        for (int i = 0; i < windowSize; i++) {
            int contactNum = wheelContactBuffer_[i];
            contactSum += contactNum;
            // TODO: Not getting stable contact
        }
        float contactAvg = contactSum / wheelContactBuffer_.Size();

        if (contactAvg > 0) {
            wheelContactTime_ += timeStep;
        } else {

            if (contactAvg < 0.00001f) {
                wheelContactTime_ = 0;
            }
        }

        if (wheelContactBuffer_.Size() > 6) {
            wheelContactBuffer_.Resize(2);
            //wheelContactBuffer_.Clear();
        }

        ApplyDownwardForce();

        ApplyAntiRollBar();

        ApplyDragBrake();

        LimitLinearAndAngularVelocity();

        AutoCorrectPitchRoll();

        UpdateDrift();


        if (fire) {


            // Retrieve rigid body
            //RigidBody *body = node_->GetComponent<RigidBody>(true);
            //body->ApplyImpulse(Vector3(12.0, 40.0, 0.0));
            //body->ApplyForce(Vector3(0.0, 100.0, 2.0));
            //URHO3D_LOGDEBUG("**VEHICLE fire -> body->ApplyForce");
            fire = false;
        }


      /*  if (engineForce_ > 0.0) {
            URHO3D_LOGDEBUGF("**VEHICLE engineForce -> %f, brake = %d", engineForce_, brake);
        }
        */

//    URHO3D_LOGDEBUGF("**VEHICLE STATUS:  -> %d", vehicle->GetWheelSkidInfoCumulative());


        for (int i = 0; i < vehicle->GetNumWheels(); i++) {
            if (brake) {
                vehicle->SetBrake(i, brakingForce_);
            } else {
                vehicle->SetBrake(i, 0.0f);
            }
        }
    }

    // DEBUG DRAW
    DebugDraw();
}

void Vehicle::PostUpdate(float timeStep)
{
    if (node_) {
        auto *vehicle = node_->GetComponent<RaycastVehicleBase>();
        auto *vehicleBody = node_->GetComponent<RigidBody>();
        auto *vehicleCollShape = node_->GetComponent<CollisionShape>();
        auto *debug = GetScene()->GetComponent<DebugRenderer>();
        if (debug) {

        Vector3 velocity = vehicleBody->GetLinearVelocity();
        Vector3 accel = (velocity - prevVelocity_) / timeStep;
        float planeAccel = Vector3(accel.x_, 0.0f, accel.z_).Length();
        prevVelocity_ = velocity;
        float boundSize;

        Vector3 pos = vehicleBody->GetPosition();
        Vector3 dimensions =  Vector3(1.0f, 1.0f, 1.0f);



            Node *lightNode = node_->GetChild("Lights");
            // Update vehicle head lamp lighting
            Light *light = lightNode->GetComponent<Light>();


            float rpmLight = curRPM_ / 86000.0f;
            if (rpmLight > 0.8f) rpmLight = 0.8f;
            light->SetBrightness(0.2f + (rpmLight));
            //light->SetBrightness(0.7f);


            // Store the raycast vehicle body
            body_ = node_->GetComponent<RigidBody>();
            body_->DrawDebugGeometry(debug, true);

            // Apply normal force relative to vehicle
            // small arbitrary multiplier
            const float velocityMultiplyer = 0.92f;
            //const float velocityMultiplyer = 1.24f;
            Vector3 downNormal = node_->GetUp() * -1.0f;
            float velocityMag = raycastVehicle_->GetBody()->GetLinearVelocity().LengthSquared() * velocityMultiplyer;
            velocityMag = Clamp( velocityMag, MIN_DOWN_FORCE, MAX_DOWN_FORCE );
            Vector3 force = velocityMag * downNormal;
            raycastVehicle_->GetBody()->ApplyForce( force  );



            // Apply normal force relative to track, i.e. wheel contact normal

            Vector3 normPosWheel0 = raycastVehicle_->GetContactPosition(0);
            Vector3 normPosWheel1 = raycastVehicle_->GetContactPosition(1);
            Vector3 normPosWheel2 = raycastVehicle_->GetContactPosition(2);
            Vector3 normPosWheel3 = raycastVehicle_->GetContactPosition(3);

            Vector3 normWheel0 = -raycastVehicle_->GetContactNormal(0);
            Vector3 normWheel1 = -raycastVehicle_->GetContactNormal(1);
            Vector3 normWheel2 = -raycastVehicle_->GetContactNormal(2);
            Vector3 normWheel3 = -raycastVehicle_->GetContactNormal(3);


            float pushMag = 10.0f + raycastVehicle_->GetBody()->GetLinearVelocity().LengthSquared() * 250.0f + raycastVehicle_->GetRPM()*100.0f;
            pushMag = Clamp( pushMag, MIN_DOWN_FORCE, MAX_DOWN_FORCE );

            //normWheel0*pushMag
            // Force line
            /*
            debug->AddLine(normPosWheel0,
                           normPosWheel0 + (normWheel0*pushMag), Color(Urho3D::Color::BLACK));


            // Force line
            debug->AddLine(normPosWheel1,
                           normPosWheel1 + (normWheel1*pushMag), Color(Urho3D::Color::WHITE));

             */

            bool groundFL = false;
            bool groundFR = false;
            bool groundBL = false;
            bool groundBR = false;
            bool wheelHit = false;


            if (raycastVehicle_) {

                if (raycastVehicle_->GetNumWheels() == 4) {


                    for (int i = 0; i < numWheels_; ++i) {
                        btWheelInfo whInfo = raycastVehicle_->GetWheel(i);

                        if (raycastVehicle_->WheelIsGrounded(i)) {

                            switch (i) {
                                case 0:
                                {
                                    // FRONT LEFT
                                    groundFL = true;
                                    travelFL = whInfo.m_raycastInfo.m_suspensionLength/whInfo.getSuspensionRestLength();
                                    break;
                                }

                                case 1:
                                {
                                    // FRONT RIGHT
                                    groundFR = true;
                                    travelFR = whInfo.m_raycastInfo.m_suspensionLength/whInfo.getSuspensionRestLength();
                                    break;
                                }

                                case 2:
                                {
                                    // BACK LEFT
                                    groundBL = true;
                                    travelBL = whInfo.m_raycastInfo.m_suspensionLength/whInfo.getSuspensionRestLength();
                                    break;
                                }

                                case 3:
                                {
                                    // BACK RIGHT
                                    groundBR = true;
                                    travelBR = whInfo.m_raycastInfo.m_suspensionLength/whInfo.getSuspensionRestLength();
                                    break;
                                }
                            }


                        } // End of Grounded wheel


                    } // End of wheel loop


                    // Apply force at front
                    float antiRollForceF = (travelFL - travelFR) * antiRoll;

                    if (groundFL) {
                        Vector3 force = Vector3(0, raycastVehicle_->GetWheel(0).m_worldTransform.getOrigin().y() * -antiRollForceF, 0);
                        // Front wheel
                        debug->AddLine(Vector3(raycastVehicle_->GetWheel(0).m_worldTransform.getOrigin()),
                                       Vector3(raycastVehicle_->GetWheel(0).m_worldTransform.getOrigin()) + (force), Color(Urho3D::Color::BLUE));
                    }

                    if (groundFR) {
                        Vector3 force = Vector3(0, raycastVehicle_->GetWheel(1).m_worldTransform.getOrigin().y() * antiRollForceF, 0);
                        // Front wheel
                        debug->AddLine(Vector3(raycastVehicle_->GetWheel(1).m_worldTransform.getOrigin()),
                                       Vector3(raycastVehicle_->GetWheel(1).m_worldTransform.getOrigin()) + (force), Color(Urho3D::Color::BLUE));
                    }


                    // Apply force at back
                    float antiRollForceB = (travelBL - travelBR) * antiRoll;

                    if (groundBL) {
                        Vector3 force = Vector3(0, raycastVehicle_->GetWheel(2).m_worldTransform.getOrigin().y() * -antiRollForceB, 0);
                        // Back wheel
                        debug->AddLine(Vector3(raycastVehicle_->GetWheel(2).m_worldTransform.getOrigin()),
                                       Vector3(raycastVehicle_->GetWheel(2).m_worldTransform.getOrigin()) + (force), Color(Urho3D::Color::BLUE));
                    }

                    if (groundBR) {
                        Vector3 force = Vector3(0, raycastVehicle_->GetWheel(3).m_worldTransform.getOrigin().y() * antiRollForceB, 0);
                        // Back wheel
                        debug->AddLine(Vector3(raycastVehicle_->GetWheel(3).m_worldTransform.getOrigin()),
                                       Vector3(raycastVehicle_->GetWheel(3).m_worldTransform.getOrigin()) + (force), Color(Urho3D::Color::BLUE));
                    }


                }

            }



            /*
            // Body box
            debug->AddBoundingBox(
                    BoundingBox(pos - Vector3(dimensions), pos + Vector3(dimensions)),
                    Color(0.0f, 1.0f, 0.0f));
    */


/*
            btCompoundShape* shape = vehicleBody->GetCompoundShape();

            shape
            //auto* hullColShape = node_->GetComponent<CollisionShape>();



            Vector3 scale = hullColShape->GetNode()->GetScale();
            BoundingBox bb = BoundingBox(-scale*8, scale*8);
            debug->AddBoundingBox(bb, Color(0.0f, 0.0f, 1.0f));
*/

            // If draw debug mode is enabled, draw physics debug geometry. Use depth test to make the result easier to interpret
//            if (drawDebug_)


            for (int i = 0; i < vehicle->GetNumWheels(); i++) {
            Node *emitter = particleEmitterNodeList_[i];
            auto *particleEmitter = emitter->GetComponent<ParticleEmitter>();




                DebugRenderer *dbgRenderer = node_->GetScene()->GetComponent<DebugRenderer>(REPLICATED);
                auto* hullBody = node_->GetComponent<RigidBody>();
                //auto* hullColShape = node_->GetComponent<CollisionShape>();
                hullBody->DrawDebugGeometry(dbgRenderer, true);

                if (hullColShape_)
                    hullColShape_->DrawDebugGeometry(dbgRenderer, true);
                for (int i = 0; i < wheels_.Size(); i++) {
            //        wheels_[i]->DrawDebugGeometry(dbgRenderer, true);
                }

                vehicle->DebugDraw(dbgRenderer);






/* DRIVING DEBUG LOGGING -> this shows traction control
                if (vehicle->WheelIsGrounded(i)) {
                    URHO3D_LOGDEBUGF("WheelIsGrounded() -> Wheel %i -> %d, velocity -> %f, accel -> %f", i,
                                     vehicle->WheelIsGrounded(i), velocity, accel);
                }
*/


                if (vehicle->WheelIsGrounded(i) &&
                (vehicle->GetWheelSkidInfoCumulative(i) < 0.9f || vehicle->GetBrake(i) > 2.0f ||
                 planeAccel > 15.0f)) {
                particleEmitterNodeList_[i]->SetWorldPosition(vehicle->GetContactPosition(i));
                if (!particleEmitter->IsEmitting()) {
                    particleEmitter->SetEmitting(true);
                }
                /*
                URHO3D_LOGDEBUG("GetWheelSkidInfoCumulative() = " +
                                String(vehicle->GetWheelSkidInfoCumulative(i)) + " " +
                                String(vehicle->GetMaxSideSlipSpeed()));*/

                /* TODO: Add skid marks here */
            } else if (particleEmitter->IsEmitting()) {
                particleEmitter->SetEmitting(false);
            }

            }

        }

    }

    // Sound effects update
    PostUpdateSound(timeStep);
}


void Vehicle::PlaySoundEffectGlobal(const String &soundName) {

    if (actorNode_) {
        auto *cache = GetSubsystem<ResourceCache>();
        auto *source = actorNode_->CreateComponent<SoundSource3D>(REPLICATED);

        source->SetNearDistance(1);  // distance up to where the volume is 100%
        source->SetFarDistance(6000);  // distance from where the volume is at 0%
        source->SetSoundType(SOUND_EFFECT);

        auto *sound = cache->GetResource<Sound>("Sounds/" + soundName);
        if (sound != nullptr) {
            source->SetAutoRemoveMode(REMOVE_COMPONENT);
            source->Play(sound);
        }
    }
}

void Vehicle::DelayedStart()
{
    Create();
}

void Vehicle::Create() {
}

void Vehicle::SetTurrentAngle(float angle) {
    if (!turrent_) return;
    turrent_->GetNode()->SetRotation(Quaternion(angle, 180.0f, 0.0f));
}

float Vehicle::GetTurrentAngle() {
    if (!turrent_) return 0;
    return turrent_->GetNode()->GetRotation().EulerAngles().x_;
}

Node* Vehicle::GetTurrent() {
    return turrent_->GetNode();
}

void Vehicle::Init(Node* node) {
    ResourceCache* cache = GetSubsystem<ResourceCache>();

    node_ = node;
    if (node_) {

        DebugRenderer *dbgRenderer = node_->GetScene()->GetComponent<DebugRenderer>(REPLICATED);

        // Raycast vehicle -> wheel raycast and body movement (linear velocity)
        raycastVehicle_ = node_->CreateComponent<RaycastVehicleBase>(LOCAL);
        raycastVehicle_->Init();

        // Store the raycast vehicle body
        body_ = node_->GetComponent<RigidBody>();
        //body_->DrawDebugGeometry(dbgRenderer, true);

        //float forwardWeightOffset = 38.0f; // Sahin
        float forwardWeightOffset = 3.0f; // Yugo

        // Push back
        node_->SetPosition(Vector3(0,0,forwardWeightOffset));
//        m_fVehicleMass = 425.0f;
        // Set body attributes
        body_->SetMass(525.0f);
        //body_->SetMass(m_fVehicleMass);
        body_->SetLinearDamping(0.12f); // Some air resistance
        body_->SetAngularDamping(0.3f);
        body_->SetCollisionLayer(2);
        // Shifting the Center of Mass forward, increasing stability
        body_->SetPosition(Vector3(0.0f,0.0f,0.0f));
        //body_->SetPosition();
        // This function is called only from the main program when initially creating the vehicle, not on scene load
        auto* cache = GetSubsystem<ResourceCache>();

        // Create model node under raycast vehicle node

        // Shift model higher than physics system
        Node* adjNode = node_->CreateChild("Model");
        //Node* adjNode2 = node_->CreateChild("Collision");
        //adjNode2->SetPosition(Vector3(0.0f,30.0f,0.0f));

        auto* hullObject = adjNode->CreateComponent<StaticModel>();
        hullObject->SetCastShadows(true);

        // Setting-up collision shape
        hullColShape_ = node_->CreateComponent<CollisionShape>();
        //auto* hullColShape = adjNode2->CreateComponent<CollisionShape>();

        Vector3 v3BoxExtents = Vector3::ONE * 8.4f;
        //v3BoxExtents.x_ *= 1.3f;

        //node_->SetScale(Vector3(12.3, 12.6f, 13.3f));
        hullColShape_->DrawDebugGeometry(dbgRenderer, true);


        //float connectionHeight = 1.1f;//-0.1f;
        //float connectionHeight = -2.1f;//-0.1f;
        float connectionHeight = -5.1f;//-0.1f;
        bool isFrontWheel = true;
        Vector3 wheelDirection(0, -1, 0);
        Vector3 wheelAxle(-1, 0, 0);
        // We use not scaled coordinates here as everything will be scaled.
        // Wheels are on bottom at edges of the chassis
        // Note we don't set wheel nodes as children of hull (while we could) to avoid scaling to affect them.
        float wheelX = (CHASSIS_WIDTH / 2.0f) + wheelWidth_;
        float wheelSpace = 10.0f;


///code/dev/MonkeyMaya_com/bin/Data/Models/Vehicles/Offroad
        //int carType = Random(0,3);
        //int carType = Random(3,6); // Toy Jeeps
        //int carType = Random(5,12);
        //int carType = 11;//Random(8,9); police

        int carType = CAR_TYPE_YUGO; // Yugo
        if (Random(1,3) == 1) {
            carType = CAR_TYPE_KART; // Kart
        }
        if (Random(1,4) == 2) {
            carType = CAR_TYPE_SAHIN; // Sahin
        }


        v3BoxExtents.x_ *= 1.3f;
        v3BoxExtents.y_ *= 1.01f;
        v3BoxExtents.z_ *= 3.6f;
        hullColShape_->SetBox(v3BoxExtents);
        adjNode->SetRotation(Quaternion(0.0f, 90.0f, -90.0f));

        this->carType = carType;
        switch (carType) {
            case 0: {
                hullObject->SetModel(cache->GetResource<Model>("Models/Vehicles/Offroad/Models/Hatchback.mdl"));
                hullObject->ApplyMaterialList("Models/Vehicles/Offroad/Models/Hatchback.txt");
                // Important for vehicle physics balancing
                node_->SetScale(Vector3(3.5, 2.6f, 3.3f));
                adjNode->SetPosition(Vector3(node_->GetPosition().x_, node_->GetPosition().y_-0.3f, node_->GetPosition().z_));

                wheelX = (CHASSIS_WIDTH / 2.0f) + wheelWidth_ + 6.2f;
                wheelSpace = 30.0f;
                connectionHeight = -2.1f;
                // Front left
                connectionPoints_[0] = Vector3(-wheelX, connectionHeight,  wheelSpace + GetWheelRadius() * 2.0f);
                // Front right
                connectionPoints_[1] = Vector3(wheelX, connectionHeight,  wheelSpace + GetWheelRadius() * 2.0f);
                // Back left
                connectionPoints_[2] = Vector3(-wheelX, connectionHeight,  -wheelSpace - GetWheelRadius() * 2.0f);
                // Back right
                connectionPoints_[3] = Vector3(wheelX, connectionHeight,  -wheelSpace - GetWheelRadius() * 2.0f);

                break;
            }

//
            case 1: {
                hullObject->SetModel(cache->GetResource<Model>("Models/Vehicles/Offroad/Models/minivan bo.mdl"));
                hullObject->ApplyMaterialList("Models/Vehicles/Offroad/Models/minivan bo.txt");
                // Important for vehicle physics balancing
                node_->SetScale(Vector3(3.5, 2.6f, 3.3f));
                adjNode->SetPosition(Vector3(node_->GetPosition().x_, node_->GetPosition().y_-0.3f, node_->GetPosition().z_));

                wheelX = (CHASSIS_WIDTH / 2.0f) + wheelWidth_ + 6.2f;
                wheelSpace = 34.0f;
                connectionHeight = -2.1f;
                // Front left
                connectionPoints_[0] = Vector3(-wheelX, connectionHeight,  wheelSpace + GetWheelRadius() * 2.0f);
                // Front right
                connectionPoints_[1] = Vector3(wheelX, connectionHeight,  wheelSpace + GetWheelRadius() * 2.0f);
                // Back left
                connectionPoints_[2] = Vector3(-wheelX, connectionHeight,  -wheelSpace - GetWheelRadius() * 2.0f);
                // Back right
                connectionPoints_[3] = Vector3(wheelX, connectionHeight,  -wheelSpace - GetWheelRadius() * 2.0f);

                break;
            }

            case 2: {
                hullObject->SetModel(cache->GetResource<Model>("Models/Vehicles/Offroad/Models/Coupe Body.mdl"));
                hullObject->ApplyMaterialList("Models/Vehicles/Offroad/Models/Coupe Body.txt");
                // Important for vehicle physics balancing
                node_->SetScale(Vector3(3.5, 2.6f, 3.3f));
                adjNode->SetPosition(Vector3(node_->GetPosition().x_, node_->GetPosition().y_-0.3f, node_->GetPosition().z_));

                wheelX = (CHASSIS_WIDTH / 2.0f) + wheelWidth_ + 6.2f;
                wheelSpace = 30.0f;
                connectionHeight = -2.1f;
                // Front left
                connectionPoints_[0] = Vector3(-wheelX, connectionHeight,  wheelSpace + GetWheelRadius() * 2.0f);
                // Front right
                connectionPoints_[1] = Vector3(wheelX, connectionHeight,  wheelSpace + GetWheelRadius() * 2.0f);
                // Back left
                connectionPoints_[2] = Vector3(-wheelX, connectionHeight,  -wheelSpace - GetWheelRadius() * 2.0f);
                // Back right
                connectionPoints_[3] = Vector3(wheelX, connectionHeight,  -wheelSpace - GetWheelRadius() * 2.0f);

                break;
            }


            case 3 ... 5: {

                // CAR TYPE: JEEP

                float scaleF = 0.3f;
                float scaleF2 = 0.4f;

                adjNode->SetScale(Vector3(scaleF, scaleF, scaleF));
                adjNode->SetRotation(Quaternion(0,0,0));

                v3BoxExtents.x_ *= 3.0f * scaleF2;
                v3BoxExtents.y_ *= 1.3f * scaleF2;
                v3BoxExtents.z_ *= 2.82f * scaleF2;


                //XMLFile *f = cache->GetResource<XMLFile>("Objects/Kart_Vehicle.xml");
                //Vector3 pos = Vector3(0, 0, 0.0f); // Yugo
                //Quaternion q = Quaternion(0, 90, 0);
                //Node *vehiclePrefab_ = GetScene()->InstantiateXML(f->GetRoot(), pos, q, REPLICATED);
                //vehiclePrefab_->SetParent(node_);

                //wheelSpace = 24.1f * scaleF; // Kart
                wheelSpace = 1.1f * scaleF; // Kart
                wheelX = ((2.6f / 2.0f) + wheelWidth_); // Kart

                hullColShape_->SetBox(v3BoxExtents);

                // Important for vehicle physics balancing
                //node_->SetScale(Vector3(3.3, 2.6f, 3.3f));
////
                hullObject->SetModel(cache->GetResource<Model>("Models/Vehicles/Offroad/Models/body-car.mdl"));

                if (carType == 3) {
                    hullObject->ApplyMaterialList("Models/Vehicles/Offroad/Models/body-car-t1.txt");
                } else if (carType == 4) {
                    hullObject->ApplyMaterialList("Models/Vehicles/Offroad/Models/body-car-t2.txt");
                } else if (carType == 5) {
                    hullObject->ApplyMaterialList("Models/Vehicles/Offroad/Models/body-car-t3.txt");
                }

                // Important for vehicle physics balancing
                //node_->SetScale(Vector3(3.3, 2.6f, 3.3f));
                node_->SetScale(Vector3(3.5, 2.6f, 3.3f));

                adjNode->SetPosition(Vector3(node_->GetPosition().x_, node_->GetPosition().y_+1.2f, node_->GetPosition().z_));

                wheelX = (CHASSIS_WIDTH / 2.0f) + wheelWidth_;
                wheelSpace = 6.3f;
                connectionHeight = -5.1f;
                // Front left
                connectionPoints_[0] = Vector3(-wheelX, connectionHeight,  wheelSpace + GetWheelRadius() * 2.0f);
                // Front right
                connectionPoints_[1] = Vector3(wheelX, connectionHeight,  wheelSpace + GetWheelRadius() * 2.0f);
                // Back left
                connectionPoints_[2] = Vector3(-wheelX, connectionHeight,  -wheelSpace - GetWheelRadius() * 2.0f);
                // Back right
                connectionPoints_[3] = Vector3(wheelX, connectionHeight,  -wheelSpace - GetWheelRadius() * 2.0f);
                break;
            }

            case 6 ... 12: {

                if (carType == CAR_TYPE_YUGO) {
                    // CAR TYPE: YUGO


/*                    v3BoxExtents.x_ *= 15.0f;
                    v3BoxExtents.y_ *= 6.5f;
                    v3BoxExtents.z_ *= 14.1f;


*/

                    float scaleF = 0.1f;
                    float scaleF2 = 0.4f;

                    ///node_->SetScale(Vector3(0.2f,0.2f,0.2f));
                    //adjNode->SetScale(Vector3(0.2f,0.2f,0.2f));
                    adjNode->SetScale(Vector3(scaleF, scaleF, scaleF));
/*                    v3BoxExtents.x_ *= 3.0f;
                    v3BoxExtents.y_ *= 1.3f;
                    v3BoxExtents.z_ *= 2.82f;
*
 */

                    v3BoxExtents.x_ *= 3.0f * scaleF2;
                    v3BoxExtents.y_ *= 1.3f * scaleF2;
                    v3BoxExtents.z_ *= 2.82f * scaleF2;
                    XMLFile *f = cache->GetResource<XMLFile>("Objects/Yugo_Vehicle.xml");
                    //XMLFile *f = cache->GetResource<XMLFile>("Objects/Sahin_Vehicle.xml");
                    //Vector3 pos = Vector3(0,0,42.0f); // Sahin
                    Vector3 pos = Vector3(0, 4.0f, 0.0f); // Yugo
                    Quaternion q = Quaternion(0, 90, 0);
                    Node *vehiclePrefab_ = GetScene()->InstantiateXML(f->GetRoot(), pos, q, REPLICATED);

                    vehiclePrefab_->SetParent(node_);
                    float vScale = 1.13f;
                    vehiclePrefab_->SetScale(Vector3(vScale, vScale, vScale*1.2));

                    //gilza_->SetWorldScale(Vector3(0.11f, 0.11f, 0.11f)); LifeTime* lt = gilza_->CreateComponent<LifeTime>(); lt->SetLifeTime(5.0f)
/*
                    // Top of Car Body

                    //hullObject->SetModel(cache->GetResource<Model>("Models/Vehicles/SetA/Models/Jeep.mdl"));
                    hullObject->SetModel(cache->GetResource<Model>("Models/Vehicles/Sahin/Models/Car_Body_Frame.mdl"));
                    hullObject->ApplyMaterialList("Models/Vehicles/SetA/Models/Car_Body_Frame.txt");
                    //
                    //code/dev/MonkeyMaya_com/bin/Data/
                    //hullObject->ApplyMaterialList("Models/Vehicles/SetA/Models/Jeep.txt");

                    Node* node;
                    StaticModel* model;

                    // Bottom Trim of Body and Inner Parts

                    // Create new child node
                    node = adjNode->CreateChild("Model");
                    model = node->CreateComponent<StaticModel>();
                    model->SetCastShadows(true);
                    //adjNode3->SetRotation(Quaternion(0.0f, -180.0f, -90.0f));
                    model->SetModel(cache->GetResource<Model>("Models/Vehicles/Sahin/Models/Car_Body_Trim.mdl"));
                    model->ApplyMaterialList("Models/Vehicles/Sahin/Models/Car_Body_Trim.txt");
                    node->SetPosition(Vector3(0,0,0));

                    // Doors

                    // Front Left Door

                    // Create new child node
                    node = adjNode->CreateChild("Model");
                    model = node->CreateComponent<StaticModel>();
                    model->SetCastShadows(true);
                    //adjNode3->SetRotation(Quaternion(0.0f, -180.0f, -90.0f));
                    model->SetModel(cache->GetResource<Model>("Models/Vehicles/Sahin/Models/Door_FL.mdl"));
                    model->ApplyMaterialList("Models/Vehicles/Sahin/Models/Door_FL.txt");
                    node->SetPosition(Vector3(0,0,0));

                    // Front Right Door

                    // Create new child node
                    node = adjNode->CreateChild("Model");
                    model = node->CreateComponent<StaticModel>();
                    model->SetCastShadows(true);
                    //adjNode3->SetRotation(Quaternion(0.0f, -180.0f, -90.0f));
                    model->SetModel(cache->GetResource<Model>("Models/Vehicles/Sahin/Models/Door_FR.mdl"));
                    model->ApplyMaterialList("Models/Vehicles/Sahin/Models/Door_FR.txt");
                    node->SetPosition(Vector3(0,0,0));

                    // Back Left Door

                    // Create new child node
                    node = adjNode->CreateChild("Model");
                    model = node->CreateComponent<StaticModel>();
                    model->SetCastShadows(true);
                    //adjNode3->SetRotation(Quaternion(0.0f, -180.0f, -90.0f));
                    model->SetModel(cache->GetResource<Model>("Models/Vehicles/Sahin/Models/Door_BL.mdl"));
                    model->ApplyMaterialList("Models/Vehicles/Sahin/Models/Door_BL.txt");
                    node->SetPosition(Vector3(0,0,0));

                    // Back Right Door

                    // Create new child node
                    node = adjNode->CreateChild("Model");
                    model = node->CreateComponent<StaticModel>();
                    model->SetCastShadows(true);
                    //adjNode3->SetRotation(Quaternion(0.0f, -180.0f, -90.0f));
                    model->SetModel(cache->GetResource<Model>("Models/Vehicles/Sahin/Models/Door_BR.mdl"));
                    model->ApplyMaterialList("Models/Vehicles/Sahin/Models/Door_BR.txt");
                    node->SetPosition(Vector3(0,0,0));


*/



                    //towards_ = Vector3(towards_.x_*cos(turningVelocity_*timeStep) - towards_.y_*sin(turningVelocity_*timeStep), towards_.x_*sin(turningVelocity_*timeStep) + towards_.y_*cos(turningVelocity_*timeStep), 0.0f);
                    //node->Rotate2D(turningVelocity_*timeStep);
                    // The angle between rotation2d and x-axis
                    //float angle = 90.0f + node->GetRotation2D();


                    // apply to x
                    //adjNode2->SetRotation(Quaternion(-90.0f, 180.0f, 0.0f));

/*
                    //connectionHeight = -4.74f;
                    //connectionHeight = -4.2f;
                    connectionHeight = -1.4f;//-2.2f;
                    //wheelSpace = 2.4f;
                    wheelSpace = 17.1f*scaleF;
                    // chassis width -> 35 * 20% -> 7
                    //wheelX = ((7 / 2.0f) + wheelWidth_);
                    wheelX = ((2.6f / 2.0f) + wheelWidth_);
*/

                    //connectionHeight = -1.4f;//-2.2f; // Sahin
                    //wheelSpace = 17.1f*scaleF; // Sahin
                    //wheelX = ((2.6f / 2.0f) + wheelWidth_); // Sahin


                    connectionHeight = -1.1f; // Kart
                    //wheelSpace = 24.1f * scaleF; // Kart
                    wheelSpace = 33.1f * scaleF; // Kart
                    wheelX = ((6.1f / 2.0f) + wheelWidth_); // Kart


                    hullColShape_->SetBox(v3BoxExtents);

                    // Important for vehicle physics balancing
                    //node_->SetScale(Vector3(3.3, 2.6f, 3.3f));
                    node_->SetScale(Vector3(0.4f, 0.35f, 0.4f));


                    // Front left
                    connectionPoints_[0] = Vector3(-wheelX, connectionHeight, wheelSpace + GetWheelRadius() * 2.0f);
                    // Front right
                    connectionPoints_[1] = Vector3(wheelX, connectionHeight, wheelSpace + GetWheelRadius() * 2.0f);
                    // Back left
                    connectionPoints_[2] = Vector3(-wheelX, connectionHeight, -wheelSpace - GetWheelRadius() * 2.0f);
                    // Back right
                    connectionPoints_[3] = Vector3(wheelX, connectionHeight, -wheelSpace - GetWheelRadius() * 2.0f);


                } else if (carType == CAR_TYPE_SAHIN) {

                    // CAR TYPE: SAHIN
                    float scaleF = 0.1f;
                    float scaleF2 = 0.4f;


                    v3BoxExtents.x_ *= 3.0f * scaleF2;
                    v3BoxExtents.y_ *= 1.3f * scaleF2;
                    v3BoxExtents.z_ *= 2.82f * scaleF2;
                    XMLFile *f = cache->GetResource<XMLFile>("Objects/Sahin_Vehicle.xml");
                    //Vector3 pos = Vector3(0,0,42.0f); // Sahin
                    Vector3 pos = Vector3(0, 2.8f, 0.0f); // Sahin
                    Quaternion q = Quaternion(0, 90, 0);
                    Node *vehiclePrefab_ = GetScene()->InstantiateXML(f->GetRoot(), pos, q, REPLICATED);

                    vehiclePrefab_->SetParent(node_);
                    float vScale = 1.13f;
                    vehiclePrefab_->SetScale(Vector3(vScale, vScale, vScale*1.2));

                    //gilza_->SetWorldScale(Vector3(0.11f, 0.11f, 0.11f)); LifeTime* lt = gilza_->CreateComponent<LifeTime>(); lt->SetLifeTime(5.0f)
/*
                    // Top of Car Body

                    //hullObject->SetModel(cache->GetResource<Model>("Models/Vehicles/SetA/Models/Jeep.mdl"));
                    hullObject->SetModel(cache->GetResource<Model>("Models/Vehicles/Sahin/Models/Car_Body_Frame.mdl"));
                    hullObject->ApplyMaterialList("Models/Vehicles/SetA/Models/Car_Body_Frame.txt");
                    //
                    //code/dev/MonkeyMaya_com/bin/Data/
                    //hullObject->ApplyMaterialList("Models/Vehicles/SetA/Models/Jeep.txt");

                    Node* node;
                    StaticModel* model;

                    // Bottom Trim of Body and Inner Parts

                    // Create new child node
                    node = adjNode->CreateChild("Model");
                    model = node->CreateComponent<StaticModel>();
                    model->SetCastShadows(true);
                    //adjNode3->SetRotation(Quaternion(0.0f, -180.0f, -90.0f));
                    model->SetModel(cache->GetResource<Model>("Models/Vehicles/Sahin/Models/Car_Body_Trim.mdl"));
                    model->ApplyMaterialList("Models/Vehicles/Sahin/Models/Car_Body_Trim.txt");
                    node->SetPosition(Vector3(0,0,0));

                    // Doors

                    // Front Left Door

                    // Create new child node
                    node = adjNode->CreateChild("Model");
                    model = node->CreateComponent<StaticModel>();
                    model->SetCastShadows(true);
                    //adjNode3->SetRotation(Quaternion(0.0f, -180.0f, -90.0f));
                    model->SetModel(cache->GetResource<Model>("Models/Vehicles/Sahin/Models/Door_FL.mdl"));
                    model->ApplyMaterialList("Models/Vehicles/Sahin/Models/Door_FL.txt");
                    node->SetPosition(Vector3(0,0,0));

                    // Front Right Door

                    // Create new child node
                    node = adjNode->CreateChild("Model");
                    model = node->CreateComponent<StaticModel>();
                    model->SetCastShadows(true);
                    //adjNode3->SetRotation(Quaternion(0.0f, -180.0f, -90.0f));
                    model->SetModel(cache->GetResource<Model>("Models/Vehicles/Sahin/Models/Door_FR.mdl"));
                    model->ApplyMaterialList("Models/Vehicles/Sahin/Models/Door_FR.txt");
                    node->SetPosition(Vector3(0,0,0));

                    // Back Left Door

                    // Create new child node
                    node = adjNode->CreateChild("Model");
                    model = node->CreateComponent<StaticModel>();
                    model->SetCastShadows(true);
                    //adjNode3->SetRotation(Quaternion(0.0f, -180.0f, -90.0f));
                    model->SetModel(cache->GetResource<Model>("Models/Vehicles/Sahin/Models/Door_BL.mdl"));
                    model->ApplyMaterialList("Models/Vehicles/Sahin/Models/Door_BL.txt");
                    node->SetPosition(Vector3(0,0,0));

                    // Back Right Door

                    // Create new child node
                    node = adjNode->CreateChild("Model");
                    model = node->CreateComponent<StaticModel>();
                    model->SetCastShadows(true);
                    //adjNode3->SetRotation(Quaternion(0.0f, -180.0f, -90.0f));
                    model->SetModel(cache->GetResource<Model>("Models/Vehicles/Sahin/Models/Door_BR.mdl"));
                    model->ApplyMaterialList("Models/Vehicles/Sahin/Models/Door_BR.txt");
                    node->SetPosition(Vector3(0,0,0));


*/


/*
                    Node *adjNode2 = adjNode->CreateChild("Model");
                    hullObjectTurrent_ = adjNode2->CreateComponent<StaticModel>();
                    hullObjectTurrent_->SetCastShadows(true);
//                    hullObjectTurrent->SetModel(cache->GetResource<Model>("Models/AssetPack/sherman-gun.mdl"));
                    //                  hullObjectTurrent->ApplyMaterialList("Models/AssetPack/sherman-gun.txt");

                    //adjNode3->SetRotation(Quaternion(0.0f, -180.0f, -90.0f));
                    hullObjectTurrent_->SetModel(cache->GetResource<Model>("Models/Vehicles/SetA/Models/Turrent.mdl"));
                    hullObjectTurrent_->ApplyMaterialList("Models/Vehicles/SetA/Models/Turrent.txt");
                    adjNode->SetPosition(Vector3(node_->GetPosition().x_, node_->GetPosition().y_ + 2.3f,
                                                 node_->GetPosition().z_ - forwardWeightOffset - 4.0f));
                    adjNode2->SetPosition(Vector3(-14.0f, -220.0f, 0.0f));
                    adjNode2->SetScale(Vector3(0.4f, 0.4f, 0.4f));
*/
                    //towards_ = Vector3(towards_.x_*cos(turningVelocity_*timeStep) - towards_.y_*sin(turningVelocity_*timeStep), towards_.x_*sin(turningVelocity_*timeStep) + towards_.y_*cos(turningVelocity_*timeStep), 0.0f);
                    //node->Rotate2D(turningVelocity_*timeStep);
                    // The angle between rotation2d and x-axis
                    //float angle = 90.0f + node->GetRotation2D();


                    // apply to x
                    //adjNode2->SetRotation(Quaternion(-90.0f, 180.0f, 0.0f));

/*
                    //connectionHeight = -4.74f;
                    //connectionHeight = -4.2f;
                    connectionHeight = -1.4f;//-2.2f;
                    //wheelSpace = 2.4f;
                    wheelSpace = 17.1f*scaleF;
                    // chassis width -> 35 * 20% -> 7
                    //wheelX = ((7 / 2.0f) + wheelWidth_);
                    wheelX = ((2.6f / 2.0f) + wheelWidth_);
*/

                    //connectionHeight = -1.4f;//-2.2f; // Sahin
                    //wheelSpace = 17.1f*scaleF; // Sahin
                    //wheelX = ((2.6f / 2.0f) + wheelWidth_); // Sahin


                    connectionHeight = -1.1f; // Kart
                    //wheelSpace = 24.1f * scaleF; // Kart
                    wheelSpace = 33.1f * scaleF; // Kart
                    wheelX = ((6.1f / 2.0f) + wheelWidth_); // Kart


                    hullColShape_->SetBox(v3BoxExtents);

                    // Important for vehicle physics balancing
                    //node_->SetScale(Vector3(3.3, 2.6f, 3.3f));
                    node_->SetScale(Vector3(0.4f, 0.35f, 0.4f));


                    // Front left
                    connectionPoints_[0] = Vector3(-wheelX, connectionHeight, wheelSpace + GetWheelRadius() * 2.0f);
                    // Front right
                    connectionPoints_[1] = Vector3(wheelX, connectionHeight, wheelSpace + GetWheelRadius() * 2.0f);
                    // Back left
                    connectionPoints_[2] = Vector3(-wheelX, connectionHeight, -wheelSpace - GetWheelRadius() * 2.0f);
                    // Back right
                    connectionPoints_[3] = Vector3(wheelX, connectionHeight, -wheelSpace - GetWheelRadius() * 2.0f);

                } else if (carType == 8) {
                    v3BoxExtents.x_ *= 14.0f;
                    v3BoxExtents.y_ *= 13.0f;
                    v3BoxExtents.z_ *= 21.0f;

                    hullObject->SetModel(cache->GetResource<Model>("Models/Vehicles/SetA/Models/Car_Truck_1_2.mdl"));
                    hullObject->ApplyMaterialList("Models/Vehicles/SetA/Models/Car_Truck_1_2.txt");

                    Node *adjNode2 = adjNode->CreateChild("Model");
                    adjNode2->SetRotation(Quaternion(0.0f, 0.0f, 0.0f));
                    auto *hullObjectBack = adjNode2->CreateComponent<StaticModel>();
                    hullObjectBack->SetCastShadows(true);
                    hullObjectBack->SetModel(cache->GetResource<Model>("Models/Vehicles/SetA/Models/Back_3.mdl"));
                    hullObjectBack->ApplyMaterialList("Models/Vehicles/SetA/Models/Back_3.txt");

                    Node *adjNode3 = adjNode2->CreateChild("Model");
                    adjNode3->SetRotation(Quaternion(0.0f, -180.0f, -90.0f));
                    auto *hullObjectCapsule = adjNode3->CreateComponent<StaticModel>();
                    hullObjectCapsule->SetCastShadows(true);
                    hullObjectCapsule->SetModel(cache->GetResource<Model>("Models/Vehicles/SetA/Models/Capsule.mdl"));
                    hullObjectCapsule->ApplyMaterialList("Models/Vehicles/SetA/Models/Capsule.txt");

                    adjNode->SetPosition(Vector3(node_->GetPosition().x_ - 4.0f, node_->GetPosition().y_ + 18.0f,
                                                 node_->GetPosition().z_ + 216.0f));
                    adjNode2->SetPosition(Vector3(50.0f, 180.0f, 0.0f));
                    adjNode3->SetPosition(Vector3(-105.0f, 95.0f, 0.0f));

                    connectionHeight = -18.7f;
                    wheelSpace = 93.0f;
                    wheelX = 11.0f + (CHASSIS_WIDTH / 2.0f) + wheelWidth_;
                } else if (carType == 9) {
                    v3BoxExtents.x_ *= 19.0f;
                    v3BoxExtents.y_ *= 13.0f;
                    v3BoxExtents.z_ *= 16.5f;

                    hullObject->SetModel(cache->GetResource<Model>("Models/Vehicles/SetA/Models/Firecar.mdl"));
                    hullObject->ApplyMaterialList("Models/Vehicles/SetA/Models/Firecar.txt");
//                    adjNode->SetPosition(Vector3(node_->GetPosition().x_-58.0f, node_->GetPosition().y_+3.0f, node_->GetPosition().z_-14.0f));

                    adjNode->SetPosition(Vector3(node_->GetPosition().x_ - 4.0f, node_->GetPosition().y_ + 18.0f,
                                                 node_->GetPosition().z_));//+216.0f));
                    //                  adjNode2->SetPosition(Vector3(50.0f, 180.0f, 0.0f));
                    //                 adjNode3->SetPosition(Vector3(-105.0f, 95.0f, 0.0f));

                    connectionHeight = -18.7f;
                    wheelSpace = 55.0f;
                    wheelX = 11.0f + (CHASSIS_WIDTH / 2.0f) + wheelWidth_;

                } else if (carType == 10) {
                    v3BoxExtents.x_ *= 14.0f;
                    v3BoxExtents.y_ *= 13.0f;
                    v3BoxExtents.z_ *= 16.0f;

                    hullObject->SetModel(cache->GetResource<Model>("Models/Vehicles/SetA/Models/Car_Truck.mdl"));
                    hullObject->ApplyMaterialList("Models/Vehicles/SetA/Models/Car_Truck.txt");

                    adjNode->SetPosition(Vector3(node_->GetPosition().x_ - 4.0f, node_->GetPosition().y_ + 18.0f,
                                                 node_->GetPosition().z_ + 100.0f));
                    //                  adjNode2->SetPosition(Vector3(50.0f, 180.0f, 0.0f));
                    //                 adjNode3->SetPosition(Vector3(-105.0f, 95.0f, 0.0f));

                    connectionHeight = -18.7f;
                    wheelSpace = 55.0f;
                    wheelX = 11.0f + (CHASSIS_WIDTH / 2.0f) + wheelWidth_;

                } else if (carType == 11) {
                    v3BoxExtents.x_ *= 14.0f;
                    v3BoxExtents.y_ *= 13.0f;
                    v3BoxExtents.z_ *= 12.0f;

                    hullObject->SetModel(cache->GetResource<Model>("Models/Vehicles/SetA/Models/Car_Police_2.mdl"));
                    hullObject->ApplyMaterialList("Models/Vehicles/SetA/Models/Car_Police_2.txt");
                    adjNode->SetPosition(Vector3(node_->GetPosition().x_ - 0.0f, node_->GetPosition().y_ + 70.0f,
                                                 node_->GetPosition().z_ - 0.0f));
                    //adjNode->SetRotation(Quaternion(0,90,0));
                    connectionHeight = -16.7f;
                    wheelSpace = 19.5f;
                    wheelX = 11.0f + (CHASSIS_WIDTH / 2.0f) + wheelWidth_;

                    adjNode->SetRotation(Quaternion(0.0f, 0.0f, -90.0f));
                } else if (carType == 12) {

                    // CAR TYPE: KART

                    float scaleF = 0.1f;
                    float scaleF2 = 0.4f;

                    adjNode->SetScale(Vector3(scaleF, scaleF, scaleF));

                    v3BoxExtents.x_ *= 3.0f * scaleF2;
                    v3BoxExtents.y_ *= 1.3f * scaleF2;
                    v3BoxExtents.z_ *= 2.82f * scaleF2;


                    XMLFile *f = cache->GetResource<XMLFile>("Objects/Kart_Vehicle.xml");
                    Vector3 pos = Vector3(0, 0.0f, 0.0f); // Yugo
                    Quaternion q = Quaternion(0, 90, 0);
                    Node *vehiclePrefab_ = GetScene()->InstantiateXML(f->GetRoot(), pos, q, REPLICATED);

                    vehiclePrefab_->SetParent(node_);

                    connectionHeight = -0.2f; // Kart
                    //wheelSpace = 24.1f * scaleF; // Kart
                    wheelSpace = 1.1f * scaleF; // Kart
                    wheelX = ((2.6f / 2.0f) + wheelWidth_); // Kart


                    hullColShape_->SetBox(v3BoxExtents);

                    // Important for vehicle physics balancing
                    //node_->SetScale(Vector3(3.3, 2.6f, 3.3f));
                    node_->SetScale(Vector3(0.4f, 0.35f, 0.4f));


                    // Front left
                    connectionPoints_[0] = Vector3(-wheelX, connectionHeight, wheelSpace + GetWheelRadius() * 2.0f);
                    // Front right
                    connectionPoints_[1] = Vector3(wheelX, connectionHeight, wheelSpace + GetWheelRadius() * 2.0f);
                    // Back left
                    connectionPoints_[2] = Vector3(-wheelX, connectionHeight, -wheelSpace - GetWheelRadius() * 2.0f);
                    // Back right
                    connectionPoints_[3] = Vector3(wheelX, connectionHeight, -wheelSpace - GetWheelRadius() * 2.0f);


                    break;
                }
            }

        }

        // Store set wheel space
        wheelSpace_ = wheelSpace;

        Vector3 pos = Vector3(0, 4.0f, 0.0f); // Yugo
        Quaternion q = Quaternion(0, 90, 0);

        Node *turrentNode = GetNode()->CreateChild("Model");
        turrent_ = turrentNode->CreateComponent<StaticModel>();
        turrent_->SetCastShadows(true);
        turrent_->SetModel(cache->GetResource<Model>("Models/AssetPack/sherman-gun.mdl"));
        turrent_->ApplyMaterialList("Models/AssetPack/sherman-gun.txt");

        //adjNode3->SetRotation(Quaternion(0.0f, -180.0f, -90.0f));
        //turrent_->SetModel(cache->GetResource<Model>("Models/Vehicles/SetA/Models/Turrent.mdl"));
        //turrent_->ApplyMaterialList("Models/Vehicles/SetA/Models/Turrent.txt");
        adjNode->SetPosition(Vector3(0,0,0));
        turrentNode->SetPosition(Vector3(-0.0f, 5.0f, 0.0f));
        turrentNode->SetRotation(body_->GetRotation());
        float scale = 0.4f;
        turrentNode->SetScale(Vector3(1.0f*scale, 1.0f*scale, 1.0f*scale));



        const Color LtBrown(0.972f, 0.780f, 0.412f);
        for (int id = 0; id < sizeof(connectionPoints_) / sizeof(connectionPoints_[0]); id++)
        {
            // Notice the create wheel is on the scene not the node, for wheels to be independent nodes
            Node* wheelNode = GetScene()->CreateChild("Wheel");
            // Store wheel node for clean up
            wheels_[id] = wheelNode;

            Vector3 connectionPoint = connectionPoints_[id];
            // Front wheels are at front (z > 0)
            // back wheels are at z < 0
            // Setting rotation according to wheel position
            bool isFrontWheel = connectionPoints_[id].z_ > 0.0f;
            auto* pWheel = wheelNode->CreateComponent<StaticModel>();

            float scale = 1;
            Model *tireModel;

            // Set tires for car type
            switch (carType) {

                case CAR_TYPE_JEEP1:
                    //scale = 0.1f;
                    //scale = 0.04f; //SetScale(Vector3(0.04f,0.04f,0.04f))
                    scale = 6.6f; //SetScale(Vector3(0.04f,0.04f,0.04f))
                    tireModel = cache->GetResource<Model>("Models/Vehicles/SetA/Models/Wheels_4.mdl");
                    if (carType == 6) scale = 6.3;
                    scale = 0.06f;

                    //Model *tireModel = cache->GetResource<Model>("Models/Vehicles/Yugo/Models/wheel.mdl");
                    pWheel->SetModel(tireModel);
                    pWheel->ApplyMaterialList("Models/Vehicles/SetA/Models/Wheels_4.txt");
//                    pWheel->ApplyMaterialList("Models/Vehicles/Sahin/Models/Wheel_FL.txt");
                    //pWheel->ApplyMaterialList("Models/Vehicles/Yugo/Models/wheel.txt");


                    //pWheel->SetMaterial(cache->GetResource<Material>("Offroad/Models/Materials/Tire.xml"));
                    wheelNode->SetScale(Vector3(scale/1.2f, scale, scale));
//                    wheelNode->SetScale(Vector3(scale, scale, scale));
                    wheelNode->SetRotation(connectionPoint.x_ >= 0.0 ? Quaternion(0.0f, 0.0f, 180.0f) : Quaternion(0.0f, 0.0f, -0.0f));
                    //wheelNode->SetRotation(connectionPoint.x_ >= 0.0 ? Quaternion(0.0f, 90.0f, 180.0f) : Quaternion(0.0f, 90.0f, -0.0f));
                    wheelNode->SetWorldPosition(Vector3(0,0,2.0f-forwardWeightOffset)+node_->GetWorldPosition() + node_->GetWorldRotation() * connectionPoints_[id]);
                    //wheelNode->SetWorldPosition(node_->GetWorldPosition() + node_->GetWorldRotation() * connectionPoints_[id]);

                    break;
                case CAR_TYPE_JEEP2:
                    break;
                case CAR_TYPE_JEEP3:
                    break;
                case CAR_TYPE_KART:
                    //scale = 0.1f;
                    //scale = 0.04f; //SetScale(Vector3(0.04f,0.04f,0.04f))
                    scale = 5.6f; //SetScale(Vector3(0.04f,0.04f,0.04f))
                    tireModel = cache->GetResource<Model>("Models/Vehicles/SetA/Models/Wheels_4.mdl");
                    if (carType == 6) scale = 6.3;
                    scale = 0.05f;

                    //Model *tireModel = cache->GetResource<Model>("Models/Vehicles/Yugo/Models/wheel.mdl");
                    pWheel->SetModel(tireModel);
                    pWheel->ApplyMaterialList("Models/Vehicles/SetA/Models/Wheels_4.txt");
//                    pWheel->ApplyMaterialList("Models/Vehicles/Sahin/Models/Wheel_FL.txt");
                    //pWheel->ApplyMaterialList("Models/Vehicles/Yugo/Models/wheel.txt");


                    //pWheel->SetMaterial(cache->GetResource<Material>("Offroad/Models/Materials/Tire.xml"));
                    wheelNode->SetScale(Vector3(scale/1.2f, scale, scale));
//                    wheelNode->SetScale(Vector3(scale, scale, scale));
                    wheelNode->SetRotation(connectionPoint.x_ >= 0.0 ? Quaternion(0.0f, 0.0f, 180.0f) : Quaternion(0.0f, 0.0f, -0.0f));
                    //wheelNode->SetRotation(connectionPoint.x_ >= 0.0 ? Quaternion(0.0f, 90.0f, 180.0f) : Quaternion(0.0f, 90.0f, -0.0f));
                    wheelNode->SetWorldPosition(Vector3(0,0,2.0f-forwardWeightOffset)+node_->GetWorldPosition() + node_->GetWorldRotation() * connectionPoints_[id]);
                    //wheelNode->SetWorldPosition(node_->GetWorldPosition() + node_->GetWorldRotation() * connectionPoints_[id]);

                    break;
                case CAR_TYPE_SAHIN:
                    //scale = 0.1f;
                    //scale = 0.04f; //SetScale(Vector3(0.04f,0.04f,0.04f))
                    scale = 4.6f; //SetScale(Vector3(0.04f,0.04f,0.04f))
                    tireModel = cache->GetResource<Model>("Models/Vehicles/SetA/Models/Wheels_4.mdl");
                    if (carType == 6) scale = 6.3;
                    scale = 0.05f;

                    //Model *tireModel = cache->GetResource<Model>("Models/Vehicles/Yugo/Models/wheel.mdl");
                    pWheel->SetModel(tireModel);
                    pWheel->ApplyMaterialList("Models/Vehicles/SetA/Models/Wheels_4.txt");
//                    pWheel->ApplyMaterialList("Models/Vehicles/Sahin/Models/Wheel_FL.txt");
                    //pWheel->ApplyMaterialList("Models/Vehicles/Yugo/Models/wheel.txt");


                    //pWheel->SetMaterial(cache->GetResource<Material>("Offroad/Models/Materials/Tire.xml"));
                    wheelNode->SetScale(Vector3(scale/1.2f, scale, scale));
//                    wheelNode->SetScale(Vector3(scale, scale, scale));
                    wheelNode->SetRotation(connectionPoint.x_ >= 0.0 ? Quaternion(0.0f, 0.0f, 180.0f) : Quaternion(0.0f, 0.0f, -0.0f));
                    //wheelNode->SetRotation(connectionPoint.x_ >= 0.0 ? Quaternion(0.0f, 90.0f, 180.0f) : Quaternion(0.0f, 90.0f, -0.0f));
                    wheelNode->SetWorldPosition(Vector3(0,0,2.0f-forwardWeightOffset)+node_->GetWorldPosition() + node_->GetWorldRotation() * connectionPoints_[id]);
                    //wheelNode->SetWorldPosition(node_->GetWorldPosition() + node_->GetWorldRotation() * connectionPoints_[id]);

                    break;
                case CAR_TYPE_YUGO:
                    //scale = 0.1f;
                    //scale = 0.04f; //SetScale(Vector3(0.04f,0.04f,0.04f))
                    scale = 4.6f; //SetScale(Vector3(0.04f,0.04f,0.04f))
                    tireModel = cache->GetResource<Model>("Models/Vehicles/SetA/Models/Wheels_4.mdl");
                    if (carType == 6) scale = 6.3;
                    scale = 0.05f;

                    //Model *tireModel = cache->GetResource<Model>("Models/Vehicles/Yugo/Models/wheel.mdl");
                    pWheel->SetModel(tireModel);
                    pWheel->ApplyMaterialList("Models/Vehicles/SetA/Models/Wheels_4.txt");
//                    pWheel->ApplyMaterialList("Models/Vehicles/Sahin/Models/Wheel_FL.txt");
                    //pWheel->ApplyMaterialList("Models/Vehicles/Yugo/Models/wheel.txt");


                    //pWheel->SetMaterial(cache->GetResource<Material>("Offroad/Models/Materials/Tire.xml"));
                    wheelNode->SetScale(Vector3(scale/1.2f, scale, scale));
//                    wheelNode->SetScale(Vector3(scale, scale, scale));
                    wheelNode->SetRotation(connectionPoint.x_ >= 0.0 ? Quaternion(0.0f, 0.0f, 180.0f) : Quaternion(0.0f, 0.0f, -0.0f));
                    //wheelNode->SetRotation(connectionPoint.x_ >= 0.0 ? Quaternion(0.0f, 90.0f, 180.0f) : Quaternion(0.0f, 90.0f, -0.0f));
                    wheelNode->SetWorldPosition(Vector3(0,0,2.0f-forwardWeightOffset)+node_->GetWorldPosition() + node_->GetWorldRotation() * connectionPoints_[id]);
                    //wheelNode->SetWorldPosition(node_->GetWorldPosition() + node_->GetWorldRotation() * connectionPoints_[id]);


                    break;


            }
            pWheel->SetMaterial(cache->GetResource<Material>("Offroad/Models/Materials/Tire.xml"));
            pWheel->SetCastShadows(true);
            CreateEmitter(connectionPoints_[id]);
            pWheel->DrawDebugGeometry(dbgRenderer, true);

            raycastVehicle_->AddWheel(wheelNode, wheelDirection, wheelAxle, suspensionRestLength_, wheelRadius_, isFrontWheel);
            numWheels_ = raycastVehicle_->GetNumWheels();
            prevWheelInContact_.Resize(numWheels_);

            raycastVehicle_->SetWheelSuspensionStiffness(id, suspensionStiffness_);
            raycastVehicle_->SetWheelDampingRelaxation(id, suspensionDamping_);
            raycastVehicle_->SetWheelDampingCompression(id, suspensionCompression_);
            raycastVehicle_->SetWheelFrictionSlip(id, wheelFriction_);
            raycastVehicle_->SetWheelRollInfluence(id, rollInfluence_);

            prevWheelInContact_[id] = false;
            //front: 0.8f
            //rear: 0.04f
            // side friction stiffness is different for front and rear wheels
            if (id < 2)
            {
//                raycastVehicle_->SetWheelSideFrictionStiffness(id, 0.9f);
                raycastVehicle_->SetWheelSideFrictionStiffness(id, 0.8f);
            }
            else
            {
                m_fRearSlip = MAX_REAR_SLIP;
                raycastVehicle_->SetWheelSideFrictionStiffness(id, MAX_REAR_SLIP);
            }
        }
        emittersCreated = true;
        raycastVehicle_->ResetWheels();


        // Setup lighting

        Node *lightNode = node_->CreateChild("Lights");
        lightNode->SetPosition(Vector3(0.0f, 140.0f, 0));
        // Update vehicle head lamp lighting
        Light *light = lightNode->CreateComponent<Light>();
        // Light* light = vehicleHeadLamp_->CreateComponent<Light>();
        //light->SetLightType(LIGHT_SPOT)
        light->SetLightType(LIGHT_POINT);
        light->SetRange(140.0f);

        float rpmLightFactor = 40.0f;
        //float rpmLight = rpm / 4500.0f;
        //if (rpmLight > 0.8f) rpmLight = 0.8f;
        //light->SetBrightness(0.2f + (rpmLight * rpmLightFactor));
        light->SetBrightness(0.7f);
        light->SetCastShadows(true);


        // Setup sounds

        String prefix = "Sounds/";
        // init sound
        engineSoundSrc_ = node_->CreateComponent<SoundSource3D>();
        engineSnd_ = cache->GetResource<Sound>(prefix + driveAudioEffect_[SOUND_FX_ENGINE_LOOP].c_str());
        engineSnd_->SetLooped(true);

        engineSoundSrc_->SetDistanceAttenuation( 1.0f, 30.0f, 0.1f );
        engineSoundSrc_->SetSoundType(SOUND_EFFECT);
        engineSoundSrc_->SetGain(0.7f);
        engineSoundSrc_->Play(engineSnd_);
        engineSoundSrc_->SetFrequency(AUDIO_FIXED_FREQ_44K * 0.05f);



        skidSoundSrc_ = node_->CreateComponent<SoundSource3D>();
        skidSnd_ = cache->GetResource<Sound>(prefix + driveAudioEffect_[SOUND_FX_TIRE_SKID1].c_str());
        skidSoundSrc_->SetSoundType(SOUND_EFFECT);
        skidSoundSrc_->SetGain(0.4f);
        skidSoundSrc_->SetDistanceAttenuation( 1.0f, 30.0f, 0.1f );
        skidSoundSrc_->SetFrequency(AUDIO_FIXED_FREQ_44K * 1.4f );

        shockSoundSrc_ = node_->CreateComponent<SoundSource3D>();
        shockSnd_ = cache->GetResource<Sound>(prefix + driveAudioEffect_[SOUND_FX_TIRE_SKID3].c_str());
        shockSoundSrc_->SetSoundType(SOUND_EFFECT);
        shockSoundSrc_->SetGain(0.7f);
        shockSoundSrc_->SetDistanceAttenuation( 1.0f, 30.0f, 0.1f );

        SetUpdateEventMask(USE_FIXEDUPDATE | USE_POSTUPDATE);

        // node collision
        SubscribeToEvent(GetNode(), E_NODECOLLISION, URHO3D_HANDLER(Vehicle, HandleVehicleCollision));
    }
}


void Vehicle::PostUpdateSound(float timeStep)
{
    int playSkidSound = 0;
    bool playShockImpactSound = false;

    if (raycastVehicle_) {

        if (raycastVehicle_->GetNumWheels() == 4) {

            for (int i = 0; i < numWheels_; ++i) {
                btWheelInfo whInfo = raycastVehicle_->GetWheel(i);

                // skid sound
                if (raycastVehicle_->WheelIsGrounded(i)) {
                    if (raycastVehicle_->GetWheelSkidInfoCumulative(i) < 0.9f) {
                        playSkidSound++;
                    }


                    // shock impact
                    if (!prevWheelInContact_[i]) {

                        Vector3 velAtWheel = raycastVehicle_->GetBody()->GetVelocityAtPoint(
                                ToVector3(whInfo.m_chassisConnectionPointCS));
                        float downLinVel = velAtWheel.DotProduct(Vector3::DOWN);
                        if (downLinVel > MIN_SHOCK_IMPACT_VEL) {
                            playShockImpactSound = true;
                        }
                    }
                }

                // update prev wheel in contact
                prevWheelInContact_[i] = whInfo.m_raycastInfo.m_isInContact;


            }

            // -ideally, you want the engine sound to sound like it's at 10k rpm w/o any pitch adjustment, and
            // we nomralize x to be from 0.1f to 1.0f by dividing by 10k in SetFrequency(AUDIO_FIXED_FREQ_44K * x)
            // -if shifting rmps sounds off then change the normalization value. for the engine prototype sound,
            // the pitch sound is low, so it's normalized by diving by 8k instead of 10k
            const float rpmNormalizedForEnginePrototype = 8000.0f;
            //engineSoundSrc_->SetFrequency(AUDIO_FIXED_FREQ_44K * curRPM_ / rpmNormalizedForEnginePrototype);

            // shock impact when transitioning from partially off ground (or air borne) to landing
            if (numWheelContacts_ <= 2 && playShockImpactSound) {
                if (!shockSoundSrc_->IsPlaying()) {
                    shockSoundSrc_->Play(shockSnd_);
                }
            }

            // skid sound
            if (playSkidSound > 1) {
                if (!skidSoundSrc_->IsPlaying()) {
                    skidSoundSrc_->Play(skidSnd_);
                }
            } else {
                skidSoundSrc_->Stop();
            }
        }
    }
}


void Vehicle::HandleVehicleCollision(StringHash eventType, VariantMap & eventData) {
    using namespace NodeCollision;

    // decrease health if player player collides with boids
    Node *collidedNode = static_cast<Node *>(eventData[P_OTHERNODE].GetPtr());

    // URHO3D_LOGDEBUGF("collide node name: [%s]", collidedNode->GetName().CString());
    // Get the other colliding body, make sure it is moving (has nonzero mass)
    auto *otherBody = static_cast<RigidBody *>(eventData[P_OTHERBODY].GetPtr());

    // Pull producer id from name
    Urho3D::String name = collidedNode->GetName();
    // Start after hyphen
    Urho3D::String producerIdStr = name.Substring(name.Find("-") + 1);
    int producerId = Urho3D::ToInt(producerIdStr.CString());

    /*
    missile-1256
    [Wed Aug 19 14:18:28 2020] DEBUG: collide node name: [missile-1256]
    [Wed Aug 19 14:18:28 2020] DEBUG: Vehicle::HandleVehicleCollision() -> MISSILE: [Node, 1515, missile-1256]
    [Wed Aug 19 14:18:28 2020] DEBUG: Vehicle::HandleVehicleCollision() node [1256] -> OPPONENT COLLISION at node [1515]
*/

    if (collidedNode->GetName() == "MysteryPickup")
    {
        URHO3D_LOGDEBUGF("***** Vehicle::FOUND PICKUP: [%s]", collidedNode->GetName().CString());
    }


    // MISSILE COLLISION
    if (collidedNode->GetName().StartsWith("missile")) {
        auto *collidedRB = static_cast<RigidBody *>(eventData[P_OTHERBODY].GetPtr());
//        auto* collidedMissile = static_cast<Node*>(eventData[{P_OTHERNODE}].GetPtr());

/*        URHO3D_LOGDEBUGF("Vehicle::HandleVehicleCollision() -> MISSILE: [%s, %d, %s] PRODUCER ID %d",
                         collidedNode->GetTypeName().CString(), collidedNode->GetID(),
                         collidedNode->GetName().CString(), producerId);
*/

        // Apply force against vehicle
        Vector3 collVel = otherBody->GetLinearVelocity();

        // Skip collision if with owner vehicle
        if (producerId == GetNode()->GetID()) {
/*            URHO3D_LOGDEBUGF(
                    "Vehicle::HandleVehicleCollision() node [%d] -> SELF COLLISION at node %d with magnitude %f",
                    GetNode()->GetID(), collidedNode->GetID(), collVel.Length());*/
        } else {
            /*URHO3D_LOGDEBUGF(
                    "Vehicle::HandleVehicleCollision() node [%d] -> OPPONENT COLLISION at node [%d] with magnitude %f",
                    GetNode()->GetID(), collidedNode->GetID(), collVel.Length());*/

            if (raycastVehicle_) {

                // Add upward forward
                collVel += Vector3(0.0, 40.0f, 0.0f);

                //raycastVehicle_->ApplyImpulse(collVel * 30.0f);

                // Remove missile
                collidedNode->Remove();
/*
            // apply downward force when some wheels are grounded
            if (numWheelContacts_ > 0 && numWheelContacts_ != numWheels_) {
                // small arbitrary multiplier
                const float velocityMultiplyer = 0.5f;
                Vector3 downNormal = node_->GetUp() * -1.0f;
                float velocityMag = raycastVehicle_->GetLinearVelocity().LengthSquared() * velocityMultiplyer;
                velocityMag = Clamp(velocityMag, MIN_DOWN_FORCE, MAX_DOWN_FORCE);
                raycastVehicle_->ApplyForce(velocityMag * downNormal);
            }*/
            }

        }
    } else {

        // ALL OTHER COLLISIONS


        auto *collidedRB = static_cast<RigidBody *>(eventData[P_OTHERBODY].GetPtr());
//        auto* collidedMissile = static_cast<Node*>(eventData[{P_OTHERNODE}].GetPtr());


        // THIS COULD BE NETWORK ACTOR TO VEHICLE COLLISION

/*
        URHO3D_LOGDEBUGF("Vehicle::HandleVehicleCollision() -> VEHICLE: [%s, %d, %s] PRODUCER ID %d",
                         collidedNode->GetTypeName().CString(), collidedNode->GetID(),
                         collidedNode->GetName().CString(), producerId);
*/
        // Apply force against vehicle
        Vector3 collVel = otherBody->GetLinearVelocity();
        float mag = collVel.Length();

        // Moving collisions
        if (mag > 1) {
            // Skip collision if with owner vehicle
            if (producerId == GetNode()->GetID()) {
     /*           URHO3D_LOGDEBUGF(
                        "Vehicle::HandleVehicleCollision() node [%d] -> SELF COLLISION at node %d with magnitude %f",
                        GetNode()->GetID(), collidedNode->GetID(), mag);*/
            } else {
/*                URHO3D_LOGDEBUGF(
                        "Vehicle::HandleVehicleCollision() node [%d] -> OPPONENT COLLISION at node [%d] with magnitude %f",
                        GetNode()->GetID(), collidedNode->GetID(), collVel.Length());
*/


                if (collidedNode->GetName().StartsWith("Actor")) {

                    lastCollVel_ = collVel;
                    //
              /*      URHO3D_LOGDEBUGF(
                            "Vehicle::HandleVehicleCollision() node [%d] -> ******ACTOR HIT magnitude %f",
                            GetNode()->GetID(), mag);*/
                }

                // Add upward forward
                //collVel += Vector3(0.0, 40.0f, 0.0f);

                //raycastVehicle_->ApplyImpulse(collVel * 30.0f);

                // Remove missile
//                    collidedNode->Remove();
/*
            // apply downward force when some wheels are grounded
            if (numWheelContacts_ > 0 && numWheelContacts_ != numWheels_) {
                // small arbitrary multiplier
                const float velocityMultiplyer = 0.5f;
                Vector3 downNormal = node_->GetUp() * -1.0f;
                float velocityMag = raycastVehicle_->GetLinearVelocity().LengthSquared() * velocityMultiplyer;
                velocityMag = Clamp(velocityMag, MIN_DOWN_FORCE, MAX_DOWN_FORCE);
                raycastVehicle_->ApplyForce(velocityMag * downNormal);
            }
                }

            }*/

            }
        }
    }
}

void Vehicle::Flip(float duration) {
    // Retrieve rigid body
    //body->ApplyImpulse(Vector3(12.0, 40.0, 0.0));
//    Vector3 direction = Vector3(0,0,1); // front
//    float mag = 10000.0f;
    // Apply impulse force in ray direction for duration of time
  //Vector3 impRayDir = -body_->GetRotation()*(direction * mag)*duration;
    //raycastVehicle_->ApplyImpulse(impRayDir);
  //  raycastVehicle_->Flip();
    //body_->SetRotation(-body_->GetRotation());
//    URHO3D_LOGDEBUGF("**[%f] [Vehicle %s]::FLIP -> body->ApplyImpulse [%f,%f,%f]", duration, this->userName_.CString(), impRayDir.x_, impRayDir.y_, impRayDir.z_);

}


void Vehicle::ApplyBooster() {
    boosterLevel_ = 5.0f;
}


void Vehicle::Fire() {

}


void Vehicle::UpdateSteering(float newSteering)
{
    // gradual change
    if ( newSteering != 0.0f )
    {
        steering_ += newSteering * m_fsteeringIncrement;
    }
    else
    {
        steering_ *= 0.90f;
    }

    steering_ = Clamp( steering_, -m_fsteeringClamp, m_fsteeringClamp );

    // angular velocity
    if ( Abs(steering_ ) > m_fsteeringClamp * 0.75f )
    {
        m_fYAngularVelocity += 0.2f;
    }
    else
    {
        m_fYAngularVelocity *= 0.98f;
    }

    m_fYAngularVelocity = Clamp(m_fYAngularVelocity, 2.0f, 4.0f);

    // apply value
    m_fVehicleSteering = steering_;

    for ( int i = 0; i < 2; ++i )
    {
        raycastVehicle_->SetSteeringValue( m_fVehicleSteering, i );
    }
}

void Vehicle::ApplyEngineForces(float accelerator, bool braking)
{
    // 4x wheel drive
    const float numDriveTrains = 2.0f;
    const float invNumDriveTrains = 1.0f/numDriveTrains;

    isBraking_ = braking;
    currentAcceleration_ = accelerator;
    m_fBreakingForce = braking?m_fmaxBreakingForce*0.5f:0.0f;
    m_fEngineForce = m_fmaxEngineForce * accelerator * invNumDriveTrains;

    // Apply booster level (booster level = 1.0f, no effect)
    m_fEngineForce = m_fEngineForce * boosterLevel_;

    for ( int i = 0; i < numWheels_; ++i )
    {
        // 4x wheel drive
        raycastVehicle_->SetEngineForce(i, m_fEngineForce);

        // apply brake to rear wheels only
        if (i > 1)
        {
            raycastVehicle_->SetBrake(i, m_fBreakingForce);
        }
    }
}

bool Vehicle::ApplyStiction(float steering, float acceleration, bool braking)
{
    const float vel = raycastVehicle_->GetBody()->GetLinearVelocity().Length();
    const float absAccel = Abs(acceleration);
    const float absSteer = Abs(steering);
    bool setStiction = false;

    if ( absSteer < M_EPSILON && absAccel < M_EPSILON &&
         numWheelContacts_ > 0 && vel < MIN_STICTION_VEL )
    {
        setStiction = true;
    }

    // slow down and change rolling friction on stiction
    for ( int i = 0; i < numWheels_; ++i )
    {
        btWheelInfo wheel = raycastVehicle_->GetWheel(i);

        if ( absAccel < M_EPSILON && !braking && vel < MIN_SLOW_DOWN_VEL )
        {
            raycastVehicle_->SetBrake( MIN_BRAKE_FORCE, i );
        }

        if ( setStiction )
        {
            wheel.m_rollInfluence = Lerp(rollInfluence_, 1.0f, 1.0f - vel/MIN_STICTION_VEL);
        }
        else
        {
            wheel.m_rollInfluence = rollInfluence_;
        }
    }

    return setStiction;
}

// Based on advice from Edy: https://forum.unity.com/threads/how-to-make-a-physically-real-stable-car-with-wheelcolliders.50643/
void Vehicle::ApplyAntiRollBar()
{

    bool groundFL = false;
    bool groundFR = false;
    bool groundBL = false;
    bool groundBR = false;
    bool wheelHit = false;


/*
    if (raycastVehicle_) {

        if (raycastVehicle_->GetSpeedKm() > 10.0f) {
            if (raycastVehicle_->GetNumWheels() == 4) {

                Vector3 vel = raycastVehicle_->GetBody()->GetLinearVelocity();
                for (int i = 0; i < numWheels_; ++i) {
                    btWheelInfo whInfo = raycastVehicle_->GetWheel(i);

                    if (raycastVehicle_->WheelIsGrounded(i)) {

                        switch (i) {
                            case 0: {
                                // FRONT LEFT
                                groundFL = true;
                                travelFL = whInfo.m_raycastInfo.m_suspensionLength / whInfo.getSuspensionRestLength();
                                break;
                            }

                            case 1: {
                                // FRONT RIGHT
                                groundFR = true;
                                travelFR = whInfo.m_raycastInfo.m_suspensionLength / whInfo.getSuspensionRestLength();
                                break;
                            }

                            case 2: {
                                // BACK LEFT
                                groundBL = true;
                                travelBL = whInfo.m_raycastInfo.m_suspensionLength / whInfo.getSuspensionRestLength();
                                break;
                            }

                            case 3: {
                                // BACK RIGHT
                                groundBR = true;
                                travelBR = whInfo.m_raycastInfo.m_suspensionLength / whInfo.getSuspensionRestLength();
                                break;
                            }
                        }


                    } // End of Grounded wheel


                } // End of wheel loop

                // Apply force at front
                float antiRollForceF = (travelFL - travelFR) * antiRoll;

                if (groundFL) {
                    Vector3 force = Vector3(0, raycastVehicle_->GetWheel(0).m_worldTransform.getOrigin().y() *
                                               -antiRollForceF, 0);
                    // Front wheel
                    raycastVehicle_->GetBody()->ApplyForce(force, Vector3(raycastVehicle_->GetWheel(
                            0).m_worldTransform.getOrigin()));
                }

                if (groundFR) {
                    Vector3 force = Vector3(0, raycastVehicle_->GetWheel(1).m_worldTransform.getOrigin().y() *
                                               antiRollForceF, 0);
                    // Front wheel
                    raycastVehicle_->GetBody()->ApplyForce(force, Vector3(raycastVehicle_->GetWheel(
                            1).m_worldTransform.getOrigin()));
                }


                // Apply force at back
                float antiRollForceB = (travelBL - travelBR) * antiRoll;

                if (groundBL) {
                    Vector3 force = Vector3(0, raycastVehicle_->GetWheel(2).m_worldTransform.getOrigin().y() *
                                               -antiRollForceB, 0);
                    // Back wheel
                    raycastVehicle_->GetBody()->ApplyForce(force, Vector3(raycastVehicle_->GetWheel(
                            2).m_worldTransform.getOrigin()));
                }

                if (groundBR) {
                    Vector3 force = Vector3(0, raycastVehicle_->GetWheel(3).m_worldTransform.getOrigin().y() *
                                               antiRollForceB, 0);
                    // Back wheel
                    raycastVehicle_->GetBody()->ApplyForce(force, Vector3(raycastVehicle_->GetWheel(
                            3).m_worldTransform.getOrigin()));
                }

            } // 4 wheels exist
        } // At least 10 km/h

    }*/



    if (raycastVehicle_) {

        if (numWheels_ >= 4 && raycastVehicle_->GetSpeedKm() > 10.0f) {
            float speedKmh = raycastVehicle_->GetSpeedKm();

            // Speed-dependent anti-roll force
            float speedFactor = Clamp(speedKmh / 100.0f, 0.3f, 2.0f);
            float baseAntiRoll = antiRoll * speedFactor;

            // Front anti-roll bar
            float travel_FL = raycastVehicle_->GetWheelSkidInfoCumulative(0);
            float travel_FR = raycastVehicle_->GetWheelSkidInfoCumulative(1);
            float antiRollForce_Front = (travel_FL - travel_FR) * baseAntiRoll;

            if (abs(antiRollForce_Front) > 1.0f) {
                Vector3 wheelPos_FL = raycastVehicle_->GetWheelPosition(0);
                Vector3 wheelPos_FR = raycastVehicle_->GetWheelPosition(1);

                raycastVehicle_->GetBody()->ApplyForce(
                        Vector3(0, -antiRollForce_Front, 0), wheelPos_FL);
                raycastVehicle_->GetBody()->ApplyForce(
                        Vector3(0, antiRollForce_Front, 0), wheelPos_FR);
            }

            // Rear anti-roll bar (slightly less aggressive)
            float travel_BL = raycastVehicle_->GetWheelSkidInfoCumulative(2);
            float travel_BR = raycastVehicle_->GetWheelSkidInfoCumulative(3);
            float antiRollForce_Rear = (travel_BL - travel_BR) * baseAntiRoll * 0.8f;

            if (abs(antiRollForce_Rear) > 1.0f) {
                Vector3 wheelPos_BL = raycastVehicle_->GetWheelPosition(2);
                Vector3 wheelPos_BR = raycastVehicle_->GetWheelPosition(3);

                raycastVehicle_->GetBody()->ApplyForce(
                        Vector3(0, -antiRollForce_Rear, 0), wheelPos_BL);
                raycastVehicle_->GetBody()->ApplyForce(
                        Vector3(0, antiRollForce_Rear, 0), wheelPos_BR);
            }
        }
    }
}



void Vehicle::ApplyDownwardForce()
{
    /*
    // apply downward force when some wheels are grounded
    if ( numWheelContacts_ > 0 && numWheelContacts_ != numWheels_ )
    {

        // Apply normal force relative to vehicle
        // small arbitrary multiplier
        //const float velocityMultiplyer = 0.92f;
        const float velocityMultiplyer = 100.12f;
        //const float velocityMultiplyer = 1.24f;
        Vector3 downNormal = node_->GetUp() * 1.0f;
        float velocityMag = raycastVehicle_->GetBody()->GetLinearVelocity().LengthSquared() * velocityMultiplyer;
        velocityMag = Clamp( velocityMag, MIN_DOWN_FORCE, MAX_DOWN_FORCE );
        Vector3 force = velocityMag * downNormal;
        raycastVehicle_->GetBody()->ApplyForce( force  );



        // Apply normal force relative to track, i.e. wheel contact normal

        Vector3 normPosWheel0 = raycastVehicle_->GetContactPosition(0);
        Vector3 normPosWheel1 = raycastVehicle_->GetContactPosition(1);
        Vector3 normPosWheel2 = raycastVehicle_->GetContactPosition(2);
        Vector3 normPosWheel3 = raycastVehicle_->GetContactPosition(3);

        Vector3 normWheel0 = -raycastVehicle_->GetContactNormal(0);
        Vector3 normWheel1 = -raycastVehicle_->GetContactNormal(1);
        Vector3 normWheel2 = -raycastVehicle_->GetContactNormal(2);
        Vector3 normWheel3 = -raycastVehicle_->GetContactNormal(3);



        float pushMag = 0.0f + raycastVehicle_->GetBody()->GetLinearVelocity().LengthSquared() * 20.0f + raycastVehicle_->GetRPM()*10.0f;
        pushMag = Clamp( pushMag, MIN_DOWN_FORCE, MAX_DOWN_FORCE );

/*
        // Normal force contact per wheel

        const float normVelMultiplyer = 20.25f;
        float normVelMag = 0;

        normVelMag = raycastVehicle_->GetBody()->GetLinearVelocity().LengthSquared() * normVelMultiplyer;
        normVelMag = Clamp( normVelMag, 1.0f, MAX_DOWN_FORCE*1000.0f );

        // Wheel 0
        Vector3 normForceWheel0 = normPosWheel0 + normVelMag * normWheel0;
        raycastVehicle_->GetBody()->ApplyForce( normForceWheel0 );

        // Wheel 1
        Vector3 normForceWheel1 = normPosWheel1 + normVelMag * normWheel1;
        raycastVehicle_->GetBody()->ApplyForce( normForceWheel1 );

        // Wheel 2
        Vector3 normForceWheel2 = normPosWheel2 + normVelMag * normWheel2;
        raycastVehicle_->GetBody()->ApplyForce( normForceWheel2 );

        // Wheel 3
        Vector3 normForceWheel3 = normPosWheel3 + normVelMag * normWheel3;
        raycastVehicle_->GetBody()->ApplyForce( normForceWheel3 );

*//*

        // Apply hard down force after wheel contact time is stable
        if (wheelContactTime_ > 1.0f) {
            // Front wheels - hood force
            raycastVehicle_->GetBody()->ApplyForce(force * 1, normPosWheel0);
            raycastVehicle_->GetBody()->ApplyForce(force * 1, normPosWheel1);
            //raycastVehicle_->GetBody()->ApplyForce(force, normPosWheel2);
            //raycastVehicle_->GetBody()->ApplyForce(force, normPosWheel3);

        }

    }
    */
    ////
/*
        // Apply normal force relative to vehicle
        // small arbitrary multiplier
        //const float velocityMultiplyer = 0.92f;
        const float velocityMultiplyer = 100.12f;
        //const float velocityMultiplyer = 1.24f;
        Vector3 downNormal = node_->GetUp() * 1.0f;
        float velocityMag = raycastVehicle_->GetBody()->GetLinearVelocity().LengthSquared() * velocityMultiplyer;
        velocityMag = Clamp( velocityMag, MIN_DOWN_FORCE, MAX_DOWN_FORCE );
        Vector3 force = velocityMag * downNormal;
        //raycastVehicle_->GetBody()->ApplyForce( force  );



        // Apply normal force relative to track, i.e. wheel contact normal

        Vector3 normPosWheel0 = raycastVehicle_->GetContactPosition(0);
        Vector3 normPosWheel1 = raycastVehicle_->GetContactPosition(1);
        Vector3 normPosWheel2 = raycastVehicle_->GetContactPosition(2);
        Vector3 normPosWheel3 = raycastVehicle_->GetContactPosition(3);

        Vector3 normWheel0 = -raycastVehicle_->GetContactNormal(0);
        Vector3 normWheel1 = -raycastVehicle_->GetContactNormal(1);
        Vector3 normWheel2 = -raycastVehicle_->GetContactNormal(2);
        Vector3 normWheel3 = -raycastVehicle_->GetContactNormal(3);



        float pushMag = 0.0f + raycastVehicle_->GetBody()->GetLinearVelocity().LengthSquared() * 20.0f + raycastVehicle_->GetRPM()*10.0f;
        pushMag = Clamp( pushMag, MIN_DOWN_FORCE, MAX_DOWN_FORCE );


        // Normal force contact per wheel

        const float normVelMultiplyer = 20.25f;
        float normVelMag = 0;

        normVelMag = raycastVehicle_->GetBody()->GetLinearVelocity().LengthSquared() * normVelMultiplyer;
        normVelMag = Clamp( normVelMag, 1.0f, MAX_DOWN_FORCE*1000.0f );

        // Wheel 0
        Vector3 normForceWheel0 = normPosWheel0 + normVelMag * normWheel0;
        raycastVehicle_->GetBody()->ApplyForce( normForceWheel0 );

        // Wheel 1
        Vector3 normForceWheel1 = normPosWheel1 + normVelMag * normWheel1;
        raycastVehicle_->GetBody()->ApplyForce( normForceWheel1 );

        // Wheel 2
        Vector3 normForceWheel2 = normPosWheel2 + normVelMag * normWheel2;
        raycastVehicle_->GetBody()->ApplyForce( normForceWheel2 );

        // Wheel 3
        Vector3 normForceWheel3 = normPosWheel3 + normVelMag * normWheel3;
        raycastVehicle_->GetBody()->ApplyForce( normForceWheel3 );

*/
/*
        // Apply hard down force after wheel contact time is stable
        if (wheelContactTime_ > 1.0f) {
            // Front wheels - hood force
            raycastVehicle_->GetBody()->ApplyForce(force * 1, normPosWheel0);
            raycastVehicle_->GetBody()->ApplyForce(force * 1, normPosWheel1);
            //raycastVehicle_->GetBody()->ApplyForce(force, normPosWheel2);
            //raycastVehicle_->GetBody()->ApplyForce(force, normPosWheel3);

        }
*/
    // Apply downward force when some wheels are grounded
    if (numWheelContacts_ > 0 && numWheelContacts_ != numWheels_) {
        // Enhanced velocity-based downward force calculation
        float speed = raycastVehicle_->GetBody()->GetLinearVelocity().Length();
        float speedKmh = raycastVehicle_->GetSpeedKm();

        // Progressive downward force that increases with speed
        // Base force + speed-dependent component
        const float BASE_DOWN_FORCE = 2000.0f;
        const float SPEED_MULTIPLIER = 150.0f; // Force per km/h
        const float MAX_DOWN_FORCE_CUSTOM = 15000.0f;

        float downwardForce = BASE_DOWN_FORCE + (speedKmh * SPEED_MULTIPLIER);
        downwardForce = Clamp(downwardForce, BASE_DOWN_FORCE, MAX_DOWN_FORCE_CUSTOM);

        Vector3 downNormal = node_->GetUp() * -1.0f;

        // Apply progressive force distribution based on speed
        if (speedKmh > 80.0f) {
            // At high speeds, apply more force to the front for stability
            float frontBias = Clamp((speedKmh - 80.0f) / 100.0f, 0.0f, 0.7f);
            float frontForce = downwardForce * (0.5f + frontBias);
            float rearForce = downwardForce * (0.5f - frontBias * 0.5f);

            // Front wheels get more downforce at high speed
            Vector3 frontPos = node_->GetPosition() + node_->GetDirection() * wheelSpace_;
            Vector3 rearPos = node_->GetPosition() - node_->GetDirection() * wheelSpace_;

            raycastVehicle_->GetBody()->ApplyForce(frontForce * downNormal, frontPos);
            raycastVehicle_->GetBody()->ApplyForce(rearForce * downNormal, rearPos);
        } else {
            // At lower speeds, apply force evenly
            raycastVehicle_->GetBody()->ApplyForce(downwardForce * downNormal);
        }
    }
}


void Vehicle::ApplyDragBrake()
{
    // apply downward force when some wheels are grounded
  //  if ( wheelContactTime_ > 0 )
  //  {
        // Apply normal force relative to vehicle
        // small arbitrary multiplier
        const float dragMultiplyer = 1.3778f;

   //     RigidBody *body = node_->GetComponent<RigidBody>(true);
        Vector3 dir = raycastVehicle_->GetBody()->GetNode()->GetDirection();
        float velocityMag = raycastVehicle_->GetBody()->GetLinearVelocity().LengthSquared() * dragMultiplyer;
        velocityMag = Clamp( velocityMag, MIN_DOWN_FORCE, MAX_DOWN_FORCE );
        Vector3 dragForce = velocityMag * dir * -1.0f;
        dragBrake_ = dragForce;

        // Apply drag brake
        raycastVehicle_->GetBody()->ApplyForce(dragBrake_);


        // Apply normal force relative to track, i.e. wheel contact normal

        Vector3 normPosWheel0 = raycastVehicle_->GetContactPosition(0);
        Vector3 normPosWheel1 = raycastVehicle_->GetContactPosition(1);
        Vector3 normPosWheel2 = raycastVehicle_->GetContactPosition(2);
        Vector3 normPosWheel3 = raycastVehicle_->GetContactPosition(3);

        Vector3 normWheel0 = -raycastVehicle_->GetContactNormal(0);
        Vector3 normWheel1 = -raycastVehicle_->GetContactNormal(1);
        Vector3 normWheel2 = -raycastVehicle_->GetContactNormal(2);
        Vector3 normWheel3 = -raycastVehicle_->GetContactNormal(3);



        float pushMag = 10.0f + raycastVehicle_->GetBody()->GetLinearVelocity().LengthSquared() * 20.0f + raycastVehicle_->GetRPM()*10.0f;
        pushMag = Clamp( pushMag, MIN_DOWN_FORCE, MAX_DOWN_FORCE );

/*
        // Normal force contact per wheel

        const float normVelMultiplyer = 20.25f;
        float normVelMag = 0;

        normVelMag = raycastVehicle_->GetBody()->GetLinearVelocity().LengthSquared() * normVelMultiplyer;
        normVelMag = Clamp( normVelMag, 1.0f, MAX_DOWN_FORCE*1000.0f );

        // Wheel 0
        Vector3 normForceWheel0 = normPosWheel0 + normVelMag * normWheel0;
        raycastVehicle_->GetBody()->ApplyForce( normForceWheel0 );

        // Wheel 1
        Vector3 normForceWheel1 = normPosWheel1 + normVelMag * normWheel1;
        raycastVehicle_->GetBody()->ApplyForce( normForceWheel1 );

        // Wheel 2
        Vector3 normForceWheel2 = normPosWheel2 + normVelMag * normWheel2;
        raycastVehicle_->GetBody()->ApplyForce( normForceWheel2 );

        // Wheel 3
        Vector3 normForceWheel3 = normPosWheel3 + normVelMag * normWheel3;
        raycastVehicle_->GetBody()->ApplyForce( normForceWheel3 );

*/

        // Apply hard down force after wheel contact time is stable
        if (wheelContactTime_ > 1.0f) {
            // Front wheels - hood force
//          Update drag brake vector
//             dragBrake_ = dragBrake_;
            // Apply drag brake
            //raycastVehicle_->GetBody()->ApplyForce(force * 1, normPosWheel0);
            //raycastVehicle_->GetBody()->ApplyForce(force * 1, normPosWheel1);
            //raycastVehicle_->GetBody()->ApplyForce(force, normPosWheel2);
            //raycastVehicle_->GetBody()->ApplyForce(force, normPosWheel3);

        }

   // }
}

const Vector3 &Vehicle::GetDragBrake() const {
    return dragBrake_;
}


void Vehicle::AutoCorrectPitchRoll()
{
    // auto correct pitch and roll while air borne
    if (numWheelContacts_ == 0)
    {
        //predictedUp eqn. from https://discourse.urho3d.io/t/constraint-class-working-on-derived-class/4081/6
        const float stability = 0.3f;
        const float speed = 1.5f;
        // Retrieve rigid body
        RigidBody *body = node_->GetComponent<RigidBody>(true);
        Vector3 predictedUp = Quaternion(raycastVehicle_->GetBody()->GetAngularVelocity().Length() * M_DEGTORAD * stability / speed,
                                         raycastVehicle_->GetBody()->GetAngularVelocity()) * node_->GetUp();
        Vector3 torqueVector = predictedUp.CrossProduct(Vector3::UP);
        torqueVector *= speed * speed * body->GetMass();
        raycastVehicle_->GetBody()->ApplyTorque(torqueVector);
    }
}

void Vehicle::LimitLinearAndAngularVelocity()
{
    // velocity limit
    Vector3 linVel = raycastVehicle_->GetBody()->GetLinearVelocity();
    if ( linVel.Length() > MAX_LINEAR_VEL_LIMIT )
    {
        raycastVehicle_->GetBody()->SetLinearVelocity( linVel.Normalized() * MAX_LINEAR_VEL_LIMIT );
    }

    // angular velocity limiters
    Vector3 v3AngVel = raycastVehicle_->GetBody()->GetAngularVelocity();
    v3AngVel.x_ = Clamp( v3AngVel.x_, -MAX_ANGULAR_VEL_LIMIT,  MAX_ANGULAR_VEL_LIMIT );
    v3AngVel.y_ = Clamp( v3AngVel.y_, -m_fYAngularVelocity,    m_fYAngularVelocity );
    v3AngVel.z_ = Clamp( v3AngVel.z_, -MAX_ANGULAR_VEL_LIMIT,  MAX_ANGULAR_VEL_LIMIT );
    raycastVehicle_->GetBody()->SetAngularVelocity( v3AngVel );
}

void Vehicle::UpdateGear()
{
    float curSpdMph = GetSpeedMPH();
    int gearIdx = 0;

    // no negative speed value
    if (curSpdMph < 0.0f) curSpdMph *= -1.0f;

    for ( int i = 0; i < (int)gearShiftSpeed_.Size()-1; ++i )
    {
        if (curSpdMph > gearShiftSpeed_[i])
        {
            gearIdx = i + 1;
        }
    }

    // up or down shift when a wheel is in contact with the ground
    if ( gearIdx != curGearIdx_ && numWheelContacts_ > 0 )
    {
        curRPM_ = upShiftRPM_ * curSpdMph/gearShiftSpeed_[curGearIdx_];

        if ( curRPM_ < downShiftRPM_ )
        {
            if (curGearIdx_ > 0)
            {
                curGearIdx_--;
            }
        }
        else if ( gearIdx > curGearIdx_ )
        {
            curGearIdx_++;
        }
    }

    // final rpm
    curRPM_ = upShiftRPM_ * curSpdMph / gearShiftSpeed_[curGearIdx_];

    if (curGearIdx_ == 0)
    {
        minIdleRPM_ = Random(1000.0f, 1025.0f);
        if ( curRPM_ < MIN_IDLE_RPM ) curRPM_ += minIdleRPM_;
    }
}

void Vehicle::UpdateDrift()
{
    // rear wheel slip condition values
    // -note1: these are rough constants, you may want to re-evaluate them
    // -note2: changes to center of mass and size of inertia change drift behavior
    const float slipConditon0 = 0.00f; // ice
    const float slipConditon1 = 0.01f; // wet pavement
    const float slipConditon2 = 0.02f; // loose dirt
    //const float slipConditon3 = 0.04f; // dirt
    const float slipConditon3 = 0.41f; // sticky dirt
    const float slipConditon4 = 0.06f; // pavement

    // set slip
    const float slipConditionValue = slipConditon3;
    const float slipMax = MAX_REAR_SLIP;

    // for demo purpose, limit the drift speed to provide high speed steering experience w/o any drifting
    const float maxDriftSpeed = 10.0f;
    const float absSteeringVal = Abs( raycastVehicle_->GetSteeringValue(0) );
    const float curSpdMph = GetSpeedMPH();

    // set rear wheel slip values
    for ( int i = 2; i < numWheels_; ++i )
    {
        // re-calc the slip value only once
        if (i == 2)
        {
            if ( currentAcceleration_ > 0.0f )
            {
                const float slipMin = (curSpdMph < maxDriftSpeed)?slipConditionValue:slipMax;
                const float slipAdj = Lerp(slipMax, slipMin, absSteeringVal/m_fsteeringClamp);
                float deltaSlip = slipAdj - m_fRearSlip;

                m_fRearSlip += deltaSlip * 0.05f;
                m_fRearSlip = Clamp(m_fRearSlip, slipConditionValue, slipMax);
            }
            else
            {
                m_fRearSlip = slipMax;
            }
        }

        // set value
        raycastVehicle_->GetWheel(i).m_sideFrictionStiffness = m_fRearSlip;
    }
}

void Vehicle::DebugDraw() {


    if (GetScene()) {
        DebugRenderer *dbgRenderer = GetScene()->GetComponent<DebugRenderer>();

        if (dbgRenderer) {
            if (raycastVehicle_) {
                // Call parent debug draw
                this->raycastVehicle_->DebugDraw(dbgRenderer);

            }
        }
    }
}

float Vehicle::getDesiredSteer() const {
    return desiredSteer_;
}

void Vehicle::setDesiredSteer(float desiredSteer) {
    desiredSteer_ = desiredSteer;
}

int Vehicle::getSteerIndex() const {
    return steerIndex_;
}

void Vehicle::setSteerIndex(int steerIndex) {
    steerIndex_ = steerIndex;
}

Node *Vehicle::getActorNode() const {
    return actorNode_;
}

void Vehicle::setActorNode(Node *actorNode) {
    actorNode_ = actorNode;
}

float Vehicle::getWheelContactTime() const {
    return wheelContactTime_;
}

void Vehicle::setEnableControls(bool enableControls) {
    enableControls_ = enableControls;
}

int Vehicle::getCarType() const {
    return carType;
}
