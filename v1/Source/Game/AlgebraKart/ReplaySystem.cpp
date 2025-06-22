#include "ReplaySystem.h"
#include "network/Vehicle.h"
#include "network/NetworkActor.h"
#include <Urho3D/Scene/Scene.h>
#include <Urho3D/Graphics/Graphics.h>
#include <Urho3D/Graphics/Renderer.h>
#include <Urho3D/Graphics/Camera.h>
#include <Urho3D/Graphics/Texture2D.h>
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

#include "ReplaySystem.h"
#include "network/Vehicle.h"
#include "network/NetworkActor.h"
#include <Urho3D/Scene/Scene.h>
#include <Urho3D/Graphics/Camera.h>
#include <Urho3D/Graphics/Viewport.h>
#include <Urho3D/Physics/RigidBody.h>
#include <Urho3D/Core/CoreEvents.h>
#include <Urho3D/IO/Log.h>
#include <Urho3D/Math/MathDefs.h>
#include <Urho3D/UI/UI.h>
#include <Urho3D/UI/Text.h>
#include <Urho3D/UI/Sprite.h>
#include <Urho3D/Resource/ResourceCache.h>

ReplaySystem::ReplaySystem(Context* context) :
        Object(context),
        replayState_(REPLAY_RECORDING),
        replayTime_(0.0f),
        cameraMode_(REPLAY_CAM_TRACKING),
        cameraAnimTime_(0.0f),
        recordingDuration_(5.0f),
        maxFrameRate_(30.0f),
        lastRecordTime_(0.0f),
        playbackSpeed_(0.5f),
        replayDuration_(3.0f),
        minCrashSpeed_(30.0f),
        flipAngleThreshold_(45.0f),
        airTimeThreshold_(2.0f),
        eventCooldownTime_(3.0f),
        autoReplayEnabled_(true),
        showReplayUI_(true),
        currentFrameIndex_(0),
        frameTime_(0.0f),
        cameraTransitionTime_(0.0f)
{
    slowMotionFactor_ = 0.2f; // 20% speed for dramatic crashes
    normalSpeedFactor_ = 1.0f;
    currentTimeDilation_ = 1.0f;
}

ReplaySystem::~ReplaySystem() {
}

void ReplaySystem::RegisterObject(Context* context) {
    context->RegisterFactory<ReplaySystem>();
}

void ReplaySystem::Initialize(Scene* scene, Camera* gameCamera) {
    scene_ = scene;
    gameCamera_ = gameCamera;

    CreateReplayCamera();

    URHO3D_LOGINFO("ReplaySystem initialized");
}

void ReplaySystem::Update(float timeStep) {
    if (replayState_ == REPLAY_PLAYING) {
        UpdateReplayPlayback(timeStep);
    } else if (replayState_ == REPLAY_RECORDING) {
        // Update recording for all tracked players
        for (auto& pair : recordingData_) {
            NetworkActor* actor = pair.first_;
            if (actor && actor->GetNode()) {
                RecordFrame(actor);
                if (autoReplayEnabled_) {
                    DetectEvents(actor, timeStep);
                }
            }
        }
    }

    // Process event queue
    if (!eventQueue_.Empty() && replayState_ != REPLAY_PLAYING) {
        ReplayEvent nextEvent = eventQueue_.Front();
        eventQueue_.Erase(0);
        StartReplay(nextEvent);
    }
}

void ReplaySystem::StartRecording(NetworkActor* actor) {
    if (!actor) return;

    recordingData_[actor] = Vector<ReplayFrame>();
    trackingData_[actor] = PlayerTrackingData();

    URHO3D_LOGINFOF("Started recording vehicle: %s", actor->GetNode()->GetName().CString());
}

void ReplaySystem::StopRecording(NetworkActor* actor) {
    if (!actor) return;

    recordingData_.Erase(actor);
    trackingData_.Erase(actor);

    URHO3D_LOGINFOF("Stopped recording vehicle: %s", actor->GetNode()->GetName().CString());
}

