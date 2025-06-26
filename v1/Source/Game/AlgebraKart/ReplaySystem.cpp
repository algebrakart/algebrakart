#include "ReplaySystem.h"
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

#include "ReplaySystem.h"
#include "network/NetworkActor.h"
#include "network/Vehicle.h"
#include <Urho3D/Scene/Scene.h>
#include <Urho3D/Scene/Node.h>
#include <Urho3D/Graphics/Camera.h>
#include <Urho3D/Physics/RigidBody.h>
#include <Urho3D/Graphics/AnimationController.h>
#include <Urho3D/Graphics/AnimatedModel.h>
#include <Urho3D/Core/Timer.h>
#include <Urho3D/IO/Log.h>
#include <Urho3D/Math/MathDefs.h>

ReplaySystem::ReplaySystem(Context* context) :
        Object(context),
        isRecording_(false),
        autoReplayEnabled_(true),
        recordingDuration_(6.0f),
        recordingTimer_(0.0f),
        isPlayingReplay_(false),
        isPaused_(false),
        playbackSpeed_(0.4f),
        playbackTimer_(0.0f),
        currentFrameIndex_(0),
        currentReplayMode_(REPLAY_GHOST_SIMULATION),
        ghostActor_(nullptr),
        ghostVehicle_(nullptr),
        cinematicTimer_(0.0f),
        cinematicMode_(false),
        cinematicShakeIntensity_(0.0f),
        frameRecordInterval_(1.0f / 30.0f), // 30 FPS recording
        lastRecordTime_(0.0f),
        maxStoredReplays_(10),
        isReplayTriggering_(false),
        lastCollisionTime_(0.0f),
        maxConcurrentReplays_(1)
{
}

ReplaySystem::~ReplaySystem()
{
    StopRecording();
    StopReplay();
    DestroyGhostEntities();
}

void ReplaySystem::Initialize(Scene* scene, Camera* camera)
{
    scene_ = scene;
    originalCamera_ = camera;
    if (camera && camera->GetNode()) {
        originalCameraNode_ = camera->GetNode();
    }

    URHO3D_LOGINFO("Enhanced Replay System initialized");
}

void ReplaySystem::Update(float timeStep)
{
    if (isRecording_) {
        UpdateRecording(timeStep);
    }

    if (isPlayingReplay_ && !isPaused_) {
        UpdatePlayback(timeStep);
    }

    if (cinematicMode_) {
        UpdateCinematicEffects(timeStep);
    }
}

void ReplaySystem::StartRecording(NetworkActor* actor)
{
    if (!actor || !actor->GetNode() || !scene_) {
        URHO3D_LOGERROR("Cannot start recording: invalid actor or scene");
        return;
    }

    // Check if actor is still valid and has a valid name
    String actorName;
    try {
        actorName = actor->GetUserName();
        if (actorName.Empty() || actorName.Length() > 255) {
            URHO3D_LOGWARNING("Actor has invalid name, using default");
            actorName = "Unknown_Actor";
        }
    } catch (...) {
        URHO3D_LOGERROR("Exception getting actor name, using default");
        actorName = "Unknown_Actor";
    }

    StopRecording(); // Stop any existing recording safely

    isRecording_ = true;
    recordingTimer_ = 0.0f;
    lastRecordTime_ = 0.0f;

    currentRecording_.Clear();
    currentRecording_.originalActor = actor;
    currentRecording_.originalVehicle = actor->vehicle_;
    currentRecording_.playerName = actorName;
    currentRecording_.recordingStartTime = GetSubsystem<Time>()->GetElapsedTime();

    URHO3D_LOGINFOF("Started recording for player: %s", actorName.CString());
}

