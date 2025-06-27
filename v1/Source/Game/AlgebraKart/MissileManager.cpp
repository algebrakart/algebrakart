#include "MissileManager.h"
#include "Missile.h"

MissileManager::MissileManager(Context* context) : Object(context)
{
}

void MissileManager::RegisterObject(Context* context)
{
    context->RegisterFactory<MissileManager>();
}

void MissileManager::Update(float timeStep)
{
    CleanupDestroyedMissiles();

    // Update missile logic could be done here if needed
    // Currently missiles update themselves
}

Missile* MissileManager::CreateMissile(NetworkActor* producer, const Vector3& position, const Vector3& direction)
{
    if (!producer)
        return nullptr;

    auto* scene = producer->GetScene();
    if (!scene)
        return nullptr;

    auto* missileNode = scene->CreateChild("Missile");
    missileNode->SetWorldPosition(position);
    missileNode->SetDirection(direction);

    auto* missile = missileNode->CreateComponent<Missile>();
    missile->SetProducer(producer);
    missile->SetInitialVelocity(direction * 50.0f);

    activeMissiles_.Push(WeakPtr<Missile>(missile));

    return missile;
}

void MissileManager::RemoveMissile(Missile* missile)
{
    if (missile)
    {
        activeMissiles_.Remove(WeakPtr<Missile>(missile));
    }
}

Vector<Missile*> MissileManager::GetMissilesTargeting(NetworkActor* actor) const
{
    Vector<Missile*> result;

    for (const auto& missilePtr : activeMissiles_)
    {
        if (auto* missile = missilePtr.Get())
        {
            if (missile->GetTarget() == actor)
            {
                result.Push(missile);
            }
        }
    }

    return result;
}

void MissileManager::CleanupDestroyedMissiles()
{
    for (int i = activeMissiles_.Size() - 1; i >= 0; --i)
    {
        if (!activeMissiles_[i].Get())
        {
            activeMissiles_.Erase(i);
        }
    }
}
