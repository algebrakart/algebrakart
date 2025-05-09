#include <Urho3D/AngelScript/ScriptFile.h>
#include <Urho3D/Core/Context.h>
#include <AlgebraKart/Pickup.h>
#include <AlgebraKart/PickupFactory.h>

PickupFactory::PickupFactory(Context* context)
        : Object(context)
{

}

PickupFactory::~PickupFactory()
{

}

void PickupFactory::Reset()
{
    pickups_.Clear();
    pickupSize_ = 0;
}

void PickupFactory::Initialize(const PODVector<Pickup>& pickups)
{
    Reset();
    pickups_ = pickups;
}

void PickupFactory::AddPickup(int type, Urho3D::Vector3 location) {
    // Create pickup
    // Deploy pickup at location
}

void PickupFactory::AddCircle(Urho3D::Vector3 center) {
    // Create circle of pickups

    float r = 1.0f;
    for (int t = 0; t < 360; t += 10) {
        Vector3 deployLoc = center + Vector3(r*cos(t), 0, r*sin(t));

    }

}