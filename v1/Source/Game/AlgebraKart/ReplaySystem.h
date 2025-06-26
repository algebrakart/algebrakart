#pragma once

#include <Urho3D/Core/Object.h>
#include <Urho3D/Container/Vector.h>
#include <Urho3D/Math/Vector3.h>
#include <Urho3D/Math/Quaternion.h>
#include <Urho3D/Input/Controls.h>

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

// Replay frame data structure
struct ReplayFrame {
    float timestamp;

    // Player/Actor data
    Vector3 actorPosition;
    Quaternion actorRotation;
    Vector3 actorVelocity;
    Vector3 actorAngularVelocity;
    bool onGround;
    bool onVehicle;
    bool entered;

    // Vehicle data (if applicable)
    Vector3 vehiclePosition;
    Quaternion vehicleRotation;
    Vector3 vehicleVelocity;
    Vector3 vehicleAngularVelocity;
    float steering;
    float engineForce;
    float brakeForce;
    float rpm;
    float speedKmH;

    // Controls input
    Controls controls;

    // Animation data
    String currentAnimation;
    float animationTime;
    float animationSpeed;

    // Camera data
    Vector3 cameraPosition;
    Quaternion cameraRotation;
    float cameraFov;

    // Additional state
    int health;
    int prana;
    bool isAlive;
};

// Replay data for a complete recording session
struct ReplayData {
    Vector<ReplayFrame> frames;
    float totalDuration;
    String playerName;
    float recordingStartTime;
    NetworkActor* originalActor;
    Vehicle* originalVehicle;

    void Clear() {
        frames.Clear();
        totalDuration = 0.0f;
        playerName.Clear();
        recordingStartTime = 0.0f;
        originalActor = nullptr;
        originalVehicle = nullptr;
    }
};

enum ReplayMode {
    REPLAY_CAMERA_ONLY,
    REPLAY_GHOST_SIMULATION,
    REPLAY_FULL_TAKEOVER
};

enum ReplayTriggerType {
    REPLAY_MANUAL,
    REPLAY_HIGH_SPEED_COLLISION,
    REPLAY_SPECTACULAR_MOMENT,
    REPLAY_RACE_FINISH,
    REPLAY_CRASH,
    REPLAY_FLIP,
    REPLAY_AIRTIME
};

class ReplaySystem : public Object
{
URHO3D_OBJECT(ReplaySystem, Object);

public:
    ReplaySystem(Context* context);
    virtual ~ReplaySystem();

    // Core functionality
    void Initialize(Scene* scene, Camera* camera);
    void Update(float timeStep);

    // Recording
    void StartRecording(NetworkActor* actor);
    void StopRecording();
    void RecordFrame(NetworkActor* actor);

    // Playback
    void TriggerReplay(NetworkActor* actor, ReplayTriggerType triggerType, ReplayMode mode = REPLAY_GHOST_SIMULATION);
    void PlayReplay(const ReplayData& replayData, ReplayMode mode);
    void StopReplay();
    void SkipReplay();
    void PauseReplay();
    void ResumeReplay();

    // Configuration
    void SetRecordingDuration(float duration) { recordingDuration_ = duration; }
    void SetPlaybackSpeed(float speed) { playbackSpeed_ = Clamp(speed, 0.1f, 4.0f); }
    void SetAutoReplayEnabled(bool enabled) { autoReplayEnabled_ = enabled; }
    void SetReplayMode(ReplayMode mode) { currentReplayMode_ = mode; }

    // State queries
    bool IsRecording() const { return isRecording_; }
    bool IsPlayingReplay() const { return isPlayingReplay_; }
    bool IsPaused() const { return isPaused_; }
    float GetPlaybackProgress() const;
    float GetRecordingTimeRemaining() const;

private:
    // Recording methods
    void UpdateRecording(float timeStep);
    void CaptureFrame(NetworkActor* actor);
    void TrimOldFrames();

    // Playback methods
    void UpdatePlayback(float timeStep);
    void ApplyReplayFrame(const ReplayFrame& frame);
    void CreateGhostEntities();
    void DestroyGhostEntities();
    void UpdateGhostActor(const ReplayFrame& frame);
    void UpdateGhostVehicle(const ReplayFrame& frame);
    void UpdateReplayCamera(const ReplayFrame& frame);
    ReplayFrame InterpolateFrames(const ReplayFrame& frame1, const ReplayFrame& frame2, float t);
    bool IsActorValid(NetworkActor* actor) const;


    // Utility methods
    void SaveOriginalCameraState();
    void RestoreOriginalCameraState();
    void CreateCinematicEffects();
    void UpdateCinematicEffects(float timeStep);

    // Core components
    WeakPtr<Scene> scene_;
    WeakPtr<Camera> originalCamera_;
    WeakPtr<Node> originalCameraNode_;

    // Recording state
    bool isRecording_;
    bool autoReplayEnabled_;
    float recordingDuration_;
    float recordingTimer_;
    ReplayData currentRecording_;
    Vector<ReplayData> savedReplays_;

    // Playback state
    bool isPlayingReplay_;
    bool isPaused_;
    float playbackSpeed_;
    float playbackTimer_;
    int currentFrameIndex_;
    ReplayData currentReplay_;
    ReplayMode currentReplayMode_;

    // Ghost entities for simulation
    SharedPtr<Node> ghostActorNode_;
    SharedPtr<Node> ghostVehicleNode_;
    NetworkActor* ghostActor_;
    Vehicle* ghostVehicle_;

    // Original state preservation
    Vector3 originalCameraPosition_;
    Quaternion originalCameraRotation_;
    float originalCameraFov_;
    bool originalCameraState_[4]; // Various camera flags

    // Cinematic effects
    float cinematicTimer_;
    bool cinematicMode_;
    Vector3 cinematicCameraOffset_;
    float cinematicShakeIntensity_;

    // Performance optimization
    float frameRecordInterval_;
    float lastRecordTime_;
    int maxStoredReplays_;

    bool isReplayTriggering_;
    float lastCollisionTime_;
    int maxConcurrentReplays_;
};