void ReplaySystem::StopRecording()
{
    if (!isRecording_) {
        URHO3D_LOGDEBUG("StopRecording called but not currently recording");
        return;
    }

    URHO3D_LOGINFO("Stopping replay recording");

    isRecording_ = false;

    try {
        if (!currentRecording_.frames.Empty()) {
            currentRecording_.totalDuration = recordingTimer_;

            // Validate the recording data before saving
            if (currentRecording_.totalDuration > 0.0f &&
                currentRecording_.frames.Size() > 0) {

                // Save to replay collection
                savedReplays_.Push(currentRecording_);

                // Limit stored replays to prevent memory issues
                while (savedReplays_.Size() > maxStoredReplays_) {
                    try {
                        savedReplays_.Erase(0);
                    } catch (...) {
                        URHO3D_LOGERROR("Exception while removing old replay");
                        break;
                    }
                }

                URHO3D_LOGINFOF("Recording stopped. Captured %d frames over %f seconds",
                                currentRecording_.frames.Size(), currentRecording_.totalDuration);
            } else {
                URHO3D_LOGWARNING("Recording data appears invalid, not saving");
            }
        } else {
            URHO3D_LOGWARNING("No frames captured during recording");
        }
    } catch (...) {
        URHO3D_LOGERROR("Exception while stopping recording");
    }

    // Always reset recording state, even if there was an error
    recordingTimer_ = 0.0f;
    lastRecordTime_ = 0.0f;

    // Clear current recording data safely
    try {
        currentRecording_.Clear();
    } catch (...) {
        URHO3D_LOGERROR("Exception while clearing recording data");
    }
}

bool ReplaySystem::IsActorValid(NetworkActor* actor) const
{
    if (!actor || !actor->GetNode()) {
        return false;
    }

    // Check if the node is still in the scene
    if (!actor->GetNode()->GetScene()) {
        return false;
    }

    // Check if the actor has valid name
    try {
        String name = actor->GetUserName();
        if (name.Empty() || name.Length() > 255) {
            return false;
        }
    } catch (...) {
        return false;
    }

    return true;
}

void ReplaySystem::CaptureFrame(NetworkActor* actor)
{
    if (!IsActorValid(actor)) {
        URHO3D_LOGWARNING("Cannot capture frame: invalid actor");
        return;
    }

    try {
        ReplayFrame frame;
        frame.timestamp = recordingTimer_;

        // Capture actor data safely
        Node* actorNode = actor->GetNode();
        if (!actorNode) return;

        frame.actorPosition = actorNode->GetWorldPosition();
        frame.actorRotation = actorNode->GetWorldRotation();

        RigidBody* actorBody = actor->GetBody();
        if (actorBody) {
            frame.actorVelocity = actorBody->GetLinearVelocity();
            frame.actorAngularVelocity = actorBody->GetAngularVelocity();
        } else {
            frame.actorVelocity = Vector3::ZERO;
            frame.actorAngularVelocity = Vector3::ZERO;
        }

        // Safely capture actor state
        frame.onGround = actor->onGround_;
        frame.onVehicle = actor->onVehicle_;
        frame.entered = actor->entered_;
        frame.health = actor->life_;
        frame.prana = actor->prana_;
        frame.isAlive = actor->alive_;

        // Capture vehicle data safely
        if (actor->vehicle_ && actor->vehicle_->GetNode()) {
            Node* vehicleNode = actor->vehicle_->GetNode();
            frame.vehiclePosition = vehicleNode->GetWorldPosition();
            frame.vehicleRotation = vehicleNode->GetWorldRotation();

            if (actor->vehicle_->GetRaycastVehicle()) {
                RigidBody* vehicleBody = actor->vehicle_->GetRaycastVehicle()->GetBody();
                if (vehicleBody) {
                    frame.vehicleVelocity = vehicleBody->GetLinearVelocity();
                    frame.vehicleAngularVelocity = vehicleBody->GetAngularVelocity();
                }
            }

            frame.steering = actor->vehicle_->GetSteering();
            frame.rpm = actor->vehicle_->GetCurrentRPM();
            frame.speedKmH = actor->vehicle_->GetSpeedKmH();
        } else {
            // Set default vehicle values
            frame.vehiclePosition = Vector3::ZERO;
            frame.vehicleRotation = Quaternion::IDENTITY;
            frame.vehicleVelocity = Vector3::ZERO;
            frame.vehicleAngularVelocity = Vector3::ZERO;
            frame.steering = 0.0f;
            frame.rpm = 0.0f;
            frame.speedKmH = 0.0f;
        }

        // Safely capture controls
        frame.controls = actor->GetControls();

        // Safely capture camera data
        if (originalCamera_ && originalCameraNode_) {
            frame.cameraPosition = originalCameraNode_->GetWorldPosition();
            frame.cameraRotation = originalCameraNode_->GetWorldRotation();
            frame.cameraFov = originalCamera_->GetFov();
        } else {
            frame.cameraPosition = Vector3::ZERO;
            frame.cameraRotation = Quaternion::IDENTITY;
            frame.cameraFov = 45.0f;
        }

        currentRecording_.frames.Push(frame);

    } catch (...) {
        URHO3D_LOGERROR("Exception while capturing replay frame");
    }
}

