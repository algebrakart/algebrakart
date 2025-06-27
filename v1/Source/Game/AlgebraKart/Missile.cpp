#include <Urho3D/Core/Context.h>
#include <Urho3D/Resource/ResourceCache.h>
#include <Urho3D/Physics/PhysicsEvents.h>
#include <Urho3D/Physics/PhysicsWorld.h>
#include <Urho3D/Physics/RigidBody.h>
#include <Urho3D/Scene/Scene.h>
#include <Urho3D/Input/Input.h>
#include <Urho3D/Scene/SceneEvents.h>
#include <Urho3D/IO/Log.h>

#include <Urho3D/Physics/CollisionShape.h>
#include <Urho3D/Graphics/Material.h>
#include <Urho3D/Graphics/Model.h>
#include <Urho3D/Graphics/ParticleEmitter.h>
#include <Urho3D/Graphics/ParticleEffect.h>
#include <Urho3D/Container/Vector.h>


#include <Urho3D/Math/Quaternion.h>

#include "Missile.h"

#include "Missile.h"
#include "network/NetworkActor.h"
#include <Urho3D/Core/Context.h>
#include <Urho3D/Graphics/Model.h>
#include <Urho3D/Graphics/Material.h>
#include <Urho3D/Graphics/ParticleEffect.h>
#include <Urho3D/Audio/Sound.h>
#include <Urho3D/Scene/Scene.h>
#include <Urho3D/IO/Log.h>
#include <Urho3D/Math/MathDefs.h>

Missile::Missile(Context* context) :
        LogicComponent(context),
        state_(MissileState::SEEKING_TARGET),
        lifetime_(0.0f),
        currentVelocity_(Vector3::ZERO),
        lockOnTime_(0.0f),
        lockOnProgress_(0.0f),
        targetLostTime_(0.0f),
        maxLockOnRange_(400.0f),
        lockOnRequiredTime_(2.0f),
        maxSpeed_(150.0f),
        acceleration_(80.0f),
        turnRate_(3.0f),
        explosionRadius_(25.0f),
        damage_(50.0f),
        timeSinceLastTargetUpdate_(0.0f)
{
}

void Missile::RegisterObject(Context* context)
{
    context->RegisterFactory<Missile>();

    URHO3D_ATTRIBUTE("Max Speed", float, maxSpeed_, 150.0f, AM_DEFAULT);
    URHO3D_ATTRIBUTE("Turn Rate", float, turnRate_, 3.0f, AM_DEFAULT);
    URHO3D_ATTRIBUTE("Explosion Radius", float, explosionRadius_, 25.0f, AM_DEFAULT);
    URHO3D_ATTRIBUTE("Damage", float, damage_, 50.0f, AM_DEFAULT);
    URHO3D_ATTRIBUTE("Lock On Range", float, maxLockOnRange_, 400.0f, AM_DEFAULT);
}

void Missile::Start()
{
    // Get required components
    rigidBody_ = GetNode()->GetComponent<RigidBody>();
    model_ = GetNode()->GetComponent<StaticModel>();

    if (!rigidBody_)
    {
        rigidBody_ = GetNode()->CreateComponent<RigidBody>();
        rigidBody_->SetMass(1.0f);
        rigidBody_->SetLinearDamping(0.1f);
        rigidBody_->SetAngularDamping(0.5f);
    }

    if (!model_)
    {
        model_ = GetNode()->CreateComponent<StaticModel>();
        auto* cache = GetSubsystem<ResourceCache>();
        model_->SetModel(cache->GetResource<Model>("Models/Missile.mdl"));
        model_->SetMaterial(cache->GetResource<Material>("Materials/Missile.xml"));
    }

    // Create collision shape
    auto* shape = GetNode()->CreateComponent<CollisionShape>();
    shape->SetCapsule(0.5f, 2.0f);

    // Create trail effect
    CreateTrailEffect();

    // Subscribe to collision events
    SubscribeToEvent(GetNode(), E_NODECOLLISION, URHO3D_HANDLER(Missile, HandleMissileCollision));

    // Set initial state
    state_ = MissileState::SEEKING_TARGET;
    lifetime_ = 0.0f;

    // Try to acquire target immediately if not set
    if (!targetActor_)
    {
        AcquireTarget();
    }
}

