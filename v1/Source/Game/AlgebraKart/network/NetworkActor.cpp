#include <Urho3D/Core/Context.h>
#include <Urho3D/Scene/Scene.h>
#include <Urho3D/Graphics/Light.h>
#include <Urho3D/Graphics/Material.h>
#include <Urho3D/Graphics/Model.h>
#include <Urho3D/Graphics/StaticModel.h>
#include <Urho3D/Graphics/Camera.h>
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
#include "Urho3D/Network/Network.h"

#include <Urho3D/DebugNew.h>
#include <AlgebraKart/Missile.h>
#include <AlgebraKart/Pickup.h>
#include <AlgebraKart/PickupFactory.h>
#include <AlgebraKart/ViewCone.h>
#include <AlgebraKart/Missile.h>
#include <AlgebraKart/MissileManager.h>

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
    // Hide arrow when player dies
    HideVehicleDirectionArrow();

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
    // Players
    context->RegisterFactory<NetworkActor>();

    // Attack/defense system
    context->RegisterFactory<Missile>();

    // Reward system
    context->RegisterFactory<Pickup>();
    context->RegisterFactory<PickupFactory>();

    // Visibilty cone
    context->RegisterFactory<ViewCone>();


    URHO3D_COPY_BASE_ATTRIBUTES(ClientObj);

    ClientObj::RegisterObject(context);

    //URHO3D_ACCESSOR_ATTRIBUTE("Is Enabled", IsEnabled, SetEnabled, bool, true, AM_DEFAULT);
    //URHO3D_ATTRIBUTE("Player Name", String, name_, String::EMPTY, AM_DEFAULT | AM_NET);


    URHO3D_ACCESSOR_ATTRIBUTE("Is Enabled", IsEnabled, SetEnabled, bool, true, AM_DEFAULT);

    URHO3D_ATTRIBUTE("Controls Yaw", float, controls_.yaw_, 0.0f, AM_DEFAULT);
    URHO3D_ATTRIBUTE("Controls Pitch", float, controls_.pitch_, 0.0f, AM_DEFAULT);
    URHO3D_ATTRIBUTE("On Ground", bool, onGround_, false, AM_DEFAULT | AM_NET);
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

    URHO3D_ATTRIBUTE("Linear Velocity", Vector3, linearVelocity_, Vector3(0,0,0), AM_DEFAULT | AM_NET);
    URHO3D_ATTRIBUTE("Angular Velocity", Vector3, angularVelocity_, Vector3(0,0,0), AM_DEFAULT | AM_NET);


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

        body_->SetUseGravity(true);
        body_->SetGravityOverride(Vector3(0,-98.0f,0));

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


        // Get the global missile manager
        if (scene_) {
            Variant managerVar = scene_->GetVar("MissileManager");
            if (!managerVar.IsEmpty()) {
                projectileManager_ = managerVar.GetCustomPtr<MissileManager>();
            }
        }

        // Initialize missile-related variables
        targetLock_ = nullptr;
        lockOnProgress_ = 0.0f;
        lockOnTime_ = 0.0f;
        lastMissileFireTime_ = 0.0f;
        missileFireCooldown_ = MISSILE_FIRE_COOLDOWN;

        // Create lock-on UI indicators
        CreateLockOnUI();

    }
}

void NetworkActor::CreateLockOnUI() {
    if (!scene_) return;

    ResourceCache* cache = GetSubsystem<ResourceCache>();

    // Create lock-on indicator
    lockOnIndicator_ = scene_->CreateChild("LockOnIndicator", LOCAL);
    lockOnIndicator_->SetEnabled(false);

    // Add a visual component (could be a Billboard or StaticModel)
    BillboardSet* billboard = lockOnIndicator_->CreateComponent<BillboardSet>();
    billboard->SetNumBillboards(1);
    billboard->SetMaterial(cache->GetResource<Material>("Materials/LockOnTarget.xml"));
    billboard->SetFaceCameraMode(FC_ROTATE_XYZ);

    Billboard* lockBillboard = billboard->GetBillboard(0);
    lockBillboard->size_ = Vector2(10.0f, 10.0f);
    lockBillboard->position_ = Vector3::ZERO;
    billboard->Commit();
}

