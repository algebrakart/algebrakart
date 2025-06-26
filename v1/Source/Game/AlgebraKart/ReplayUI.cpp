#include "ReplayUI.h"
#include "network/Vehicle.h"
#include "network/NetworkActor.h"
#include <Urho3D/Scene/Scene.h>
#include <Urho3D/Graphics/Graphics.h>
#include <Urho3D/Graphics/Renderer.h>
#include <Urho3D/Graphics/Camera.h>
#include <Urho3D/Graphics/Texture2D.h>
#include <Urho3D/Graphics/Material.h>
#include <Urho3D/Graphics/Technique.h>
#include <Urho3D/Physics/RigidBody.h>
#include <Urho3D/Core/CoreEvents.h>
#include <Urho3D/IO/Log.h>
#include <Urho3D/Math/MathDefs.h>
#include <Urho3D/UI/UI.h>
#include <Urho3D/UI/Text.h>
#include <Urho3D/UI/Sprite.h>
#include <Urho3D/UI/Font.h>
#include <Urho3D/Resource/ResourceCache.h>
#include <Urho3D/Core/Timer.h>

ReplayUI::ReplayUI(Context* context) :
        Object(context),
        uiVisible_(false)
{
}

ReplayUI::~ReplayUI() {
}

void ReplayUI::Initialize() {
    auto* ui = GetSubsystem<UI>();
    auto* cache = GetSubsystem<ResourceCache>();
    auto* graphics = GetSubsystem<Graphics>();

    // Create replay UI container
    replayContainer_ = ui->GetRoot()->CreateChild<UIElement>();
    replayContainer_->SetSize(400, 100);
    replayContainer_->SetAlignment(HA_CENTER, VA_TOP);
    replayContainer_->SetPosition(0, 50);
    replayContainer_->SetVisible(false);

    // Replay status text
    replayText_ = replayContainer_->CreateChild<Text>();
    replayText_->SetText("INSTANT REPLAY");
    replayText_->SetFont(cache->GetResource<Font>("Fonts/Anonymous Pro.ttf"), 24);
    replayText_->SetColor(Color::YELLOW);
    replayText_->SetAlignment(HA_CENTER, VA_TOP);
    replayText_->SetPosition(0, 0);

    // Progress bar background
    auto* progressBg = replayContainer_->CreateChild<Sprite>();
    progressBg->SetTexture(cache->GetResource<Texture2D>("Textures/ProgressBarBg.png"));
    progressBg->SetSize(300, 10);
    progressBg->SetAlignment(HA_CENTER, VA_TOP);
    progressBg->SetPosition(0, 40);
    progressBg->SetColor(Color(0.3f, 0.3f, 0.3f, 0.8f));

    // Progress bar
    replayProgressBar_ = progressBg->CreateChild<Sprite>();
    replayProgressBar_->SetTexture(cache->GetResource<Texture2D>("Textures/ProgressBar.png"));
    replayProgressBar_->SetSize(0, 10);
    replayProgressBar_->SetAlignment(HA_LEFT, VA_TOP);
    replayProgressBar_->SetPosition(0, 0);
    replayProgressBar_->SetColor(Color::RED);

    // Speed text
    replaySpeedText_ = replayContainer_->CreateChild<Text>();
    replaySpeedText_->SetText("0.5x SPEED");
    replaySpeedText_->SetFont(cache->GetResource<Font>("Fonts/Anonymous Pro.ttf"), 16);
    replaySpeedText_->SetColor(Color::WHITE);
    replaySpeedText_->SetAlignment(HA_CENTER, VA_TOP);
    replaySpeedText_->SetPosition(0, 60);
}

void ReplayUI::UpdateReplayUI(const ReplayTriggerType& event, float progress) {
    if (!uiVisible_) return;

    // Update progress bar
    if (replayProgressBar_) {
        int progressWidth = (int)(300.0f * progress);
        replayProgressBar_->SetSize(progressWidth, 10);
    }

    // Update event type text
    if (replayText_) {
        String eventTypeText;
        switch (event) {
            case REPLAY_CRASH: eventTypeText = "CRASH REPLAY"; break;
            case REPLAY_FLIP: eventTypeText = "FLIP REPLAY"; break;
            case REPLAY_HIGH_SPEED_COLLISION: eventTypeText = "COLLISION REPLAY"; break;
            case REPLAY_AIRTIME: eventTypeText = "AIRTIME REPLAY"; break;
            case REPLAY_MANUAL: eventTypeText = "INSTANT REPLAY"; break;
        }
        replayText_->SetText(eventTypeText);
    }
}

void ReplayUI::SetVisible(bool visible) {
    uiVisible_ = visible;
    if (replayContainer_) {
        replayContainer_->SetVisible(visible);
    }
}