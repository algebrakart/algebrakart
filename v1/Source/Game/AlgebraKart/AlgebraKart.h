#pragma once


#include <Urho3D/Core/CoreEvents.h>
#include <Urho3D/Engine/Engine.h>
#include <Urho3D/Graphics/Camera.h>
#include <Urho3D/Graphics/Graphics.h>
#include <Urho3D/Graphics/Octree.h>
#include <Urho3D/Graphics/Renderer.h>
#include <Urho3D/Graphics/Zone.h>
#include <Urho3D/Graphics/Material.h>
#include <Urho3D/Graphics/Model.h>
#include <Urho3D/Graphics/StaticModel.h>
#include <Urho3D/Input/Controls.h>
#include <Urho3D/Input/Input.h>
#include <Urho3D/IO/Log.h>
#include <Urho3D/Network/Network.h>
#include <Urho3D/Network/Connection.h>
#include <Urho3D/UI/Button.h>
#include <Urho3D/UI/Font.h>
#include <Urho3D/UI/LineEdit.h>
#include <Urho3D/UI/Text.h>
#include <Urho3D/UI/Text3D.h>
#include <Urho3D/UI/UI.h>
#include <Urho3D/UI/UIEvents.h>
#include <Urho3D/Scene/SmoothedTransform.h>


// Audio Spectrum Analyzer
#include <AlgebraKart/beat/AudioSpectrumAnalyzer.h>
#include <AlgebraKart/beat/AudioSpectrumAxis.h>
#include <AlgebraKart/beat/AudioSpectrumRender.h>
#include <AlgebraKart/beat/AudioSpectrumCapture.h>

#include <AlgebraKart/ai/evolution_manager.h>
#include <Urho3D/UI/Window.h>
#include <AlgebraKart/network/NetworkActor.h>
#include <AlgebraKart/network/csp/CSP_Client.h>
#include <AlgebraKart/network/csp/CSP_Server.h>
#include <AlgebraKart/network/csp/CSP_messages.h>
#include "Game.h"
#include "AlgebraKart/network/Vehicle.h"
#include "ReplaySystem.h"
#include "ReplayUI.h"
#include <AlgebraKart/PixelImage.h>

#include "beat/BeatSequencerUI.h"
#include "ProceduralVegetation.h"

#define GAME_NAME "AlgebraKart"

// Sound fx mappings
#define SOUND_FX_ENGINE_START 0
#define SOUND_FX_ENGINE_LOOP 1
#define SOUND_FX_ENGINE_REV 2
#define SOUND_FX_TIRE_SKID1 3
#define SOUND_FX_TIRE_SKID2 4
#define SOUND_FX_TIRE_SKID3 5
#define SOUND_FX_ENGINE_BRAKE 6
#define SOUND_FX_ENGINE_BOOST 7

#define SOUND_FX_DIALOGUE_START_GAME 0

std::vector<std::string> driveAudioEffect = {
        "drive/v1-engine-start.ogg",
        "drive/v1-engine-loop.ogg",
        "drive/v1-engine-rev.ogg",
        "drive/v1-engine-skid1.ogg",
        "drive/v1-engine-skid2.ogg",
        "drive/v1-engine-skid3.ogg",
        "drive/v1-engine-brake.ogg",
        "drive/v1-engine-boost.ogg"
};

std::vector<std::string> dialogueAudioEffect = {
        "dialogue/algebrakart-s0.ogg"
};


#define CAM_RAYCAST_TIME_WAIT 0.02f

#define NUM_DRUM_MACHINE_PADS 64


namespace Urho3D {

class Button;
class Connection;
class Scene;
class Text;
class UIElement;
class Node;
class LineEdit;
class DropDownList;

//class Controls;
class RigidBody;


// Marker Map Tokens
Vector3 bkgMarkerToken = Vector3(0.5, 1, 0.5); // Black
Vector3 trackMarkerToken = Vector3(0.500000059604645, 1, 0.643137276172638); // #494949
Vector3 treeMarkerToken = Vector3(0.5, 1, 0.594117641448975); // #303030
Vector3 waypointToken = Vector3(0.5, 1.00000035762787, 0.927451014518738); // #dadada
}

class Character2D;
class Player;
class EvolutionManager;

