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

void PickupFactory::Initialize(const PODVector<Pickup>& pickups, bool largeIndices)
{
    Reset();
    pickups_ = pickups;
}