void ReplaySystem::TriggerReplay(NetworkActor* actor, ReplayEventType eventType) {
    if (!actor || replayState_ == REPLAY_PLAYING) return;

    if (!recordingData_.Contains(actor) || recordingData_[actor].Empty()) {
        URHO3D_LOGWARNING("No recording data available for replay");
        return;
    }

    ReplayEvent event;
    event.eventType_ = eventType;
    event.eventTime_ = recordingData_[actor].Back().timestamp_;
    event.eventLocation_ = actor->GetNode()->GetWorldPosition();
    event.playerName_ = actor->GetNode()->GetName();
    event.frames_ = recordingData_[actor];
    event.severity_ = CalculateEventSeverity(actor, eventType);

    if (replayState_ == REPLAY_RECORDING) {
        StartReplay(event);
    } else {
        eventQueue_.Push(event);
    }
}

void ReplaySystem::SkipReplay() {
    if (replayState_ == REPLAY_PLAYING) {
        EndReplay();
    }
}

void ReplaySystem::RecordFrame(NetworkActor* actor) {
    if (!actor || !actor->GetNode()) return;

    Time* time = GetSubsystem<Time>();
    float currentTime = time->GetElapsedTime();

    // Limit frame rate to avoid excessive memory usage
    if (currentTime - lastRecordTime_ < 1.0f / maxFrameRate_) {
        return;
    }
    lastRecordTime_ = currentTime;

    Node* actorNode = actor->GetNode();

    ReplayFrame frame(
            currentTime,
            actorNode->GetWorldPosition(),
            actorNode->GetWorldRotation(),
            actor->GetLinearVelocity(),
            actor->GetAngularVelocity(),
            0,
            actor->onGround_
    );

    Vector<ReplayFrame>& frames = recordingData_[actor];
    frames.Push(frame);

    // Remove old frames (keep only recordingDuration_ seconds)
    while (!frames.Empty() &&
           frames.Back().timestamp_ - frames.Front().timestamp_ > recordingDuration_) {
        frames.Erase(0);
    }
}

void ReplaySystem::DetectEvents(NetworkActor* actor, float timeStep) {
    if (!actor || !trackingData_.Contains(actor)) return;

    PlayerTrackingData& data = trackingData_[actor];

    // Cooldown check
    if (data.eventCooldown_.GetMSec(false) < eventCooldownTime_ * 1000.0f) {
        return;
    }

    RigidBody* body = actor->GetBody();
    if (!body) return;

    Vector3 currentPos = actor->GetNode()->GetWorldPosition();
    Vector3 currentVel = actor->GetLinearVelocity();
    float currentSpeed = actor->GetAcceleration();
    bool onGround = actor->onGround_;

    // Update tracking data
    if (!data.wasOnGround_ && onGround) {
        data.lastContactTime_ = 0.0f;
    }
    data.lastContactTime_ += timeStep;

    if (!onGround) {
        data.airTime_ += timeStep;
    } else {
        data.airTime_ = 0.0f;
    }

    // Crash detection (sudden velocity change)
    if (data.lastSpeed_ > minCrashSpeed_ && currentSpeed < data.lastSpeed_ * 0.3f) {
        Vector3 velocityChange = currentVel - data.lastVelocity_;
        if (velocityChange.Length() > 20.0f) {
            TriggerReplay(actor, REPLAY_CRASH);
            data.eventCooldown_.Reset();
            URHO3D_LOGINFOF("Crash detected for vehicle: %s", actor->GetNode()->GetName().CString());
        }
    }

    // Flip detection
    if (IsVehicleFlipping(actor)) {
        data.flipTime_ += timeStep;
        if (data.flipTime_ > 1.0f) {
            TriggerReplay(actor, REPLAY_FLIP);
            data.eventCooldown_.Reset();
            data.flipTime_ = 0.0f;
            URHO3D_LOGINFOF("Flip detected for vehicle: %s", actor->GetNode()->GetName().CString());
        }
    } else {
        data.flipTime_ = 0.0f;
    }

    // Airtime detection
    if (data.airTime_ > airTimeThreshold_ && data.lastSpeed_ > 20.0f) {
        TriggerReplay(actor, REPLAY_AIRTIME);
        data.eventCooldown_.Reset();
        URHO3D_LOGINFOF("Airtime event detected for vehicle: %s", actor->GetNode()->GetName().CString());
    }

    // High speed collision
    if (onGround && currentSpeed > 50.0f &&
        (currentPos - data.lastPosition_).Length() < currentSpeed * timeStep * 0.1f) {
        TriggerReplay(actor, REPLAY_HIGH_SPEED_COLLISION);
        data.eventCooldown_.Reset();
        URHO3D_LOGINFOF("High speed collision detected for vehicle: %s", actor->GetNode()->GetName().CString());
    }

    // Update tracking data
    data.lastPosition_ = currentPos;
    data.lastVelocity_ = currentVel;
    data.lastSpeed_ = currentSpeed;
    data.wasOnGround_ = onGround;
}