struct ParticlePool {
    bool used; // Is particle emitter used?
    int usedBy; // Node id using particle emitter
    SharedPtr<Node> node; // Scene node
    float lastEmit;
    float currEmit;
    float timeout;
};


class GameResourceRouter : public ResourceRouter
{
    URHO3D_OBJECT(GameResourceRouter, ResourceRouter);

    /// Construct.
    explicit GameResourceRouter(Context* context) :
        ResourceRouter(context)
    {
        //auto* cache = GetSubsystem<ResourceCache>();
        //cache->AddResourceDir("/apk/");
    }

    /// Check if request is for an resource file and reroute to mobile version if necessary.
    void Route(String& name, ResourceRequest requestType) override
    {

        String extension = GetExtension(name);
        if (extension == ".as")
        {
            auto* cache = GetSubsystem<ResourceCache>();
            String replaced = ReplaceExtension(name, ".asc");
            // Note: ResourceCache prevents recursive calls to the resource routers so this is OK, the nested Exists()
            // check does not go through the router again
            if (!cache->Exists(name) && cache->Exists(replaced))
                name = replaced;
        }
    }
};


/* /\
  /  \
 / || \ lgebraKart.com
 --- ---

*/
class AlgebraKart : public Game
{
    URHO3D_OBJECT(AlgebraKart, Game);
public:
    /// Construct.
    explicit AlgebraKart(Context* context);
    /// Setup after engine initialization and before running the main loop.
    void Start() override;
    /// Setup before engine initialization. Modifies the engine parameters.
    void Setup() override;
    void Stop() override;
    void MoveCamera(float timeStep);


    void SpectrumAnalyzer() {};
    void SetGrounded(bool grounded);
    Vector3 GetCurrentMovement() const;
    void SetMovementParameters(float walkSpeed, float runMultiplier, float jumpForce,
                               float damping, float stiffness);
private:

    // Network functions
    void CreateServerSubsystem();
    void CreateUI();
    void CreateClientUI();

   // void SetupViewport();
    void ChangeDebugHudText();
    Button* CreateButton(const String& text, int width);
    void UpdateButtons();

    // CLIENT CALLS
    // Process Client-side Prediction controls
    Controls SampleCSPControls();

    void PlayDrumMachine();

    // SERVER CALLS
    // Raycast from mouse position
    bool Raycast(Camera* pCamera, float maxDistance, Vector3& hitPos, Drawable*& hitDrawable);
    // Place object node based on raycast
    void PlaceObject(Camera* pCamera, int type);
    // Generate object
    Node* GenerateObject(Vector3 &pos, Quaternion &rot, int type);

    void DrawWave(const Vector2 &start, int length, int pixelAmplitude, float t, const Color &color);
    void DrawFFT(const Vector2 &start, int length, int pixelAmplitude, const Color &color);

    // Handlers ////
    // Handle AI bots, attaches to FFN and Genetic Algorithm in Evolution Manager
    void HandleAI(float timeStep);

    void HandlePhysicsPreStep(StringHash eventType, VariantMap &eventData);
    void HandleLoginListRefresh(StringHash eventType, VariantMap &eventData);
    void HandleRaceRank(StringHash eventType, VariantMap &eventData);
    void HandleRaceVictory(StringHash eventType, VariantMap &eventData);
    void HandleRadioTrackNext(StringHash eventType, VariantMap &eventData);

