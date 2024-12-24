#include <Urho3D/Core/Context.h>
#include <Urho3D/Scene/Scene.h>
#include <Urho3D/Graphics/Light.h>
#include <Urho3D/Graphics/Material.h>
#include <Urho3D/Graphics/Model.h>
#include <Urho3D/Graphics/StaticModel.h>
#include <Urho3D/Physics/RigidBody.h>
#include <Urho3D/Physics/CollisionShape.h>
#include <Urho3D/Resource/ResourceCache.h>
#include <Urho3D/Input/Input.h>
#include <Urho3D/UI/Text3D.h>
#include <Urho3D/UI/Font.h>
#include <Urho3D/IO/Log.h>
#include <Urho3D/Network/Connection.h>
#include <Urho3D/Physics/PhysicsUtils.h>
#include <Urho3D/Math/MathDefs.h>
#include <Urho3D/Graphics/DebugRenderer.h>
#include <Urho3D/Core/Variant.h>


#include "NetworkActor.h"
#include "Urho3D/IO/MemoryBuffer.h"

#include <Urho3D/DebugNew.h>
#include <AlgebraKart/Missile.h>
#include <AlgebraKart/Pickup.h>

#define PI 3.1415926

/*
String mdlFile = "Models/Player/Shino/FBX/Models/Shino.mdl";
                                            //Models/Armature_idle_01_idle_01.ani
String idleAniFile = "Models/Player/Shino/FBX/Models/Armature_idle_01_idle_01.ani";
String walkAniFile = "Models/Player/Shino/FBX/Models/Armature_walk_walk.ani";
String matFile = "Models/Player/Shino/FBX/Models/Shino.txt";

*/

#define REST_VELOCITY_THRESHOLD 0.5f
String mdlFile = "Models/Player/Bino/Models/f_8.mdl";
String naMarkerMdlFile = "Models/Tracks/Models/circle.mdl";
String naMarkerAMdlFile = "Models/Objects/Models/cursor1.mdl";
String naMarkerAMatFile = "Models/Objects/Models/cursor1.txt";
String naMarkerBMdlFile = "Models/Objects/Models/cursor2.mdl";
String naMarkerBMatFile = "Models/Objects/Models/cursor2.txt";
String naMarkerCMdlFile = "Models/Objects/Models/cursor3.mdl";
String naMarkerCMatFile = "Models/Objects/Models/cursor3.txt";

////code/dev/MonkeyMaya_com/bin/Data/Models/Objects/Models/cursor1.mdl


//Models/Armature_idle_01_idle_01.ani
//String idleAniFile = "Models/Player/Bino/Models/r_f_8_r_f_8_Run_r_f_8_Run.ani";
//String walkAniFile = "Models/Player/Bino/Models/r_f_8_r_f_8_Run_r_f_8_Run.ani";
String idleAniFile = "Models/Player/Bino/Models/Idle.ani";
String walkAniFile = "Models/Player/Bino/Models/Walk.ani";
String runAniFile = "Models/Player/Bino/Models/Run.ani";
String jumpAniFile = "Models/Player/Bino/Models/Victory.ani";
String victoryAniFile = "Models/Player/Bino/Models/Victory.ani";
String defeatAniFile = "Models/Player/Bino/Models/Defeat.ani";
String matFile = "Models/Player/Bino/Models/f_8.txt";



//=============================================================================
// NETWORK ACTOR
//=============================================================================
NetworkActor::NetworkActor(Context *context)
        : ClientObj(context), mass_(10.0f),
        alive_(true),
        onVehicle_(false),
        showMarker_(false),
        enableControls_(false),
        initialSet_(false),
          onGround_(false),
          okToJump_(true),
          inAirTimer_(0.0f)
        {
      SetUpdateEventMask(USE_NO_EVENT);

    SetLife(100);
    SetPrana(25);
    SetSpeed(0.0f);
    SetMaxSpeed(5.0f);
    SetDamping(0.015f);
    SetAcceleration(0.03f);
    SetBrake(0.05f);
    SetTowards(Vector3(0.0f, 1.0f, 0.0f));
    SetBulletType("AP");

    lastFire_ = 0;
    toTarget_ = Vector3(0,0,0);
    targetAgentIndex_ = 0;
    disconnected_ = false;


    thrust_   = 1024.0f;
    maxSpeed_ = 100.23f;

    doJump_ = false;
    onGround_ = false;
    acceleration_ = 0.0f;

    entered_ = false;

    lastWaypoint_ = 0;
    lapNum_ = 1;
    lapTime_ = 0;
    pickupTime_ = 0;
    pickups_ = nullptr;

    numPickups_ = 0;
    activePickup_ = 0;
}

NetworkActor::~NetworkActor() {

    SetUpdateEventMask(USE_NO_EVENT);
    Kill();
}

void NetworkActor::Kill() {
    if (this) {
        URHO3D_LOGINFOF("**** DESTROYING NetworkActor OBJECT -> %d", this->id_);
        if (vehicle_) {
            vehicle_->SetEnabled(false);
            vehicle_->Kill();
        }

        if (node_) {
            URHO3D_LOGINFOF("**** DESTROYING CLIENT NODE OBJECT -> %d", this->id_);
            //node_->RemoveAllChildren();
            node_->Remove();
        }

        this->killed_ = true;
    }
}

void NetworkActor::SetScene(Scene *scene) {
    scene_ = scene;
}

