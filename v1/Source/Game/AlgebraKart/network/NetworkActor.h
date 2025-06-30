#pragma once

#include <Urho3D/UI/Text3D.h>
#include "ClientObj.h"
#include "Vehicle.h"
#include <AlgebraKart/beat/Sequencer.h>
#include <AlgebraKart/Missile.h>
#include <AlgebraKart/MissileManager.h>

#include "../GameController.h"
#include "../../../Urho3D/Graphics/AnimatedModel.h"
#include "../../../Urho3D/Graphics/AnimationController.h"
#include "../../../Urho3D/Scene/SplinePath.h"

namespace Urho3D
{
class Controls;
class Node;
class RigidBody;
class Connection;
class SplinePath;
}

using namespace Urho3D;
//=============================================================================
//=============================================================================
static const unsigned NTWK_CTRL_BACK = (1<<0);
static const unsigned NTWK_CTRL_FORWARD    = (1<<1);
static const unsigned NTWK_CTRL_LEFT    = (1<<2);
static const unsigned NTWK_CTRL_RIGHT   = (1<<3);
static const unsigned NTWK_CTRL_ENTER     = (1<<5);
static const unsigned NTWK_CTRL_JUMP     = (1<<6);
static const unsigned NTWK_CTRL_FIRE     = (1<<7);
static const unsigned NTWK_CTRL_USE     = (1<<8);


static const unsigned NETWORKACTOR_COL_LAYER = 2;
static const int MAX_MAT_COUNT = 9;


// Car type definitions
#define CAR_TYPE_JEEP1 3
#define CAR_TYPE_JEEP2 4
#define CAR_TYPE_JEEP3 5
#define CAR_TYPE_YUGO 6
#define CAR_TYPE_KART 12
#define CAR_TYPE_SAHIN 7


//=============================================================================
//=============================================================================
class NetworkActor : public ClientObj
{
    URHO3D_OBJECT(NetworkActor, ClientObj)

public:
    NetworkActor(Context* context);
    ~NetworkActor();
   
    static void RegisterObject(Context* context);

    void DebugDraw();
    void ComputeSteerForce();

    virtual void DelayedStart();
    virtual void SetControls(const Controls &controls);

    void Create();
    void SetConnection(Connection* connection);
    void Init(Node* node);

    void HandleNodeCollision(StringHash eventType, VariantMap& eventData);

    const WeakPtr<Connection> GetConnection() const { return connection_; }
    void SetNode(Node* node);
//    const Node* GetNode() const { return node_; };

    int GetLife() { return life_; }
    void SetLife(int m_life) { life_ = m_life; }

    int GetPrana() { return prana_; }
    void SetPrana(int m_prana) { prana_ = m_prana; }

    void SetWaypoints(const Vector<Vector3>* waypoints) {

        if (waypoints_) {
            delete waypoints_;
            waypoints_ = nullptr;
        }

        Vector<Vector3>* newWPs = new Vector<Vector3>();
        // Copy waypoints into local list
        for (int i = 0; i < waypoints->Size(); i++) {
            newWPs->Push(Vector3(waypoints->At(i)));
        }

        waypoints_ = newWPs;
    }

    void ReverseWaypoints(const Vector<Vector3>* waypoints) {

        if (waypoints_) {
            delete waypoints_;
            waypoints_ = nullptr;
        }

        Vector<Vector3>* newWPs = new Vector<Vector3>();
        // Copy waypoints into local list
        for (int i = waypoints->Size(); i > 0; i--) {
            newWPs->Push(Vector3(waypoints->At(i)));
        }

        waypoints_ = newWPs;
    }


    void SetTarget(Vector3 toTarget) {
        toTarget_ = toTarget;
    }

    void SetProjectileTarget(Vector3 hitTarget) {
        hitTarget_ = hitTarget;
    }

    int getAgentIndex() const {
        return agentIndex;
    }

    float GetLastFire() { return lastFire_; }
    void SetLastFire(float lastFire) { lastFire_ = lastFire; };

    float GetMass() { return mass_; }
    void SetMass(float m_mass) { mass_ = m_mass; }
    SharedPtr<RigidBody> GetBody() { return body_; }