    void HandleConnect(StringHash eventType, VariantMap& eventData);
    void HandleDisconnect(StringHash eventType, VariantMap& eventData);
    void HandleStartServer(StringHash eventType, VariantMap& eventData);
    void HandleConnectionStatus(StringHash eventType, VariantMap& eventData);
    void HandleClientObjectID(StringHash eventType, VariantMap& eventData);
    void HandleClientIdentity(StringHash eventType, VariantMap& eventData);
    void HandleExit(StringHash eventType, VariantMap& eventData);
    void HandleConnectionFailed(StringHash eventType, VariantMap &eventData);
    void HandleClientDisconnected(StringHash eventType, VariantMap& eventData);
    void HandleServerConnected(StringHash eventType, VariantMap& eventData);
    void HandleServerDisconnected(StringHash eventType, VariantMap& eventData);
    void HandlePlayerRespawned(StringHash eventType, VariantMap& eventData);
    void HandlePlayerLoaded(StringHash eventType, VariantMap& eventData);
    /// Handle log message event; pipe it also to the chat display.
    void HandleLogMessage(StringHash eventType, VariantMap& eventData);
    /// Handle pressing the send chat message.
    void HandleSendChat();
    /// Handle an incoming network message.
    void HandleNetworkMessage(StringHash eventType, VariantMap& eventData);
    /// Handle the logic update event.
    void HandleUpdate(StringHash eventType, VariantMap& eventData);
    void HandleDelayUpdate(StringHash eventType, VariantMap& eventData);
    void HandleRenderUpdate(StringHash eventType, VariantMap &eventData);
    /// Handle the logic post update event.
    void HandlePostUpdate(StringHash eventType, VariantMap& eventData);
    /// Handle the post render update event.
    void HandlePostRenderUpdate(StringHash eventType, VariantMap& eventData);
    // Final render before back buffer flip
    void HandleEndRendering(StringHash eventType, VariantMap &eventData);
    // Resource loaded
    // Server
    void HandleResourceBackgroundLoaded(StringHash eventType, VariantMap& eventData);
    // Client
    void HandleClientResourceLoadFinished(StringHash eventType, VariantMap& eventData);
    void HandleClientResourceLoadAttempt(StringHash eventType, VariantMap& eventData);

    // Client UI update
    void HandleClientUIUpdate();

    void HandleJoystickHatMove(StringHash eventType, VariantMap &eventData);
    /// Handle the end rendering event.
    void HandleSceneRendered(StringHash eventType, VariantMap& eventData);
    void HandleNodeCollision(StringHash eventType, VariantMap& eventData);
    /// Handle the contact begin event (Box2D contact listener).
    void HandleCollisionBegin(StringHash eventType, VariantMap& eventData);
    /// Handle the contact end event (Box2D contact listener).
    void HandleCollisionEnd(StringHash eventType, VariantMap& eventData);
    /// Handle scene update event to control camera's pitch and yaw for all samples.
    void HandleSceneUpdate(StringHash eventType, VariantMap& eventData);
    /// Handle a client connecting to the server.
    void HandleClientConnected(StringHash eventType, VariantMap& eventData);
    /// Handle remote event from server which tells our controlled object node ID.
    void HandleKeyDown(StringHash eventType, VariantMap& eventData);
    /// Handle 'PLAY' button released event.
    void HandlePlayButton(StringHash eventType, VariantMap& eventData);
    void HandleClosePressed(StringHash eventType, VariantMap& eventData);
    void HandleClientSceneLoaded(StringHash eventType, VariantMap& eventData);
    void HandlePlayerStateUpdate(StringHash eventType, VariantMap& eventData);
    void HandlePlayerPickUpState(StringHash eventType, VariantMap& eventData);
    void HandleNetworkUpdateSent(StringHash eventType, VariantMap& eventData);
    void HandleUpdateParticlePool(float timeStep);

    // Handle collision replay
    void HandleCollisionReplay(StringHash eventType, VariantMap &eventData);
    // Helpers
    float GetMaxCollisionImpulse(VariantMap& eventData);
    Vehicle* FindVehicleFromNode(Node* node);
    NetworkActor* FindNetworkActorFromNode(Node* node);

        // Async loading functions
    void NodeRegisterLoadTriggers(Node *node);
    void HandleLoadTriggerEntered(StringHash eventType, VariantMap& eventData);
    void HandleLoadProgress(StringHash eventType, VariantMap& eventData);
    void HandleLevelLoaded(StringHash eventType, VariantMap& eventData);

    // Chat message functions
    void HandleChatMessage(StringHash eventType, VariantMap& eventData);
    void ShowChatText(const String& row);

    // Start Server
    void DoStartServer();
    // Play button
    void DoPlay();
    // Connect to server (client start)
    void DoConnect();

    // Creates ////
    /// Construct the scene content.
    //void CreateScene();
    void CreateEmptyScene(Context* context);
    // Viewport
    void InitiateViewport(Context* context, Scene* scene, Camera* camera, int id);
    void SetupMenuViewport(); // Setup menu view
    void SetupSequencerViewport(); // Setup sequencer view
    void SetupGameViewports(); // Setup game view


    // Game Map
    void InitiateGameMap(Scene* scene);
    // Init Genetic Algorithm generator: Evolution Manager
    static void InitEvolutionManager();