void NetworkActor::RegisterObject(Context *context) {
    context->RegisterFactory<NetworkActor>();
    context->RegisterFactory<Missile>();
    context->RegisterFactory<Pickup>();

    URHO3D_COPY_BASE_ATTRIBUTES(ClientObj);

    ClientObj::RegisterObject(context);

    //URHO3D_ACCESSOR_ATTRIBUTE("Is Enabled", IsEnabled, SetEnabled, bool, true, AM_DEFAULT);
    //URHO3D_ATTRIBUTE("Player Name", String, name_, String::EMPTY, AM_DEFAULT | AM_NET);


    URHO3D_ACCESSOR_ATTRIBUTE("Is Enabled", IsEnabled, SetEnabled, bool, true, AM_DEFAULT);

    URHO3D_ATTRIBUTE("Controls Yaw", float, controls_.yaw_, 0.0f, AM_DEFAULT);
    URHO3D_ATTRIBUTE("Controls Pitch", float, controls_.pitch_, 0.0f, AM_DEFAULT);
    URHO3D_ATTRIBUTE("On Ground", bool, onGround_, false, AM_DEFAULT);
    URHO3D_ATTRIBUTE("OK To Jump", bool, okToJump_, true, AM_DEFAULT);
    URHO3D_ATTRIBUTE("In Air Timer", float, inAirTimer_, 0.0f, AM_DEFAULT);

    URHO3D_ATTRIBUTE("On Vehicle", bool, onVehicle_, false, AM_DEFAULT);
    URHO3D_ATTRIBUTE("Entered", bool, entered_, false, AM_DEFAULT);
    URHO3D_ATTRIBUTE("Enable Controls", bool, enableControls_, false, AM_DEFAULT);

    URHO3D_ATTRIBUTE("Num Pickups", int, numPickups_, 0, AM_DEFAULT);
    URHO3D_ATTRIBUTE("Active Pickup", int, activePickup_, 0, AM_DEFAULT);

    URHO3D_ATTRIBUTE("Life", int, life_, 0, AM_DEFAULT);
    URHO3D_ATTRIBUTE("Prana", int, prana_, 0, AM_DEFAULT);

    URHO3D_ATTRIBUTE("RPM", float, rpm_, 0, AM_DEFAULT);
    URHO3D_ATTRIBUTE("Velocity", float, velocity_, 0, AM_DEFAULT);
    URHO3D_ATTRIBUTE("Steer", float, steer_, 0, AM_DEFAULT);

//    URHO3D_ATTRIBUTE("Bone Axis", Vector3, boneAxis_, Vector3(0, 0, 1), AM_DEFAULT);

/*    URHO3D_ATTRIBUTE("Stiffness", float, stiffness_, 0.1f, AM_DEFAULT);
    URHO3D_ATTRIBUTE("Mass", float, mass_, 0.9f, AM_DEFAULT);
    URHO3D_ATTRIBUTE("Damping", float, damping_, 0.75f, AM_DEFAULT);
    URHO3D_ATTRIBUTE("Gravity", float, gravity_, 0.75f, AM_DEFAULT);

    URHO3D_ATTRIBUTE("Allow Stretch", bool, squashAndStretch_, true, AM_DEFAULT);
    URHO3D_ATTRIBUTE("Side Stretch", float, sideStretch_, 0.15f, AM_DEFAULT);
    URHO3D_ATTRIBUTE("Front Stretch", float, frontStretch_, 0.15f, AM_DEFAULT);
*/
    /*
    URHO3D_ATTRIBUTE("On Vehicle", bool, onVehicle_, 0.0f, AM_DEFAULT);
    URHO3D_ATTRIBUTE("Alive", bool, alive_, 0.0f, AM_DEFAULT);
    URHO3D_ATTRIBUTE("Can Enter", float, canEnter_, 0.0f, AM_DEFAULT);
    URHO3D_ATTRIBUTE("Entered", bool, entered_, 0.0f, AM_DEFAULT);
    URHO3D_ATTRIBUTE("Show Marker", bool, showMarker_, 0.0f, AM_DEFAULT);
    URHO3D_ATTRIBUTE("Mark Type", int, markType_, 0.0f, AM_DEFAULT);
*/

}

void NetworkActor::DelayedStart()
{
    Create();
}

void NetworkActor::Init(Node* node) {

    auto* cache = GetSubsystem<ResourceCache>();

    node_ = node;
    if (node_) {


        Node* objectNode = node_->CreateChild("Actor", REPLICATED);
        //modelNode->SetScale(4.0f);
        objectNode->SetScale(4.0f);
        objectNode->SetPosition(Vector3(0.0f,1.0f, 0.0f));
//        objectNode->SetRotation(Quaternion(0, -90, 0.0f));

        // spin node
        Node* adjustNode = objectNode->CreateChild("AdjNode");
        //adjustNode->SetRotation( Quaternion(-90, Vector3(0,1,0) ) );
        adjustNode->SetRotation( Quaternion(-90, Vector3(0,1,0) ) );

        Node* markerNode = adjustNode->CreateChild("Marker", REPLICATED);
        //markerNode->SetScale(4.0f);
        markerNode->SetPosition(Vector3(0,1.0f,0));


        model_ = adjustNode->CreateComponent<AnimatedModel>();
        body_ = objectNode->CreateComponent<RigidBody>();
        collisionShape_ = objectNode->CreateComponent<CollisionShape>();
        animCtrl_ = adjustNode->CreateComponent<AnimationController>();


        // Create marker on model adjust node
        markers_.Push(markerNode->CreateComponent<StaticModel>());
        markers_[0]->SetModel(cache->GetResource<Model>(naMarkerAMdlFile));
        markers_[0]->ApplyMaterialList(naMarkerAMatFile);
        markers_[0]->SetEnabled(false);
        markers_.Push(markerNode->CreateComponent<StaticModel>());
        markers_[1]->SetModel(cache->GetResource<Model>(naMarkerBMdlFile));
        markers_[1]->ApplyMaterialList(naMarkerBMatFile);
        markers_[1]->SetEnabled(false);
        markers_.Push(markerNode->CreateComponent<StaticModel>());
        markers_[2]->SetModel(cache->GetResource<Model>(naMarkerCMdlFile));
        markers_[2]->ApplyMaterialList(naMarkerCMatFile);
        markers_[2]->SetEnabled(false);

        // Free actor mark
        markType_ = 0;
        showMarker_ = true;

        model_->SetCastShadows(true);

        wpActiveIndex_ = 0;
        targetAgentIndex_ = 0;

        model_->SetModel(cache->GetResource<Model>(mdlFile));
        model_->SetCastShadows(true);
        //model_->SetMaterial(cache->GetResource<Material>(matFile));
        model_->ApplyMaterialList(matFile);

        body_->SetFriction(0.1f);
        //body_->SetMass(1.0f);
        body_->SetMass(28.0f);
        body_->SetRestitution(0.0f);
        body_->SetLinearDamping(0.88f);
        body_->SetLinearRestThreshold(0.0f);
        body_->SetAngularDamping(0.8f);
        // Set zero angular factor so that physics doesn't turn the character on its own.
        // Instead we will control the character yaw manually
        body_->SetAngularFactor(Vector3::ZERO);
        // Set the rigidbody to signal collision also when in rest, so that we get ground collisions properly
        body_->SetCollisionEventMode(COLLISION_ALWAYS);
        body_->SetCollisionLayer(2);
        body_->SetAngularRestThreshold(0.0f);

        //body_->SetUseGravity(false);

        // Set rigid body kinematic mode. In kinematic mode forces are not applied to the rigid body.
        // Disable physics
        //body_->SetKinematic(true);
        collisionShape_->SetCapsule(0.4f, 0.8f, Vector3::UP * 0.4f);
       // collisionShape_->SetCapsule(0.01f, 0.5f, Vector3::UP * 0.3f);


        animCtrl_->PlayExclusive(walkAniFile, 0, true);
        animCtrl_->SetSpeed(walkAniFile, 0.2f);
        animCtrl_->SetStartBone(walkAniFile, "root");


        // register
        SetUpdateEventMask(USE_UPDATE | USE_FIXEDUPDATE);

        enableControls_ = true;

        // update serializable of the change
        SetAttribute("Color Index", Variant(50));
        SetAttribute("Position", Variant(node_->GetPosition()));

    }
}