void ReplaySystem::UpdateRecording(float timeStep)
{
    recordingTimer_ += timeStep;

    // Check if we should record a frame
    if (recordingTimer_ - lastRecordTime_ >= frameRecordInterval_) {
        if (currentRecording_.originalActor) {
            CaptureFrame(currentRecording_.originalActor);
            lastRecordTime_ = recordingTimer_;
        }
    }

    // Trim old frames to maintain recording duration
    TrimOldFrames();
}

void ReplaySystem::TrimOldFrames()
{
    // Remove frames older than recording duration
    float cutoffTime = recordingTimer_ - recordingDuration_;

    while (!currentRecording_.frames.Empty() &&
           currentRecording_.frames[0].timestamp < cutoffTime) {
        currentRecording_.frames.Erase(0);
    }
}

void ReplaySystem::TriggerReplay(NetworkActor* actor, ReplayTriggerType triggerType, ReplayMode mode)
{
    // Prevent multiple simultaneous replay triggers
    if (isReplayTriggering_ || isPlayingReplay_) {
        URHO3D_LOGDEBUG("Replay trigger ignored - replay already in progress");
        return;
    }

    if (!actor || !actor->GetNode()) {
        URHO3D_LOGWARNING("Cannot trigger replay: invalid actor");
        return;
    }

    if (currentRecording_.frames.Empty()) {
        URHO3D_LOGWARNING("Cannot trigger replay: no recorded data available");
        return;
    }

    // Set flag to prevent race conditions
    isReplayTriggering_ = true;

    // Get player name safely
    String playerName = "Unknown";
    try {
        String actorName = actor->GetUserName();
        if (!actorName.Empty() && actorName.Length() < 256) { // Sanity check
            playerName = actorName;
        }
    } catch (...) {
        URHO3D_LOGERROR("Exception while getting player name, using default");
    }

    // Stop current recording safely
    StopRecording();

    // Use the most recent recording
    currentReplay_ = currentRecording_;
    currentReplayMode_ = mode;

    // Additional safety checks before playing
    if (currentReplay_.frames.Empty()) {
        URHO3D_LOGERROR("No frames available after recording stop");
        isReplayTriggering_ = false;
        return;
    }

    // Validate the actor is still valid
    if (!currentReplay_.originalActor || !currentReplay_.originalActor->GetNode()) {
        URHO3D_LOGERROR("Original actor is no longer valid");
        isReplayTriggering_ = false;
        return;
    }

    try {
        PlayReplay(currentReplay_, mode);
    } catch (...) {
        URHO3D_LOGERROR("Exception during replay playback initialization");
        isReplayTriggering_ = false;
        return;
    }

    String modeStr = (mode == REPLAY_CAMERA_ONLY) ? "camera-only" :
                     (mode == REPLAY_GHOST_SIMULATION) ? "ghost simulation" : "full takeover";

    URHO3D_LOGINFOF("Triggered %s replay for %s with %d frames",
                    modeStr.CString(),
                    playerName.CString(),
                    currentReplay_.frames.Size());

    // Clear the flag
    isReplayTriggering_ = false;
}