    SharedPtr<Node> SpawnPlayer(); // Spawn local
    Node* SpawnPlayer(Connection* connection); // Spawn with connection
    void SendWelcomeMessage(Connection* connection); // Send welcome message
    std::shared_ptr<NetworkActor> SpawnPlayer(unsigned int id); // Spawn for ai bot
    void DestroyPlayer(Connection* connection); // Clean player with connection

    void StartMultiplayerGameSession();
    bool isMultiplayer_;
    bool handlersSet = false;

    void CreateAgents();

    /// Subscribe to application-wide logic update events.
    void SubscribeToEvents();


    /// Handle reloading the scene.
    void ReloadScene(bool reInit);
    void LoadLevel(int id);
    void PlaySoundEffect(const String& soundName);
    void PlaySoundEffectGlobal(const String& soundName);
    SoundSource3D *PlaySoundEffectLocal(const String& soundName);
    void PlayMusic(const String &soundName);

    HashMap<String, float> sndFxPlay_;

    void SetParticleEmitter(int hitId, float contactX, float contactY, int type, float timeStep);


    // Update menu state (change visibility of buttons)
    void UpdateUIState(bool state);


    // Display Genetic Algorithm Evolution Manager statistics
    void ShowEvolutionManagerStats();

    void InitMsgWindow(String title, String message);

    void OutputLoginListToConsole();

    String SaveScene(bool initial);

    Controls sample_input();
    void apply_input(Node* playerNode, const Controls& controls);
    void apply_input(Connection* connection, const Controls& controls);

    // Racing formation constants
    const int GRID_COLUMNS = 4;  // Number of columns in starting grid
    const float GRID_SPACING_X = 25.0f;  // Distance between columns
    const float GRID_SPACING_Z = 35.0f;  // Distance between rows
    const Vector3 GRID_START_POSITION = Vector3(0.0f, 20.0f, -200.0f);  // Starting line position
    const Vector3 RACING_DIRECTION = Vector3(-1.0f, 0.0f, 0.0f);  // Forward direction

    // Track next available grid position
    int nextGridPosition_ = 0;

    // Helper method to calculate grid position
    Vector3 GetGridPosition(int gridIndex);
    void ResetGridPositions() { nextGridPosition_ = 0; }

    void UpdateClientObjects();

    struct RadioTrack {
        String artistName;
        String artistLink;
        String trackName;
    };

    void PlayNextTrack(); // Load radio stream

    // Loading Screen
    SharedPtr<UIElement> loadingScreen_;
    SharedPtr<Sprite> loadingBackgroundSprite_;
    SharedPtr<Sprite> loadingLogoSprite_;
    SharedPtr<Text> loadingText_;
    SharedPtr<Text> loadingTipText_;
    SharedPtr<Sprite> loadingProgressBarBg_;
    SharedPtr<Sprite> loadingProgressBar_;
    float loadingProgress_;
    float loadingTipTimer_;
    Vector<String> loadingTips_;
    int currentTipIndex_;

    void CreateLoadingScreen();
    void ShowLoadingScreen();
    void HideLoadingScreen();
    void UpdateLoadingProgress(float progress, const String& status = "");
    void UpdateLoadingScreen(float timeStep);
    ////

    HashMap<Connection*, float> welcomeMsgMap_; // Welcome message map for delayed send to clients from server

    String clientName_;
    Vector<SoundSource3D*> radioTrackQueue_;
    Vector<RadioTrack> radioTrackInfoQueue_;
    int radioTrack_ = 0;
    int nextRadioTrack_ = 0;
    SoundSource3D *_radioStream;

    // Audio spectrum analyzer
    AudioSpectrumCapture *capturer_;
    AudioSpectrumAnalyzer *analyzer_;
    AudioSpectrumRenderer *renderer_;

    struct {
        int characterSize = 0;
        float bottomLevel = 0;
        float topLevel = 0;
        float minFreq = 0;
        float maxFreq = 0;
        float axisLogBase = 0;
        int fftSize = 0;
        int inputSize = 0;
        int windowSize = 0;
        float smoothing = 0;
        bool displayAxis = false;
        std::string lineFeed;
        int samplingFrequency = 0;
    } audioSpectrumOptions_;




    bool accelSndPlaying_;
    WeakPtr<SoundSource3D> lastSnd_;
    bool playDrumMachine_;