void NetworkActor::Create() {

    node_ = GetNode();
    enableControls_ = true;

    // Retrieve client network actor data
    Node* modelNode = node_->GetChild("Actor", REPLICATED);
    node_ = modelNode;
    model_ = modelNode->GetComponent<AnimatedModel>();
    body_ = modelNode->GetComponent<RigidBody>();

    // Default pick up: battery
    LoadPickup(3, 0); // Battery
    for (int i = 0; i < 100; i++) {
        LoadPickup(1, 0); // Balloon
    }

    // Update pickup attributes
    numPickups_ = pickups_->Size();
    if (numPickups_ == 0) {
        activePickup_ = -1;
    } else {
        activePickup_ = pickups_->Back().type;
    }
    // Submit updated attributes over network
    Urho3D::Component::MarkNetworkUpdate();



    // Register update
    SetUpdateEventMask(USE_UPDATE | USE_FIXEDUPDATE);

    // Register collision
    SubscribeToEvent(E_NODECOLLISION, URHO3D_HANDLER(NetworkActor, HandleNodeCollision));
}

void NetworkActor::SetNode(Node* node)
{
    node_ = node;
}

void NetworkActor::SetConnection(Connection* connection)
{
    connection_ = connection;
}

void NetworkActor::SwapMat() {
    ResourceCache *cache = GetSubsystem<ResourceCache>();

    int idx = Random(MAX_MAT_COUNT);
    while (idx == colorIdx_) {
        idx = Random(MAX_MAT_COUNT);
    }

    // update serializable of the change
    SetAttribute("Color Index", Variant(idx));

    String matName = ToString("NetDemo/ballmat%i.xml", colorIdx_);
    StaticModel *ballModel = node_->GetComponent<StaticModel>();
    ballModel->SetMaterial(cache->GetResource<Material>(matName));
}

void NetworkActor::SetControls(const Controls &controls) {
    controls_ = controls;
    if (vehicle_ && onVehicle_ && entered_) {
        vehicle_->SetControls(controls);
    }
}

void NetworkActor::Flip() {

    Quaternion modRot = vehicle_->GetRaycastVehicle()->GetBody()->GetRotation() * Quaternion(0,0,90);
    vehicle_->GetRaycastVehicle()->GetBody()->SetRotation(modRot);

/*
    float qx;
    float qy;
    float qz;
    float qw;

    // Read current transform from data frame
    btTransform trans;
    vehicle_->GetBody()->getWorldTransform(trans);
    qx = trans.getRotation().x();
    qy = trans.getRotation().y();
    qz = trans.getRotation().z();
    qw = trans.getRotation().w();
    URHO3D_LOGDEBUGF("NetworkActor::Flip() -> [q.x: %f, q.y: %f, q.z: %f, q.w: %f]", qx, qy, qz, qw);

    btTransform updateTrans = btTransform(trans.getRotation().inverse());
    //updateTrans.setRotation(btQuaternion(trans.getRotation().x(), trans.getRotation().y(), trans.getRotation().z(), trans.getRotation().w()));


    vehicle_->GetBody()->setWorldTransform(updateTrans);

    qx = updateTrans.getRotation().x();
    qy = updateTrans.getRotation().y();
    qz = updateTrans.getRotation().z();
    qw = updateTrans.getRotation().w();
    URHO3D_LOGDEBUGF("NetworkActor::Flip() AFTER -> [q.x: %f, q.y: %f, q.z: %f, q.w: %f]", qx, qy, qz, qw);*/
}

void NetworkActor::FindTargetByProximity() {
    Vector3 center = body_->GetPosition();
    float radius = 200.0f;
    PhysicsWorld *physicsWorld = GetScene()->GetComponent<PhysicsWorld>();
    PODVector<RigidBody *> hitResults{};
    physicsWorld->GetRigidBodies(hitResults, Sphere{center, radius}, 2);

    for (RigidBody *b: hitResults) {
        Vector3 pos = b->GetNode()->GetWorldPosition();


        Vector3 delta = center-pos;
        if (delta.Length() > 40.0f) {
            toTarget_ = pos;
        }
    }
}

void NetworkActor::FindTargetByWaypoint(int id) {
    if (steerSplinePath) {
        id = id % steerSplinePath->getControlPoints().Size();
        Vector3 center = steerSplinePath->getControlPoints()[id]->GetWorldPosition();
        SetTarget(center);
    }
}

void NetworkActor::ApplyMovement(float timeStep) {
    Quaternion rot{ node_->GetRotation() };


    // Apply move to network actor

    // axis
    const StringHash axisHashList[SDL_CONTROLLER_AXIS_MAX / 2] = {VAR_AXIS_0, VAR_AXIS_1, VAR_AXIS_2};
    // left stick - vehicle
    Variant lStick = controls_.extraData_[VAR_AXIS_0];
    Vector2 lAxisVal = lStick.GetVector2();

    // right stick
    Variant rStick = controls_.extraData_[VAR_AXIS_1];
    Vector2 rAxisVal = rStick.GetVector2();

    setMove(Vector3(lStick.GetVector2().x_, 0, lStick.GetVector2().y_));

    // Next move
    move_ = move_.Normalized() * Pow(move_.Length() * 1.05f, 2.0f);
    if (move_.LengthSquared() > 0.0f)
        move_.Normalize();

    //Vector3 direction{0.90f * move_ + (0.1f*body_->GetLinearVelocity() * Vector3{ 1.0f, 0.0f, 1.0f })};


    // Adjust controls yaw
    //cos θ = (a · b) / (|a| |b|)


    // Apply Movement

    float moveMag = move_.Length();

    if (moveMag > 0) {

        // If in air, allow control, but slower than when on ground
        //body_->ApplyImpulse(rot * move_ * (softGrounded ? MOVE_FORCE : INAIR_MOVE_FORCE));
//        const float MOVE_FORCE = 0.45f;
        //const float MOVE_FORCE = 13.5f;
        const float MOVE_FORCE = 588.5f;


        Vector3 impulse;
        if (controls_.yaw_ < 0) {

            if (controls_.yaw_ > -90) {
                impulse =
                        Quaternion(controls_.yaw_ * -1, Vector3::UP) * body_->GetRotation() * Vector3::FORWARD *
                        MOVE_FORCE * moveMag;
            } else {
                impulse =
                        Quaternion(controls_.yaw_ + 180.0f, Vector3::UP) * body_->GetRotation() * Vector3::FORWARD *
                        MOVE_FORCE * moveMag;

            }
        } else {
            impulse =
                    Quaternion(controls_.yaw_ - 90.0f, Vector3::UP) * body_->GetRotation() * Vector3::FORWARD *
                    MOVE_FORCE * moveMag;
        }
        lastImpulse_ = impulse;
        body_->ApplyImpulse(impulse*timeStep);
        //body_->ApplyImpulse(Vector3(impulse.x_, 0, impulse.z_));
      //  GetNode()->SetPosition(body_->GetNode()->GetWorldPosition());

    } else {
        // Slow down in opposite direction
        body_->ApplyImpulse(-body_->GetLinearVelocity()*timeStep);
    }


/*
    // Apply force to rigid body of actor
    bool run = false;
    Vector3 force{ move_.Length() < 0.05f ? Vector3::ZERO : move_ * thrust_ * timeStep };
    force *= 1.0f + 0.25f * run;

    if (body_->GetLinearVelocity().Length() < (maxSpeed_ * (1.0f + 0.42f * run))
         || (body_->GetLinearVelocity().Normalized() + force.Normalized()).Length() < M_SQRT2 )
    {
        body_->ApplyForce(force);
    }
*/

}

