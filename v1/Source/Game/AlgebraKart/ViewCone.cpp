#include "ViewCone.h"

#include <Urho3D/Core/Context.h>
#include <Urho3D/Scene/Scene.h>
#include <Urho3D/Scene/SceneEvents.h>
#include <Urho3D/Graphics/Geometry.h>
#include <Urho3D/Graphics/IndexBuffer.h>
#include <Urho3D/Graphics/Model.h>
#include <Urho3D/Graphics/StaticModel.h>
#include <Urho3D/Graphics/VertexBuffer.h>
#include <Urho3D/Graphics/Material.h>
#include <Urho3D/Graphics/Technique.h>
#include <Urho3D/Graphics/Graphics.h>
#include <Urho3D/Resource/ResourceCache.h>
#include <Urho3D/IO/Log.h>
#include <Urho3D/Math/MathDefs.h>
#include <Urho3D/Math/Ray.h>
#include <Urho3D/Physics/PhysicsEvents.h>
#include <Urho3D/Physics/PhysicsWorld.h>
#include <Urho3D/Physics/RigidBody.h>
#include <Urho3D/Physics/CollisionShape.h>
#include <Urho3D/Math/Quaternion.h>


ViewCone::ViewCone(Context* context) :
        LogicComponent(context),
        viewDistance_(50.0f),
        viewAngle_(60.0f),
        viewHeight_(5.0f),
        coneColor_(Color(0.0f, 1.0f, 0.0f, 0.3f)),
        visible_(true),
        updateFromRotation_(true),
        segments_(16),
        geometryDirty_(true),
        lastRotation_(Quaternion::IDENTITY),
        initialized_(false)
{
    // Ensure segments is within safe bounds
    segments_ = Clamp(segments_, 8u, 32u);
}

ViewCone::~ViewCone() = default;

void ViewCone::RegisterObject(Context* context)
{
    context->RegisterFactory<ViewCone>();

    URHO3D_ATTRIBUTE("View Distance", float, viewDistance_, 50.0f, AM_DEFAULT);
    URHO3D_ATTRIBUTE("View Angle", float, viewAngle_, 60.0f, AM_DEFAULT);
    URHO3D_ATTRIBUTE("View Height", float, viewHeight_, 5.0f, AM_DEFAULT);
    URHO3D_ATTRIBUTE("Cone Color", Color, coneColor_, Color(0.0f, 1.0f, 0.0f, 0.3f), AM_DEFAULT);
    URHO3D_ATTRIBUTE("Visible", bool, visible_, true, AM_DEFAULT);
    URHO3D_ATTRIBUTE("Update From Rotation", bool, updateFromRotation_, true, AM_DEFAULT);
    URHO3D_ATTRIBUTE("Segments", unsigned, segments_, 16, AM_DEFAULT);
}

void ViewCone::SetViewDistance(float distance)
{
    if (distance != viewDistance_)
    {
        viewDistance_ = Max(distance, 1.0f);
        geometryDirty_ = true;

        if (initialized_ && staticModel_)
            UpdateConeMesh();
    }
}

void ViewCone::SetViewAngle(float angle)
{
    if (angle != viewAngle_)
    {
        viewAngle_ = Clamp(angle, 1.0f, 180.0f);
        geometryDirty_ = true;

        if (initialized_ && staticModel_)
            UpdateConeMesh();
    }
}

void ViewCone::SetViewHeight(float height)
{
    if (height != viewHeight_)
    {
        viewHeight_ = Max(height, 0.1f);
        geometryDirty_ = true;

        if (initialized_ && staticModel_)
            UpdateConeMesh();

        if (coneNode_)
            coneNode_->SetPosition(Vector3(0, viewHeight_ * 0.5f, 0));
    }
}

void ViewCone::SetConeColor(const Color& color)
{
    if (color != coneColor_)
    {
        coneColor_ = color;
        if (coneMaterial_)
        {
            coneMaterial_->SetShaderParameter("MatDiffColor", coneColor_);
        }
    }
}