    Controls GetControls() { return controls_; }


    float GetSpeed() { return speed_; }
    void SetSpeed(float m_speed) { speed_ = m_speed; }

    float GetMaxSpeed() { return maxSpeed_; }
    void SetMaxSpeed(float m_maxSpeed) { maxSpeed_ = m_maxSpeed; }

    float GetDamping() { return damping_; }
    void SetDamping(float m_damping) { damping_ = m_damping; }
    void Damping() { speed_ -= damping_; if (speed_ <= 0) speed_ = 0; }

    float GetAcceleration() { return acceleration_; }
    void SetAcceleration(float m_acceleration) { acceleration_ = m_acceleration; }
    void Accelerate() { speed_ += acceleration_; if (speed_ > maxSpeed_) speed_ = maxSpeed_; }

    float GetBrake() { return brake_; }
    void SetBrake(float m_brake) { brake_ = m_brake; }
    void Brake() { speed_ -= brake_; if (speed_ < 0) speed_ = 0; }

    Vector3 GetTowards() { return towards_; }
    void SetTowards(Vector3 m_towards) { towards_ = m_towards; }

    String GetBulletType() { return bulletType_; }
    void SetBulletType(String m_bulletType) { bulletType_ = m_bulletType; }

    void Flip();
    void EnterVehicle();

    SharedPtr<Vehicle> GetVehicle() { return vehicle_; }

    void FindTargetByWaypoint(int id);
    void FindTargetByProximity();

    void Respawn();
    void RespawnVehicle();
    void Fire(Vector3 target);
    void Fire();
    void CreatePickup(int type, Vector3 location);

    void Kill();

    void Run();
    void Walk();
    void Jump();

    void ApplyMovement(float timeStep);
    void ApplyThrust(float timeStep);

    /// Grounded flag for movement.
    bool onGround_;
    /// Jump flag.
    bool okToJump_;
    /// In air timer. Due to possible physics inaccuracy, character can be off ground for max. 1/10 second and still be allowed to move.
    float inAirTimer_;


public:
    void SwapMat();
    virtual void SetScene(Scene *scene);
    virtual void FixedUpdate(float timeStep);
    // Mark NetworkActor disconnected, clean-up delayed
    void MarkDisconnected() { disconnected_ = true; }
    //Sequencer &GetSequencer();

        // PEER
    /// Movement controls. Assigned by the main program each physics update step.
    bool disconnected_;
    Controls controls_;
    float useButtonDownTime_;
    WeakPtr<Connection> connection_;
    WeakPtr<Scene> scene_;
    Timer updateTimer_;
    ///

    /// Flag when player is dead.
    bool killed_;
    bool isAI_;
    int agentIndex;
    int id_;
    int type_;
    unsigned int wpActiveIndex_;
    int targetAgentIndex_;


    /// Bullets
    String bulletType_;
    Vector<Vector3>* waypoints_ = nullptr;
    Vector3 toTarget_ = Vector3::ZERO;
    Vector3 hitTarget_ = Vector3::ZERO;

    const Vector3 &getToTarget() const;

    bool steerSet_;

    bool isSteerSet() const;

    void setSteerSet(bool steerSet);

    void LoadPickup(int type, float time);
    int GetPickUpItemState();

    /// parameter

    float mass_;
    float speed_;
    float maxSpeed_;
    float thrust_;
    float damping_;
    float acceleration_;
    float brake_;
    Vector3 towards_;

    bool enableControls_;
    Vector3 move_;
    float upThrust_;

    const Vector3 &getMove() const;

    void setMove(const Vector3 &move);

    bool initialSet_;


    float lastEnter_;
    float lastFire_;
    float heading_;
    bool isReady_;
    int life_;
    int prana_;
    float rpm_;
    float velocity_;
    float steer_;

    int score_;
    int health_;

    Vector3 force_;
    Vector3 offset_;

    float changeTargetTime_;
    bool autoSteering_;

    bool doJump_;

    Vector3 linearVelocity_;
    Vector3 angularVelocity_;

