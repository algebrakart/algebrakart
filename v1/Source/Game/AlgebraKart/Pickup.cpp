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

#include "Pickup.h"


Pickup::Pickup(Context* context) : LogicComponent(context)
{
    type_ = 0;
    fuel_ = 18.0f;
	SetThrust(0);
	SetDetectionRange(3.0f);
	SetBoomRange(0.3f);
	SetDamage(20.0f);
	duration_ = 0.0f;
}

void Pickup::RegisterObject(Context* context)
{
	context->RegisterFactory<Pickup>();
}

void Pickup::Start()
{
    ResourceCache* cache = GetSubsystem<ResourceCache>();

    node_ = GetScene()->CreateChild("pickup");
    node_->SetRotation(Quaternion(0.0f, 0.0f, 0.0f));

    pCollisionShape_ = node_->CreateComponent<CollisionShape>();
    pObject_ = node_->CreateComponent<StaticModel>();
    pRigidBody_ = node_->CreateComponent<RigidBody>();

    pRigidBody_->SetMass(1.0f);
    pRigidBody_->SetUseGravity(false);
    pRigidBody_->SetTrigger(true);

    pRigidBody_->SetEnabled(false);
    pObject_->SetEnabled(false);

    float scale = 32.0f;

    if (type_ == 0) {
        Model *projModel = cache->GetResource<Model>("Models/pickup_Wings.mdl");

        pObject_->SetModel(projModel);
        pObject_->ApplyMaterialList("Models/pickup_Wings.txt");
        pObject_->SetCastShadows(true);


        Model *projColModel = cache->GetResource<Model>("Models/pickup_Balloon.mdl");
        pObject_->GetNode()->SetScale(scale);
/*
        pCollisionShape_->SetConvexHull(projColModel);
        pCollisionShape_->GetNode()->SetRotation(Quaternion(0.0, 0.0f, -90.0f));
        //  pCollisionShape_->GetNode()->SetScale(Vector3(0.3f, 0.3f, 0.3f));
        pCollisionShape_->GetNode()->SetScale(scale);
*/

        // particle emitter
        Node *pNodeEmitter = GetScene()->CreateChild();
//    pNodeEmitter->SetPosition( emitPos );
        pParticleEmitter_ = pNodeEmitter->CreateComponent<ParticleEmitter>();
        pParticleEmitter_->SetEffect(cache->GetResource<ParticleEffect>("Offroad/Particles/Dust.xml"));
        pParticleEmitter_->SetEmitting(false);

//    particleEmitterNodeList_.Push( pNodeEmitter );

        // node collision
        SubscribeToEvent(GetNode(), E_NODECOLLISION, URHO3D_HANDLER(Pickup, HandlePickupCollision));
//    SubscribeToEvent(GetNode(), E_NODECOLLISIONSTART, URHO3D_HANDLER(Missile, HandleNodeCollision));
    }
    // register
    SetUpdateEventMask(USE_UPDATE | USE_FIXEDUPDATE);

}

void Pickup::AssignProducer(int producerId) {

    // Update pickup name
    Urho3D::String nodeName = Urho3D::String("pickup-") + Urho3D::String(producerId);
    producerId_ = producerId;
    node_->SetName(nodeName.CString());
    //    node_->SetName("updated-missile");

//    node_->SetName("missile-" + producerId_);
        //node_->SetPosition(spawnLoc);
//        GetNode()->SetPosition(node_->GetPosition());
        //pRigidBody_->SetPosition(node_->GetPosition());
        //		bullet0->GetComponent<RigidBody2D>()->SetLinearVelocity(Vector2(towards_.x_, towards_.y_).Normalized() * 10.0f);
        URHO3D_LOGDEBUGF("Pickup::AssignProducer() -> %s, %d, [%f,%f,%f]", nodeName.CString(), producerId_, node_->GetPosition().x_,
                         node_->GetPosition().y_,
                         node_->GetPosition().z_);

       // pRigidBody_->SetEnabled(true);
        pObject_->SetEnabled(true);
}

void Pickup::Update(float timeStep)
{

}

