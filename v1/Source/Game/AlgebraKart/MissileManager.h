#pragma once

#include <Urho3D/Scene/Component.h>
#include <Urho3D/Container/Vector.h>

using namespace Urho3D;

class Missile;
class NetworkActor;

/// Manages all active missiles in the game
class MissileManager : public Component
{
URHO3D_OBJECT(MissileManager, Component);

public:
    /// Construct.
    explicit MissileManager(Context* context);
    /// Destruct.
    virtual ~MissileManager() = default;

    /// Register object factory.
    static void RegisterObject(Context* context);

    /// Update all missiles (called automatically by Component system)
    void Update(float timeStep);

    /// Create a new missile
    Missile* CreateMissile(Scene* scene, NetworkActor* producer, const Vector3& position, const Vector3& direction);

    /// Remove a missile
    void RemoveMissile(Missile* missile);

    /// Get all active missiles
    const Vector<WeakPtr<Missile>>& GetActiveMissiles() const { return activeMissiles_; }

    /// Get missiles targeting a specific actor
    Vector<Missile*> GetMissilesTargeting(NetworkActor* actor) const;

    /// Cleanup destroyed missiles
    void CleanupDestroyedMissiles();

private:
    Vector<WeakPtr<Missile>> activeMissiles_;
};