void Missile::FixedUpdate(float timeStep)
{
    lifetime_ += timeStep;

    // Expire missile after 10 seconds
    if (lifetime_ > 10.0f)
    {
        state_ = MissileState::EXPIRED;
        GetNode()->Remove();
        return;
    }

    switch (state_)
    {
        case MissileState::SEEKING_TARGET:
            UpdateSeeking(timeStep);
            break;

        case MissileState::LOCKED_ON:
            UpdateLockOn(timeStep);
            break;

        case MissileState::TRACKING:
            UpdateTracking(timeStep);
            break;

        case MissileState::IMPACT:
        case MissileState::EXPIRED:
            // Do nothing, missile should be removed
            break;
    }

    // Apply physics
    if (rigidBody_ && state_ != MissileState::IMPACT)
    {
        rigidBody_->SetLinearVelocity(currentVelocity_);

        // Orient missile to velocity direction
        if (currentVelocity_.LengthSquared() > 0.1f)
        {
            Quaternion rotation = Quaternion(Vector3::FORWARD, currentVelocity_.Normalized());
            GetNode()->SetRotation(rotation);
        }
    }
}

void Missile::Update(float timeStep)
{
    // Update trail effect position
    if (trailEmitter_)
    {
        trailEmitter_->GetNode()->SetWorldPosition(GetNode()->GetWorldPosition());
    }
}

void Missile::UpdateSeeking(float timeStep)
{
    // Try to acquire a target
    if (!targetActor_)
    {
        if (!AcquireTarget())
        {
            // No target found, continue in straight line
            if (currentVelocity_.LengthSquared() < 1.0f)
            {
                currentVelocity_ = GetNode()->GetDirection() * maxSpeed_ * 0.5f;
            }
            return;
        }
    }

    // Target acquired, begin lock-on
    state_ = MissileState::LOCKED_ON;
    lockOnTime_ = 0.0f;
    PlayLockOnSound();
}

void Missile::UpdateLockOn(float timeStep)
{
    if (!targetActor_)
    {
        state_ = MissileState::SEEKING_TARGET;
        return;
    }

    lockOnTime_ += timeStep;
    lockOnProgress_ = lockOnTime_ / lockOnRequiredTime_;

    // Check if target is still in range
    Vector3 targetPos = targetActor_->GetNode()->GetWorldPosition();
    float distance = (targetPos - GetNode()->GetWorldPosition()).Length();

    if (distance > maxLockOnRange_)
    {
        // Target out of range, return to seeking
        ClearTarget();
        state_ = MissileState::SEEKING_TARGET;
        return;
    }

    // Update missile velocity toward target
    UpdateGuidance(timeStep);

    // Lock-on complete?
    if (lockOnProgress_ >= 1.0f)
    {
        state_ = MissileState::TRACKING;
        URHO3D_LOGINFO("Missile locked onto target!");
    }
}

void Missile::UpdateTracking(float timeStep)
{
    if (!targetActor_)
    {
        state_ = MissileState::SEEKING_TARGET;
        return;
    }

    // Update target information
    Vector3 targetPos = targetActor_->GetNode()->GetWorldPosition();
    Vector3 targetVel = Vector3::ZERO;

    if (auto* targetRigidBody = targetActor_->GetNode()->GetComponent<RigidBody>())
    {
        targetVel = targetRigidBody->GetLinearVelocity();
    }

    lastKnownTargetPos_ = targetPos;
    lastKnownTargetVel_ = targetVel;
    timeSinceLastTargetUpdate_ = 0.0f;

    // Check if target is still in reasonable range
    float distance = (targetPos - GetNode()->GetWorldPosition()).Length();
    if (distance > maxLockOnRange_ * 2.0f)
    {
        targetLostTime_ += timeStep;
        if (targetLostTime_ > 3.0f) // Give up after 3 seconds
        {
            ClearTarget();
            state_ = MissileState::SEEKING_TARGET;
            return;
        }
    }
    else
    {
        targetLostTime_ = 0.0f;
    }

    // Update guidance
    UpdateGuidance(timeStep);
}