void ReplaySystem::StartReplay(const ReplayEvent& event) {
    if (event.frames_.Empty()) return;

    currentReplay_ = event;
    replayState_ = REPLAY_PLAYING;
    replayTime_ = 0.0f;
    currentFrameIndex_ = 0;
    frameTime_ = 0.0f;
    cameraAnimTime_ = 0.0f;

    // Determine camera mode based on event type
    cameraMode_ = GetCameraMode(event.eventType_);

    // Switch to replay camera
    if (replayCamera_ && gameCamera_) {
        auto* graphics = GetSubsystem<Graphics>();
        auto* renderer = GetSubsystem<Renderer>();

        // Store original camera position
        cameraStartPos_ = gameCamera_->GetNode()->GetWorldPosition();
        cameraStartRot_ = gameCamera_->GetNode()->GetWorldRotation();

        // Calculate starting replay camera position
        if (!event.frames_.Empty()) {
            ReplayFrame firstFrame = event.frames_[0];
            Vector3 startPos = CalculateCinematicCameraPosition(firstFrame.position_, 0.0f);
            replayCameraNode_->SetWorldPosition(startPos);
            replayCameraNode_->LookAt(firstFrame.position_, Vector3::UP);
        }

        // Switch viewport to replay camera
        SharedPtr<Viewport> viewport = SharedPtr(renderer->GetViewport(0));
        viewport->SetCamera(replayCamera_);
    }

    URHO3D_LOGINFOF("Started replay for event type: %d", (int)event.eventType_);
}

void ReplaySystem::UpdateReplayPlayback(float timeStep) {
    if (currentReplay_.frames_.Empty()) {
        EndReplay();
        return;
    }

    replayTime_ += timeStep * playbackSpeed_;
    frameTime_ += timeStep * playbackSpeed_;
    cameraAnimTime_ += timeStep;

    // Check if replay is finished
    if (replayTime_ > replayDuration_) {
        EndReplay();
        return;
    }

    // Find current and next frames for interpolation
    float targetTime = currentReplay_.frames_[0].timestamp_ + replayTime_;

    while (currentFrameIndex_ < (int)currentReplay_.frames_.Size() - 1 &&
           currentReplay_.frames_[currentFrameIndex_ + 1].timestamp_ < targetTime) {
        currentFrameIndex_++;
    }

    if (currentFrameIndex_ >= (int)currentReplay_.frames_.Size() - 1) {
        EndReplay();
        return;
    }

    // Interpolate between frames
    ReplayFrame currentFrame = currentReplay_.frames_[currentFrameIndex_];
    ReplayFrame nextFrame = currentReplay_.frames_[currentFrameIndex_ + 1];

    float frameDeltaTime = nextFrame.timestamp_ - currentFrame.timestamp_;
    float interpolationFactor = 0.0f;

    if (frameDeltaTime > 0.0f) {
        interpolationFactor = Clamp((targetTime - currentFrame.timestamp_) / frameDeltaTime, 0.0f, 1.0f);
    }

    ReplayFrame interpolatedFrame = InterpolateFrames(currentFrame, nextFrame, interpolationFactor);

    // Update replay camera
    UpdateReplayCamera(interpolatedFrame, timeStep);

    // Update UI if enabled
    if (showReplayUI_) {
        // TODO: Update replay UI with progress
        float progress = replayTime_ / replayDuration_;
        // UpdateReplayUI(currentReplay_, progress);
    }
}