    // Async level loading
    WeakPtr<Node> curLevel_;
    WeakPtr<Node> nextLevel_;
    String        levelPathName_;
    String        levelLoadPending_;
    bool          levelLoading_; // Server level load
    bool          clientLevelLoading_; // Client level load
    float         loadProgress_;

    WeakPtr<Text> levelText_;
    WeakPtr<Text> triggerText_;
    WeakPtr<Text> progressText_;
    WeakPtr<Text> progressResText_;

    // Network Actor Hash Maps
    HashMap<Connection*, WeakPtr<ClientObj>> actorMap_;
    HashMap<unsigned int, WeakPtr<ClientObj>> aiActorMap_;
    Connection *lastConnection_; // More recent connected client

    bool started_; // Is Game started?

    // Login received list
    Vector<Urho3D::String> loginList_;

    // Race rank received lists
    Vector<Urho3D::String> rcvPlayerIdList_;
    Vector<Urho3D::String> rcvRankList_;
    Vector<Urho3D::String> rcvTimeList_;

    Controls ntwkControls_;
    Controls lastControls_;

    SharedPtr<Window> msgWindow_;

    SharedPtr<Terrain> terrain_;
    Vector<Vector3> trees_;

    Vector<Vector3> waypoints_;
    Vector<Vector3> waypointsWorld_;
    unsigned int wpStartIndex_;

    #define CAMERA_RAY_DISTANCE_LIMIT 70.0f
    // Camera objects
    SharedPtr<Camera> serverCam_;
    SharedPtr<Camera> clientCam_;
    SharedPtr<Camera> seqCam_;
    float seqCamOffset_ = 0;

    SharedPtr<CollisionShape> terrainShape_;


    // Network messages
    Vector<Connection*> sendLoadMsg_;
    Vector<Connection*> sendSpawnMsg_;
    float lastRaceRank_ = 0; // Race rank timer
    float lastRaceVictory_ = 0; // Race rank timer

    // Client-side Prediction
    SharedPtr<CSP_Client> cspClient_;
    SharedPtr<PhysicsWorld> clientPhysicsWorld_;
    SharedPtr<CSP_Server> cspServer_;

    // Camera attributes
    float camSpin_;
    float camSpin2_;
    float camZoom_;

    int camMode_;
    float lastCamRaycast = 0; // Last camera raycast

    Vector3 heliCamView_; // Helicopter view

    Vector3 flyCam_; // Fly camera position
    Vector3 flyTargetCam_; // Fly camera target position
    Vector3 snapCam_; // Vehicle snap camera position
    SharedPtr<Node> camTargetNode_;

    Vector<Vector3> focusObjects_;
    unsigned int focusIndex_;
    double lastPlayerStateTime_;

    //WeakPtr<TileMap3D> tileMap_;

    /// Flag for drawing debug geometry.
    bool drawDebug_{};
    bool doSpecial_{};
    /// Scaling factor based on tiles' aspect ratio.
    float moveSpeedScale_{};

    SharedPtr<Text> versionText_;
    SharedPtr<Text> studioText_;
    SharedPtr<Text> gameNameText_;

    SharedPtr<Text> healthText_;
    SharedPtr<Text> healthProgBarText_;
    SharedPtr<Text> powerBarText_;
    SharedPtr<Text> powerBarProgBarText_;
    SharedPtr<Text> rpmBarText_;
    SharedPtr<Text> rpmBarProgBarText_;
    SharedPtr<Text> velBarText_;
    SharedPtr<Text> velBarProgBarText_;
    SharedPtr<Text> pranaBarText_;
    SharedPtr<Text> pranaBarProgBarText_;

    /// Strings printed so far.
    Vector<String> chatHistory_;
    /// Chat text element.
    SharedPtr<Text> chatHistoryText_;
    /// Server address / chat message line editor element.
    SharedPtr<LineEdit> textEdit_;

    // Menu background sprite.
    SharedPtr<Sprite> bkgSprite_;
    float bkgAngle_;

    /// Health sprite.
    SharedPtr<Sprite> HealthSprite_;

    /// Powerbar P1 sprite.
    SharedPtr<Sprite> powerBarP1Sprite_;
    /// Powerbar Bkg P1 sprite.
    SharedPtr<Sprite> powerBarBkgP1Sprite_;