void ViewCone::SetVisible(bool enable)
{
    if (enable != visible_)
    {
        visible_ = enable;
        if (coneNode_)
            coneNode_->SetEnabled(IsEnabled() && visible_);
    }
}

void ViewCone::SetUpdateFromRotation(bool enable)
{
    updateFromRotation_ = enable;
}

void ViewCone::SetSegments(unsigned segments)
{
    unsigned newSegments = Clamp(segments, 8u, 32u);
    if (newSegments != segments_)
    {
        segments_ = newSegments;
        geometryDirty_ = true;

        if (initialized_ && staticModel_)
            UpdateConeMesh();
    }
}

void ViewCone::CreateConeGeometry()
{
    if (!initialized_)
        return;

    auto* cache = GetSubsystem<ResourceCache>();

    // Create material
    coneMaterial_ = new Material(context_);

    // Try to load the custom technique, fall back to basic alpha if not found
    Technique* technique = cache->GetResource<Technique>("Techniques/NoTextureUnlitAlpha.xml");
    if (!technique)
    {
        technique = cache->GetResource<Technique>("Techniques/NoTextureAlpha.xml");
        if (!technique)
        {
            URHO3D_LOGERROR("Could not load alpha technique for ViewCone");
            technique = cache->GetResource<Technique>("Techniques/NoTexture.xml");
        }
    }

    if (technique)
        coneMaterial_->SetTechnique(0, technique);

    coneMaterial_->SetShaderParameter("MatDiffColor", coneColor_);
    coneMaterial_->SetCullMode(CULL_NONE);

    UpdateConeMesh();
}