void ReplaySystem::PlayReplay(const ReplayData& replayData, ReplayMode mode)
{
    if (replayData.frames.Empty()) {
        URHO3D_LOGWARNING("Cannot play replay: no frames available");
        return;
    }

    StopReplay(); // Stop any current replay

    currentReplay_ = replayData;
    currentReplayMode_ = mode;
    isPlayingReplay_ = true;
    isPaused_ = false;
    playbackTimer_ = 0.0f;
    currentFrameIndex_ = 0;

    // Save original camera state
    SaveOriginalCameraState();

    // Create ghost entities if needed
    if (mode == REPLAY_GHOST_SIMULATION || mode == REPLAY_FULL_TAKEOVER) {
        CreateGhostEntities();
    }

    // Enable cinematic mode
    cinematicMode_ = true;
    cinematicTimer_ = 0.0f;
    CreateCinematicEffects();

    URHO3D_LOGINFOF("Started replay playback: %f seconds, %d frames",
                    replayData.totalDuration, replayData.frames.Size());
}

ReplayFrame ReplaySystem::InterpolateFrames(const ReplayFrame& frame1, const ReplayFrame& frame2, float t)
{
    ReplayFrame result = frame1; // Start with first frame

    // Interpolate positions and rotations
    result.actorPosition = frame1.actorPosition.Lerp(frame2.actorPosition, t);
    result.actorRotation = frame1.actorRotation.Slerp(frame2.actorRotation, t);
    result.actorVelocity = frame1.actorVelocity.Lerp(frame2.actorVelocity, t);

    result.vehiclePosition = frame1.vehiclePosition.Lerp(frame2.vehiclePosition, t);
    result.vehicleRotation = frame1.vehicleRotation.Slerp(frame2.vehicleRotation, t);
    result.vehicleVelocity = frame1.vehicleVelocity.Lerp(frame2.vehicleVelocity, t);

    result.cameraPosition = frame1.cameraPosition.Lerp(frame2.cameraPosition, t);
    result.cameraRotation = frame1.cameraRotation.Slerp(frame2.cameraRotation, t);

    // Interpolate scalar values
    result.steering = Lerp(frame1.steering, frame2.steering, t);
    result.rpm = Lerp(frame1.rpm, frame2.rpm, t);
    result.speedKmH = Lerp(frame1.speedKmH, frame2.speedKmH, t);
    result.cameraFov = Lerp(frame1.cameraFov, frame2.cameraFov, t);

    // Use nearest neighbor for discrete values
    result.controls = t < 0.5f ? frame1.controls : frame2.controls;
    result.currentAnimation = t < 0.5f ? frame1.currentAnimation : frame2.currentAnimation;
    result.onGround = t < 0.5f ? frame1.onGround : frame2.onGround;
    result.onVehicle = t < 0.5f ? frame1.onVehicle : frame2.onVehicle;
    result.entered = t < 0.5f ? frame1.entered : frame2.entered;

    return result;
}

void ReplaySystem::ApplyReplayFrame(const ReplayFrame& frame)
{
    switch (currentReplayMode_) {
        case REPLAY_CAMERA_ONLY:
            UpdateReplayCamera(frame);
            break;

        case REPLAY_GHOST_SIMULATION:
            UpdateGhostActor(frame);
            UpdateGhostVehicle(frame);
            UpdateReplayCamera(frame);
            break;

        case REPLAY_FULL_TAKEOVER:
            // Apply to original entities (be careful with this mode)
            if (currentReplay_.originalActor && currentReplay_.originalActor->GetNode()) {
                UpdateGhostActor(frame); // Apply to original
                UpdateGhostVehicle(frame); // Apply to original
            }
            UpdateReplayCamera(frame);
            break;
    }
}