    /// RPM P1 sprite.
    SharedPtr<Sprite> rpmBarP1Sprite_;
    /// RPM Bkg P1 sprite.
    SharedPtr<Sprite> rpmBarBkgP1Sprite_;

    /// Velocity P1 sprite.
    SharedPtr<Sprite> velBarP1Sprite_;
    /// Velocity Bkg P1 sprite.
    SharedPtr<Sprite> velBarBkgP1Sprite_;

    /// HUD vehicle damage sprite.
    SharedPtr<Sprite> vDamageSprite_;
    Vector<SharedPtr<Sprite>> vDamage_FL_LineSprites_; // Front left wheel
    Vector<SharedPtr<Sprite>> vDamage_FR_LineSprites_; // Front rear wheel
    Vector<SharedPtr<Sprite>> vDamage_BL_LineSprites_; // Back left wheel
    Vector<SharedPtr<Sprite>> vDamage_BR_LineSprites_; // Back right wheel
    Vector<SharedPtr<Sprite>> vDamage_FA_LineSprites_; // Front axle
    Vector<SharedPtr<Sprite>> vDamage_RA_LineSprites_; // Rear axle

    Vector<Texture2D*> hudPickupItemTextures_;
    /// HUD pick up sprite.
    SharedPtr<Sprite> pickupSprite_;
    /// HUD pick up item sprite.
    SharedPtr<Sprite> pickupItemSprite_;

    /// Mini-map P1 sprite.
    SharedPtr<Sprite> miniMapP1Sprite_;

    /// Mini-map waypoint sprite.
    SharedPtr<Sprite> miniMapWPSprite_;

    /// Mini-map Bkg sprite.
    SharedPtr<Sprite> miniMapBkgSprite_;
    /// Marker map Bkg sprite.
    SharedPtr<Sprite> markerMapBkgSprite_;

    /// Steering wheel
    SharedPtr<Sprite> steerWheelSprite_;
    /// Steering actor
    SharedPtr<Sprite> steerActorSprite_;

    // Speedometer
    SharedPtr<Sprite> speedometerBkgSprite_;
    SharedPtr<Sprite> speedometerNeedleSprite_;
    SharedPtr<Text> speedometerText_;

    // BOTS
    // Sprite map of ffn
    HashMap<int, Sprite*> ffnBotSprite_;
    /// Steering wheel
    SharedPtr<Sprite> steerWheelBotSprite_;
    /// Velocity sprite.
    SharedPtr<Sprite> velBarBotSprite_;
    /// Velocity Bkg sprite.
    SharedPtr<Sprite> velBarBkgBotSprite_;

    // Radio spectrum view sprite
    SharedPtr<Sprite> radioSpectrumSprite_;
    SharedPtr<PixelImage> radioVisualizerImg_;
    SharedPtr<Texture2D> radioSpectrumTexture_;

    /// Particle pool
    ParticlePool particlePool_[20];

    #define VDAMAGE_BODY_NUM_LINES 8

    #define NUM_DEBUG_FIELDS 16
    // Debug text
    Text* debugText_[NUM_DEBUG_FIELDS];

    #define NUM_RANK_FIELDS 10
    // Rank text
    Text* rankText_[NUM_RANK_FIELDS];

    #define NUM_LOGIN_FIELDS 16
    // Login List text
    Text* loginListText_[NUM_LOGIN_FIELDS];

    #define NUM_RADIO_TRACK_FIELDS 24
    // Radio text
    Text* radioText_[NUM_RADIO_TRACK_FIELDS];

    Vector<String> spectrumAnalyzer_;

    #define NUM_STAT_FIELDS 8
    Text* genAlgStatsText_[NUM_STAT_FIELDS];

    float radioSpectrumWidth_ = 164.0f;
    float radioSpectrumHeight_ = 24.0f;

    WeakPtr<Node> raceTrack_;
    WeakPtr<CollisionShape> trackColShape_;

    WeakPtr<Text>  textKmH_;

    // smooth step
    Quaternion     vehicleRot_;
    Vector3        targetCameraPos_;
    float          springVelocity_;

    // dbg
    WeakPtr<Text>  textStatus_;
    Timer          fpsTimer_;
    int            framesCount_;

    float upTime_ = 0; // Server uptime calculation