void ViewCone::UpdateConeMesh()
{
    if (!geometryDirty_ || !staticModel_ || !initialized_)
        return;

    try
    {
        URHO3D_LOGINFO("ViewCone: Starting mesh update");

        segments_ = Clamp(segments_, 8u, 32u);

        unsigned vertexCount = 2 + (segments_ + 1) * 2;
        unsigned indexCount = segments_ * 12;

        URHO3D_LOGINFO("ViewCone: vertexCount=" + String(vertexCount) +
                       ", indexCount=" + String(indexCount));

        if (vertexCount > 1000 || indexCount > 10000)
        {
            URHO3D_LOGERROR("ViewCone: Vertex/index count too high, aborting");
            return;
        }

        coneModel_ = new Model(context_);
        SharedPtr<Geometry> geometry(new Geometry(context_));
        SharedPtr<VertexBuffer> vb(new VertexBuffer(context_));
        SharedPtr<IndexBuffer> ib(new IndexBuffer(context_));

        PODVector<VertexElement> elements;
        elements.Push(VertexElement(TYPE_VECTOR3, SEM_POSITION));
        elements.Push(VertexElement(TYPE_VECTOR4, SEM_COLOR));

        if (!vb->SetSize(vertexCount, elements))
        {
            URHO3D_LOGERROR("ViewCone: Failed to set vertex buffer size");
            return;
        }

        auto* vertexData = (float*)vb->Lock(0, vertexCount);
        if (!vertexData)
        {
            URHO3D_LOGERROR("ViewCone: Failed to lock vertex buffer");
            return;
        }

        float angleStep = viewAngle_ * M_DEGTORAD / segments_;
        float startAngle = -viewAngle_ * 0.5f * M_DEGTORAD;
        unsigned vertexIndex = 0;

        // Tip vertex
        vertexData[vertexIndex * 7 + 0] = 0.0f;
        vertexData[vertexIndex * 7 + 1] = 0.0f;
        vertexData[vertexIndex * 7 + 2] = viewDistance_;
        vertexData[vertexIndex * 7 + 3] = coneColor_.r_;
        vertexData[vertexIndex * 7 + 4] = coneColor_.g_;
        vertexData[vertexIndex * 7 + 5] = coneColor_.b_;
        vertexData[vertexIndex * 7 + 6] = coneColor_.a_;
        vertexIndex++;

        // Center vertex
        vertexData[vertexIndex * 7 + 0] = 0.0f;
        vertexData[vertexIndex * 7 + 1] = 0.0f;
        vertexData[vertexIndex * 7 + 2] = 0.0f;
        vertexData[vertexIndex * 7 + 3] = coneColor_.r_;
        vertexData[vertexIndex * 7 + 4] = coneColor_.g_;
        vertexData[vertexIndex * 7 + 5] = coneColor_.b_;
        vertexData[vertexIndex * 7 + 6] = coneColor_.a_;
        vertexIndex++;

        // Arc vertices
        for (unsigned i = 0; i <= segments_; ++i)
        {
            if (vertexIndex >= vertexCount - 1)
            {
                URHO3D_LOGERROR("ViewCone: Vertex index overflow detected");
                break;
            }

            float angle = startAngle + i * angleStep;
            float x = Sin(angle) * viewDistance_;
            float z = Cos(angle) * viewDistance_;

            // Top vertex
            vertexData[vertexIndex * 7 + 0] = x;
            vertexData[vertexIndex * 7 + 1] = viewHeight_ * 0.5f;
            vertexData[vertexIndex * 7 + 2] = z;
            vertexData[vertexIndex * 7 + 3] = coneColor_.r_;
            vertexData[vertexIndex * 7 + 4] = coneColor_.g_;
            vertexData[vertexIndex * 7 + 5] = coneColor_.b_;
            vertexData[vertexIndex * 7 + 6] = coneColor_.a_ * 0.5f;
            vertexIndex++;

            // Bottom vertex
            vertexData[vertexIndex * 7 + 0] = x;
            vertexData[vertexIndex * 7 + 1] = -viewHeight_ * 0.5f;
            vertexData[vertexIndex * 7 + 2] = z;
            vertexData[vertexIndex * 7 + 3] = coneColor_.r_;
            vertexData[vertexIndex * 7 + 4] = coneColor_.g_;
            vertexData[vertexIndex * 7 + 5] = coneColor_.b_;
            vertexData[vertexIndex * 7 + 6] = coneColor_.a_ * 0.5f;
            vertexIndex++;
        }

        vb->Unlock();

        if (!ib->SetSize(indexCount, false))
        {
            URHO3D_LOGERROR("ViewCone: Failed to set index buffer size");
            return;
        }

        auto* indexData = (unsigned short*)ib->Lock(0, indexCount);
        if (!indexData)
        {
            URHO3D_LOGERROR("ViewCone: Failed to lock index buffer");
            return;
        }

        unsigned indexIndex = 0;
        for (unsigned i = 0; i < segments_; ++i)
        {
            unsigned topCurrent = 2 + i * 2;
            unsigned bottomCurrent = 2 + i * 2 + 1;
            unsigned topNext = 2 + (i + 1) * 2;
            unsigned bottomNext = 2 + (i + 1) * 2 + 1;

            if (topNext >= vertexCount || bottomNext >= vertexCount)
            {
                URHO3D_LOGERROR("ViewCone: Index out of bounds detected at segment " + String(i));
                break;
            }

            // Side triangles
            indexData[indexIndex++] = topCurrent;
            indexData[indexIndex++] = bottomCurrent;
            indexData[indexIndex++] = topNext;

            indexData[indexIndex++] = bottomCurrent;
            indexData[indexIndex++] = bottomNext;
            indexData[indexIndex++] = topNext;

            // Top triangle
            indexData[indexIndex++] = 0;
            indexData[indexIndex++] = topNext;
            indexData[indexIndex++] = topCurrent;

            // Bottom triangle
            indexData[indexIndex++] = 1;
            indexData[indexIndex++] = bottomCurrent;
            indexData[indexIndex++] = bottomNext;
        }

        ib->Unlock();

        geometry->SetVertexBuffer(0, vb);
        geometry->SetIndexBuffer(ib);
        geometry->SetDrawRange(TRIANGLE_LIST, 0, indexCount);

        coneModel_->SetNumGeometries(1);
        coneModel_->SetGeometry(0, 0, geometry);
        coneModel_->SetBoundingBox(BoundingBox(Vector3(-viewDistance_, -viewHeight_, -viewDistance_),
                                               Vector3(viewDistance_, viewHeight_, viewDistance_)));

        staticModel_->SetModel(coneModel_);
        staticModel_->SetMaterial(coneMaterial_);
        staticModel_->SetCastShadows(false);

        geometryDirty_ = false;
        URHO3D_LOGINFO("ViewCone: Mesh update completed successfully");
    }
    catch (...)
    {
        URHO3D_LOGERROR("ViewCone: Exception during mesh creation");
        CreateSimpleCone();
    }
}