void ReplaySystem::CreateGhostEntities()
{
    if (!scene_ || !currentReplay_.originalActor) {
        URHO3D_LOGERROR("Cannot create ghost entities: invalid scene or actor");
        return;
    }

    DestroyGhostEntities(); // Clean up any existing ghosts

    auto* cache = GetSubsystem<ResourceCache>();
    if (!cache) {
        URHO3D_LOGERROR("Cannot access ResourceCache");
        return;
    }

    // Create ghost actor with safety checks
    ghostActorNode_ = scene_->CreateChild("ReplayGhostActor", LOCAL);
    if (!ghostActorNode_) {
        URHO3D_LOGERROR("Failed to create ghost actor node");
        return;
    }

    ghostActor_ = ghostActorNode_->CreateComponent<NetworkActor>();
    if (!ghostActor_) {
        URHO3D_LOGERROR("Failed to create ghost NetworkActor component");
        ghostActorNode_->Remove();
        ghostActorNode_.Reset();
        return;
    }

    // Copy actor setup from original with null checks
    Node* originalActorNode = currentReplay_.originalActor->GetNode();
    if (originalActorNode) {
        // Copy model and animations with safety checks
        auto* originalModel = originalActorNode->GetComponent<AnimatedModel>();
        if (originalModel && originalModel->GetModel()) {
            auto* ghostModel = ghostActorNode_->CreateComponent<AnimatedModel>();
            if (ghostModel) {
                ghostModel->SetModel(originalModel->GetModel());
                ghostModel->SetCastShadows(false);

                // Safely create translucent materials
                for (unsigned i = 0; i < ghostModel->GetNumGeometries(); ++i) {
                    auto* material = ghostModel->GetMaterial(i);
                    if (material) {
                        try {
                            SharedPtr<Material> ghostMaterial = material->Clone();
                            if (ghostMaterial) {
                                // Check if technique exists before using it
                                auto* technique = cache->GetResource<Technique>("Techniques/DiffAlpha.xml");
                                if (!technique) {
                                    // Fallback to a basic technique
                                    technique = cache->GetResource<Technique>("Techniques/Diff.xml");
                                }

                                if (technique) {
                                    ghostMaterial->SetTechnique(0, technique);
                                    ghostMaterial->SetShaderParameter("MatDiffColor", Vector4(1.0f, 1.0f, 1.0f, 0.5f));
                                    ghostModel->SetMaterial(i, ghostMaterial);
                                } else {
                                    URHO3D_LOGWARNING("Could not load technique for ghost material");
                                }
                            }
                        } catch (...) {
                            URHO3D_LOGERROR("Exception while creating ghost material");
                        }
                    }
                }
            }
        }

        // Create rigid body safely
        auto* ghostBody = ghostActorNode_->CreateComponent<RigidBody>();
        if (ghostBody) {
            ghostBody->SetKinematic(true);
            ghostBody->SetCollisionLayer(0); // No collisions
        }
    }

    // Create ghost vehicle with safety checks
    if (currentReplay_.originalVehicle && currentReplay_.originalVehicle->GetNode()) {
        ghostVehicleNode_ = scene_->CreateChild("ReplayGhostVehicle", LOCAL);
        if (!ghostVehicleNode_) {
            URHO3D_LOGERROR("Failed to create ghost vehicle node");
            return;
        }

        ghostVehicle_ = ghostVehicleNode_->CreateComponent<Vehicle>();
        if (!ghostVehicle_) {
            URHO3D_LOGWARNING("Failed to create ghost Vehicle component");
        }

        // Copy vehicle model safely
        Node* originalVehicleNode = currentReplay_.originalVehicle->GetNode();
        auto* originalModel = originalVehicleNode->GetComponent<StaticModel>();
        if (originalModel && originalModel->GetModel()) {
            auto* ghostModel = ghostVehicleNode_->CreateComponent<StaticModel>();
            if (ghostModel) {
                ghostModel->SetModel(originalModel->GetModel());
                ghostModel->SetCastShadows(false);

                // Safely create vehicle material
                auto* material = originalModel->GetMaterial();
                if (material) {
                    try {
                        SharedPtr<Material> ghostMaterial = material->Clone();
                        if (ghostMaterial) {
                            auto* technique = cache->GetResource<Technique>("Techniques/DiffAlpha.xml");
                            if (!technique) {
                                technique = cache->GetResource<Technique>("Techniques/Diff.xml");
                            }

                            if (technique) {
                                ghostMaterial->SetTechnique(0, technique);
                                ghostMaterial->SetShaderParameter("MatDiffColor", Vector4(0.8f, 0.8f, 1.0f, 0.6f));
                                ghostModel->SetMaterial(ghostMaterial);
                            }
                        }
                    } catch (...) {
                        URHO3D_LOGERROR("Exception while creating ghost vehicle material");
                    }
                }
            }
        }

        // Create kinematic body safely
        auto* ghostBody = ghostVehicleNode_->CreateComponent<RigidBody>();
        if (ghostBody) {
            ghostBody->SetKinematic(true);
            ghostBody->SetCollisionLayer(0);
        }
    }

    URHO3D_LOGINFO("Successfully created ghost entities for replay");
}

