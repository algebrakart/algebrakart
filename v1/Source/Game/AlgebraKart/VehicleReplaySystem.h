#pragma once

#include <Urho3D/Core/Object.h>
#include <Urho3D/Container/Vector.h>
#include <Urho3D/Math/Vector3.h>
#include <Urho3D/Math/Quaternion.h>
#include <Urho3D/Scene/Node.h>
#include <Urho3D/Graphics/Camera.h>
#include <Urho3D/Core/Timer.h>
#include <Urho3D/UI/UIElement.h>
#include <Urho3D/UI/Text.h>
#include <Urho3D/UI/Sprite.h>


using namespace Urho3D;

// Forward declarations
class Vehicle;
class NetworkActor;

/// Replay frame data structure
struct ReplayFrame {
    float timestamp_;
    Vector3 position_;
    Quaternion rotation_;
    Vector3 velocity_;
    Vector3 angularVelocity_;
    float speed_;
    bool onGround_;

    ReplayFrame() : timestamp_(0.0f), speed_(0.0f), onGround_(true) {}

    ReplayFrame(float timestamp, const Vector3& pos, const Quaternion& rot,
                const Vector3& vel, const Vector3& angVel, float speed, bool grounded)
            : timestamp_(timestamp), position_(pos), rotation_(rot),
              velocity_(vel), angularVelocity_(angVel), speed_(speed), onGround_(grounded) {}
};

/// Types of replay events
enum ReplayEventType {
    REPLAY_CRASH = 0,
    REPLAY_FLIP,
    REPLAY_HIGH_SPEED_COLLISION,
    REPLAY_AIRTIME,
    REPLAY_MANUAL // Player triggered
};

/// Replay event data
struct ReplayEvent {
    ReplayEventType eventType_;
    float eventTime_;
    Vector3 eventLocation_;
    String vehicleName_;
    Vector<ReplayFrame> frames_;
    float severity_; // 0.0 - 1.0 for event intensity

    ReplayEvent() : eventType_(REPLAY_CRASH), eventTime_(0.0f), severity_(0.0f) {}
};

/// Camera modes for replay viewing
enum ReplayCameraMode {
    REPLAY_CAM_TRACKING = 0,     // Smooth tracking camera
    REPLAY_CAM_CINEMATIC,        // Cinematic angles
    REPLAY_CAM_ORBIT,            // Orbiting around crash point
    REPLAY_CAM_SLOW_MOTION,      // Slow motion with close-up
    REPLAY_CAM_MULTI_ANGLE       // Multiple quick angle changes
};

/// Replay system states
enum ReplayState {
    REPLAY_RECORDING = 0,
    REPLAY_PLAYING,
    REPLAY_PAUSED,
    REPLAY_DISABLED
};

/// Main Vehicle Replay System
class VehicleReplaySystem : public Object {
URHO3D_OBJECT(VehicleReplaySystem, Object);

public:
    /// Constructor
    explicit VehicleReplaySystem(Context* context);

    /// Destructor
    ~VehicleReplaySystem() override;

    /// Register object factory
    static void RegisterObject(Context* context);

    /// Initialize the replay system
    void Initialize(Scene* scene, Camera* gameCamera);

    /// Update the replay system
    void Update(float timeStep);

    /// Start recording a vehicle
    void StartRecording(Vehicle* vehicle);

    /// Stop recording a vehicle
    void StopRecording(Vehicle* vehicle);

    /// Manually trigger a replay event
    void TriggerReplay(Vehicle* vehicle, ReplayEventType eventType = REPLAY_MANUAL);

    /// Check if currently playing a replay
    bool IsPlayingReplay() const { return replayState_ == REPLAY_PLAYING; }

    /// Skip current replay
    void SkipReplay();

    /// Enable/disable automatic replay detection
    void SetAutoReplayEnabled(bool enabled) { autoReplayEnabled_ = enabled; }

    /// Set replay recording duration (seconds before event)
    void SetRecordingDuration(float duration) { recordingDuration_ = duration; }

    /// Set replay playback speed multiplier
    void SetPlaybackSpeed(float speed) { playbackSpeed_ = speed; }

    /// Get current replay camera
    Camera* GetReplayCamera() { return replayCamera_; }

private:
    /// Record a frame for a vehicle
    void RecordFrame(Vehicle* vehicle);