void NetworkActor::FixedUpdate(float timeStep) {
    if (!node_) {
        return;
    }

    // Increment pick up timer
    pickupTime_ += timeStep;

    // DEBUG DRAW
    DebugDraw();

    // Sequencer update: play time step
    //sequencer_->Play(timeStep); // should happen automatic -> with sequencer to object -> update mask set



    Vector3 localCenter = Vector3(0,0,0);

    if (body_) {
        if (vehicle_) {
            rpm_ = vehicle_->GetCurrentRPM();
            velocity_ = vehicle_->GetRaycastVehicle()->GetSpeedKm();
            steer_ = vehicle_->GetSteering();
            // Submit updated attributes over network
            Urho3D::Component::MarkNetworkUpdate();


            if (onVehicle_ && entered_) {
                // Align network actor orientation once entered vehicle

                Vector3 bodyOffset = Vector3(-0.0f, 0.4f, -0.0f);
                switch (vehicle_->getCarType()) {
                    case CAR_TYPE_JEEP1:
                        break;
                    case CAR_TYPE_JEEP2:
                        break;
                    case CAR_TYPE_JEEP3:
                        break;
                    case CAR_TYPE_KART:
                        bodyOffset = Vector3(-0.0f, 0.4f, -0.0f);
                        break;
                    case CAR_TYPE_SAHIN:
                        bodyOffset = Vector3(-0.0f, 0.4f, 3.2f);
                        break;
                    case CAR_TYPE_YUGO:
                        bodyOffset = Vector3(-0.0f, 0.4f, 3.2f);
                        break;
                }

                body_->SetPosition(vehicle_->GetRaycastVehicle()->GetBody()->GetPosition()+bodyOffset);
                body_->SetRotation(vehicle_->GetRaycastVehicle()->GetBody()->GetRotation());
            }


            if (sequencer_->GetPlaySource()->GetNode()) {
                if (body_->GetNode()) {
                    // Sequencer update
                    sequencer_->GetPlaySource()->GetNode()->SetPosition(body_->GetNode()->GetPosition());
                }
            }

            localCenter = body_->GetPosition();
            Vector3 distToVehicle = vehicle_->GetRaycastVehicle()->GetBody()->GetPosition()-localCenter;


            float dist = distToVehicle.LengthSquared();
            if (distToVehicle.LengthSquared() < 40.0f) {

                if (!entered_) {
                    // Close to vehicle mark
                    markType_ = 1;
                    canEnter_ = true;
                } else {
                    // In vehicle
                    markType_ = 2;
                }


            } else {
                // Too far
                canEnter_ = false;
                markType_ = 0;
            }


        }
    }

    // Update the in air timer. Reset if grounded
    if (!onGround_)
        inAirTimer_ += timeStep;
    else {
        // On ground
        inAirTimer_ = 0.0f;
    }


    if (showMarker_) {

        if (!markers_.Empty()) {
            //marker_->GetNode()->SetPosition(localCenter);
//                marker_->GetNode()->SetRotation(Quaternion(lastContactNorm_));

            //Quaternion endRot = Quaternion(0, 0, 0);
            //Vector3 cNorm = lastContactNorm_.Normalized();
            //endRot.FromLookRotation(Vector3(cNorm.x_, cNorm.y_, cNorm.z_), Vector3::UP);
            //endRot.FromLookRotation(Vector3(0, cNorm.y_, 0), Vector3::UP);
            //marker_->GetNode()->SetRotation(endRot);
            //marker_->GetNode()->SetEnabled(true);

            //marker_->SetModel(cache->GetResource<Model>(naMarkerAMdlFile));
            //marker_->ApplyMaterialList(naMarkerAMatFile);


            for (int i = 0; i < markers_.Size(); i++) {
                markers_[i]->SetEnabled((markType_ == i));
            }

        }

    } else {
        for (int i = 0; i < markers_.Size(); i++) {
//            markers_[i]->SetEnabled(false);
        }
    }



    if (toTarget_ == Vector3(0,0,0)) {
        if (vehicle_) {
            FindTargetByWaypoint(vehicle_->getSteerIndex());
        }
    }

    // Snaps network actor to vehicle
    if (vehicle_) {

        // Set to control if actor on vehicle
        vehicle_->setEnableControls(onVehicle_);

        // Snap to vehicle once
/*        if (!initialSet_) {
//            this->position_ = vehicle_->GetRaycastVehicle()->GetNode()->GetPosition();
  //          node_->SetRotation(vehicle_->GetRaycastVehicle()->GetNode()->GetRotation());
            initialSet_ = true;
        }
*/


        vehicle_->setActorNode(this->node_);


        // Once target found, calculate angle
        Vector3 delta = toTarget_-vehicle_->GetNode()->GetWorldPosition();
        double angleRadians = atan2(delta.z_, delta.x_);

        // Update movement & animation
        const Quaternion& rot = node_->GetRotation();
        Vector3 moveDir = Vector3::ZERO;
        const Vector3& velocity = body_->GetLinearVelocity();
        // Velocity on the XZ plane
        Vector3 planeVelocity(velocity.x_, 0.0f, velocity.z_);

        // Update rotation according to direction of the player's movement.
        //Vector3 velocity = body_->GetLinearVelocity();




        if (!doJump_) {

            // Update animation
            if (velocity.Length() > REST_VELOCITY_THRESHOLD) {

                animCtrl_->PlayExclusive(walkAniFile, 1, true, 0.15f);
                animCtrl_->SetSpeed(walkAniFile, velocity.Length() * 0.4f);
                animCtrl_->SetStartBone(walkAniFile, "Ctrl_all");

            } else {

                animCtrl_->PlayExclusive(idleAniFile, 1, true, 0.15f);
                animCtrl_->SetStartBone(idleAniFile, "Ctrl_all");
            }

        } else {

            animCtrl_->PlayExclusive(jumpAniFile, 1, false, 0.15f);
            animCtrl_->SetStartBone(jumpAniFile, "Ctrl_all");
            doJump_ = false;
        }





        int vehDegrees = vehicle_->GetNode()->GetRotation().YawAngle();
        //int degrees = round((angleRadians * 180.0f / PI));
        int theta_deg = fmod(atan2(delta.z_, delta.x_)/M_PI*180,360);
        //vehicle_->SetTurrentAngle(theta_deg-180.0f);


        vehicle_->SetTurrentAngle(turrentAngle_);

        turrentAngle_ = turrentAngle_ + 0.1f;

    }

    // update prev
    lastFire_ += timeStep;
    lastEnter_ += timeStep;

    //float limit = Random(1.0f, 4.0f);



    if (onGround_) {
        if (controls_.buttons_ & NTWK_CTRL_JUMP) {
            // Jump
            Jump();
            onGround_ = false;
        }
    }


    if (enableControls_) {

        if (!entered_) {
            // Controls conditional on being on-foot

            // Read controls generate vehicle control instruction
            if (controls_.buttons_ & NTWK_CTRL_LEFT) {
                move_ += Vector3::LEFT;
                controls_.yaw_ += -1.0f;
             //   ntwkControls_.yaw_

                //URHO3D_LOGDEBUGF("**NETWORK ACTOR LEFT** -> %l", controls_.buttons_);
            }

            if (controls_.buttons_ & NTWK_CTRL_RIGHT) {
                move_ += Vector3::RIGHT;
                controls_.yaw_ += 1.0f;
                //URHO3D_LOGDEBUGF("**NETWORK ACTOR RIGHT** -> %l", controls_.buttons_);
            }

            if (controls_.buttons_ & NTWK_CTRL_FORWARD) {
                move_ += Vector3::FORWARD;
                Run();
                //URHO3D_LOGDEBUGF("**NETWORK ACTOR FORWARD** -> %l", controls_.buttons_);
            }

            if (controls_.buttons_ & NTWK_CTRL_BACK) {
                //move_ = Vector3(0.0f, 0.0f, 0.0f);
                acceleration_ = 0;
                //URHO3D_LOGDEBUGF("**NETWORK ACTOR BACK** -> %l", controls_.buttons_);
            }

            if (controls_.buttons_ & NTWK_CTRL_FIRE) {
                // FIRE
                //fire = true;
                //URHO3D_LOGDEBUGF("%s -> FIRE = %l", vehicleName.CString(), controls_.buttons_);
                URHO3D_LOGDEBUGF("**NETWORK ACTOR FIRE** -> %l", controls_.buttons_);
                float limit = 0.4f;
                if (lastFire_ > limit) {
                    if (controls_.buttons_ & NTWK_CTRL_FIRE) {
                        // Fire uses pick up
                        UsePickup();
                        lastFire_ = 0;
                    }
                }
            }
        }

        float enterLimit = 0.15f;
        if (lastEnter_ > enterLimit) {
            if (controls_.buttons_ & NTWK_CTRL_ENTER) {
                // ENTER CAR (if close enough)
                EnterVehicle();
                URHO3D_LOGDEBUGF("**NETWORK ACTOR ENTER** -> %l", controls_.buttons_);

                // On not enter, flip vehicle
                if (!entered_)
                    Flip();
            }
            lastEnter_ = 0;
        }



        // Normalize move vector
        if (move_.LengthSquared() > 0.0f)
            move_.Normalize();
    }

}