void ReplaySystem::UpdateReplayCamera(const ReplayFrame& frame)
{
    if (!originalCamera_ || !originalCameraNode_) return;

    // Create dynamic camera movement based on replay mode
    Vector3 targetPosition = frame.cameraPosition;
    Quaternion targetRotation = frame.cameraRotation;

    // Add cinematic camera movement
    if (cinematicMode_) {
        // Add slight camera shake for excitement
        Vector3 shake = Vector3(
                (Random(2.0f) - 1.0f) * cinematicShakeIntensity_,
                (Random(2.0f) - 1.0f) * cinematicShakeIntensity_,
                (Random(2.0f) - 1.0f) * cinematicShakeIntensity_
        );
        targetPosition += shake;

        // Add dynamic camera angles
        Vector3 offset = cinematicCameraOffset_;
        offset *= Sin(cinematicTimer_ * 0.5f) * 0.3f; // Subtle movement
        targetPosition += offset;
    }

    // Smoothly move camera
    Vector3 currentPos = originalCameraNode_->GetWorldPosition();
    Quaternion currentRot = originalCameraNode_->GetWorldRotation();

    float smoothing = 0.85f; // Adjust for camera smoothness
    originalCameraNode_->SetWorldPosition(currentPos.Lerp(targetPosition, smoothing));
    originalCameraNode_->SetRotation(currentRot.Slerp(targetRotation, smoothing));

    // Update camera properties
    originalCamera_->SetFov(frame.cameraFov);
}

void ReplaySystem::StopReplay()
{
    if (!isPlayingReplay_) return;

    URHO3D_LOGINFO("Stopping replay playback");

    isPlayingReplay_ = false;
    isPaused_ = false;
    cinematicMode_ = false;
    isReplayTriggering_ = false; // Clear the triggering flag

    // Safely restore original camera state
    try {
        RestoreOriginalCameraState();
    } catch (...) {
        URHO3D_LOGERROR("Exception while restoring camera state");
    }

    // Safely clean up ghost entities
    try {
        DestroyGhostEntities();
    } catch (...) {
        URHO3D_LOGERROR("Exception while destroying ghost entities");
    }

    // Safely clear replay data
    try {
        currentReplay_.Clear();
    } catch (...) {
        URHO3D_LOGERROR("Exception while clearing replay data");
    }

    // Resume recording if auto-replay is enabled and we have a valid actor
    if (autoReplayEnabled_ && currentReplay_.originalActor &&
        currentReplay_.originalActor->GetNode()) {
        try {
            StartRecording(currentReplay_.originalActor);
        } catch (...) {
            URHO3D_LOGERROR("Exception while restarting recording");
        }
    }

    URHO3D_LOGINFO("Replay playback stopped successfully");
}