void Missile::UpdateGuidance(float timeStep)
{
    Vector3 targetPos;

    if (targetActor_)
    {
        // Use current target position
        targetPos = targetActor_->GetNode()->GetWorldPosition();

        // Lead the target
        if (auto* targetRigidBody = targetActor_->GetNode()->GetComponent<RigidBody>())
        {
            Vector3 targetVel = targetRigidBody->GetLinearVelocity();
            targetPos = CalculateInterceptPoint(targetPos, targetVel);
        }
    }
    else
    {
        // Use last known position
        targetPos = lastKnownTargetPos_;
    }

    // Calculate desired velocity
    Vector3 desiredVelocity = SteerTowards(targetPos, timeStep);

    // Apply acceleration toward desired velocity
    Vector3 acceleration = (desiredVelocity - currentVelocity_) * acceleration_ * timeStep;
    currentVelocity_ += acceleration;

    // Limit speed
    if (currentVelocity_.Length() > maxSpeed_)
    {
        currentVelocity_ = currentVelocity_.Normalized() * maxSpeed_;
    }
}

Vector3 Missile::CalculateInterceptPoint(const Vector3& targetPos, const Vector3& targetVel)
{
    Vector3 missilePos = GetNode()->GetWorldPosition();
    Vector3 toTarget = targetPos - missilePos;

    // Simple lead calculation
    float timeToTarget = toTarget.Length() / maxSpeed_;
    return targetPos + targetVel * timeToTarget;
}

Vector3 Missile::SteerTowards(const Vector3& target, float timeStep)
{
    Vector3 missilePos = GetNode()->GetWorldPosition();
    Vector3 desired = (target - missilePos).Normalized() * maxSpeed_;

    // Limit turning rate
    Vector3 currentDir = currentVelocity_.Normalized();
    Vector3 desiredDir = desired.Normalized();

    float maxTurn = turnRate_ * timeStep;
    float angle = currentDir.Angle(desiredDir);

    if (angle > maxTurn)
    {
        // Limit turn rate
        Vector3 axis = currentDir.CrossProduct(desiredDir).Normalized();
        Quaternion rotation(maxTurn * M_RADTODEG, axis);
        desiredDir = rotation * currentDir;
        desired = desiredDir * maxSpeed_;
    }

    return desired;
}

bool Missile::AcquireTarget()
{
    if (!producerActor_)
        return false;

    auto* scene = GetScene();
    if (!scene)
        return false;

    // Find all NetworkActors in range
    PODVector<Node*> actors;
    scene->GetChildrenWithComponent<NetworkActor>(actors, true);

    NetworkActor* bestTarget = nullptr;
    float bestPriority = 0.0f;
    Vector3 missilePos = GetNode()->GetWorldPosition();

    for (auto* actorNode : actors)
    {
        auto* actor = actorNode->GetComponent<NetworkActor>();
        if (!actor || actor == producerActor_)
            continue;

        // Check if this actor can be targeted
        if (!producerActor_->CanTargetActor(actor))
            continue;

        float distance = (actorNode->GetWorldPosition() - missilePos).Length();
        if (distance > maxLockOnRange_)
            continue;

        float priority = CalculateTargetPriority(actor);
        if (priority > bestPriority)
        {
            bestPriority = priority;
            bestTarget = actor;
        }
    }

    if (bestTarget)
    {
        LockOnTarget(bestTarget);
        return true;
    }

    return false;
}

float Missile::CalculateTargetPriority(NetworkActor* actor) const
{
    if (!actor)
        return 0.0f;

    Vector3 missilePos = GetNode()->GetWorldPosition();
    Vector3 targetPos = actor->GetNode()->GetWorldPosition();

    float distance = (targetPos - missilePos).Length();
    float priority = 1.0f / (distance + 1.0f); // Closer targets have higher priority

    // Bonus for targets in front of missile
    Vector3 forward = GetNode()->GetDirection();
    Vector3 toTarget = (targetPos - missilePos).Normalized();
    float alignment = forward.DotProduct(toTarget);
    if (alignment > 0.0f)
    {
        priority *= (1.0f + alignment);
    }

    return priority;
}

void Missile::LockOnTarget(NetworkActor* target)
{
    targetActor_ = target;
    lockOnProgress_ = 0.0f;
    lockOnTime_ = 0.0f;
    targetLostTime_ = 0.0f;

    if (target)
    {
        target->AddThreat(producerActor_);
        URHO3D_LOGINFOF("Missile acquiring lock on target: %s", target->GetNode()->GetName().CString());
    }
}