/*
/* AUTO-STEERING CODE
    if (toTarget_ != Vector3::ZERO) {
        // Only pass once rigid body is setup
        if (pRigidBody_) {
            // Compute steer force
//            ComputeSteerForce();
            if (force_ != Vector3::ZERO) {

//            force_ = Vector3(1.0f, 0.0f, 1.0f);

                if (wpActiveIndex_ < 0)
                    return;

//                float wpOffsetX = -mapDim_ / 2;
//                float wpOffsetY = -mapDim_ / 2;
                // Convert marker position to world position for track
                //          float wpPosX = (((float)waypoints_->At(wpActiveIndex_).x_ / (float)miniMapWidth_)*mapDim_)+wpOffsetX;
//            float wpPosZ = (((float)waypoints_->At(wpActiveIndex_).z_ / (float)miniMapHeight_)*mapDim_)+wpOffsetY;


                //Vector3 tgt = Vector3(waypoints_->At(wpActiveIndex_).x_, waypoints_->At(wpActiveIndex_).y_, waypoints_->At(wpActiveIndex_).z_);


                // Calculate distance to waypoint
                Vector3 v = vehicle_->GetNode()->GetPosition() - toTarget_;// + Vector3(-1500,0,-1500);
                float steering = v.Normalized().DotProduct((vehicle_->GetNode()->GetDirection()))+0.4f;
/*
                if (steering > 1.0f) {
                    steering = -1.0f;
                }

                if (steering < -1.0f) {
                    steering = 1.0f;
                }
*/


/*    if (autoSteering_) {
        URHO3D_LOGINFOF("***** Player AUTO-STEERING ENABLED - Vehicle Steer [%f]", vehicle_->GetSteering());
        URHO3D_LOGINFOF("***** Player - waypoint [%d]=[%f,%f,%f,%f]", wpActiveIndex_,
                        waypoints_->At(wpActiveIndex_).x_, waypoints_->At(wpActiveIndex_).y_,
                        waypoints_->At(wpActiveIndex_).z_, steering);
        URHO3D_LOGINFOF("***** Player - target =[%f,%f,%f]", toTarget_.x_, toTarget_.y_, toTarget_.z_);

        // Enable auto-steering
        vehicle_->UpdateSteering(steering);
    }*/