void ReplaySystem::DestroyGhostEntities()
{
    // Safely destroy ghost actor
    if (ghostActorNode_) {
        try {
            if (ghostActorNode_->GetScene()) { // Check if still in scene
                ghostActorNode_->Remove();
            }
        } catch (...) {
            URHO3D_LOGERROR("Exception while removing ghost actor node");
        }
        ghostActorNode_.Reset();
        ghostActor_ = nullptr;
    }

    // Safely destroy ghost vehicle
    if (ghostVehicleNode_) {
        try {
            if (ghostVehicleNode_->GetScene()) { // Check if still in scene
                ghostVehicleNode_->Remove();
            }
        } catch (...) {
            URHO3D_LOGERROR("Exception while removing ghost vehicle node");
        }
        ghostVehicleNode_.Reset();
        ghostVehicle_ = nullptr;
    }

    URHO3D_LOGDEBUG("Ghost entities destroyed");
}

void ReplaySystem::UpdatePlayback(float timeStep)
{
    if (!isPlayingReplay_ || currentReplay_.frames.Empty()) {
        URHO3D_LOGWARNING("UpdatePlayback called but no valid replay data");
        StopReplay();
        return;
    }

    // Safety check for frame bounds
    if (currentFrameIndex_ < 0 || currentFrameIndex_ >= currentReplay_.frames.Size()) {
        URHO3D_LOGERROR("Invalid frame index in replay playback");
        StopReplay();
        return;
    }

    playbackTimer_ += timeStep * playbackSpeed_;

    // Find current frame(s) for interpolation
    int nextFrameIndex = currentFrameIndex_ + 1;

    // Check if we've reached the end
    if (currentFrameIndex_ >= currentReplay_.frames.Size() - 1) {
        URHO3D_LOGINFO("Replay reached end, stopping");
        StopReplay();
        return;
    }

    // Find the correct frame range with safety checks
    while (nextFrameIndex < currentReplay_.frames.Size() &&
           currentReplay_.frames[nextFrameIndex].timestamp <= playbackTimer_) {
        currentFrameIndex_++;
        nextFrameIndex++;

        // Additional safety check
        if (currentFrameIndex_ >= currentReplay_.frames.Size() - 1) {
            break;
        }
    }

    if (currentFrameIndex_ >= currentReplay_.frames.Size() - 1) {
        URHO3D_LOGINFO("Replay completed");
        StopReplay();
        return;
    }

    // Validate frame indices before interpolation
    if (nextFrameIndex >= currentReplay_.frames.Size()) {
        URHO3D_LOGWARNING("Next frame index out of bounds");
        StopReplay();
        return;
    }

    try {
        // Interpolate between frames
        const ReplayFrame& currentFrame = currentReplay_.frames[currentFrameIndex_];
        const ReplayFrame& nextFrame = currentReplay_.frames[nextFrameIndex];

        float frameDelta = nextFrame.timestamp - currentFrame.timestamp;
        float t = frameDelta > 0 ? (playbackTimer_ - currentFrame.timestamp) / frameDelta : 0.0f;
        t = Clamp(t, 0.0f, 1.0f);

        ReplayFrame interpolatedFrame = InterpolateFrames(currentFrame, nextFrame, t);
        ApplyReplayFrame(interpolatedFrame);
    } catch (...) {
        URHO3D_LOGERROR("Exception during frame interpolation/application");
        StopReplay();
    }
}