void Missile::ClearTarget()
{
    if (targetActor_)
    {
        targetActor_->RemoveThreat(producerActor_);
    }

    targetActor_.Reset();
    lockOnProgress_ = 0.0f;
    lockOnTime_ = 0.0f;
}

void Missile::SetInitialVelocity(const Vector3& velocity)
{
    currentVelocity_ = velocity;
    if (rigidBody_)
    {
        rigidBody_->SetLinearVelocity(velocity);
    }
}

void Missile::HandleMissileCollision(StringHash eventType, VariantMap& eventData)
{
    using namespace NodeCollision;

    auto* otherNode = static_cast<Node*>(eventData[P_OTHERNODE].GetPtr());
    if (!otherNode)
        return;

    // Check if we hit our target or any NetworkActor
    auto* targetActor = otherNode->GetComponent<NetworkActor>();
    if (targetActor && targetActor != producerActor_)
    {
        URHO3D_LOGINFOF("Missile hit target: %s", targetActor->GetNode()->GetName().CString());

        // Apply damage to target
        // targetActor->TakeDamage(damage_, producerActor_);

        Explode();
        return;
    }

    // Check if we hit terrain or other solid objects
    auto* rigidBody = otherNode->GetComponent<RigidBody>();
    if (rigidBody && rigidBody->GetMass() == 0.0f) // Static object
    {
        URHO3D_LOGINFO("Missile hit terrain");
        Explode();
    }
}

void Missile::Explode()
{
    state_ = MissileState::IMPACT;

    // Create explosion effect
    auto* cache = GetSubsystem<ResourceCache>();
    auto* explosionNode = GetScene()->CreateChild("MissileExplosion");
    explosionNode->SetWorldPosition(GetNode()->GetWorldPosition());

    auto* explosion = explosionNode->CreateComponent<ParticleEmitter>();
    explosion->SetEffect(cache->GetResource<ParticleEffect>("Particle/Explosion.xml"));

    // Play explosion sound
    auto* sound = explosionNode->CreateComponent<SoundSource3D>();
    sound->SetSoundType(SOUND_EFFECT);
    sound->Play(cache->GetResource<Sound>("Sounds/Explosion.wav"));

    // Remove explosion after a few seconds
//    explosionNode->
//    auto* lifetimeComponent = explosionNode->CreateComponent<LifeTime>();
//    lifetimeComponent->SetLifeTime(5.0f);

    // Apply area damage
    PODVector<RigidBody*> rigidBodies;
    GetScene()->GetComponent<PhysicsWorld>()->GetRigidBodies(rigidBodies,
                                                             Sphere(GetNode()->GetWorldPosition(), explosionRadius_));

    for (auto* body : rigidBodies)
    {
        auto* actor = body->GetNode()->GetComponent<NetworkActor>();
        if (actor && actor != producerActor_)
        {
            float distance = (body->GetNode()->GetWorldPosition() - GetNode()->GetWorldPosition()).Length();
            float damageFactor = 1.0f - (distance / explosionRadius_);
            float finalDamage = damage_ * Clamp(damageFactor, 0.0f, 1.0f);

            // Apply damage
            // actor->TakeDamage(finalDamage, producerActor_);

            // Apply explosion force
            Vector3 force = (body->GetPosition() - GetNode()->GetWorldPosition()).Normalized() * 1000.0f;
            body->ApplyImpulse(force);
        }
    }

    // Clear target threat
    ClearTarget();

    // Remove missile
    GetNode()->Remove();
}

void Missile::PlayLockOnSound()
{
    if (!soundSource_)
    {
        soundSource_ = GetNode()->CreateComponent<SoundSource3D>();
        soundSource_->SetSoundType(SOUND_EFFECT);
    }

    auto* cache = GetSubsystem<ResourceCache>();
    soundSource_->Play(cache->GetResource<Sound>("Sounds/MissileLock.wav"));
}

void Missile::CreateTrailEffect()
{
    auto* trailNode = GetNode()->CreateChild("MissileTrail");
    trailNode->SetPosition(Vector3(0, 0, -1.0f)); // Behind missile

    trailEmitter_ = trailNode->CreateComponent<ParticleEmitter>();
    auto* cache = GetSubsystem<ResourceCache>();
    trailEmitter_->SetEffect(cache->GetResource<ParticleEffect>("Particle/MissileTrail.xml"));
}

