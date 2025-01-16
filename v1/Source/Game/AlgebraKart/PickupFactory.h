#pragma once

#include <AlgebraKart/Pickup.h>
#include <Urho3D/IO/Log.h>

// Helper class for building pickups.
class PickupFactory : public Object
{
URHO3D_OBJECT(PickupFactory, Object);

public:
    // Construct.
    PickupFactory(Context* context);
    // Destruct.
    virtual ~PickupFactory();

    // Reset all stored data.
    void Reset();
    // Initialize pickup factory.
    void Initialize(const PODVector<Pickup>& pickupElements, bool largeIndices);

    // Add pickup
    void AddPickup(int type, Vector3 location);

private:
    // Pickup elements
    PODVector<Pickup> pickups_;

    /// pickups size.
    unsigned pickupSize_ = 0;
};