#pragma once

#include <Urho3D/Core/Object.h>
#include <Urho3D/Container/Vector.h>
#include <Urho3D/Math/Vector3.h>
#include <Urho3D/Math/Quaternion.h>
#include <Urho3D/Input/Controls.h>

#include "ReplaySystem.h"
#include "Urho3D/UI/UIElement.h"
#include "Urho3D/UI/Text.h"
#include "Urho3D/UI/Sprite.h"

namespace Urho3D
{
    class Scene;
    class Node;
    class Camera;
    class RigidBody;
    class AnimationController;
}

using namespace Urho3D;

// Forward declarations
class NetworkActor;
class Vehicle;

class ReplayUI : public Object
{
URHO3D_OBJECT(ReplayUI, Object);

public:
    explicit ReplayUI(Context* context);
    virtual ~ReplayUI();

    void Initialize();
    void SetVisible(bool visible);
    void UpdateReplayUI(const ReplayTriggerType& event, float progress);
    /// UI elements
    SharedPtr<UIElement> replayContainer_;
    SharedPtr<Text> replayText_;
    SharedPtr<Sprite> replayProgressBar_;
    SharedPtr<Text> replaySpeedText_;
    bool uiVisible_;

};