void ViewCone::CreateSimpleCone()
{
    // Implementation provided in previous artifact
    URHO3D_LOGINFO("ViewCone: Creating simple fallback cone (stub)");
    // For now, just disable the cone if complex geometry fails
    if (coneNode_)
        coneNode_->SetEnabled(false);
    geometryDirty_ = false;
}

// ALL REQUIRED VIRTUAL FUNCTION IMPLEMENTATIONS:

void ViewCone::Start()
{
    URHO3D_LOGINFO("ViewCone: Starting initialization");

    if (!GetScene())
    {
        URHO3D_LOGERROR("ViewCone: No scene available");
        return;
    }

    ResourceCache* cache = GetSubsystem<ResourceCache>();
    if (!cache)
    {
        URHO3D_LOGERROR("ViewCone: No resource cache available");
        return;
    }

    // Create child node for the cone
    coneNode_ = GetScene()->CreateChild("ViewCone");
    if (!coneNode_)
    {
        URHO3D_LOGERROR("ViewCone: Failed to create cone node");
        return;
    }

    coneNode_->SetPosition(Vector3(0, viewHeight_ * 0.5f, 0));

    // Create static model component for visual representation
    staticModel_ = coneNode_->CreateComponent<StaticModel>();
    if (!staticModel_)
    {
        URHO3D_LOGERROR("ViewCone: Failed to create static model component");
        return;
    }

    // FIXED PHYSICS SETUP - Choose ONE of the following approaches:

    // APPROACH 1: Trigger-based collision detection (RECOMMENDED)
    if (true) // Set to true to use this approach
    {
        // Create collision shape for trigger detection
        pCollisionShape_ = coneNode_->CreateComponent<CollisionShape>();
        pRigidBody_ = coneNode_->CreateComponent<RigidBody>();

        if (pCollisionShape_ && pRigidBody_)
        {
            // Configure as kinematic trigger (no mass, no gravity, trigger enabled)
            pRigidBody_->SetMass(0.0f);        // Mass 0 = kinematic body
            pRigidBody_->SetKinematic(true);    // Explicitly set as kinematic
            pRigidBody_->SetUseGravity(false);  // No gravity
            pRigidBody_->SetTrigger(true);      // Enable trigger events

            // Set collision shape to match cone geometry (simplified)
            // Using a box approximation for the cone
            pCollisionShape_->SetBox(Vector3(viewDistance_, viewHeight_, viewDistance_));

            // Subscribe to trigger events instead of collision events
            SubscribeToEvent(coneNode_, E_NODECOLLISIONSTART, URHO3D_HANDLER(ViewCone, HandleTriggerEnter));
            SubscribeToEvent(coneNode_, E_NODECOLLISIONEND, URHO3D_HANDLER(ViewCone, HandleTriggerExit));
        }
    }
    else
    {
        // APPROACH 2: No physics, pure visual (if you don't need collision detection)
        // Just create the visual representation without physics
        URHO3D_LOGINFO("ViewCone: Using visual-only mode (no physics)");
    }

    // Mark as initialized
    initialized_ = true;

    // Create the visual geometry
    CreateConeGeometry();

    // Register for updates
    SetUpdateEventMask(USE_UPDATE | USE_FIXEDUPDATE);

    URHO3D_LOGINFO("ViewCone: Initialization completed");
}