    // Network objects
    HashMap<Connection*, WeakPtr<Node> > serverObjects_;
    SharedPtr<UIElement> buttonContainer_;
    SharedPtr<Button> playButton_;
    SharedPtr<Button> disconnectButton_;
    SharedPtr<Button> startServerButton_;
    SharedPtr<Button> exitButton_;
    SharedPtr<DropDownList> gameServerDropDownList_;
    int gameServerSelected_ = 0;

    SharedPtr<Text> instructionsText_;
    Vector<SharedPtr<Text>> hudTextList_;

    VariantMap clientMap_;

    HashMap<Connection*, WeakPtr<Text3D>> actorTextMap_;
    HashMap<unsigned int, WeakPtr<Text3D>> aiActorTextMap_;

    // Line segment for spawning players on track
    Vector3 starAMarker_;
    bool starAMarkerSet_;
    Vector3 starBMarker_;
    bool starBMarkerSet_;


    Vector3 tentAMarker_;
    bool buildTentAMarkerSet_;

    Vector2 joySteer_;


    unsigned int networkActorNodeId_;
    unsigned int vehicleNodeId_;
    bool isServer_;
    int lastNumNodes_;
    bool isSnapped_;

    // Vehicle track steering
    int splineSize_ = 0;
    SharedPtr<StaticModel> splineModel_;
    PODVector<Node *> steerMarks_;

    // Pickups
    PODVector<Node *> pickupMarks_;

    // Ranking
    Vector<Pair<int,float>> rankList_;

    bool bkgMusicPlaying_;
    bool bkgMusic_;
    bool sndFx_;

    /// Packets in per second
    SharedPtr<Text> packetsIn_;
    /// Packets out per second
    SharedPtr<Text> packetsOut_;
    /// Packet counter UI update timer
    Timer packetCounterTimer_;

    void ArrangePlayersInFormation();

    void StartRace();

    // === MOVEMENT CONFIGURATION ===
    // Springy movement variables (these should already exist based on the code)
    Vector3 targetMovement_;          // Target movement direction
    Vector3 currentMovement_;         // Current smoothed movement
    Vector3 movementVelocity_;        // Velocity for spring damping
    float walkSpeed_;                 // Base walking speed
    float runSpeed_;                  // Running speed multiplier
    float jumpForce_;                 // Jump force magnitude
    float movementDamping_;           // Movement spring damping
    float movementStiffness_;         // Movement spring stiffness

    bool isRunning_;                  // Is character running
    bool isGrounded_;                 // Is character on ground
    float jumpCooldown_;              // Jump cooldown timer
    float currentJumpCooldown_;       // Current jump cooldown

    float currentYaw_;                // Current character rotation
    float targetYaw_;                 // Target character rotation
    float yawRotationSpeed_;          // How fast character rotates

    // VU Meter UI elements
    SharedPtr<UIElement> radioContainer_;
    SharedPtr<UIElement> vuMeterContainer_;
    Vector<SharedPtr<Sprite>> vuMeterBars_;
    Vector<SharedPtr<Sprite>> vuMeterPeakBars_;
    Vector<float> vuLevels_;
    Vector<float> vuPeakLevels_;
    Vector<float> vuPeakHoldTime_;

    // VU meter configuration
    const int VU_METER_BARS = 20;  // Number of bars per channel
    const int VU_METER_CHANNELS = 2; // Stereo
    const float VU_PEAK_HOLD_TIME = 1.0f; // Peak hold duration in seconds
    const float VU_PEAK_DECAY_RATE = 0.8f; // Peak decay rate

    // Camera mode enumeration
    enum CameraMode {
        CAMERA_FIRST_PERSON = 0,
        CAMERA_THIRD_PERSON = 1,
        CAMERA_ISOMETRIC = 2,
        CAMERA_FREEFLY = 3,
    };

    // Camera system variables
    CameraMode currentCameraMode_ = CAMERA_THIRD_PERSON;
    float isometricCameraHeight_ = 500.0f;
    float isometricCameraAngle_ = 45.0f;
    Vector3 cameraOffset_;
    float cameraDistance_ = 50.0f;
    float cameraHeight_ = 20.0f;
    float cameraLookAhead_ = 10.0f;
    float cameraSmoothSpeed_ = 5.0f;
    Vector3 smoothCameraPosition_;
    Quaternion smoothCameraRotation_;
    bool cameraInitialized_ = false;