void ReplaySystem::UpdateGhostActor(const ReplayFrame& frame)
{
    if (!ghostActorNode_ || !ghostActorNode_->GetScene()) {
        URHO3D_LOGWARNING("Ghost actor node is invalid");
        return;
    }

    try {
        // Update position and rotation safely
        ghostActorNode_->SetWorldPosition(frame.actorPosition);
        ghostActorNode_->SetWorldRotation(frame.actorRotation);

        // Update animation if we have animation controller
        auto* animController = ghostActorNode_->GetComponent<AnimationController>();
        if (animController && !frame.currentAnimation.Empty()) {
            animController->PlayExclusive(frame.currentAnimation, 0, true);
            animController->SetTime(frame.currentAnimation, frame.animationTime);
            animController->SetSpeed(frame.currentAnimation, frame.animationSpeed);
        }

        // Update rigid body velocity for smooth movement
        auto* body = ghostActorNode_->GetComponent<RigidBody>();
        if (body) {
            body->SetLinearVelocity(frame.actorVelocity);
            body->SetAngularVelocity(frame.actorAngularVelocity);
        }
    } catch (...) {
        URHO3D_LOGERROR("Exception while updating ghost actor");
    }
}

void ReplaySystem::UpdateGhostVehicle(const ReplayFrame& frame)
{
    if (!ghostVehicleNode_ || !ghostVehicleNode_->GetScene()) {
        URHO3D_LOGDEBUG("Ghost vehicle node is invalid");
        return;
    }

    try {
        // Update position and rotation safely
        ghostVehicleNode_->SetWorldPosition(frame.vehiclePosition);
        ghostVehicleNode_->SetWorldRotation(frame.vehicleRotation);

        // Update rigid body velocity
        auto* body = ghostVehicleNode_->GetComponent<RigidBody>();
        if (body) {
            body->SetLinearVelocity(frame.vehicleVelocity);
            body->SetAngularVelocity(frame.vehicleAngularVelocity);
        }

        // Update vehicle-specific visuals if available
        if (ghostVehicle_) {
            // Add any vehicle-specific visual updates here
        }
    } catch (...) {
        URHO3D_LOGERROR("Exception while updating ghost vehicle");
    }
}

void ReplaySystem::SaveOriginalCameraState()
{
    if (originalCamera_ && originalCameraNode_) {
        originalCameraPosition_ = originalCameraNode_->GetWorldPosition();
        originalCameraRotation_ = originalCameraNode_->GetWorldRotation();
        originalCameraFov_ = originalCamera_->GetFov();
    }
}

void ReplaySystem::RestoreOriginalCameraState()
{
    if (originalCamera_ && originalCameraNode_) {
        originalCameraNode_->SetWorldPosition(originalCameraPosition_);
        originalCameraNode_->SetRotation(originalCameraRotation_);
        originalCamera_->SetFov(originalCameraFov_);
    }
}

void ReplaySystem::CreateCinematicEffects()
{
    cinematicCameraOffset_ = Vector3(Random(10.0f) - 5.0f, Random(5.0f), Random(10.0f) - 5.0f);
    cinematicShakeIntensity_ = 1.0f;
}

void ReplaySystem::UpdateCinematicEffects(float timeStep)
{
    cinematicTimer_ += timeStep;

    // Reduce shake intensity over time
    cinematicShakeIntensity_ = Max(0.0f, cinematicShakeIntensity_ - timeStep * 2.0f);
}

float ReplaySystem::GetPlaybackProgress() const
{
    if (!isPlayingReplay_ || currentReplay_.frames.Empty()) return 0.0f;

    return playbackTimer_ / currentReplay_.totalDuration;
}

float ReplaySystem::GetRecordingTimeRemaining() const
{
    if (!isRecording_) return 0.0f;

    return Max(0.0f, recordingDuration_ - recordingTimer_);
}

void ReplaySystem::SkipReplay()
{
    if (isPlayingReplay_) {
        StopReplay();
    }
}

void ReplaySystem::PauseReplay()
{
    isPaused_ = true;
}

void ReplaySystem::ResumeReplay()
{
    isPaused_ = false;
}