void Pickup::FixedUpdate(float timeStep)
{

	// Update the duration
	duration_ += timeStep;

    if (pRigidBody_) {
        pRigidBody_->SetRotation(pRigidBody_->GetRotation()+Quaternion(0.01f*timeStep, 0, 0));

        if (pParticleEmitter_) {
            if (node_) {
                //pParticleEmitter_->GetNode()->SetPosition(node_->GetPosition());
                //pParticleEmitter_->SetEmitting(true);
            }
        }
        // Set Rotation
        //Vector3 velocity = Vector3(pRigidBody_->GetLinearVelocity());

        //	node_->SetWorldRotation(Quaternion(Vector3::UP, velocity));
        // Apply thrust to the missile
//        pRigidBody_->ApplyForce(velocity.Normalized() * thrust);
    }
}

/*
//URHO3D_PARAM(2349, "extraData");
void Missile::HandleNodeCollision(StringHash eventType, VariantMap & eventData)
{
    using namespace NodeCollision;

    // increase score if missile collides with boids
    Node* collidedNode = static_cast<Node*>(eventData[P_OTHERNODE].GetPtr());
    URHO3D_LOGDEBUGF("***** Missile::HandleMissileCollision() -> collidedNode: [%s]", collidedNode->GetName().CString());


    if (collidedNode->GetName() == "boid")
    {
        // emitt particle effect when boid has been hit
        Node* particle = collidedNode->CreateChild("Particle");
        particle->SetPosition(Vector3(0.0f, 0.0f, 0.0f));
        particle->SetScale(2.0f);
        ParticleEmitter* emitter = particle->CreateComponent<ParticleEmitter>();
        emitter->SetEffect(GetSubsystem<ResourceCache>()->GetResource<ParticleEffect>("Particle/Burst.xml"));
    }
}*/

void Pickup::HandlePickupCollision(StringHash eventType, VariantMap & eventData)
{
    using namespace NodeCollision;

    Node* collidedNode = static_cast<Node*>(eventData[P_OTHERNODE].GetPtr());
    URHO3D_LOGDEBUGF("***** Pickup::HandlePickupCollision() -> collidedNode: [%s]", collidedNode->GetName().CString());

    if (collidedNode->GetName() == "boid")
    {
        // emitt particle effect when boid has been hit
        Node* particle = collidedNode->CreateChild("Particle");
        particle->SetPosition(Vector3(0.0f, 0.0f, 0.0f));
        particle->SetScale(2.0f);
        ParticleEmitter* emitter = particle->CreateComponent<ParticleEmitter>();
        emitter->SetEffect(GetSubsystem<ResourceCache>()->GetResource<ParticleEffect>("Particle/Burst.xml"));
    }
}

float Pickup::GetFuel() const {
    return fuel_;
}

void Pickup::SetFuel(float fuel) {
    fuel_ = fuel;
}

/*
void Missile::HandleContactBegin(StringHash eventType, VariantMap& eventData)
{	
	/// Clients should not update the component on its own
	using namespace NodeBeginContact2D;	
	SharedPtr<Node>othernode(static_cast<Node*>(eventData[P_OTHERNODE].GetPtr()));
	// Do not track the launcher and the launcher's flare
	//if (othernode == GetProducer()) return;
	HeatSource* otherSource = othernode->GetComponent<HeatSource>();	
	//If the other node is not a heatsource then return
	if (!otherSource)return;
	//Toolkit::Print("Source:" + otherSource->GetNode()->GetName()+" Fighter:"+GetProducer()->GetName());
	//Toolkit::Print("target:" + String(otherSource->GetOwner()->GetID()));
	if (otherSource->GetOwner() == GetProducer()) return;	
	// If othernode is a heatsource then push it into the targetqueue;
	targetnodes_.Push(othernode);	
	//Toolkit::Print("got you!");
}

void Missile::HandleContactEnd(StringHash eventType, VariantMap& eventData)
{
	/// Clients should not update the component on its own
	Network* network = GetSubsystem<Network>();
	if (!network->IsServerRunning()) {
		return;
	}

	using namespace Urho3D;
	// If the target is out of tracking range, then erase it.
	using namespace NodeEndContact2D;
	SharedPtr<Node>othernode(static_cast<Node*>(eventData[P_OTHERNODE].GetPtr()));
	HeatSource* heatsource = othernode->GetComponent<HeatSource>();
	if (!heatsource) return;
	//4test Find it in the queue and erase it
	if(targetnodes_.Contains(othernode))	targetnodes_.Erase(	targetnodes_.Find(othernode));
	//Toolkit::Print("lost you!");
}
*/