    // Minimap system
    SharedPtr<Camera> minimapCam_;
    SharedPtr<Node> minimapCamNode_;
    SharedPtr<Viewport> minimapViewport_;

    // Minimap player markers
    Vector<SharedPtr<Node>> minimapPlayerMarkers_;
    HashMap<Connection*, SharedPtr<Node>> minimapPlayerMap_;
    HashMap<unsigned int, SharedPtr<Node>> minimapBotMap_;

    // Minimap configuration
    float minimapSize_ = 200.0f;  // Size of minimap in pixels
    float minimapScale_ = 0.1f;   // World to minimap scale
    Vector3 trackCenter_ = Vector3::ZERO;  // Center of the track
    float trackBounds_ = 1000.0f;  // Track boundary size
    // Enhanced minimap configuration
    float minimapZoomLevel_ = 150.0f;     // Current zoom level
    float minimapMinZoom_ = 50.0f;        // Minimum zoom (closest)
    float minimapMaxZoom_ = 500.0f;       // Maximum zoom (farthest)
    bool minimapRotateWithPlayer_ = true;  // Should minimap rotate with player
    float minimapBorderPadding_ = 10.0f;   // Padding from screen edge
    Vector3 minimapTargetPosition_;        // Target position for smooth following
    float minimapSmoothSpeed_ = 10.0f;     // Camera smoothing speed
    // UI overlay elements
    SharedPtr<Sprite> minimapBorderSprite_;
    SharedPtr<Sprite> minimapDirectionSprite_;

    // Add thread safety for audio
    mutable Mutex audioMutex_;
    bool audioProcessingEnabled_;

    // Replay System
    SharedPtr<ReplaySystem> replaySystem_;
    SharedPtr<ReplayUI> replayUI_;
    bool replaySystemEnabled_;

    float lastReplayTriggerTime_;
    static constexpr float REPLAY_COOLDOWN_TIME = 3.0f;

    SharedPtr<ProceduralVegetation> vegetationSystem_;
    Node* vegetationNode_;
    bool vegetationEnabled_;
    float vegetationUpdateTimer_;

    /// Beat Sequencer UI
    SharedPtr<BeatSequencerUI> beatSequencerUI_;

    /// Enhanced sequencer integration
    void UpdateBeatSequencerUI(float timeStep);
    void UpdateEnhanced3DSequencer(float timeStep, int currentBeat, bool isPlaying);
    void HandleBeatToggled(StringHash eventType, VariantMap& eventData);
    void HandleSequencerPlayToggle(StringHash eventType, VariantMap& eventData);
    void HandleSequencerStop(StringHash eventType, VariantMap& eventData);
    void HandleSequencerRecordToggle(StringHash eventType, VariantMap& eventData);

    Vector3 CalculatePlayersCenterPosition();

    // Minimap methods
    void SetupMinimapViewport();
    void UpdateMinimap(float timeStep);
    void CreateMinimapPlayerMarker(Connection* connection, const Color& color);
    void CreateMinimapBotMarker(unsigned int botId, const Color& color);
    void UpdateMinimapMarkers();
    void CalculateTrackBounds();
    void DrawMinimapOverlay();

    void SetCameraMode(CameraMode mode);
    void UpdateCameraFirstPerson(float timeStep);
    void UpdateCameraThirdPerson(float timeStep);
    void UpdateCameraIsometric(float timeStep);
    NetworkActor* GetLocalNetworkActor();

    void UpdateAudioSpectrum(float timeStep);
    void UpdateNetworkStatusDisplay(float ping);
    void UpdateVehicleDamageDisplay(Vehicle* v, float timeStep);

    // VU meter methods
    void CreateVUMeter();
    void UpdateVUMeter(float timeStep);
    float CalculateVULevel(const Vector<float>& spectrum, int startFreq, int endFreq);

    Color GetVUBarColor(int barIndex);

    void UpdateVUMeterSmooth(float timeStep);

    float CalculateChannelLevel(int channel, const Vector<float> &spectrum);
    void MonitorMemoryUsage(float timeStep);

    void CleanupAudioResources();
    void ProcessAudioStream();


    };

// Safety filter censored words
String swearWords[] =  { "falcon" };