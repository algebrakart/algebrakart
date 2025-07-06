#pragma once

#include "AlgebraKart/network/NetworkActor.h"

#include <Urho3D/Scene/LogicComponent.h>
#include <Urho3D/Physics/RigidBody.h>
#include <Urho3D/Physics/CollisionShape.h>
#include <Urho3D/Graphics/ParticleEmitter.h>
#include <Urho3D/Graphics/StaticModel.h>
#include <Urho3D/Audio/SoundSource3D.h>

using namespace Urho3D;

class NetworkActor;

enum class MissileState
{
    SEEKING_TARGET,
    LOCKED_ON,
    TRACKING,
    IMPACT,
    EXPIRED
};

class Missile : public LogicComponent
{
URHO3D_OBJECT(Missile, LogicComponent);

public:
    /// Construct.
    explicit Missile(Context* context);
    /// Destruct.
    virtual ~Missile() = default;

    /// Register object factory and attributes.
    static void RegisterObject(Context* context);

    /// Handle startup. Called by LogicComponent base class.
    void Start() override;
    /// Handle physics world update. Called by LogicComponent base class.
    void FixedUpdate(float timeStep) override;
    /// Handle update
    void Update(float timeStep) override;

    // Configuration
    void SetTarget(NetworkActor* target) { targetActor_ = target; }
    void SetProducer(NetworkActor* producer) { producerActor_ = producer; }
    void SetLaunchPosition(const Vector3& pos) { launchPosition_ = pos; }
    void SetInitialVelocity(const Vector3& velocity);

    // Target locking
    bool AcquireTarget();
    void LockOnTarget(NetworkActor* target);
    void ClearTarget();

    // Getters
    NetworkActor* GetTarget() const { return targetActor_; }
    NetworkActor* GetProducer() const { return producerActor_; }
    MissileState GetState() const { return state_; }
    float GetLockOnProgress() const { return lockOnProgress_; }
    bool IsLocked() const { return state_ == MissileState::LOCKED_ON || state_ == MissileState::TRACKING; }

private:
    void UpdateSeeking(float timeStep);
    void UpdateLockOn(float timeStep);
    void UpdateTracking(float timeStep);
    void UpdateGuidance(float timeStep);
    void CheckCollisions();
    void Explode();
    void PlayLockOnSound();
    void CreateTrailEffect();

    Vector3 CalculateInterceptPoint(const Vector3& targetPos, const Vector3& targetVel);
    Vector3 SteerTowards(const Vector3& target, float timeStep);
    float CalculateTargetPriority(NetworkActor* actor) const;

    // Event handlers
    void HandleMissileCollision(StringHash eventType, VariantMap& eventData);

    // Core properties
    WeakPtr<NetworkActor> targetActor_;
    WeakPtr<NetworkActor> producerActor_;
    MissileState state_;
    float lifetime_;
    Vector3 launchPosition_;
    Vector3 currentVelocity_;

    // Locking properties
    float lockOnTime_;
    float lockOnProgress_;
    float targetLostTime_;
    float maxLockOnRange_;
    float lockOnRequiredTime_;

    // Missile physics
    float maxSpeed_;
    float acceleration_;
    float turnRate_;
    float explosionRadius_;
    float damage_;

    // Components
    WeakPtr<RigidBody> rigidBody_;
    WeakPtr<StaticModel> model_;
    WeakPtr<ParticleEmitter> trailEmitter_;
    WeakPtr<SoundSource3D> soundSource_;

    // Target tracking
    Vector3 lastKnownTargetPos_;
    Vector3 lastKnownTargetVel_;
    float timeSinceLastTargetUpdate_;

};