// Add these new event handlers to replace HandleNodeCollision
void ViewCone::HandleTriggerEnter(StringHash eventType, VariantMap& eventData)
{
    using namespace NodeCollisionStart;

    Node* otherNode = static_cast<Node*>(eventData[P_OTHERNODE].GetPtr());
    if (otherNode)
    {
        URHO3D_LOGINFO("ViewCone: Object entered view cone: " + otherNode->GetName());

        // Add your game logic here - e.g., detecting enemies entering sight
        // You might want to:
        // 1. Check if the object is an enemy/target
        // 2. Add it to a list of visible objects
        // 3. Trigger AI behavior changes
        // 4. Send events to other systems
    }
}

void ViewCone::HandleTriggerExit(StringHash eventType, VariantMap& eventData)
{
    using namespace NodeCollisionEnd;

    Node* otherNode = static_cast<Node*>(eventData[P_OTHERNODE].GetPtr());
    if (otherNode)
    {
        URHO3D_LOGINFO("ViewCone: Object left view cone: " + otherNode->GetName());

        // Add your game logic here - e.g., removing objects from sight
        // You might want to:
        // 1. Remove from visible objects list
        // 2. Update AI behavior
        // 3. Stop tracking this object
    }
}

// Alternative: Ray-casting based vision system (more accurate than trigger)
void ViewCone::CheckLineOfSight()
{
    if (!initialized_)
        return;

    PhysicsWorld* physicsWorld = GetScene()->GetComponent<PhysicsWorld>();
    if (!physicsWorld)
        return;

    Vector3 start = GetNode()->GetWorldPosition();
    Vector3 forward = GetNode()->GetWorldDirection();

    // Cast multiple rays within the cone angle to detect objects
    float angleStep = viewAngle_ * M_DEGTORAD / 8; // 8 rays
    float startAngle = -viewAngle_ * 0.5f * M_DEGTORAD;

    for (int i = 0; i < 8; ++i)
    {
        float angle = startAngle + i * angleStep;
        Vector3 direction = Quaternion(angle * M_RADTODEG, Vector3::UP) * forward;
        Vector3 end = start + direction * viewDistance_;

        PhysicsRaycastResult result;
        physicsWorld->RaycastSingle(result, Ray(start, direction), viewDistance_);

        if (result.body_)
        {
            URHO3D_LOGDEBUG("ViewCone: Ray hit object at distance " +
                            String(result.distance_) + ": " +
                            result.body_->GetNode()->GetName());
            // Process the hit object
        }
    }
}

void ViewCone::DelayedStart()
{
    // Called after all components have started
    // Override this if you need to access other components after they've all started
}

void ViewCone::Stop()
{
    // Called when component is being removed or disabled
    // Clean up any resources here
    if (coneNode_)
    {
        coneNode_->Remove();
        coneNode_.Reset();
    }

    staticModel_.Reset();
    pRigidBody_.Reset();
    pCollisionShape_.Reset();
    coneModel_.Reset();
    coneMaterial_.Reset();

    initialized_ = false;
}

void ViewCone::Update(float timeStep)
{
    // Called every frame - add any per-frame logic here
    // This is where you might update visibility or other real-time effects
}

void ViewCone::PostUpdate(float timeStep)
{
    // Called after all Update() calls - useful for cleanup or final calculations
}

void ViewCone::FixedUpdate(float timeStep)
{
    if (!initialized_ || !updateFromRotation_)
        return;

    Quaternion currentRotation = GetNode()->GetWorldRotation();
    if (currentRotation != lastRotation_)
    {
        lastRotation_ = currentRotation;
    }
}

void ViewCone::FixedPostUpdate(float timeStep)
{
    // Called after all FixedUpdate() calls
}

void ViewCone::HandleNodeCollision(StringHash eventType, VariantMap& eventData)
{
    URHO3D_LOGINFO("ViewCone: Node collision detected");
}