//vehicle_->GetRigidBody()->
//            pRigidBody_->ApplyForce(force_);
/*

            Vector3 vel = pRigidBody_->GetLinearVelocity();

            float d = vel.Length();
            if (d < 10.0f) {
                d = 10.0f;
                pRigidBody_->SetLinearVelocity(vel.Normalized() * d);
            } else if (d > 50.0f) {
                d = 50.0f;
                pRigidBody_->SetLinearVelocity(vel.Normalized() * d);
            }

            Quaternion endRot = Quaternion(0, 0, 0);
            Vector3 nVel = vel.Normalized();
            endRot.FromLookRotation(nVel, Vector3::UP);
            endRot = endRot * Quaternion(90, 0, 0);
            pRigidBody_->SetRotation(endRot);

            Vector3 p = pRigidBody_->GetPosition();
            if (p.y_ < 10.0f) {
                p.y_ = 10.0f;
                pRigidBody_->SetPosition(p);
            } else if (p.y_ > 150.0f) {
                p.y_ = 150.0f;
                pRigidBody_->SetPosition(p);
            }
            }


        }
    }

*/

////


void NetworkActor::ComputeSteerForce() {

    //set the force member variable to zero
    force_ = Vector3(0, 0, 0);

    if (!waypoints_)
        return;

    if ((waypoints_->Empty())) {
        return;
    }
    //Attraction force
/*
    Vector3 CoM; //centre of mass, accumulated total
    int nAttract = 0; //count number of neighbours
    //set the force member variable to zero
    force_ = Vector3(0, 0, 0);
    //Search Neighbourhood
    for (int i = 0; i < numberOfBoids; i++)
    {
        //the current boid?
        if (this == &boidList[i]) continue;
        //sep = vector position of this boid from current oid
        Vector3 sep = pRigidBody->GetPosition() - boidList[i].pRigidBody->GetPosition();
        float d = sep.Length(); //distance of boid
        if (d < Range_FAttract)
        {
            //with range, so is a neighbour
            CoM += boidList[i].pRigidBody->GetPosition();
            nAttract++;
        }
    }
    if (nAttract > 0)
    {
        CoM /= nAttract;
        Vector3 dir = (CoM - pRigidBody->GetPosition()).Normalized();
        Vector3 vDesired = dir * FAttract_Vmax;
        force += (vDesired - pRigidBody->GetLinearVelocity())*FAttract_Factor;
    }
    if (nAttract > 5)
    {
        // stop checking once 5 neighbours have been found
        return;
    }

    //seperation force
    Vector3 sepForce;
    int nRepel = 0;
    for (int i = 0; i < numberOfBoids; i++)
    {
        //the current boid?
        if (this == &boidList[i]) continue;
        //sep = vector position of this boid from current oid
        Vector3 sep = pRigidBody->GetPosition() - boidList[i].pRigidBody->GetPosition();
        float d = sep.Length(); //distance of boid
        if (d < Range_FRepel)
        {
            sepForce += (sep / sep.Length());
            nRepel++;
        }
    }
    if (nRepel > 0)
    {
        sepForce *= FRepel_Factor;
        force += sepForce;
    }
    if (nRepel > 5)
    {
        // stop checking once 5 neighbours have been found
        return;
    }
    */

    //Allignment direction
    Vector3 align;
    int nAlign = 0;


    //sep = vector position of this boid from current oid

//    Vector3 sep = pRigidBody->GetPosition() - boidList[i].pRigidBody->GetPosition();
//    float d = sep.Length(); //distance of boid
//    if (d < Range_FRepel)


    Vector3 toTarget;
    if (!waypoints_->Empty()) {

//        URHO3D_LOGDEBUGF("Player::ComputeSteerForce() waypoints -> [%d] -> set to  %d", waypoints_->Size(), wpActiveIndex_);

        // Calculate distance to waypoint
        //toTarget = vehicle_->GetNode()->GetPosition() - waypoints_->At(wpActiveIndex_);
    } else {
        return;
    }


    //    Vector3 dir = sep.Normalized();
//    float d = sep.Length(); // distance of boid

    //float steer = toTarget * speed / d;

    float speed = 1.0f;
    Vector3 desiredVelocity = toTarget.Normalized() * speed;


    /*
    if (d < Range_FAlign)
    {
        align += boidList[i].pRigidBody->GetLinearVelocity();
        nAlign++;
    }*/

//    force_ += ToVector3(ToBtVector3(desiredVelocity) - vehicle_->GetRaycastVehicle()->GetBody()->getLinearVelocity());

    /*
    if (nAlign > 0)
    {
        align /= nAlign;

        Vector3 finalVel = align;

//        force_ += (finalVel - pRigidBody_->GetLinearVelocity()) * FAlign_Factor;
    }
    if (nAlign > 5)
    {
        // stop checking once 5 neighbours have been found
        return;
    }*/
}


void NetworkActor::Fire() {

    //

    //Quaternion modRot = vehicle_->GetRaycastVehicle()->GetBody()->GetRotation() * Quaternion(0,90,0);
    //vehicle_->GetRaycastVehicle()->GetBody()->SetRotation(modRot);

    Fire(toTarget_);

}

