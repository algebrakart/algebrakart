#pragma once

#include <Urho3D/Scene/LogicComponent.h>
#include <Urho3D/Core/Context.h>
#include <Urho3D/Graphics/Geometry.h>
#include <Urho3D/Graphics/Model.h>
#include <Urho3D/Graphics/StaticModel.h>
#include <Urho3D/Graphics/Material.h>
#include <Urho3D/Math/Color.h>
#include <Urho3D/Math/Vector3.h>
#include <Urho3D/Scene/Node.h>
#include <Urho3D/Physics/CollisionShape.h>
#include <Urho3D/Physics/RigidBody.h>

#include <AlgebraKart/network/NetworkActor.h>

using namespace Urho3D;

/// ViewCone object for displaying AI sight ranges in Commandos-style
class ViewCone : public LogicComponent
{
URHO3D_OBJECT(ViewCone, LogicComponent);

public:
    /// Construct.
    explicit ViewCone(Context* context);
    /// Destruct.
    ~ViewCone() override;

    /// Register object factory and attributes.
    static void RegisterObject(Context* context);

    /// Set view distance (sight range).
    void SetViewDistance(float distance);
    /// Set view angle in degrees (field of view).
    void SetViewAngle(float angle);
    /// Set view cone height.
    void SetViewHeight(float height);
    /// Set cone color and transparency.
    void SetConeColor(const Color& color);
    /// Set visibility of the view cone.
    void SetVisible(bool enable);
    /// Set whether to update cone direction from node rotation.
    void SetUpdateFromRotation(bool enable);
    /// Set number of segments for cone resolution.
    void SetSegments(unsigned segments);

    /// Get view distance.
    float GetViewDistance() const { return viewDistance_; }
    /// Get view angle.
    float GetViewAngle() const { return viewAngle_; }
    /// Get view height.
    float GetViewHeight() const { return viewHeight_; }
    /// Get cone color.
    const Color& GetConeColor() const { return coneColor_; }
    /// Get visibility state.
    bool IsVisible() const { return visible_; }
    /// Get update from rotation state.
    bool GetUpdateFromRotation() const { return updateFromRotation_; }
    /// Get number of segments.
    unsigned GetSegments() const { return segments_; }

    // Override virtual functions from LogicComponent
    /// Handle startup. Called by LogicComponent base class.
    void Start() override;
    /// Handle delayed startup. Called by LogicComponent base class.
    void DelayedStart() override;
    /// Handle shutdown. Called by LogicComponent base class.
    void Stop() override;
    /// Handle scene update. Called by LogicComponent base class.
    void Update(float timeStep) override;
    /// Handle scene post-update. Called by LogicComponent base class.
    void PostUpdate(float timeStep) override;
    /// Handle physics world update. Called by LogicComponent base class.
    void FixedUpdate(float timeStep) override;
    /// Handle physics world post-update. Called by LogicComponent base class.
    void FixedPostUpdate(float timeStep) override;

    /// Event handle functions
    void HandleNodeCollision(StringHash eventType, VariantMap& eventData);
    /// Handle trigger enter events
    void HandleTriggerEnter(StringHash eventType, VariantMap& eventData);
    /// Handle trigger exit events
    void HandleTriggerExit(StringHash eventType, VariantMap& eventData);
    /// Check line of sight using raycasting
    void CheckLineOfSight();
    void DebugConeState();

private:
    /// Create the cone geometry.
    void CreateConeGeometry();
    /// Update the cone mesh.
    void UpdateConeMesh();
    /// Create simple fallback cone geometry.
    void CreateSimpleCone();

    /// View distance (sight range).
    float viewDistance_;
    /// View angle in degrees.
    float viewAngle_;
    /// Height of the view cone.
    float viewHeight_;
    /// Color of the cone.
    Color coneColor_;
    /// Visibility flag.
    bool visible_;
    /// Whether to update from node rotation.
    bool updateFromRotation_;
    /// Number of cone segments.
    unsigned segments_;

    /// Cone geometry node.
    SharedPtr<Node> coneNode_;
    /// Static model component for rendering.
    SharedPtr<StaticModel> staticModel_;
    SharedPtr<RigidBody> pRigidBody_;
    SharedPtr<CollisionShape> pCollisionShape_;
    /// Model for the cone.
    SharedPtr<Model> coneModel_;
    /// Material for the cone.
    SharedPtr<Material> coneMaterial_;
    /// Flag to track if geometry needs rebuilding.
    bool geometryDirty_;
    /// Last rotation for update detection.
    Quaternion lastRotation_;
    /// Initialization state flag.
    bool initialized_;

    // Visible nodes in view cone
    Vector<SharedPtr<Node>> visibleNodes_;
};