void NetworkActor::UpdateLockOnUI(float timeStep) {
    if (!lockOnIndicator_) return;

    if (HasTargetLock() && lockOnProgress_ > 0.0f) {
        lockOnIndicator_->SetEnabled(true);

        // Position indicator above target
        Vector3 targetPos = targetLock_->GetNode()->GetWorldPosition();
        lockOnIndicator_->SetWorldPosition(targetPos + Vector3(0, 15, 0));

        // Animate based on lock progress
        float scale = 0.5f + (lockOnProgress_ * 0.5f);
        lockOnIndicator_->SetScale(scale);

        // Change color based on lock progress
        BillboardSet* billboard = lockOnIndicator_->GetComponent<BillboardSet>();
        if (billboard) {
            Color lockColor = Color::YELLOW.Lerp(Color::RED, lockOnProgress_);
            billboard->GetMaterial()->SetShaderParameter("MatDiffColor", lockColor);
        }
    } else {
        lockOnIndicator_->SetEnabled(false);
    }
}

void NetworkActor::PlayMissileFireSound() {
    // Use the existing sound system from AlgebraKart
    auto* cache = GetSubsystem<ResourceCache>();
    auto* sound = cache->GetResource<Sound>("Sounds/MissileFire.wav");

    if (sound) {
        auto* source = GetNode()->CreateComponent<SoundSource3D>();
        source->SetSoundType(SOUND_EFFECT);
        source->SetAutoRemoveMode(REMOVE_COMPONENT);
        source->Play(sound);
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
    LoadPickup(1, 0); // Balloon
    // Update pickup attributes
    numPickups_ = pickups_->Size();
    if (numPickups_ == 0) {
        activePickup_ = -1;
    } else {
        activePickup_ = pickups_->Back().type;
    }

    // Create vehicle direction arrow
    CreateVehicleDirectionArrow();

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

void NetworkActor::SetProjectileManager(SharedPtr<MissileManager> manager) {
    projectileManager_ = manager;
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
    if (onVehicle_) {
        return; // Don't move when in vehicle
    }

    Vector3 moveDir = Vector3::ZERO;
    bool hasMovement = false;

    // Handle each direction independently to avoid diagonal movement
    if (controls_.buttons_ & NTWK_CTRL_FORWARD) {
        moveDir += Vector3::FORWARD; // Local forward direction
        hasMovement = true;
        //URHO3D_LOGDEBUG("NetworkActor - Moving FORWARD");
    }
    if (controls_.buttons_ & NTWK_CTRL_BACK) {
        moveDir += Vector3::BACK; // Local backward direction
        hasMovement = true;
        //URHO3D_LOGDEBUG("NetworkActor - Moving BACK");
    }
    if (controls_.buttons_ & NTWK_CTRL_LEFT) {
        //moveDir += Vector3::LEFT; // Local left direction
        //hasMovement = true;
        //URHO3D_LOGDEBUG("NetworkActor - Moving LEFT");
    }
    if (controls_.buttons_ & NTWK_CTRL_RIGHT) {
        //moveDir += Vector3::RIGHT; // Local right direction
        //hasMovement = true;
        //URHO3D_LOGDEBUG("NetworkActor - Moving RIGHT");
    }

    // Apply movement force
    if (hasMovement) {
        moveDir.Normalize();

        // Transform movement direction from local to world space
        // This makes the character move relative to its current orientation
        Quaternion rotation = body_->GetRotation();
        Vector3 worldMoveDir = rotation * moveDir;

        /*
        URHO3D_LOGDEBUGF("NetworkActor - Local moveDir: [%f,%f,%f]",
                         moveDir.x_, moveDir.y_, moveDir.z_);
        URHO3D_LOGDEBUGF("NetworkActor - World moveDir: [%f,%f,%f]",
                         worldMoveDir.x_, worldMoveDir.y_, worldMoveDir.z_);
        */
        Vector3 force = worldMoveDir * 800.0f; // Movement force magnitude

        //URHO3D_LOGDEBUGF("NetworkActor - Applying force: [%f,%f,%f]",
        //                 force.x_, force.y_, force.z_);

        body_->ApplyForce(force);
    }
}



void NetworkActor::ApplyRotation(float timeStep) {

    // Only apply rotation if yaw is significant enough
    // Apply rotation from controls
    if (Abs(controls_.yaw_) > 0.1f) { // Add deadzone to prevent drift
        // Get current rotation
        Quaternion currentRotation = body_->GetRotation();

        // Calculate the rotation amount
        float yawDifference = controls_.yaw_ * timeStep * 90.0f; // 90 degrees per second max

        // Apply rotation
        Quaternion yawRotation(yawDifference, Vector3::UP);
        Quaternion newRotation = currentRotation * yawRotation;
        body_->SetRotation(newRotation);
        //URHO3D_LOGDEBUGF("NetworkActor - Set rotation: [%f,%f,%f,%f]",
        //                 newRotation.x_, newRotation.y_, newRotation.z_, newRotation.w_);

    }
}

void NetworkActor::FixedUpdate(float timeStep) {
    if (!node_) {
        return;
    }

    // Increment pick up timer
    pickupTime_ += timeStep;

    // DEBUG DRAW
    DebugDraw();

    // Store latest rigid body values
    linearVelocity_ = GetBody()->GetLinearVelocity();
    angularVelocity_ = GetBody()->GetAngularVelocity();


    /*
    if (pickupTime_ < 1.0f) {
        CreatePickup(3, GetBody()->GetPosition());
        CreatePickup(3, GetBody()->GetPosition() + Vector3(30, 0, 0));
        CreatePickup(0, GetBody()->GetPosition() + Vector3(0, 500, 0));
    }*/

    // Sequencer update: play time step
    //sequencer_->Play(timeStep); // should happen automatic -> with sequencer to object -> update mask set



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

        // Update vehicle direction arrow
        UpdateVehicleDirectionArrow();

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




        if (animCtrl_) {
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

            if (entered_) {
                animCtrl_->PlayExclusive(victoryAniFile, 1, false, 0.15f);
                animCtrl_->SetAnimationTime(7.7f);
                animCtrl_->SetStartBone(victoryAniFile, "Ctrl_all");
            }
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
                        Fire();
                        lastFire_ = 0;
                    }
                }
            }

            if (controls_.buttons_ & NTWK_CTRL_JUMP) {
                URHO3D_LOGDEBUGF("**NETWORK ACTOR JUMP** -> %l", controls_.buttons_);
            }

            if (controls_.buttons_ & NTWK_CTRL_USE) {
                URHO3D_LOGDEBUGF("**NETWORK ACTOR USE** -> %l %f", controls_.buttons_, useButtonDownTime_);
                useButtonDownTime_ += timeStep;
            } else {
                useButtonDownTime_ = 0;
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

    Vector3 localCenter = Vector3(0, 0, 0);
    if (body_ && vehicle_) {
        // Apply movement and rotation when not in vehicle
        if (!onVehicle_) {
            ApplyMovement(timeStep);
            ApplyRotation(timeStep);
        } else {
            // In vehicle
            rpm_ = vehicle_->GetCurrentRPM();
            velocity_ = vehicle_->GetRaycastVehicle()->GetSpeedKm();
            steer_ = vehicle_->GetSteering();
            // Submit updated attributes over network
            Urho3D::Component::MarkNetworkUpdate();


            if (onVehicle_ && entered_) {
                // Align network actor orientation once entered vehicle

                Vector3 bodyOffset = Vector3(-0.0f, 0.4f, -0.4f);
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

                body_->SetPosition(vehicle_->GetRaycastVehicle()->GetBody()->GetPosition() + bodyOffset);
                body_->SetRotation(vehicle_->GetRaycastVehicle()->GetBody()->GetRotation());
            }
        }

        // Body-only logic
        if (body_) {

            if (sequencer_) {
                if (sequencer_->GetPlaySource()->GetNode()) {
                    if (body_->GetNode()) {
                        // Sequencer update
                        sequencer_->GetPlaySource()->GetNode()->SetPosition(body_->GetNode()->GetPosition());
                    }
                }
            }

            localCenter = body_->GetPosition();
            Vector3 distToVehicle = vehicle_->GetRaycastVehicle()->GetBody()->GetPosition() - localCenter;

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

    // Update target locking system
    UpdateTargetLocking(timeStep);

    // Update missile UI
    UpdateLockOnUI(timeStep);
}

void NetworkActor::UpdateTargetLocking(float timeStep) {
    if (!projectileManager_) return;

    // Auto-acquire targets if none locked
    if (!targetLock_) {
        NetworkActor* nearestEnemy = FindNearestEnemy(LOCK_ON_RANGE);
        if (nearestEnemy && CanTargetActor(nearestEnemy)) {
            SetTargetLock(nearestEnemy);
        }
    }

    // Update lock-on progress
    if (targetLock_) {
        Vector3 targetPos = targetLock_->GetNode()->GetWorldPosition();
        Vector3 myPos = GetNode()->GetWorldPosition();
        float distance = (targetPos - myPos).Length();

        // Check if target is still in range and valid
        if (distance > LOCK_ON_RANGE || targetLock_->killed_) {
            ClearTargetLock();
            return;
        }

        // Progress lock-on
        lockOnTime_ += timeStep;
        lockOnProgress_ = lockOnTime_ / LOCK_ON_TIME_REQUIRED;

        if (lockOnProgress_ >= 1.0f) {
            // Target is fully locked - can fire missiles
            lockOnProgress_ = 1.0f;
        }
    }

    // Update missile fire cooldown
    if (lastMissileFireTime_ > 0.0f) {
        lastMissileFireTime_ -= timeStep;
        if (lastMissileFireTime_ < 0.0f) {
            lastMissileFireTime_ = 0.0f;
        }
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

void NetworkActor::CreateVehicleDirectionArrow()
{
    if (vehicleDirectionArrow_)
        return;

    auto* cache = GetSubsystem<ResourceCache>();

    // Create arrow node as child of scene instead of actor for more control
    vehicleDirectionArrow_ = GetScene()->CreateChild("VehicleArrow", LOCAL);
    vehicleDirectionArrow_->SetPosition(GetBody()->GetPosition() + Vector3(0, 8.0f, 0));

    // Create a billboard set for the arrow
    auto* billboardSet = vehicleDirectionArrow_->CreateComponent<BillboardSet>();
    billboardSet->SetNumBillboards(1);
    billboardSet->SetMaterial(cache->GetResource<Material>("Materials/ArrowMaterial.xml"));
    billboardSet->SetSorted(true);

    // Configure the billboard
    Billboard* billboard = billboardSet->GetBillboard(0);
    billboard->position_ = Vector3::ZERO;
    billboard->size_ = Vector2(4.0f, 4.0f);
    billboard->enabled_ = true;
    billboard->color_ = Color::WHITE;

    // Commit changes
    billboardSet->Commit();

    // Initially hidden
    vehicleDirectionArrow_->SetEnabled(false);
    showVehicleArrow_ = false;
    arrowBobTime_ = 0.0f;
}

void NetworkActor::UpdateVehicleDirectionArrow()
{
    if (!vehicleDirectionArrow_ || !vehicle_ || !vehicle_->GetNode())
        return;

    // Show arrow only when not in vehicle and vehicle exists
    bool shouldShow = !onVehicle_ && alive_ && vehicle_->GetNode();

    if (shouldShow != showVehicleArrow_)
    {
        vehicleDirectionArrow_->SetEnabled(shouldShow);
        showVehicleArrow_ = shouldShow;
    }

    if (shouldShow)
    {
        // Calculate direction from player to vehicle
        Vector3 playerPos = GetBody()->GetPosition();
        Vector3 vehiclePos = vehicle_->GetPosition();
        Vector3 direction = (vehiclePos - playerPos);
        float distance = direction.Length();

        // Don't show arrow if vehicle is very close
        if (distance < 5.0f)
        {
            vehicleDirectionArrow_->SetEnabled(false);
            showVehicleArrow_ = false;
            return;
        }

        if (shouldShow != showVehicleArrow_)
        {
            vehicleDirectionArrow_->SetEnabled(shouldShow);
            showVehicleArrow_ = shouldShow;
        }

            // Always update arrow position to follow player
            float timeStep = GetSubsystem<Time>()->GetTimeStep();
            arrowBobTime_ += timeStep * 4.0f;
            float bob = Sin(arrowBobTime_) * 0.8f;

            // Since arrow is now a scene child, just set its position directly
            Vector3 arrowPos = playerPos + Vector3(0, 8.0f + bob, 0);
            vehicleDirectionArrow_->SetPosition(arrowPos);

            // Get camera for billboard calculations
        Camera* camera = nullptr;
        Node* cameraNode = GetScene()->GetChild("Camera", true);
        if (cameraNode)
            camera = cameraNode->GetComponent<Camera>();

        if (camera)
        {
            // Calculate rotation to point toward vehicle
            // Project direction onto horizontal plane (XZ plane)
            Vector3 horizontalDirection = Vector3(direction.x_, 0, direction.z_);
            horizontalDirection.Normalize();

            // Get camera's right and forward vectors
            Vector3 cameraForward = camera->GetNode()->GetDirection();
            Vector3 cameraRight = camera->GetNode()->GetRight();

            // Project camera vectors onto horizontal plane
            Vector3 camForwardFlat = Vector3(cameraForward.x_, 0, cameraForward.z_).Normalized();
            Vector3 camRightFlat = Vector3(cameraRight.x_, 0, cameraRight.z_).Normalized();

            // Calculate angle relative to camera
            float dotForward = horizontalDirection.DotProduct(camForwardFlat);
            float dotRight = horizontalDirection.DotProduct(camRightFlat);
            float angle = atan2(dotRight, dotForward) * M_RADTODEG;

            // Update billboard
            auto* billboardSet = vehicleDirectionArrow_->GetComponent<BillboardSet>();
            if (billboardSet)
            {
                Billboard* billboard = billboardSet->GetBillboard(0);

                // Scale based on distance
                float baseSize = Clamp(distance * 0.02f, 2.0f, 6.0f);
                float pulse = 0.9f + 0.1f * Sin(arrowBobTime_ * 3.0f);
                billboard->size_ = Vector2(baseSize * pulse, baseSize * pulse);

                // Rotate the billboard to point toward vehicle
                billboard->rotation_ = angle;

                // Color based on distance (closer = more urgent)
                float urgency = Clamp((20.0f - distance) / 20.0f, 0.0f, 1.0f);
                billboard->color_ = Color(1.0f, 1.0f - urgency * 0.5f, 1.0f - urgency * 0.8f);

                billboardSet->Commit();
            }
        }
        URHO3D_LOGDEBUGF("Updating arrow: playerPos=[%f,%f,%f], vehiclePos=[%f,%f,%f]",
                         playerPos.x_, playerPos.y_, playerPos.z_,
                         vehiclePos.x_, vehiclePos.y_, vehiclePos.z_);

    }

}

void NetworkActor::HideVehicleDirectionArrow()
{
    if (vehicleDirectionArrow_)
    {
        vehicleDirectionArrow_->SetEnabled(false);
        showVehicleArrow_ = false;
    }
}

void NetworkActor::Fire() {

    //

    //Quaternion modRot = vehicle_->GetRaycastVehicle()->GetBody()->GetRotation() * Quaternion(0,90,0);
    //vehicle_->GetRaycastVehicle()->GetBody()->SetRotation(modRot);

//    Fire(toTarget_);
    Fire(this->hitTarget_);

}

void NetworkActor::Respawn() {
    // Retrieve client network actor data
    Node* modelNode = node_->GetChild("Actor", REPLICATED);
    //node_ = modelNode;
    //model_ = modelNode->GetComponent<AnimatedModel>();
    //body_ = modelNode->GetComponent<RigidBody>();
    if (modelNode)
        modelNode->SetPosition(Vector3(0, 0, 0));

    // Submit updated attributes over network
    Urho3D::Component::MarkNetworkUpdate();
}

void NetworkActor::RespawnVehicle() {

}

void NetworkActor::CreatePickup(int type, Vector3 location) {
    Scene *scene = GetScene();
    SharedPtr<Node> n;
    Node *pickup0 = scene->CreateChild("pickup", REPLICATED);
    Pickup *newP = pickup0->CreateComponent<Pickup>();
    // Set the position of the pickup
    pickup0->SetWorldPosition(location);

    newP->SetProducer(this->GetNode()->GetID());
    URHO3D_LOGDEBUGF("NetworkActor::Pickup generated [%d] -> [%f,%f,%f]", this->GetNode()->GetID(),
                     newP->GetNode()->GetPosition().x_,
                     newP->GetNode()->GetPosition().y_,
                     newP->GetNode()->GetPosition().z_);
}

void NetworkActor::Fire(Vector3 target) {

    if (!node_)
        return;

    // Not on vehicle
    if (!entered_) {
    }

    if (vehicle_) {
        Vector3 source = body_->GetPosition();
        if (entered_) {
            source = vehicle_->GetTurrent()->GetPosition();
        }
        projectileManager_->CreateMissile(GetScene(), this, body_->GetPosition(), target);
    }
}


void NetworkActor::DebugDraw() {
    if (!vehicle_)
        return;
/*
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
    }*/
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

    // Hide the direction arrow when entering vehicle
    HideVehicleDirectionArrow();

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

const Vector3 &NetworkActor::GetLinearVelocity() const {
    return linearVelocity_;
}

const Vector3 &NetworkActor::GetAngularVelocity() const {
    return angularVelocity_;
}

NetworkActor* NetworkActor::FindNearestEnemy(float maxRange)
{
    auto* scene = GetScene();
    if (!scene)
        return nullptr;

    PODVector<Node*> actors;
    scene->GetChildrenWithComponent<NetworkActor>(actors, true);

    NetworkActor* nearestEnemy = nullptr;
    float nearestDistance = maxRange;
    Vector3 myPos = GetNode()->GetWorldPosition();

    for (auto* actorNode : actors)
    {
        auto* actor = actorNode->GetComponent<NetworkActor>();
        if (!actor || actor == this)
            continue;

        if (!CanTargetActor(actor))
            continue;

        float distance = (actorNode->GetWorldPosition() - myPos).Length();
        if (distance < nearestDistance)
        {
            nearestDistance = distance;
            nearestEnemy = actor;
        }
    }

    return nearestEnemy;
}

bool NetworkActor::CanTargetActor(NetworkActor* target) const
{
    if (!target || target == this)
        return false;

    // Add team/faction checks here if needed
    // For now, can target any other actor

    // Check if target is alive
    if (!target->alive_)
        return false;

    // Check line of sight (optional)
    // TODO: Implement raycast to check for obstacles

    return true;
}

void NetworkActor::SetTargetLock(NetworkActor* target)
{
    if (targetLock_ != target)
    {
        ClearTargetLock();
        targetLock_ = target;
        lockOnProgress_ = 0.0f;
        lockOnTime_ = 0.0f;

        if (target)
        {
            target->AddThreat(this);
            ShowLockOnIndicator(true);
        }
    }
}

void NetworkActor::ClearTargetLock()
{
    if (targetLock_)
    {
        targetLock_->RemoveThreat(this);
        targetLock_.Reset();
    }

    lockOnProgress_ = 0.0f;
    lockOnTime_ = 0.0f;
    ShowLockOnIndicator(false);
}

void NetworkActor::FireMissile()
{
    if (!CanFireMissile())
        return;

    NetworkActor* target = targetLock_;
    if (!target)
    {
        target = FindNearestEnemy(LOCK_ON_RANGE);
    }

    FireMissile(target);
}

void NetworkActor::FireMissile(NetworkActor* target)
{
    if (!CanFireMissile())
        return;

    // Create missile
    auto* missileNode = GetScene()->CreateChild("Missile");
    Vector3 spawnPos = GetNode()->GetWorldPosition() + GetNode()->GetDirection() * 5.0f + Vector3(0, 2, 0);
    missileNode->SetWorldPosition(spawnPos);
    missileNode->SetRotation(GetNode()->GetRotation());

    auto* missile = missileNode->CreateComponent<Missile>();
    missile->SetProducer(this);
    if (target)
    {
        missile->SetTarget(target);
    }

    // Set initial velocity
    Vector3 initialVel = GetNode()->GetDirection() * 50.0f;
    if (auto* myBody = GetNode()->GetComponent<RigidBody>())
    {
        initialVel += myBody->GetLinearVelocity();
    }
    missile->SetInitialVelocity(initialVel);

    lastMissileFireTime_ = GetSubsystem<Time>()->GetElapsedTime();

    // Clear lock after firing
    ClearTargetLock();

    URHO3D_LOGINFO("Missile fired!");
}

bool NetworkActor::CanFireMissile() const
{
    float currentTime = GetSubsystem<Time>()->GetElapsedTime();
    return (currentTime - lastMissileFireTime_) >= MISSILE_FIRE_COOLDOWN;
}

void NetworkActor::AddThreat(NetworkActor* threat)
{
    if (threat && !beingTargetedBy_.Contains(WeakPtr<NetworkActor>(threat)))
    {
        beingTargetedBy_.Push(WeakPtr<NetworkActor>(threat));

        if (beingTargetedBy_.Size() == 1)
        {
            // First threat, show warning
            ShowLockOnIndicator(true);
        }
    }
}

void NetworkActor::RemoveThreat(NetworkActor* threat)
{
    if (threat)
    {
        beingTargetedBy_.Remove(WeakPtr<NetworkActor>(threat));

        if (beingTargetedBy_.Empty())
        {
            // No more threats
            ShowLockOnIndicator(false);
        }
    }
}

void NetworkActor::ShowLockOnIndicator(bool show)
{
    // Implementation depends on your UI system
    // Create/show/hide lock-on reticle or warning indicators
    if (show && !lockOnIndicator_)
    {
        // Create lock-on indicator UI element
        // lockOnIndicator_ = CreateLockOnReticle();
    }
    else if (!show && lockOnIndicator_)
    {
        // Hide indicator
        // lockOnIndicator_->SetVisible(false);
    }
}