void NetworkActor::Fire(Vector3 target) {

    if (!node_)
        return;

    // Not on vehicle
    if (!entered_) {
        Scene *scene = GetScene();

        // On-foot fire

        SharedPtr<Node> n;
        Node *bullet0 = scene->CreateChild("bullet", REPLICATED);
        Missile *newM = bullet0->CreateComponent<Missile>();
        // Set the position and rotation of the bullet
        bullet0->SetWorldPosition(body_->GetPosition() + Vector3(0,0.0f,0.0f));
        //   bullet0->SetWorldRotation(Quaternion(Vector3::UP, towards_));
//		bullet0->GetComponent<RigidBody2D>()->SetLinearVelocity(Vector2(towards_.x_, towards_.y_).Normalized() * 10.0f);


        newM->SetProducer(node_->GetID());


        // Store local missile list
        //missileList_.Push(vehicle_->GetNode()->GetID());

//        VariantMap& eventData = GetNode()->GetEventDataMap();
//        eventData["owner"] = SharedPtr<Player>(this);

//        eventData["missileOwner"] = this->GetID();
        //       vehicle_->SendEvent(E_NODECOLLISION, eventData);



        // Set the ownership of the bullet to the Player
//        bullet0->GetComponent<Missile>()->SetProducer(SharedPtr<Node>(vehicle_->GetNode()));

        Node *tgt = scene->CreateChild("missileTarget", REPLICATED);
        //tgt->>SetPosition(0f,0f,0f);
        tgt->SetPosition(target);
        newM->AddTarget(SharedPtr<Node>(tgt));
        // Assign the producer node
        newM->AssignProducer(GetNode()->GetID(),
                             GetNode()->GetPosition() + Vector3(40.0f, 2.0f, 0.0f));
        URHO3D_LOGDEBUGF("NetworkActor::Fire() [%d] -> [%f,%f,%f]", GetNode()->GetID(),
                         newM->GetNode()->GetPosition().x_,
                         newM->GetNode()->GetPosition().y_,
                         newM->GetNode()->GetPosition().z_);


        // }
    }


    if (vehicle_) {
        Scene *scene = GetScene();

        SharedPtr<Node> n;
        Node *bullet0 = scene->CreateChild("bullet", REPLICATED);
        Missile *newM = bullet0->CreateComponent<Missile>();
        // Set the position and rotation of the bullet
        bullet0->SetWorldPosition(body_->GetPosition() + Vector3(0,0.0f,0.0f));
     //   bullet0->SetWorldRotation(Quaternion(Vector3::UP, towards_));
//		bullet0->GetComponent<RigidBody2D>()->SetLinearVelocity(Vector2(towards_.x_, towards_.y_).Normalized() * 10.0f);

        newM->SetProducer(vehicle_->GetNode()->GetID());


        // Store local missile list
        //missileList_.Push(vehicle_->GetNode()->GetID());

//        VariantMap& eventData = GetNode()->GetEventDataMap();
//        eventData["owner"] = SharedPtr<Player>(this);

//        eventData["missileOwner"] = this->GetID();
        //       vehicle_->SendEvent(E_NODECOLLISION, eventData);



        // Set the ownership of the bullet to the Player
//        bullet0->GetComponent<Missile>()->SetProducer(SharedPtr<Node>(vehicle_->GetNode()));

        Node *tgt = scene->CreateChild("missileTarget", REPLICATED);
        //tgt->>SetPosition(0f,0f,0f);
        tgt->SetPosition(target);
        newM->AddTarget(SharedPtr<Node>(tgt));
        // Assign the producer node
        newM->AssignProducer(vehicle_->GetNode()->GetID(),
                             vehicle_->GetRaycastVehicle()->GetNode()->GetPosition() + Vector3(0.0f, 0.0f, 0.0f));
        URHO3D_LOGDEBUGF("NetworkActor::Fire() [%d] -> [%f,%f,%f]", vehicle_->GetNode()->GetID(),
                         newM->GetNode()->GetPosition().x_,
                         newM->GetNode()->GetPosition().y_,
                         newM->GetNode()->GetPosition().z_);


        // }
    }
}


void NetworkActor::DebugDraw() {
    if (!vehicle_)
        return;

    if (GetScene()) {
        DebugRenderer *dbgRenderer = GetScene()->GetComponent<DebugRenderer>();

        if (dbgRenderer) {

            collisionShape_->DrawDebugGeometry(dbgRenderer, true);

            Vector3 localCenter = body_->GetPosition();
            //dbgRenderer->AddLine(localCenter, (localCenter+move_*40.0f), Color(1.0f, 1.0, 0.0));


            //dbgRenderer->AddLine(localCenter, (localCenter+GetNode()->GetDirection()*40.0f), Color(1.0f, 0.0, 0.0));
            //dbgRenderer->AddLine(localCenter, (localCenter+lastImpulse_*40.0f), Color(1.0f, 0.0, 0.0));

            // TO VEHICLE
            //dbgRenderer->AddLine(localCenter, vehicle_->GetRaycastVehicle()->GetBody()->GetPosition(), Color(0.0f, 0.0, 1.0));

            //Vector3 nodePos = GetNode()->GetPosition();
            //dbgRenderer->AddLine(localCenter, toTarget_, Color(1.0f, 1.0, 0.0));


            //Vector3 nodePos = GetNode()->GetPosition();
            //dbgRenderer->AddLine(localCenter, toTarget_, Color(1.0f, 0.0, 1.0));




            if (vehicle_->GetDragBrake().Length() > 0) {
//            dragBrake_
            //    dbgRenderer->AddLine(localCenter, localCenter + vehicle_->GetDragBrake() * 1.0f, Color(1.0f, 1.0, 0.0));
            }
            /// DRAW COLLISION LINES
            //dbgRenderer->AddLine(nodePos, nodePos+lastCollVel_*300.0f, Color(0.0f, 1.0, 0.0));



//        dbgRenderer->AddLine(posWS, posWS + node_->R, color);
            //dbgRenderer->AddLine(posWS, posWS + this->vehicle_->GetNode()->GetDirection() * 40.0f, Color::CYAN);
            //dbgRenderer->AddLine(posWS, toTarget_, Color::YELLOW);
        }
    }
}

void NetworkActor::setSteerSet(bool steerSet) {
    steerSet_ = steerSet;
}

bool NetworkActor::isSteerSet() const {
    return steerSet_;
}

const Vector3 &NetworkActor::getToTarget() const {
    return toTarget_;
}

void NetworkActor::HandleNodeCollision(StringHash eventType, VariantMap& eventData)
{
    // Check collision contacts and see if character is standing on ground (look for a contact that has near vertical normal)
    using namespace NodeCollision;

    Node* collidedNode = static_cast<Node*>(eventData[P_OTHERNODE].GetPtr());
//    URHO3D_LOGDEBUGF("***** NetworkActor::HandleNodeCollision() -> collidedNode: [%s]", collidedNode->GetName().CString());

    if (collidedNode) {
        if (vehicle_) {
            if (!onVehicle_) {
                // Found a vehicle?
                String collNodeStr = collidedNode->GetName();
                unsigned idx = collNodeStr.Find("vehicle");
                if (idx > 0) {


                    if (collidedNode == vehicle_->GetNode()) {
                        URHO3D_LOGDEBUGF("***** NetworkActor::FOUND VEHICLE: [%s]", collNodeStr.CString());
                        // Set On Vehicle
                        onVehicle_ = true;
                    }
                }
            }


        }

        if (collidedNode->GetName().StartsWith("MysteryPickup")) {
            URHO3D_LOGDEBUGF("***** NetworkActor::FOUND PICKUP: [%s]", collidedNode->GetName().CString());
            if (collidedNode->GetName() == "MysteryPickup01") {
                // Balloon
                LoadPickup(1, pickupTime_);
            } else if (collidedNode->GetName() == "MysteryPickup02") {
                // Gift
                LoadPickup(2, pickupTime_);
            } else if (collidedNode->GetName() == "MysteryPickup03") {
                // Heart with wings
                LoadPickup(3, pickupTime_);
            } else if (collidedNode->GetName() == "MysteryPickup04") {
                // Battery
                LoadPickup(4, pickupTime_);
            } else if (collidedNode->GetName() == "MysteryPickup05") {
                // Nitrous
                LoadPickup(5, pickupTime_);
            } else if (collidedNode->GetName() == "MysteryPickup06") {
                // Wheel
                LoadPickup(6, pickupTime_);
            }

            // Update pickup attributes
            numPickups_ = pickups_->Size();
            if (numPickups_ == 0) {
                activePickup_ = -1;
            } else {
                activePickup_ = pickups_->Back().type;
            }
            // Submit updated attributes over network
            Urho3D::Component::MarkNetworkUpdate();

            // Clear pick up
            collidedNode->Remove();
        }


        MemoryBuffer contacts(eventData[P_CONTACTS].GetBuffer());

        while (!contacts.IsEof()) {
            Vector3 contactPosition = contacts.ReadVector3();
            Vector3 contactNormal = contacts.ReadVector3();
            /*float contactDistance = */contacts.ReadFloat();
            /*float contactImpulse = */contacts.ReadFloat();

            // Store contact normal for marker adjustment
            lastContactNorm_ = contactNormal;

            // If contact is below node center and pointing up, assume it's a ground contact
            if (contactPosition.y_ < (node_->GetPosition().y_ + 1.0f)) {
                float level = contactNormal.y_;
                if (level > 0.75)
                    onGround_ = true;
            }
        }
    }
}