    // Marker
    Vector<StaticModel*> markers_;
    // Animated model
    AnimatedModel* model_;
    // Rigid body
    SharedPtr<RigidBody> body_;
    // Collision shape
    CollisionShape* collisionShape_;
    // Animation controller
    AnimationController* animCtrl_;

    // Player state variables
    bool alive_;

    bool IsAlive() const;

    bool onVehicle_;
    bool canEnter_;
    bool entered_;
    bool showMarker_;
    int markType_;
    Vector3 lastImpulse_;
    Vector3 lastContactNorm_;

    // Vehicle track navigation
    float lastWaypoint_;
    int lapNum_;
    float lapTime_;
    float totalTime_;
    Vector<float> lapTimes_;

    String name_;
    Vector3 vehiclePos_;
//    WeakPtr<Node> nodeInfo_;
//    Controls prevControls_;


    // Pick ups
    float pickupTime_;
    struct PickupData {
        int type; // type [0 = mana-missile, 1 = speed-up, 2 = power-up]
        float time; // Time of pickup
    };
    Vector<PickupData>* pickups_;
    int numPickups_;
    int activePickup_;

    // Pick up attributes
    float powerTime_;
    float powerLevel_;


    /// The controllable vehicle component.
    SharedPtr<Vehicle> vehicle_;
    SharedPtr<Text3D> floatingText_;

    SharedPtr<SplinePath> steerSplinePath;

    float turrentAngle_ = 0;

    // Movement and physics state
    bool isGrounded_;           // Is the character on ground
    float jumpForce_;           // Jump force magnitude
    float jumpCooldown_;        // Jump cooldown duration
    float currentJumpCooldown_; // Current jump cooldown timer

    SharedPtr<Node> vehicleDirectionArrow_;
    bool showVehicleArrow_;
    float arrowBobTime_;
    SharedPtr<Camera> activeCam_;

    // Target locking data
    WeakPtr<NetworkActor> targetLock_;
    float lockOnProgress_;
    float lockOnTime_;
    float lastMissileFireTime_;
    float missileFireCooldown_;

    // Threat tracking
    PODVector<WeakPtr<NetworkActor>> beingTargetedBy_;

    // UI elements for lock-on
    SharedPtr<Node> lockOnIndicator_;
    SharedPtr<Node> threatWarning_;

    SharedPtr<MissileManager> projectileManager_;

    // Lock-on configuration
    const float LOCK_ON_RANGE = 400.0f;
    const float LOCK_ON_TIME_REQUIRED = 2.0f;
    const float MISSILE_FIRE_COOLDOWN = 3.0f;
    const float LOCK_ON_ANGLE_THRESHOLD = 30.0f; // degrees


    void UsePickup();
    void ApplyRotation(float timeStep);

    void CreateVehicleDirectionArrow();
    void UpdateVehicleDirectionArrow();
    void HideVehicleDirectionArrow();
    const Vector3 &GetLinearVelocity() const;
    const Vector3 &GetAngularVelocity() const;

    // Target locking system
    NetworkActor* FindNearestEnemy(float maxRange = 500.0f);
    bool CanTargetActor(NetworkActor* target) const;
    void SetTargetLock(NetworkActor* target);
    void ClearTargetLock();
    NetworkActor* GetTargetLock() const { return targetLock_; }
    bool HasTargetLock() const { return targetLock_ != nullptr; }

    // Projectile system
    void SetProjectileManager(SharedPtr<MissileManager> manager);
    void FireMissile();
    void FireMissile(NetworkActor* target);
    bool CanFireMissile() const;

    // Lock-on UI
    void CreateLockOnUI();
    void UpdateLockOnUI(float timeStep);
    void ShowLockOnIndicator(bool show);
    void PlayMissileFireSound();
    void UpdateTargetLocking(float timeStep);

        // Threat detection
    bool IsBeingTargeted() const { return beingTargetedBy_.Size() > 0; }
    const PODVector<WeakPtr<NetworkActor>>& GetThreats() const { return beingTargetedBy_; }
    void AddThreat(NetworkActor* threat);
    void RemoveThreat(NetworkActor* threat);

};