    /// Detect crash/flip events
    void DetectEvents(Vehicle* vehicle, float timeStep);

    /// Start playing a replay
    void StartReplay(const ReplayEvent& event);

    /// Update replay playback
    void UpdateReplayPlayback(float timeStep);

    /// End current replay
    void EndReplay();

    /// Create replay camera
    void CreateReplayCamera();

    /// Update replay camera position
    void UpdateReplayCamera(const ReplayFrame& frame, float timeStep);

    /// Calculate cinematic camera position
    Vector3 CalculateCinematicCameraPosition(const Vector3& targetPos, float time);

    /// Get camera mode for event type
    ReplayCameraMode GetCameraMode(ReplayEventType eventType);

    /// Interpolate between two replay frames
    ReplayFrame InterpolateFrames(const ReplayFrame& a, const ReplayFrame& b, float t);

    /// Check if vehicle is flipping
    bool IsVehicleFlipping(Vehicle* vehicle);

    /// Check if vehicle crashed
    bool IsVehicleCrashed(Vehicle* vehicle);

    /// Calculate event severity
    float CalculateEventSeverity(Vehicle* vehicle, ReplayEventType eventType);

private:
    /// Scene reference
    WeakPtr<Scene> scene_;

    /// Original game camera
    WeakPtr<Camera> gameCamera_;

    /// Replay camera
    SharedPtr<Camera> replayCamera_;
    SharedPtr<Node> replayCameraNode_;

    /// Currently tracked vehicles
    HashMap<Vehicle*, Vector<ReplayFrame>> recordingData_;

    /// Vehicle tracking data for event detection
    struct VehicleTrackingData {
        Vector3 lastPosition_;
        Vector3 lastVelocity_;
        float lastSpeed_;
        float airTime_;
        float flipTime_;
        bool wasOnGround_;
        float lastContactTime_;
        Timer eventCooldown_;

        VehicleTrackingData() : lastSpeed_(0.0f), airTime_(0.0f), flipTime_(0.0f),
                                wasOnGround_(true), lastContactTime_(0.0f) {}
    };
    HashMap<Vehicle*, VehicleTrackingData> trackingData_;

    /// Current replay state
    ReplayState replayState_;

    /// Current replay event being played
    ReplayEvent currentReplay_;

    /// Replay playback time
    float replayTime_;

    /// Replay camera mode
    ReplayCameraMode cameraMode_;

    /// Camera animation time
    float cameraAnimTime_;

    /// Recording settings
    float recordingDuration_;     // Seconds to record before event
    float maxFrameRate_;          // Max frames per second to record
    float lastRecordTime_;

    /// Playback settings
    float playbackSpeed_;
    float replayDuration_;        // How long to show replay

    /// Event detection settings
    float minCrashSpeed_;         // Minimum speed for crash detection
    float flipAngleThreshold_;    // Angle threshold for flip detection
    float airTimeThreshold_;      // Air time threshold for airtime events
    float eventCooldownTime_;     // Time between events for same vehicle

    /// System settings
    bool autoReplayEnabled_;
    bool showReplayUI_;

    /// Frame interpolation
    int currentFrameIndex_;
    float frameTime_;

    /// Camera animation data
    Vector3 cameraStartPos_;
    Vector3 cameraTargetPos_;
    Quaternion cameraStartRot_;
    Quaternion cameraTargetRot_;
    float cameraTransitionTime_;

    /// Event queue for multiple simultaneous events
    Vector<ReplayEvent> eventQueue_;

    float slowMotionFactor_;
    float normalSpeedFactor_;
    float currentTimeDilation_;
};

/// Replay UI component for showing replay status
class ReplayUI : public Object {
URHO3D_OBJECT(ReplayUI, Object);

public:
    explicit ReplayUI(Context* context);
    ~ReplayUI() override;

    /// Initialize UI elements
    void Initialize();

    /// Update UI during replay
    void UpdateReplayUI(const ReplayEvent& event, float progress);

    /// Show/hide replay UI
    void SetVisible(bool visible);

private:
    /// UI elements
    SharedPtr<UIElement> replayContainer_;
    SharedPtr<Text> replayText_;
    SharedPtr<Sprite> replayProgressBar_;
    SharedPtr<Text> replaySpeedText_;
    bool uiVisible_;
};