void ReplaySystem::EndReplay() {
    replayState_ = REPLAY_RECORDING;

    // Switch back to game camera
    if (gameCamera_) {
        auto* renderer = GetSubsystem<Renderer>();
        SharedPtr<Viewport> viewport = SharedPtr(renderer->GetViewport(0));
        viewport->SetCamera(gameCamera_);
    }

    URHO3D_LOGINFO("Replay ended");
}

void ReplaySystem::CreateReplayCamera() {
    if (!scene_) return;

    replayCameraNode_ = scene_->CreateChild("ReplayCamera", LOCAL);
    replayCamera_ = replayCameraNode_->CreateComponent<Camera>();

    // Configure replay camera
    replayCamera_->SetFarClip(24000.0f);
    replayCamera_->SetFov(60.0f);

    URHO3D_LOGINFO("Replay camera created");
}

void ReplaySystem::UpdateReplayCamera(const ReplayFrame& frame, float timeStep) {
    if (!replayCamera_ || !replayCameraNode_) return;

    Vector3 targetPos = CalculateCinematicCameraPosition(frame.position_, cameraAnimTime_);
    Vector3 lookAtPos = frame.position_;

    // Smooth camera movement
    Vector3 currentPos = replayCameraNode_->GetWorldPosition();
    Vector3 newPos = currentPos.Lerp(targetPos, timeStep * 3.0f);

    replayCameraNode_->SetWorldPosition(newPos);
    replayCameraNode_->LookAt(lookAtPos, Vector3::UP);
}

Vector3 ReplaySystem::CalculateCinematicCameraPosition(const Vector3& targetPos, float time) {
    Vector3 cameraPos = targetPos;

    switch (cameraMode_) {
        case REPLAY_CAM_TRACKING: {
            // Smooth tracking from behind and above
            cameraPos += Vector3(-30.0f, 20.0f, -40.0f);
            break;
        }

        case REPLAY_CAM_CINEMATIC: {
            // Multiple cinematic angles
            float angle = time * 0.5f;
            float radius = 50.0f;
            cameraPos.x_ += Cos(angle) * radius;
            cameraPos.z_ += Sin(angle) * radius;
            cameraPos.y_ += 25.0f + Sin(time * 2.0f) * 10.0f;
            break;
        }

        case REPLAY_CAM_ORBIT: {
            // Orbit around the crash point
            float angle = time * 1.0f;
            float radius = 40.0f;
            cameraPos.x_ += Cos(angle) * radius;
            cameraPos.z_ += Sin(angle) * radius;
            cameraPos.y_ += 15.0f;
            break;
        }

        case REPLAY_CAM_SLOW_MOTION: {
            // Close-up slow motion
            cameraPos += Vector3(-15.0f, 8.0f, -20.0f);
            break;
        }

        case REPLAY_CAM_MULTI_ANGLE: {
            // Switch between multiple angles
            int angleIndex = (int)(time * 2.0f) % 4;
            switch (angleIndex) {
                case 0: cameraPos += Vector3(-40.0f, 15.0f, 0.0f); break;
                case 1: cameraPos += Vector3(0.0f, 30.0f, -40.0f); break;
                case 2: cameraPos += Vector3(40.0f, 15.0f, 0.0f); break;
                case 3: cameraPos += Vector3(0.0f, 10.0f, 40.0f); break;
            }
            break;
        }
    }

    return cameraPos;
}

