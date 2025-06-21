#include "VehicleReplaySystem.h"
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

#include "VehicleReplaySystem.h"
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

VehicleReplaySystem::VehicleReplaySystem(Context* context) :
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
}

VehicleReplaySystem::~VehicleReplaySystem() {
}

void VehicleReplaySystem::RegisterObject(Context* context) {
    context->RegisterFactory<VehicleReplaySystem>();
}

void VehicleReplaySystem::Initialize(Scene* scene, Camera* gameCamera) {
    scene_ = scene;
    gameCamera_ = gameCamera;

    CreateReplayCamera();

    URHO3D_LOGINFO("VehicleReplaySystem initialized");
}

void VehicleReplaySystem::Update(float timeStep) {
    if (replayState_ == REPLAY_PLAYING) {
        UpdateReplayPlayback(timeStep);
    } else if (replayState_ == REPLAY_RECORDING) {
        // Update recording for all tracked vehicles
        for (auto& pair : recordingData_) {
            Vehicle* vehicle = pair.first_;
            if (vehicle && vehicle->GetNode()) {
                RecordFrame(vehicle);
                if (autoReplayEnabled_) {
                    DetectEvents(vehicle, timeStep);
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

void VehicleReplaySystem::StartRecording(Vehicle* vehicle) {
    if (!vehicle) return;

    recordingData_[vehicle] = Vector<ReplayFrame>();
    trackingData_[vehicle] = VehicleTrackingData();

    URHO3D_LOGINFOF("Started recording vehicle: %s", vehicle->GetNode()->GetName().CString());
}

void VehicleReplaySystem::StopRecording(Vehicle* vehicle) {
    if (!vehicle) return;

    recordingData_.Erase(vehicle);
    trackingData_.Erase(vehicle);

    URHO3D_LOGINFOF("Stopped recording vehicle: %s", vehicle->GetNode()->GetName().CString());
}

void VehicleReplaySystem::TriggerReplay(Vehicle* vehicle, ReplayEventType eventType) {
    if (!vehicle || replayState_ == REPLAY_PLAYING) return;

    if (!recordingData_.Contains(vehicle) || recordingData_[vehicle].Empty()) {
        URHO3D_LOGWARNING("No recording data available for replay");
        return;
    }

    ReplayEvent event;
    event.eventType_ = eventType;
    event.eventTime_ = recordingData_[vehicle].Back().timestamp_;
    event.eventLocation_ = vehicle->GetNode()->GetWorldPosition();
    event.vehicleName_ = vehicle->GetNode()->GetName();
    event.frames_ = recordingData_[vehicle];
    event.severity_ = CalculateEventSeverity(vehicle, eventType);

    if (replayState_ == REPLAY_RECORDING) {
        StartReplay(event);
    } else {
        eventQueue_.Push(event);
    }
}

void VehicleReplaySystem::SkipReplay() {
    if (replayState_ == REPLAY_PLAYING) {
        EndReplay();
    }
}

void VehicleReplaySystem::RecordFrame(Vehicle* vehicle) {
    if (!vehicle || !vehicle->GetNode()) return;

    Time* time = GetSubsystem<Time>();
    float currentTime = time->GetElapsedTime();

    // Limit frame rate to avoid excessive memory usage
    if (currentTime - lastRecordTime_ < 1.0f / maxFrameRate_) {
        return;
    }
    lastRecordTime_ = currentTime;

    RigidBody* body = vehicle->GetBody();
    if (!body) return;

    Node* vehicleNode = vehicle->GetNode();

    ReplayFrame frame(
            currentTime,
            vehicleNode->GetWorldPosition(),
            vehicleNode->GetWorldRotation(),
            body->GetLinearVelocity(),
            body->GetAngularVelocity(),
            vehicle->GetSpeedKmH(),
            vehicle->GetRaycastVehicle()->OnGround()
    );

    Vector<ReplayFrame>& frames = recordingData_[vehicle];
    frames.Push(frame);

    // Remove old frames (keep only recordingDuration_ seconds)
    while (!frames.Empty() &&
           frames.Back().timestamp_ - frames.Front().timestamp_ > recordingDuration_) {
        frames.Erase(0);
    }
}

void VehicleReplaySystem::DetectEvents(Vehicle* vehicle, float timeStep) {
    if (!vehicle || !trackingData_.Contains(vehicle)) return;

    VehicleTrackingData& data = trackingData_[vehicle];

    // Cooldown check
    if (data.eventCooldown_.GetMSec(false) < eventCooldownTime_ * 1000.0f) {
        return;
    }

    RigidBody* body = vehicle->GetBody();
    if (!body) return;

    Vector3 currentPos = vehicle->GetNode()->GetWorldPosition();
    Vector3 currentVel = body->GetLinearVelocity();
    float currentSpeed = vehicle->GetSpeedKmH();
    bool onGround = vehicle->GetRaycastVehicle()->OnGround();

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
            TriggerReplay(vehicle, REPLAY_CRASH);
            data.eventCooldown_.Reset();
            URHO3D_LOGINFOF("Crash detected for vehicle: %s", vehicle->GetNode()->GetName().CString());
        }
    }

    // Flip detection
    if (IsVehicleFlipping(vehicle)) {
        data.flipTime_ += timeStep;
        if (data.flipTime_ > 1.0f) {
            TriggerReplay(vehicle, REPLAY_FLIP);
            data.eventCooldown_.Reset();
            data.flipTime_ = 0.0f;
            URHO3D_LOGINFOF("Flip detected for vehicle: %s", vehicle->GetNode()->GetName().CString());
        }
    } else {
        data.flipTime_ = 0.0f;
    }

    // Airtime detection
    if (data.airTime_ > airTimeThreshold_ && data.lastSpeed_ > 20.0f) {
        TriggerReplay(vehicle, REPLAY_AIRTIME);
        data.eventCooldown_.Reset();
        URHO3D_LOGINFOF("Airtime event detected for vehicle: %s", vehicle->GetNode()->GetName().CString());
    }

    // High speed collision
    if (onGround && currentSpeed > 50.0f &&
        (currentPos - data.lastPosition_).Length() < currentSpeed * timeStep * 0.1f) {
        TriggerReplay(vehicle, REPLAY_HIGH_SPEED_COLLISION);
        data.eventCooldown_.Reset();
        URHO3D_LOGINFOF("High speed collision detected for vehicle: %s", vehicle->GetNode()->GetName().CString());
    }

    // Update tracking data
    data.lastPosition_ = currentPos;
    data.lastVelocity_ = currentVel;
    data.lastSpeed_ = currentSpeed;
    data.wasOnGround_ = onGround;
}

void VehicleReplaySystem::StartReplay(const ReplayEvent& event) {
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

void VehicleReplaySystem::UpdateReplayPlayback(float timeStep) {
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

void VehicleReplaySystem::EndReplay() {
    replayState_ = REPLAY_RECORDING;

    // Switch back to game camera
    if (gameCamera_) {
        auto* renderer = GetSubsystem<Renderer>();
        SharedPtr<Viewport> viewport = SharedPtr(renderer->GetViewport(0));
        viewport->SetCamera(gameCamera_);
    }

    URHO3D_LOGINFO("Replay ended");
}

void VehicleReplaySystem::CreateReplayCamera() {
    if (!scene_) return;

    replayCameraNode_ = scene_->CreateChild("ReplayCamera", LOCAL);
    replayCamera_ = replayCameraNode_->CreateComponent<Camera>();

    // Configure replay camera
    replayCamera_->SetFarClip(24000.0f);
    replayCamera_->SetFov(60.0f);

    URHO3D_LOGINFO("Replay camera created");
}

void VehicleReplaySystem::UpdateReplayCamera(const ReplayFrame& frame, float timeStep) {
    if (!replayCamera_ || !replayCameraNode_) return;

    Vector3 targetPos = CalculateCinematicCameraPosition(frame.position_, cameraAnimTime_);
    Vector3 lookAtPos = frame.position_;

    // Smooth camera movement
    Vector3 currentPos = replayCameraNode_->GetWorldPosition();
    Vector3 newPos = currentPos.Lerp(targetPos, timeStep * 3.0f);

    replayCameraNode_->SetWorldPosition(newPos);
    replayCameraNode_->LookAt(lookAtPos, Vector3::UP);
}

Vector3 VehicleReplaySystem::CalculateCinematicCameraPosition(const Vector3& targetPos, float time) {
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

ReplayCameraMode VehicleReplaySystem::GetCameraMode(ReplayEventType eventType) {
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

ReplayFrame VehicleReplaySystem::InterpolateFrames(const ReplayFrame& a, const ReplayFrame& b, float t) {
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

bool VehicleReplaySystem::IsVehicleFlipping(Vehicle* vehicle) {
    if (!vehicle || !vehicle->GetNode()) return false;

    Vector3 up = vehicle->GetNode()->GetWorldRotation() * Vector3::UP;
    float dotProduct = up.DotProduct(Vector3::UP);
    float angle = Acos(Clamp(dotProduct, -1.0f, 1.0f)) * M_RADTODEG;

    return angle > flipAngleThreshold_;
}

bool VehicleReplaySystem::IsVehicleCrashed(Vehicle* vehicle) {
    if (!vehicle) return false;

    RigidBody* body = vehicle->GetBody();
    if (!body) return false;

    float speed = vehicle->GetSpeedKmH();
    Vector3 velocity = body->GetLinearVelocity();

    // Check if vehicle is moving very slowly but has high angular velocity (spinning)
    return speed < 5.0f && body->GetAngularVelocity().Length() > 5.0f;
}

float VehicleReplaySystem::CalculateEventSeverity(Vehicle* vehicle, ReplayEventType eventType) {
    if (!vehicle) return 0.0f;

    float severity = 0.0f;
    float speed = vehicle->GetSpeedKmH();

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
            if (trackingData_.Contains(vehicle)) {
                float airTime = trackingData_[vehicle].airTime_;
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