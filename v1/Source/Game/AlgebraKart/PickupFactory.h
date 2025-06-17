#pragma once

#include <AlgebraKart/Pickup.h>
#include <Urho3D/IO/Log.h>

// Helper class for building pickups.
class PickupFactory : public LogicComponent
{
URHO3D_OBJECT(PickupFactory, LogicComponent);

public:
    // Construct.
    PickupFactory(Context* context);
    // Destruct.
    virtual ~PickupFactory();

    // Reset all stored data.
    void Reset();
    // Initialize pickup factory.
    void Initialize(const PODVector<Pickup>& pickups);

    // Add pickup
    void AddPickup(int type, Vector3 location);

    // Add pickup circle
    void AddCircle(Vector3 center);
private:
    // Pickup elements
    PODVector<Pickup> pickups_;

    /// pickups size.
    unsigned pickupSize_ = 0;
};