ReplayCameraMode ReplaySystem::GetCameraMode(ReplayEventType eventType) {
    switch (eventType) {
        case REPLAY_CRASH:
            return REPLAY_CAM_SLOW_MOTION;
        case REPLAY_FLIP:
            return REPLAY_CAM_ORBIT;
        case REPLAY_HIGH_SPEED_COLLISION:
            return REPLAY_CAM_MULTI_ANGLE;
        case REPLAY_AIRTIME:
            return REPLAY_CAM_CINEMATIC;
        case REPLAY_MANUAL:
        default:
            return REPLAY_CAM_TRACKING;
    }
}

ReplayFrame ReplaySystem::InterpolateFrames(const ReplayFrame& a, const ReplayFrame& b, float t) {
    ReplayFrame result;
    result.timestamp_ = Lerp(a.timestamp_, b.timestamp_, t);
    result.position_ = a.position_.Lerp(b.position_, t);
    result.rotation_ = a.rotation_.Slerp(b.rotation_, t);
    result.velocity_ = a.velocity_.Lerp(b.velocity_, t);
    result.angularVelocity_ = a.angularVelocity_.Lerp(b.angularVelocity_, t);
    result.speed_ = Lerp(a.speed_, b.speed_, t);
    result.onGround_ = t < 0.5f ? a.onGround_ : b.onGround_;
    return result;
}

bool ReplaySystem::IsVehicleFlipping(NetworkActor* actor) {
    if (!actor || !actor->GetNode()) return false;

    Vector3 up = actor->GetNode()->GetWorldRotation() * Vector3::UP;
    float dotProduct = up.DotProduct(Vector3::UP);
    float angle = Acos(Clamp(dotProduct, -1.0f, 1.0f)) * M_RADTODEG;

    return angle > flipAngleThreshold_;
}

bool ReplaySystem::IsVehicleCrashed(NetworkActor* actor) {
    if (!actor) return false;

    float speed = actor->GetAcceleration();
    Vector3 velocity = actor->GetLinearVelocity();

    // Check if vehicle is moving very slowly but has high angular velocity (spinning)
    return speed < 5.0f && actor->GetAngularVelocity().Length() > 5.0f;
}

float ReplaySystem::CalculateEventSeverity(NetworkActor* actor, ReplayEventType eventType) {
    if (!actor) return 0.0f;

    float severity = 0.0f;
    float speed = actor->GetAcceleration();

    switch (eventType) {
        case REPLAY_CRASH:
            severity = Clamp(speed / 100.0f, 0.0f, 1.0f);
            break;
        case REPLAY_FLIP:
            severity = 0.7f; // Flips are generally significant
            break;
        case REPLAY_HIGH_SPEED_COLLISION:
            severity = Clamp(speed / 120.0f, 0.5f, 1.0f);
            break;
        case REPLAY_AIRTIME:
            if (trackingData_.Contains(actor)) {
                float airTime = trackingData_[actor].airTime_;
                severity = Clamp(airTime / 5.0f, 0.3f, 0.8f);
            }
            break;
        case REPLAY_MANUAL:
        default:
            severity = 0.5f;
            break;
    }

    return severity;
}

// ReplayUI Implementation
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
    progressBg->SetTexture(cache->GetResource<Texture2D>("Textures/UI/UIElement.png"));
    progressBg->SetSize(300, 10);
    progressBg->SetAlignment(HA_CENTER, VA_TOP);
    progressBg->SetPosition(0, 40);
    progressBg->SetColor(Color(0.3f, 0.3f, 0.3f, 0.8f));

    // Progress bar
    replayProgressBar_ = progressBg->CreateChild<Sprite>();
    replayProgressBar_->SetTexture(cache->GetResource<Texture2D>("Textures/UI/UIElement.png"));
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

void ReplayUI::UpdateReplayUI(const ReplayEvent& event, float progress) {
    if (!uiVisible_) return;

    // Update progress bar
    if (replayProgressBar_) {
        int progressWidth = (int)(300.0f * progress);
        replayProgressBar_->SetSize(progressWidth, 10);
    }

    // Update event type text
    if (replayText_) {
        String eventTypeText;
        switch (event.eventType_) {
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