void NetworkActor::Run() {
    acceleration_ = 10.0f;
}

void NetworkActor::Walk() {
    acceleration_ = 2.0f;
}

void NetworkActor::Jump() {
    doJump_ = true;
    upThrust_ = 1800.0f;
}

const Vector3 &NetworkActor::getMove() const {
    return move_;
}

void NetworkActor::setMove(const Vector3 &move) {
    move_ = move;
}

void NetworkActor::EnterVehicle() {

    if (entered_) {
        // On already entered, exit vehicle
        onVehicle_ =  false;
        entered_ = false;
        // Submit updated attributes over network
        Urho3D::Component::MarkNetworkUpdate();
        return;
    }

    // Check for permission
    if (!canEnter_)
        return;

    // Snap actor to vehicle
    onVehicle_ =  true;
    entered_ = true;
    // Submit updated attributes over network
    Urho3D::Component::MarkNetworkUpdate();

}

void NetworkActor::ApplyThrust(float timeStep) {
    if (upThrust_ > 0) {
        body_->ApplyImpulse(Vector3::UP * upThrust_ * timeStep);
        // Reset impulse accumulator
        upThrust_ = 0;
    }
}

void NetworkActor::LoadPickup(int type, float time) {

    PickupData newP;
    newP.type = type;
    newP.time = time;
    Vector<PickupData>* pickups2_ = new Vector<PickupData>();

    if (pickups_) {
        if (!pickups_->Empty()) {
            for (int i = 0; i < pickups_->Size(); i++) {
                PickupData oldP = pickups_->At(i);
                PickupData oldP2;
                oldP2.type = oldP.type;
                oldP2.time = oldP.time;
                pickups2_->Push(oldP2);
            }
        }
    }
    pickups2_->Push(newP);


    int numItems = pickups2_->Size();

    pickups_ = pickups2_;

    // TODO: clean up previous pickups vector
}

// Server-side pickups is working, the player update state being sent from server to client is not getting through
// In certain cases, the client receives 0, ... , 0's only
void NetworkActor::UsePickup() {
    if (pickups_->Empty())
        return;

//    Pickup p = pickups_->At(0);
    PickupData p = pickups_->Back();
    bool used = false;
    // Apply pick up
    switch (p.type) {
        case 1:
            // Balloon

            // Not on vehicle
            if (!entered_) {
                Scene *scene = GetScene();

                // On-foot fire

                SharedPtr<Node> n;
                Node *pickup0 = scene->CreateChild("pickup", REPLICATED);
                Pickup *newM = pickup0->CreateComponent<Pickup>();
                pickup0->SetPosition(body_->GetPosition());

                //   bullet0->SetWorldRotation(Quaternion(Vector3::UP, towards_));
//		bullet0->GetComponent<RigidBody2D>()->SetLinearVelocity(Vector2(towards_.x_, towards_.y_).Normalized() * 10.0f);

                newM->SetProducer(node_->GetID());


                // Store local missile list
                //missileList_.Push(vehicle_->GetNode()->GetID());

//        VariantMap& eventData = GetNode()->GetEventDataMap();
//        eventData["owner"] = SharedPtr<Player>(this);

//        eventData["missileOwner"] = this->GetID();
                //       vehicle_->SendEvent(E_NODECOLLISION, eventData);



                // Set the ownership of the bullet to the Player
//        bullet0->GetComponent<Missile>()->SetProducer(SharedPtr<Node>(vehicle_->GetNode()));

                Node *tgt = scene->CreateChild("pickupTarget", REPLICATED);
                //tgt->>SetPosition(0f,0f,0f);
               // tgt->SetPosition(target);
                newM->AddTarget(SharedPtr<Node>(tgt));
                // Assign the producer node
                newM->AssignProducer(GetNode()->GetID());
                URHO3D_LOGDEBUGF("NetworkActor::UsePickup() BALLOON [%d] -> [%f,%f,%f]", GetNode()->GetID(),
                                 newM->GetNode()->GetPosition().x_,
                                 newM->GetNode()->GetPosition().y_,
                                 newM->GetNode()->GetPosition().z_);

                Fire();


                // }
            }
            used = true;
            break;
        case 2:
            // Heart with wings
            // TODO: Do something
            used = true;
            break;
        case 3:
            // Battery
            powerLevel_ = 25.0f;
            powerTime_ = 0;
            prana_ += powerLevel_;
            used = true;
            break;
        case 4:
            // N2O
            if (entered_) {
                vehicle_->ApplyBooster();
                used = true;
            }
            break;
        case 5:
            break;
        case 6:
            break;
    }


    // Clear used pick up
    if (used) {
        pickups_->Pop();
    }

    // Update pickup attributes
    numPickups_ = pickups_->Size();
    if (numPickups_ == 0) {
        activePickup_ = -1;
    } else {
        activePickup_ = pickups_->Back().type;
    }
    // Submit updated attributes over network
    Urho3D::Component::MarkNetworkUpdate();
}

// Returns the next pick up item avilable, the type is returned.
int NetworkActor::GetPickUpItemState() {
    //if (pickups_->Empty()) return -1;
//    int numItems = pickups_->Size();
//    URHO3D_LOGDEBUGF("GetPickUpItemState numItems=%d, currItem=%d", numItems, pickups_->Back().type);
    //if (!pickups_->Empty()) return pickups_->Back().type;

    // Update pickup attributes
    if (numPickups_ == 0) {
        return -1;
    } else {
        if (numPickups_ > 0) {
            return pickups_->Back().type;
        }
    }
}