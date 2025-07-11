/*
     Algebra Kart - algebrakart.com
     MIT LICENSE, Copyright 2024, Bad Zindagi Music

*/
#include <sstream>
#include <string>
#include <iostream>
#include <Urho3D/Audio/Audio.h>
#include <Urho3D/UI/Button.h>
#include <Urho3D/Urho2D/CollisionBox2D.h>
#include <Urho3D/Core/CoreEvents.h>
#include <Urho3D/Graphics/DebugRenderer.h>
#include <Urho3D/Engine/Engine.h>
#include <Urho3D/UI/Font.h>
#include <Urho3D/Input/Input.h>
#include <Urho3D/IO/MemoryBuffer.h>
#include <Urho3D/IO/Log.h>
#include <Urho3D/Graphics/AnimatedModel.h>
#include <Urho3D/Graphics/Animation.h>
#include <Urho3D/Graphics/AnimationState.h>
#include <Urho3D/Graphics/AnimationController.h>
#include <Urho3D/Graphics/BillboardSet.h>
#include <Urho3D/Graphics/Camera.h>
#include <Urho3D/Graphics/Graphics.h>
#include <Urho3D/Graphics/GraphicsEvents.h>
#include <Urho3D/Graphics/Model.h>
#include <Urho3D/Graphics/Light.h>
#include <Urho3D/Graphics/Material.h>
#include <Urho3D/Graphics/Octree.h>
#include <Urho3D/Graphics/Renderer.h>
#include <Urho3D/Graphics/RenderPath.h>
#include <Urho3D/Graphics/Zone.h>
#include <Urho3D/Graphics/Technique.h>
#include <Urho3D/Resource/ResourceCache.h>
#include <Urho3D/Scene/Scene.h>
#include <Urho3D/UI/Font.h>
#include <Urho3D/UI/Text.h>
#include <Urho3D/UI/DropDownList.h>
#include <Urho3D/Audio/SoundSource3D.h>
#include <Urho3D/Audio/Sound.h>
#include <Urho3D/Audio/SoundListener.h>
#include <Urho3D/Core/CoreEvents.h>
#include <Urho3D/Core/ProcessUtils.h>
#include <Urho3D/Engine/Engine.h>
#include <Urho3D/Graphics/StaticModel.h>
#include <Urho3D/Graphics/Terrain.h>
#include <Urho3D/Graphics/TerrainPatch.h>
#include <Urho3D/Graphics/Zone.h>
#include <Urho3D/IO/FileSystem.h>
#include <Urho3D/Input/Input.h>
#include <Urho3D/Physics/CollisionShape.h>
#include <Urho3D/Physics/Constraint.h>
#include <Urho3D/Physics/PhysicsWorld.h>
#include <Urho3D/Physics/PhysicsUtils.h>
#include <Urho3D/Input/Controls.h>
#include <Urho3D/Input/Input.h>
#include <Urho3D/IO/Log.h>
#include <Urho3D/Network/Network.h>
#include <Urho3D/Network/Connection.h>
#include <Urho3D/Network/NetworkEvents.h>
#include <Urho3D/Physics/PhysicsEvents.h>
#include <Urho3D/Physics/PhysicsWorld.h>
#include <Urho3D/Physics/RigidBody.h>
#include <Urho3D/UI/UI.h>
#include <Urho3D/UI/UIEvents.h>


#include <Urho3D/IO/IOEvents.h>
#include <Urho3D/IO/VectorBuffer.h>

#include <Urho3D/Urho2D/AnimatedSprite2D.h>
#include <Urho3D/Urho2D/AnimationSet2D.h>
#include <Urho3D/UI/BorderImage.h>
#include <Urho3D/Core/Context.h>
#include <Urho3D/IO/File.h>
#include <Urho3D/Urho2D/ParticleEffect2D.h>
#include <Urho3D/Urho2D/ParticleEmitter2D.h>
#include <Urho3D/Urho2D/RigidBody2D.h>
#include <Urho3D/Core/StringUtils.h>
#include <Urho3D/Graphics/Texture2D.h>
#include <Urho3D/Urho2D/TileMap2D.h>
//#include <Urho3D/Urho2D/TileMap3D.h>
#include <Urho3D/Urho2D/TmxFile2D.h>
#include <Urho3D/Scene/ValueAnimation.h>

#include "network/Server.h"
#include "network/ClientObj.h"
#include "network/NetworkActor.h"


#include <Urho3D/Physics/RaycastVehicle.h>
#include <Urho3D/Physics/RigidBody.h>
#include <Urho3D/Resource/ResourceCache.h>
#include <Urho3D/Scene/Scene.h>
#include <Urho3D/UI/Font.h>
#include <Urho3D/UI/Text.h>

#include <Urho3D/Urho2D/PhysicsEvents2D.h>
#include <Urho3D/Urho2D/PhysicsWorld2D.h>
#include <Urho3D/Graphics/Renderer.h>
#include <Urho3D/Resource/ResourceCache.h>
#include <Urho3D/Urho2D/RigidBody2D.h>
#include <Urho3D/Scene/Scene.h>
#include <Urho3D/Scene/SceneEvents.h>
#include <Urho3D/Core/StringUtils.h>
#include <Urho3D/Urho2D/TileMap2D.h>
//#include <Urho3D/Urho2D/TileMapLayer2D.h>
//#include <Urho3D/Urho2D/TileMap3D.h>
//#include <Urho3D/Urho2D/TileMapLayer3D.h>
#include <Urho3D/Urho2D/TmxFile2D.h>
#include <Urho3D/UI/UI.h>
#include <Urho3D/UI/UIEvents.h>

#include <Urho3D/Urho2D/ParticleEffect2D.h>
#include <Urho3D/Urho2D/ParticleEmitter2D.h>


#include <Urho3D/DebugNew.h>

#include "GameController.h"
#include "AlgebraKart.h"
#include <AlgebraKart/ai/evolution_manager.h>

//#include "RaycastVehicle.h"
#include "AlgebraKart/network/Vehicle.h"
#include "WheelTrackModel.h"
#include "SmoothStep.h"

#include <vector>
#include <cstdint>
#include <cmath>
#include <cassert>
#include <sstream>
#include <limits>

// Audio Spectrum Analyzer
#include <AlgebraKart/beat/AudioSpectrumAnalyzer.h>
#include <AlgebraKart/beat/AudioSpectrumAxis.h>
#include <AlgebraKart/beat/AudioSpectrumRender.h>
#include <AlgebraKart/beat/AudioSpectrumCapture.h>

// AgentSim shared libs
#include "shared_libs.h"
#include "types.h"

#include "../../Urho3D/Input/Controls.h"
#include "../../Urho3D/Scene/SplinePath.h"
#include "../../Urho3D/Resource/ResourceEvents.h"
#include "AlgebraKart/beat/BufferData.h"
#include <AlgebraKart/PixelImage.h>
#include <AlgebraKart/Constants.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include <algorithm>
#include <iostream>
#include <list>
#include <string>
#include <utility>

#include "ViewCone.h"
#include "../../Urho3D/UI/ProgressBar.h"

using namespace std;


//#define CSP_DEBUG

// Identifier for the chat network messages
const int MSG_CHAT = 153;
const int MSG_NODE_ERROR = 156;


//#define INGAME_FONT "Fonts/m6x11.ttf"
//#define INGAME_FONT "Fonts/RulerGold.ttf"
//#define INGAME_FONT2 "Fonts/SinsGold.ttf"
//#define INGAME_FONT3 "Fonts/RulerGold.ttf"
//#define INGAME_FONT4 "Fonts/HopeGold.ttf"

#define INGAME_FONT "Fonts/Poppins-Black.ttf"
#define INGAME_FONT2 "Fonts/Poppins-Black.ttf"
#define INGAME_FONT3 "Fonts/Poppins-Regular.ttf"
#define INGAME_FONT4 "Fonts/Poppins-Black.ttf"


#define SHOW_STATS 1

std::vector<std::string> gameServers = {
        "localhost", // local game server
        "www.algebrakart.com", // remote game server (default)
        "10.0.2.2", // android server address
};

std::vector<std::string> bzRadioTracksArtistName = {
        "Bhagatriks",
        "Bhagatriks",
        "Bhagatriks",
        "Bhagatriks",
        "Bhagatriks",
        "Bhagatriks",
        "Bhagatriks",
        "MonkeyMaya"
};


std::vector<std::string> bzRadioTracksArtistLink = {
        "IG: @bhagatriks",
        "IG: @bhagatriks",
        "IG: @bhagatriks",
        "IG: @bhagatriks",
        "IG: @bhagatriks",
        "IG: @bhagatriks",
        "IG: @bhagatriks",
        "Free cruising!"
};


std::vector<std::string> bzRadioTracksTrackName = {
        "BZradio/110-walk-away.ogg",
        "BZradio/100-miami-ride.ogg",
        "BZradio/105-hotsun-lp.ogg",
        "BZradio/110-happy-saturday.ogg",
        "BZradio/120-flovibe.ogg",
        "BZradio/130-breakabeat.ogg",
        "BZradio/150-trappp.ogg",
        "BZradio/mm-theme-rp01.ogg"
};



int currTrack_ = 0;


/// [AB] STABILITY NOTES //

// -> Added safety in engine code to detect bad population and pause evolving on bad population
// Migrate evolution code from Server base engine code
/* Genetic Algorithm throws asserts for too small population size
 * Move spawner to agent who joins and spawns
 *
 */

//
/// \return


// =============================================================================
// Cross-Platform Memory Monitoring and Limits
// =============================================================================

// Cross-platform memory usage function
size_t GetProcessMemoryUsage() {
#ifdef _WIN32
    PROCESS_MEMORY_COUNTERS pmc;
    if (GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc))) {
        return pmc.WorkingSetSize;
    }
    return 0;
#elif defined(__linux__)
    // Read from /proc/self/status for detailed memory info
    FILE* file = fopen("/proc/self/status", "r");
    if (!file) return 0;

    char line[256];
    size_t vmRSS = 0;

    while (fgets(line, sizeof(line), file)) {
        if (strncmp(line, "VmRSS:", 6) == 0) {
            // Extract RSS in kB and convert to bytes
            sscanf(line, "VmRSS: %zu kB", &vmRSS);
            vmRSS *= 1024; // Convert kB to bytes
            break;
        }
    }

    fclose(file);
    return vmRSS;
#elif defined(__APPLE__)
    // macOS implementation using task_info
    struct mach_task_basic_info info;
    mach_msg_type_number_t infoCount = MACH_TASK_BASIC_INFO_COUNT;

    if (task_info(mach_task_self(), MACH_TASK_BASIC_INFO,
                  (task_info_t)&info, &infoCount) != KERN_SUCCESS) {
        return 0;
    }

    return info.resident_size;
#else
    // Fallback - return 0 if platform not supported
    return 0;
#endif
}

// Get virtual memory usage (useful for detecting memory leaks)
size_t GetProcessVirtualMemoryUsage() {
#ifdef _WIN32
    PROCESS_MEMORY_COUNTERS pmc;
    if (GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc))) {
        return pmc.PagefileUsage;
    }
    return 0;
#elif defined(__linux__)
    FILE* file = fopen("/proc/self/status", "r");
    if (!file) return 0;

    char line[256];
    size_t vmSize = 0;

    while (fgets(line, sizeof(line), file)) {
        if (strncmp(line, "VmSize:", 7) == 0) {
            sscanf(line, "VmSize: %zu kB", &vmSize);
            vmSize *= 1024; // Convert kB to bytes
            break;
        }
    }

    fclose(file);
    return vmSize;
#elif defined(__APPLE__)
    struct mach_task_basic_info info;
    mach_msg_type_number_t infoCount = MACH_TASK_BASIC_INFO_COUNT;

    if (task_info(mach_task_self(), MACH_TASK_BASIC_INFO,
                  (task_info_t)&info, &infoCount) != KERN_SUCCESS) {
        return 0;
    }

    return info.virtual_size;
#else
    return 0;
#endif
}

// Cross-platform memory monitoring
void AlgebraKart::MonitorMemoryUsage(float timeStep) {
    static float memoryCheckTimer = 0.0f;
    memoryCheckTimer += timeStep;

    if (memoryCheckTimer >= 2.0f) { // Check every 2 seconds during crisis
        memoryCheckTimer = 0.0f;

        size_t physicalMemory = GetProcessMemoryUsage();
        size_t virtualMemory = GetProcessVirtualMemoryUsage();

        // Log every check during high memory usage
        if (physicalMemory > 1024 * 1024 * 1024) { // Over 1GB
            MemoryTracker::LogMemoryState("MONITORING");
            MemoryTracker::LogSceneStats(scene_);
        }

        // EMERGENCY TRIGGERS
        if (physicalMemory > 2048 * 1024 * 1024) { // Over 2GB - CRITICAL
            URHO3D_LOGERROR("CRITICAL MEMORY USAGE - TRIGGERING EMERGENCY CLEANUP!");

            // Trigger all emergency cleanups
            //EmergencySceneCleanup();
            //EmergencyAudioCleanup();
            //EvolutionManager::EmergencyCleanup();
            ForceMemoryCleanup();

            // If still over 2.5GB, restart might be needed
            size_t postCleanupMemory = GetProcessMemoryUsage();
            if (postCleanupMemory > 2560 * 1024 * 1024) { // 2.5GB
                URHO3D_LOGERROR("MEMORY CLEANUP FAILED - SERVER RESTART RECOMMENDED!");

                // Log final diagnostics
                MemoryTracker::LogMemoryState("POST_EMERGENCY_CLEANUP");
                MemoryTracker::LogSceneStats(scene_);

                // Could trigger graceful shutdown here if needed
                // engine_->Exit();
            }
        }
    }
}

// Force cleanup when memory is critical
void AlgebraKart::ForceMemoryCleanup() {
    URHO3D_LOGINFO("Performing forced memory cleanup...");

    // Clean up audio
    CleanupAudioResources();

    // Clean up particle pool aggressively (fixed array)
    for (int i = 0; i < 20; ++i) { // Match your array size
        if (particlePool_[i].node) {
            particlePool_[i].node->Remove();
            particlePool_[i].node.Reset();
        }
        if (particlePool_[i].emitter) {
            particlePool_[i].emitter.Reset();
        }
        particlePool_[i].active = false;
        particlePool_[i].timeToLive = 0.0f;
    }

    // Clean up old UI elements
    auto* ui_ = GetSubsystem<UI>();
        if (ui_ && ui_->GetRoot()) {
        auto children = ui_->GetRoot()->GetChildren();
        for (auto child : children) {
            if (child->GetName().StartsWith("temp_") ||
                child->GetName().StartsWith("effect_")) {
                child->Remove();
            }
        }
    }

    // Force garbage collection on scene
    if (scene_) {
        // Remove temporary nodes
        PODVector<Node*> tempNodes;
        scene_->GetChildrenWithTag(tempNodes, "temporary", true);
        for (Node* node : tempNodes) {
            node->Remove();
        }
    }

    // Clear caches
    auto* cache = GetSubsystem<ResourceCache>();
    if (cache) {
        cache->ReleaseAllResources(false);
    }

    URHO3D_LOGINFO("Forced cleanup completed");
}

// =============================================================================
// 8. FIX: Proper Resource Cache Management
// =============================================================================

// In AlgebraKart.cpp - Complete resource cache management
void AlgebraKart::ManageResourceCache(float timeStep) {
    static float cacheCleanupTimer = 0.0f;
    static float aggressiveCleanupTimer = 0.0f;
    static const float CACHE_CLEANUP_INTERVAL = 60.0f; // Clean every minute
    static const float AGGRESSIVE_CLEANUP_INTERVAL = 300.0f; // Aggressive clean every 5 minutes

    cacheCleanupTimer += timeStep;
    aggressiveCleanupTimer += timeStep;

    // Regular cache cleanup
    if (cacheCleanupTimer >= CACHE_CLEANUP_INTERVAL) {
        cacheCleanupTimer = 0.0f;

        auto* cache = GetSubsystem<ResourceCache>();
        if (cache) {
            // Get memory usage before cleanup
            size_t memoryBefore = GetProcessMemoryUsage();

            // Release unused resources (don't force release of actively used resources)
            cache->ReleaseAllResources(false);

            // Log cleanup results
            size_t memoryAfter = GetProcessMemoryUsage();
            if (memoryBefore > 0 && memoryAfter > 0) {
                int memoryFreed = (int)((memoryBefore - memoryAfter) / (1024 * 1024));
                if (memoryFreed > 0) {
                    URHO3D_LOGINFOF("Resource cache cleaned - freed %d MB", memoryFreed);
                } else {
                    URHO3D_LOGINFO("Resource cache cleaned - no memory freed");
                }
            } else {
                URHO3D_LOGINFO("Resource cache cleaned");
            }
        }
    }

    // Aggressive cleanup when memory is high
    if (aggressiveCleanupTimer >= AGGRESSIVE_CLEANUP_INTERVAL) {
        aggressiveCleanupTimer = 0.0f;

        size_t currentMemory = GetProcessMemoryUsage();
        if (currentMemory > MEMORY_WARNING_THRESHOLD) {
            URHO3D_LOGINFO("Performing aggressive resource cache cleanup due to high memory usage");

            auto* cache = GetSubsystem<ResourceCache>();
            if (cache) {
                // More aggressive cleanup - force release even actively used resources
                cache->ReleaseAllResources(true);

                // Also clear any cached UI resources
                auto* ui = GetSubsystem<UI>();
                if (ui) {
                    // Clear cached UI elements that aren't currently displayed
                    auto root = ui->GetRoot();
                    if (root) {
                        // Remove hidden UI elements to free memory
                        PODVector<UIElement*> hiddenElements;
                        root->GetChildren(hiddenElements, true);

                        for (UIElement* element : hiddenElements) {
                            if (!element->IsVisible() &&
                                element->GetName().StartsWith("temp_")) {
                                element->Remove();
                            }
                        }
                    }
                }
            }
        }
    }
}

// Add helper function for cache statistics
void AlgebraKart::LogResourceCacheStats() {
    auto* cache = GetSubsystem<ResourceCache>();
    if (!cache) return;

    // Get cache statistics (if available in your Urho3D version)
    unsigned totalResources = 0;
    unsigned long long totalMemory = 0;

    // Count resources by type
    const auto& resources = cache->GetAllResources();
    for (const auto& pair : resources) {
        totalResources++;
        // Note: Exact memory usage per resource might not be available
        // This is a rough estimate
    }

    URHO3D_LOGINFOF("Resource Cache Stats - Total resources: %u", totalResources);
}

// =============================================================================
// 9. FIX: Scene Node Cleanup Implementation
// =============================================================================

// In AlgebraKart.cpp - Complete scene cleanup implementation
void AlgebraKart::CleanupExcessNodes() {
    if (!scene_) return;

    URHO3D_LOGINFO("Starting excess node cleanup...");

    unsigned removedNodes = 0;
    PODVector<Node*> nodesToRemove;

    // Get all child nodes
    PODVector<Node*> allNodes;
    scene_->GetChildrenWithTag(allNodes, "", true); // Get all nodes

    for (Node* node : allNodes) {
        if (!node) continue;

        bool shouldRemove = false;
        String nodeName = node->GetName();

        // Remove temporary nodes
        if (nodeName.StartsWith("temp_") ||
            nodeName.StartsWith("effect_") ||
            nodeName.StartsWith("particle_")) {
            shouldRemove = true;
        }

        // Remove nodes without any components
        if (node->GetNumComponents() == 0 && node->GetNumChildren() == 0) {
            shouldRemove = true;
        }

        // Remove orphaned vehicle nodes
        if (node->HasComponent<Vehicle>()) {
            Vehicle* vehicle = node->GetComponent<Vehicle>();
            if (!vehicle || !vehicle->GetNode()) {
                shouldRemove = true;
            }
        }

        // Remove orphaned NetworkActor nodes
        if (node->HasComponent<NetworkActor>()) {
            NetworkActor* actor = node->GetComponent<NetworkActor>();
            if (!actor || !actor->IsAlive() || !actor->GetConnection()) {
                shouldRemove = true;
            }
        }

        // Remove old particle emitter nodes that are no longer emitting
        if (node->HasComponent<ParticleEmitter>()) {
            ParticleEmitter* emitter = node->GetComponent<ParticleEmitter>();
            if (!emitter || !emitter->IsEmitting()) {
                // Check if particle effect has been running for too long
                static const float MAX_PARTICLE_AGE = 30.0f; // 30 seconds
                if (node->GetScene()) {
                    // Clean up dead/expired particles in the fixed array
                    for (int i = 0; i < PARTICLE_POOL_SIZE; ++i) {
                        ParticlePool &particle = particlePool_[i];
                        float particleAge = scene_->GetElapsedTime() - particle.creationTime;
                        if (particleAge > particle.timeToLive) {
                            shouldRemove = true;
                        }
                    }
                }
            }
        }

        // Remove nodes that are too far from any players (optimization)
        if (nodeName.Contains("debris") || nodeName.Contains("pickup")) {
            bool nearPlayer = false;
            Vector3 nodePos = node->GetWorldPosition();

            // Check distance to any network actors
            PODVector<Node*> actorNodes;
            scene_->GetChildrenWithComponent<NetworkActor>(actorNodes, true);

            for (Node* actorNode : actorNodes) {
                if (actorNode && actorNode->GetComponent<NetworkActor>()) {
                    float distance = (nodePos - actorNode->GetWorldPosition()).Length();
                    if (distance < 500.0f) { // Within 500 units
                        nearPlayer = true;
                        break;
                    }
                }
            }

            if (!nearPlayer) {
                shouldRemove = true;
            }
        }

        if (shouldRemove) {
            nodesToRemove.Push(node);
        }
    }

    // Remove the marked nodes
    for (Node* node : nodesToRemove) {
        if (node && node->GetScene()) {
            node->Remove();
            removedNodes++;
        }
    }

    if (removedNodes > 0) {
        URHO3D_LOGINFOF("Cleaned up %u excess nodes", removedNodes);
    } else {
        URHO3D_LOGINFO("No excess nodes found to clean up");
    }
}

URHO3D_DEFINE_APPLICATION_MAIN(AlgebraKart)

AlgebraKart::AlgebraKart(Context *context) :
        Game(context),
        networkActorNodeId_(0), vehicleNodeId_(0), lastNumNodes_(0), camSpin_(0), camSpin2_(0), camZoom_(20.0f),
        flyCam_(Vector3(0,0,0)),
        flyTargetCam_(Vector3(1.0,0,0)),
        snapCam_(Vector3(0,0,0)),
        lastPlayerStateTime_(0),
        isServer_(false),
        isSnapped_(false),
        drawDebug_(true),
        bkgMusic_(true), // Disabling this will disable radio
        bkgMusicPlaying_(false),
        sndFx_(true),
        starAMarker_(Vector3(0,0,0)),
        starBMarker_(Vector3(0,0,0)),
        tentAMarker_(Vector3(0,0,0)),
        starAMarkerSet_(false),
        starBMarkerSet_(false),
        buildTentAMarkerSet_(false),
        levelLoading_(false),
        playDrumMachine_(false),
        capturer_(nullptr),
        renderer_(nullptr),
        audioProcessingEnabled_(false),
        currentYaw_(0.0f),
        targetYaw_(0.0f),
        yawRotationSpeed_(120.0f),      // degrees per second
        minimapZoomLevel_(200.0f),
        minimapMinZoom_(50.0f),
        minimapMaxZoom_(500.0f),
        minimapSmoothSpeed_(5.0f),
        minimapSize_(150.0f),
        minimapBorderPadding_(10.0f),
        minimapRotateWithPlayer_(false),
        minimapTargetPosition_(Vector3::ZERO),
        replaySystemEnabled_(true),
        lastReplayTriggerTime_(0.0f),
        currentSteeringValue_(0.0f),
        hasValidSteeringData_(false)
{


    // Custom load into radio
    std::string artistName = "Bhagatriks";
    std::string artistLink = "IG: @bhagatriks";
    std::string trackFile = "BZradio/110-walk-away.ogg";

    bzRadioTracksArtistName.push_back(artistName);
    bzRadioTracksArtistLink.push_back(artistLink);
    bzRadioTracksTrackName.push_back(trackFile);

    // Add this to AlgebraKart constructor in AlgebraKart.cpp:
    walkSpeed_ = 5.0f;
    runSpeed_ = 2.0f;                    // Multiplier for running
    jumpForce_ = 8.0f;
    isRunning_ = false;
    isGrounded_ = true;
    jumpCooldown_ = 0.5f;
    currentJumpCooldown_ = 0.0f;
    targetMovement_ = Vector3::ZERO;
    currentMovement_ = Vector3::ZERO;

    // Initialize camera obstruction system
    cameraObstructed_ = false;
    targetCameraDistance_ = 50.0f;
    currentCameraDistance_ = 50.0f;
    cameraObstructionSmoothSpeed_ = 8.0f;
    maxCameraDistance_ = 200.0f;
    minCameraDistance_ = 10.0f;
    obstructionCheckTimer_ = 0.0f;
    obstructionCheckInterval_ = 0.1f; // Check every 100ms
    hasValidCameraPosition_ = false;

    // Set camera mode
    SetCameraMode(CAMERA_THIRD_PERSON);
}

AlgebraKart::~AlgebraKart() {
    // FIX: Unsubscribe from all events to prevent handler leaks
    UnsubscribeFromAllEvents();

    // Clean up all subsystems
    CleanupAudioResources();

    UI *ui = GetSubsystem<UI>();

    // Clean up UI
    if (ui && ui->GetRoot()) {
        ui->GetRoot()->RemoveAllChildren();
    }

    // Clean up evolution manager
    EvolutionManager::clean();

    // Clean up particle pool (fixed array)
    for (int i = 0; i < 20; ++i) { // Match your array size
        if (particlePool_[i].node) {
            particlePool_[i].node->Remove();
            particlePool_[i].node.Reset();
        }
        if (particlePool_[i].emitter) {
            particlePool_[i].emitter.Reset();
        }
        particlePool_[i].active = false;
        particlePool_[i].timeToLive = 0.0f;
    }

    URHO3D_LOGINFO("AlgebraKart destructor completed");
}

void AlgebraKart::Setup() {
    Game::Setup();
}

// SETUP AI BOTS
void AlgebraKart::InitEvolutionManager() {

    std::cout << "Evolution Manager -> starting..." << std::endl;
    EvolutionManager::getInstance()->startEvolution();
    EvolutionManager *evolutionManager = EvolutionManager::getInstance();

    // Create shape for each agent
    std::vector<std::shared_ptr<Agent>> agents = evolutionManager->getAgents();
    std::cout << "Evolution Manager -> number of agents = " << agents.size() << std::endl;

    for (int i = 0; i < agents.size(); i++) {
        // Randomly place agents
        //       agents[i]->setPosition(cyclone::Vector3(rd(), rd(), rd()));
    }

    /*
    // Test Case 1
    auto genotype = new Genotype();
    std::cout << "Generating random genotype..." << std::endl;
    genotype = genotype->generateRandom(10, 0.4, 20.4);
    genotype->outputToConsole();
    genotype->saveToFile("genotype01.data");
    std::cout << "Genotype saved to disk." << std::endl;

    std::cout << "Loading genotype from disk..." << std::endl;
    genotype = genotype->loadFromFile("genotype01.data");
    std::cout << "Loaded genotype from disk." << std::endl;
    */

}


void AlgebraKart::ShowEvolutionManagerStats() {
    std::vector<std::shared_ptr<Agent>> agents = EvolutionManager::getInstance()->getAgents();
    std::vector<std::shared_ptr<AgentController>> controllers = EvolutionManager::getInstance()->getAgentControllers();

    char buffer[255];
    int aliveCount = EvolutionManager::getInstance()->agentsAliveCount;
    const int maxRows = 20;
    char *strText[maxRows];

    for (int i = 0; i < maxRows; i++) {
        strText[i] = new char[80];
    }

    for (int i = 0; i < maxRows; i++) {

        switch (i) {

            case maxRows - 1:
                sprintf(strText[i], "%d alive out of %d population.", aliveCount, agents.size());
                break;
            case maxRows - 2:
                sprintf(strText[i], "%d generation out of %d generations.",
                        EvolutionManager::getGeneticAlgorithm()->generationCount_, RestartAfter);
                break;
            case maxRows - 3:
                sprintf(strText[i], "======actorNode = scene_->GetChild(server->GetClientHashA().ToHash());====================================================================");
                break;
            case maxRows - 4:
                sprintf(strText[i], "agent[0].timeSinceLastCheckpoint: %f",
                        controllers[0]->getTimeSinceLastCheckpoint());
                break;
            default:
                sprintf(strText[i], "");
                break;

        }

        //       if (strText[i])
//            renderText(5, 5 + (10 * i), strText[i], NULL);

    }

    for (int i = 0; i < maxRows; i++) {
        delete strText[i];
    }
}

void AlgebraKart::Start() {

    for (int i = 0; i < sizeof(particlePool_) / sizeof(*particlePool_); i++) {
        particlePool_[i].used = false;
        particlePool_[i].usedBy = -1;
    }

    // Don't start until past menu
    started_ = false;

    // Execute base class startup
    Game::Start();

    CreateServerSubsystem();
    // Register subsystems/objects
    context_->RegisterSubsystem(new GameController(context_));

    // Reset focus index
    focusIndex_ = 0;

    // Random seed
    SetRandomSeed(Time::GetSystemTime());


    if (headless_) {
        URHO3D_LOGINFO("Starting AlgebraKart in headless server mode");

        // Only initialize essential subsystems for server
        CreateEmptyScene(context_);

        // Initialize AI/Evolution Manager for bots
        InitEvolutionManager();

        // Start server immediately in headless mode
        DoStartServer();

        // Subscribe only to essential events
        SubscribeToEvents();

    } else {
        URHO3D_LOGINFO("Starting AlgebraKart in client/windowed mode");

        // Full initialization for client
        CreateLoadingScreen();
        CreateEmptyScene(context_);
        UpdateLoadingProgress(0.1f, "Creating UI...");
        CreateUI();
        UpdateLoadingProgress(0.15f, "Loading sound system...");

        // Enable 3D audio (attach to camera node when available)
        // Enable for 3D sounds to work (attach to camera node)
        SoundListener *listener = scene_->CreateComponent<SoundListener>(LOCAL);
        GetSubsystem<Audio>()->SetListener(listener);

        // you can set master volumes for the different kinds if sounds, 30% for music
        GetSubsystem<Audio>()->SetMasterGain(SOUND_MUSIC, 0.3);
        GetSubsystem<Audio>()->SetMasterGain(SOUND_EFFECT, 0.3);

        // Initialize game systems
        UpdateLoadingProgress(0.20f, "Loading evolution manager...");
        InitEvolutionManager();
        CreateServerSubsystem();

        UpdateLoadingProgress(0.25f, "Preparing event system...");
        // Subscribe to all events
        SubscribeToEvents();

        // Initialize replay system after scene creation
        replaySystem_ = new ReplaySystem(context_);
        replaySystem_->Initialize(scene_, clientCam_);

        // Configure replay settings
        replaySystem_->SetAutoReplayEnabled(true);
        replaySystem_->SetRecordingDuration(6.0f);  // Record 6 seconds before event
        replaySystem_->SetPlaybackSpeed(0.4f);      // 40% speed for dramatic effect

        // Initialize replay UI
        replayUI_ = new ReplayUI(context_);
        replayUI_->Initialize();

        // Subscribe to collision events for immediate replay triggers
        SubscribeToEvent(E_NODECOLLISION, URHO3D_HANDLER(AlgebraKart, HandleCollisionReplay));

        URHO3D_LOGINFO("Replay system initialized");
        UpdateLoadingProgress(0.30f, "Connecting to server...");

        // Set initial state
        UpdateUIState(false);
        ChangeDebugHudText();
        Game::InitMouseMode(MM_FREE);
        fpsTimer_.Reset();
        framesCount_ = 0;

    }
}


void AlgebraKart::Stop() {

    // Free evolution manager
    EvolutionManager::clean();

    // Dump engine resources
    Game::Stop();

    // Clean up all client connections first
    for (auto& pair : actorMap_) {
        Connection* conn = pair.first_;
        // Trigger proper cleanup
        VariantMap eventData;
        eventData[ClientDisconnected::P_CONNECTION] = conn;
        HandleClientDisconnected(StringHash(), eventData);
    }


    actorMap_.Clear();
    loginList_.Clear();
}

void AlgebraKart::CreateAgents() {
    // Create initial set of agents

    for (int i = 0; i < EvolutionManager::getInstance()->getAgents().size(); i++) {
        if (!EvolutionManager::getInstance()->getNetworkActors().empty())
            EvolutionManager::getInstance()->getNetworkActors()[0]->Kill();
    }

    ResourceCache *cache = GetSubsystem<ResourceCache>();
    for (int i = 0; i < EvolutionManager::getInstance()->getAgents().size(); i++) {

        URHO3D_LOGDEBUGF("CreateAgents -> SpawnPlayer: %d", (i));
        // Spawn ai bot and generate NetworkActor
        EvolutionManager::getInstance()->getNetworkActors().push_back(SpawnPlayer(i));
    }
}

void AlgebraKart::CreateEmptyScene(Context* context) {
    scene_ = MakeShared<Scene>(context_);
    scene_->SetName("MainScene");
    scene_->SetUpdateEnabled(false);


    /*
    scene_->CreateComponent<Octree>();
    float minUnivSpace = -25000.0f;
    float maxUnivSpace = 25000.0f;
    scene_->GetComponent<Octree>()->SetSize(
            BoundingBox(minUnivSpace, maxUnivSpace), 3);
*/
    //DebugRenderer *dbgRenderer = scene_->CreateComponent<DebugRenderer>();
    // Server will authoratively managed physics locally
    PhysicsWorld *physicsWorld = scene_->CreateComponent<PhysicsWorld>(LOCAL);
    physicsWorld->SetFps(60);
    //physicsWorld->DrawDebugGeometry(true);
    //physicsWorld->SetDebugRenderer(dbgRenderer);
    // Disable interpolation (need determinism)
    physicsWorld->SetInterpolation(false);

    // server requires client hash and scene info
    Server *server = GetSubsystem<Server>();
    server->RegisterScene(scene_);

}

void AlgebraKart::UpdateUIState(bool state) {

    // Do the opposite for menu
    versionText_->SetVisible(!state);
    gameNameText_->SetVisible(!state);
    //studioText_->SetVisible(!state);
   // instructionsText_->SetVisible(!state);
  //  hudText_->SetVisible(!state);

  if (showMenu_) {
      buttonContainer_->SetVisible(!state);
  }

    bkgSprite_->SetVisible(!state);

    // On client
    if (!isServer_) {

        if (powerBarP1Sprite_) {
            // Create sprite and add to the UI layout
            powerBarP1Sprite_->SetVisible(state);
            powerBarBkgP1Sprite_->SetVisible(state);
            rpmBarP1Sprite_->SetVisible(state);
            rpmBarBkgP1Sprite_->SetVisible(state);
            velBarP1Sprite_->SetVisible(state);
            velBarBkgP1Sprite_->SetVisible(state);
            //miniMapP1Sprite_->SetVisible(state);
            //miniMapWPSprite_->SetVisible(state);
            //miniMapBkgSprite_->SetVisible(state);
            //markerMapBkgSprite_->SetVisible(state);
            steerWheelSprite_->SetVisible(state);
            steerActorSprite_->SetVisible(state);
            vDamageSprite_->SetVisible(state);



            // Create the UI for displaying the remaining lifes
//    lifeUI->SetVisible(false);
//    auto *lifeText = lifeUI->CreateChild<Text>("LifeText2");

            powerBarText_->SetVisible(state);
            rpmBarText_->SetVisible(state);
            velBarText_->SetVisible(state);


            // Debug text
            for (int i = 0; i < NUM_DEBUG_FIELDS; i++) {
                debugText_[i]->SetVisible(!state);
            }

            // Rank text
            for (int i = 0; i < NUM_RANK_FIELDS; i++) {
                rankText_[i]->SetVisible(state);
            }

            // Radio text
            for (int i = 0; i < NUM_RADIO_TRACK_FIELDS; i++) {
                radioText_[i]->SetVisible(state);
            }


            networkInfo_->SetVisible(state);
        }

    } else {
        // On server

        // Debug text
        for (int i = 0; i < NUM_DEBUG_FIELDS; i++) {
            if (debugText_[i])
               debugText_[i]->SetVisible(!state);
        }

        if (networkInfo_)
            networkInfo_->SetVisible(!state);
    }
}

Node* AlgebraKart::GenerateObject(Vector3 &pos, Quaternion &rot, int type)
{
    auto *cache = GetSubsystem<ResourceCache>();
    Node* newObjNode_ = scene_->CreateChild("ServerGeneratedObject", REPLICATED);
    // model
    StaticModel* ballModel = newObjNode_->GetOrCreateComponent<StaticModel>();

    String modelStr = "";

    switch (type) {
        case 0: {
            modelStr = "Models/Sphere.mdl";
            break;
        }
        case 1: {
            modelStr = "Models/Sphere.mdl";
            break;
        }
    }

    newObjNode_->SetPosition(pos+Vector3(0,10,0));
    newObjNode_->SetRotation(rot);
    newObjNode_->SetScale(30.0f);
    ballModel->SetModel(cache->GetResource<Model>(modelStr));
    ballModel->SetCastShadows(true);


    scene_->MarkReplicationDirty(newObjNode_);

    return newObjNode_;
}

void AlgebraKart::PlaceObject(Camera* pCamera, int type)
{
    Vector3 hitPos;
    Drawable* hitDrawable;

    //if (Raycast(pCamera, 250.0f, hitPos, hitDrawable))

    // Not hitting anything
    if (Raycast(pCamera, 2500000.0f, hitPos, hitDrawable))
    {
        Node* targetNode = hitDrawable->GetNode();
        Quaternion q = targetNode->GetRotation();
        Node* node = GenerateObject(hitPos, q, 0);
        URHO3D_LOGDEBUGF("AlgebraKart::PlaceObject()->GenerateObject() @ [%f, %f, %f]", hitPos.x_, hitPos.y_, hitPos.z_);
    }
}

bool AlgebraKart::Raycast(Camera* pCamera, float maxDistance, Vector3& hitPos, Drawable*& hitDrawable)
{
    hitDrawable = nullptr;

    auto* ui = GetSubsystem<UI>();
    IntVector2 pos = ui->GetCursorPosition();

    auto* graphics = GetSubsystem<Graphics>();
    auto* camera = pCamera;
    // Adjust with delta for mouse

    Input *input = GetSubsystem<Input>();

    IntVector2 mouseMove = input->GetMousePosition();

    Ray cameraRay = camera->GetScreenRay((float)(mouseMove.x_) / graphics->GetWidth(), (float)(mouseMove.y_) / graphics->GetHeight());
    // Pick only geometry objects, not eg. zones or lights, only get the first (closest) hit
    PODVector<RayQueryResult> results;
    RayOctreeQuery query(results, cameraRay, RAY_TRIANGLE, maxDistance, DRAWABLE_GEOMETRY);


    scene_->GetComponent<Octree>()->RaycastSingle(query);

    if (results.Size())
    {
        RayQueryResult& result = results[0];
        hitPos = result.position_;
        hitDrawable = result.drawable_;
        return true;
    }

    return false;
}

void AlgebraKart::HandleResourceBackgroundLoaded(StringHash eventType, VariantMap& eventData) {

    using namespace ResourceBackgroundLoaded;

    String progressStr = String("Loading: ") + String(Round(scene_->GetAsyncProgress() * 100.0f));
    UpdateLoadingProgress(scene_->GetAsyncProgress(), "Connected! Loading resources...");

    int sticks = Round(scene_->GetAsyncProgress() * (float) 40);
    String progressBar = "";
    for (int i = 0; i < sticks; i++) {
        progressBar += "|";
    }

    auto *resource = static_cast<Resource *>(eventData[P_RESOURCE].GetPtr());
    String resourceName = resource->GetName();

    if (!headless_) {
        progressText_->SetText(progressBar);
        progressResText_->SetText(progressStr + String("% ") + resourceName);
        progressResText_->SetVisible(levelLoading_);
    } else {
        // HEADLESS MODE
        String progStr = progressStr + String("% ") + resourceName;
        URHO3D_LOGINFOF("%s", progStr.CString());
    }
}


void AlgebraKart::HandleClientResourceLoadAttempt(StringHash eventType, VariantMap& eventData) {

    using namespace LoadSucceed;

    String progressStr = String("Load: ") + String(Round(0 * 100.0f));

    int sticks = Round(0 * (float) 40);
    String progressBar = "";
    for (int i = 0; i < sticks; i++) {
        progressBar += "|";
    }

    String resourceName = eventData[P_RESOURCENAME].GetString();

    if (!headless_) {
        progressText_->SetText(progressBar);
        //String resourceName = resource->GetName();
        progressResText_->SetText(progressStr + String("% ") + resourceName);
        progressText_->SetVisible(true);
        progressResText_->SetVisible(true);
    } else {
        // HEADLESS MODE
        String progStr = progressStr + String("% ") + resourceName;
        URHO3D_LOGINFOF("%s", progStr.CString());
    }

    if (clientLevelLoading_) {
        // Show updated frame
 //       engine_->SetPauseMinimized(true);
 //       engine_->SetPauseMinimized(true);
        //engine_->Update();
         //engine_->Render();

        // Update load progress
        loadProgress_ = scene_->GetAsyncProgress();
    }




}

void AlgebraKart::HandleClientResourceLoadFinished(StringHash eventType, VariantMap& eventData) {

    using namespace LoadSucceed;

    String progressStr = String("Finished: ") + String(Round(1 * 100.0f));

    int sticks = Round(1 * (float) 40);
    String progressBar = "";
    for (int i = 0; i < sticks; i++) {
        progressBar += "|";
    }

    String resourceName = eventData[P_RESOURCENAME].GetString();

    if (!headless_) {
        progressText_->SetText(progressBar);


        //String resourceName = resource->GetName();
        progressResText_->SetText(progressStr + String("% ") + resourceName);
        progressText_->SetVisible(true);
        progressResText_->SetVisible(true);

    } else {
        // HEADLESS MODE
        String progStr = progressStr + String("% ") + resourceName;
        URHO3D_LOGINFOF("%s", progStr.CString());
    }


    if (clientLevelLoading_) {
        // Show updated frame
        //engine_->SetPauseMinimized(true);
       // engine_->RunFrame();
        //engine_->Update();
        //engine_->Render();

        // Update load progress
        loadProgress_ = scene_->GetAsyncProgress();
    }

}



void AlgebraKart::HandleClientSceneLoaded(StringHash eventType, VariantMap& eventData)
{
    // SERVER CODE
    UpdateLoadingProgress(0.8f, "Loading game world...");
    using namespace ClientSceneLoaded;
    URHO3D_LOGINFO("HandleClientSceneLoaded");
    URHO3D_LOGINFOF("Client: Scene checksum -> %s", ToStringHex(scene_->GetChecksum()).CString());

    // Client stores client object id
    auto *client = static_cast<Connection *>(eventData[P_CONNECTION].GetPtr());

    URHO3D_LOGINFOF("Client: Client info -> connected %d", client->IsConnected());

    // Once client joins start
    started_ = true;


    if (actorMap_[client]) {
        // Enable for 3D sounds to work (attach to camera node)
        SoundListener *listener = actorMap_[client]->GetNode()->CreateComponent<SoundListener>(REPLICATED);
        GetSubsystem<Audio>()->SetListener(listener);

        // you can set master volumes for the different kinds if sounds, 30% for music
        GetSubsystem<Audio>()->SetMasterGain(SOUND_MUSIC, 0.3);
        GetSubsystem<Audio>()->SetMasterGain(SOUND_EFFECT, 0.3);

    }

    // Server: send a delayed loaded message to client
    sendLoadMsg_.Push(client);


}

void AlgebraKart::SubscribeToEvents() {

    // Subscribe to key events for camera switching
    SubscribeToEvent(E_KEYDOWN, URHO3D_HANDLER(AlgebraKart, HandleKeyDown));

    // Subscribe to UI element events
    //SubscribeToEvent(textEdit_, E_TEXTFINISHED, URHO3D_HANDLER(AlgebraKart, HandleSendChat));
//    SubscribeToEvent(sendButton_, E_RELEASED, URHO3D_HANDLER(AlgebraKart, HandleSend));

    // Last chance to update before back buffer flip
//    SubscribeToEvent(E_ENDRENDERING, URHO3D_HANDLER(AlgebraKart, HandleEndRendering));
    SubscribeToEvent(E_BEGINRENDERING, URHO3D_HANDLER(AlgebraKart, HandleEndRendering));
    //E_BEGINRENDERING

    // Server asset loading
    SubscribeToEvent(E_RESOURCEBACKGROUNDLOADED, URHO3D_HANDLER(AlgebraKart, HandleResourceBackgroundLoaded));

    // Client asset loading
    SubscribeToEvent(E_LOADSUCCEED, URHO3D_HANDLER(AlgebraKart, HandleClientResourceLoadFinished));
    SubscribeToEvent(E_LOADATTEMPT, URHO3D_HANDLER(AlgebraKart, HandleClientResourceLoadAttempt));

    // Subscribe to log messages so that we can pipe them to the chat window
    SubscribeToEvent(E_LOGMESSAGE, URHO3D_HANDLER(AlgebraKart, HandleLogMessage));

    // Subscribe to network events
    SubscribeToEvent(E_NETWORKMESSAGE, URHO3D_HANDLER(AlgebraKart, HandleNetworkMessage));
    SubscribeToEvent(E_CLIENTSCENELOADED, URHO3D_HANDLER(AlgebraKart, HandleClientSceneLoaded));
    SubscribeToEvent(E_CLIENTCONNECTED, URHO3D_HANDLER(AlgebraKart, HandleClientConnected));

    // Subscribe to button actions
    SubscribeToEvent(playButton_, E_RELEASED, URHO3D_HANDLER(AlgebraKart, HandlePlayButton));
//    SubscribeToEvent(connectButton_, E_RELEASED, URHO3D_HANDLER(AlgebraKart, HandleConnect));
    SubscribeToEvent(disconnectButton_, E_RELEASED, URHO3D_HANDLER(AlgebraKart, HandleDisconnect));
    SubscribeToEvent(startServerButton_, E_RELEASED, URHO3D_HANDLER(AlgebraKart, HandleStartServer));
    SubscribeToEvent(exitButton_, E_RELEASED, URHO3D_HANDLER(AlgebraKart, HandleExit));

    SubscribeToEvent(E_CONNECTFAILED, URHO3D_HANDLER(AlgebraKart, HandleConnectionFailed));

    // Subscribe to server events
    SubscribeToEvent(E_SERVERSTATUS, URHO3D_HANDLER(AlgebraKart, HandleConnectionStatus));

    // Subscribe to player state events
    SubscribeToEvent(E_PLAYERSTATE, URHO3D_HANDLER(AlgebraKart, HandlePlayerStateUpdate));
    // Events sent between client & server (remote events) must be explicitly registered or else they are not allowed to be received
    GetSubsystem<Network>()->RegisterRemoteEvent(E_PLAYERSTATE);

    // Subscribe to player pickup state events
    SubscribeToEvent(E_PLAYERPICKUPSTATE, URHO3D_HANDLER(AlgebraKart, HandlePlayerPickUpState));
    // Events sent between client & server (remote events) must be explicitly registered or else they are not allowed to be received
    GetSubsystem<Network>()->RegisterRemoteEvent(E_PLAYERPICKUPSTATE);

    // E_CHATMESSAGE
    // Subscribe to chat message events
    SubscribeToEvent(E_CHATMESSAGE, URHO3D_HANDLER(AlgebraKart, HandleChatMessage));
    // Events sent between client & server (remote events) must be explicitly registered or else they are not allowed to be received
    GetSubsystem<Network>()->RegisterRemoteEvent(E_CHATMESSAGE);


    SubscribeToEvent(E_CLIENTOBJECTID, URHO3D_HANDLER(AlgebraKart, HandleClientObjectID));
    // Events sent between client & server (remote events) must be explicitly registered or else they are not allowed to be received
    GetSubsystem<Network>()->RegisterRemoteEvent(E_CLIENTOBJECTID);

    SubscribeToEvent(E_LOGINLISTREFRESH, URHO3D_HANDLER(AlgebraKart, HandleLoginListRefresh));
    // Events sent between client & server (remote events) must be explicitly registered or else they are not allowed to be received
    GetSubsystem<Network>()->RegisterRemoteEvent(E_LOGINLISTREFRESH);


    SubscribeToEvent(E_NETWORKUPDATESENT, URHO3D_HANDLER(AlgebraKart, HandleNetworkUpdateSent));

    // Subscribe HandlePostUpdate() function for processing post update events
    SubscribeToEvent(E_POSTUPDATE, URHO3D_HANDLER(AlgebraKart, HandlePostUpdate));

    // Subscribe to PostRenderUpdate to draw debug geometry
    SubscribeToEvent(E_POSTRENDERUPDATE, URHO3D_HANDLER(AlgebraKart, HandlePostRenderUpdate));

  //  SubscribeToEvent(E_JOYSTICKHATMOVE, URHO3D_HANDLER(AlgebraKart, HandleJoystickHatMove));


    // Subscribe to fixed timestep physics updates for setting or applying controls
    SubscribeToEvent(E_PHYSICSPRESTEP, URHO3D_HANDLER(AlgebraKart, HandlePhysicsPreStep));

    // Subscribe the update event for the delayed events
    //SubscribeToEvent(E_POSTUPDATE, URHO3D_HANDLER(AlgebraKart, HandleDelayUpdate));

}

// CLIENT CODE
// Sample client input
Controls AlgebraKart::SampleCSPControls()
{
    Controls controls;
    controls.Reset(); // Clear all previous state


    auto* input = GetSubsystem<Input>();

    // ONLY use keyboard input, ignore controller for now
    bool forward = input->GetKeyDown(KEY_W) || input->GetKeyDown(KEY_UP);
    bool back = input->GetKeyDown(KEY_S) || input->GetKeyDown(KEY_DOWN);
    bool left = input->GetKeyDown(KEY_A) || input->GetKeyDown(KEY_LEFT);
    bool right = input->GetKeyDown(KEY_D) || input->GetKeyDown(KEY_RIGHT);

    bool accel = forward;
    bool use = input->GetKeyDown(KEY_SPACE); // GameController: ntwkControls_.IsDown(BUTTON_X);
    bool enter = input->GetKeyDown(KEY_RETURN); // GameController: ntwkControls_.IsDown(BUTTON_Y);SetRotation(
    bool brake = input->GetKeyDown(KEY_SHIFT); // GameController: ntwkControls_.IsDown(BUTTON_A));
    bool fire = input->GetKeyDown(KEY_SPACE); // GameController: ntwkControls_.IsDown(BUTTON_B));


    // Set buttons based on keyboard only
    if (forward) controls.buttons_ |= NTWK_CTRL_FORWARD;
    if (back) controls.buttons_ |= NTWK_CTRL_BACK;
    if (left) controls.buttons_ |= NTWK_CTRL_LEFT;
    if (right) controls.buttons_ |= NTWK_CTRL_RIGHT;
    if (use) controls.buttons_ |= NTWK_CTRL_USE;
    if (fire) controls.buttons_ |= NTWK_CTRL_FIRE;
    if (enter) controls.buttons_ |= NTWK_CTRL_ENTER;


    /*
    // axis
    const StringHash axisHashList[SDL_CONTROLLER_AXIS_MAX / 2] = {VAR_AXIS_0, VAR_AXIS_1, VAR_AXIS_2};
    // left stick - vehicle
    Variant lStick = ntwkControls_.extraData_[VAR_AXIS_0];
    Vector2 lAxisVal = lStick.GetVector2();

    // right stick
    Variant rStick = ntwkControls_.extraData_[VAR_AXIS_1];
    Vector2 rAxisVal = rStick.GetVector2();

    joySteer_ = lStick.GetVector2();
    float actorAccel = (rStick.GetVector2().y_ * 1.25f) + (keybdAccel*4.0f);

    // Set yaw to angle from axis calculation
    float joyAngle = 0;

    // Calculate joyAngle
    float theta = atan2(lAxisVal.y_,lAxisVal.x_);
    float thetaAng = theta*180.0f/M_PI;
    float delta = 0;
    float angle = 0;
    if (lAxisVal.x_ > 0) {
        if (lAxisVal.y_ < 0) {
            // x > 0 and y < 0
            delta = 360.0f;
            angle = delta + thetaAng;
        } else {
            // x > 0 and y > 0
            delta = 0;
            angle = delta + thetaAng;
        }
    } else {
        // x < 0
        if (lAxisVal.y_ < 0) {
            // x < 0 and y < 0
            delta = 360.0f;
            angle = delta + thetaAng;
        } else {
            // x < 0 and y > 0
            delta = 0;
            angle = delta + thetaAng;
        }
    }
    joyAngle = angle;

    // Rotate joy entry to align to screen
    joyAngle -= 180.0f;
     */

    /*
    bool accel = (input->GetKeyDown(Urho3D::KEY_UP) || input->GetKeyDown(KEY_W) || ntwkControls_.IsDown(BUTTON_B) || (actorAccel < -0.9f));
    bool use = input->GetKeyDown(KEY_SPACE) || ntwkControls_.IsDown(BUTTON_X);
    bool enter = input->GetKeyDown(KEY_F) || ntwkControls_.IsDown(BUTTON_Y);
    bool brake = (input->GetKeyDown(KEY_S) || ntwkControls_.IsDown(BUTTON_A));
    bool fire = (input->GetKeyDown(Urho3D::KEY_B) || ntwkControls_.IsDown(BUTTON_B));
*/


    if (input->GetKeyDown(Urho3D::KEY_RETURN)) {
        if (!textEdit_->GetText().Empty()) {
            // Send chat
            HandleSendChat();
        }
    }

    // Sound controller flags

    // Found network player
    if (isSnapped_) {

        if (!playDrumMachine_)
            // Set play source for client object sequencer
            PlayDrumMachine();

        String actorName = String("actor-") + clientName_;
        Node *actorNode = scene_->GetChild(actorName);

        String vehicleName = String("vehicle-") + clientName_;
        Node *vehicleNode = scene_->GetChild(vehicleName);
        //BLUE-304-vehicle

        Vector3 bodyPos;
        Quaternion rotation;

        if (actorNode) {
            // Retrieve Actor
            ClientObj *actor = actorNode->GetDerivedComponent<ClientObj>();

            if (actor) {
                NetworkActor *na = actorNode->GetDerivedComponent<NetworkActor>();
                Vehicle *v = vehicleNode->GetDerivedComponent<Vehicle>();
                if (na) {
                    auto *body = na->GetNode()->GetComponent<RigidBody>(true);
                    if (body) {
                        // CLIENT RIGID BODY RETRIEVED
                        bodyPos = body->GetPosition();
                        rotation = na->GetNode()->GetRotation();

                        if (na->entered_) {


                            // PLAY ENGINE SOUNDS IN VEHICLE ONLY

                            if (accel && !accelSndPlaying_) {
                                // Rev
                                SoundSource3D *snd = PlaySoundEffectLocal(driveAudioEffect[SOUND_FX_ENGINE_REV].c_str());
                                accelSndPlaying_ = true;
                                lastSnd_ = snd;
                            }

                            if (brake) {
                                // Brake
                                SoundSource3D *snd = PlaySoundEffectLocal(driveAudioEffect[SOUND_FX_ENGINE_BRAKE].c_str());
                                lastSnd_ = snd;
                            }

                        } else {
                            // Outside vehicle

                            /*
                            // jump
                            if (jump) {
                                // Jump
                                SoundSource3D *snd = PlaySoundEffectLocal(driveAudioEffect[SOUND_FX_ENGINE_BOOST].c_str());
                                lastSnd_ = snd;
                            }*/
                        }
                    }
                }
            }
        }
    }


    // Stop accel sound on button up
    if (!accel) {
        if (lastSnd_) {
            if (lastSnd_->IsPlaying()) {
                if (!lastSnd_.Expired()) {
                    lastSnd_->Stop();
                }
                accelSndPlaying_ = false;
            }
        }
    }


    // Store set controls in network control
    ntwkControls_ = controls;

    //csp->predict();
    //cspClient_->predict();
    return controls;
}

void AlgebraKart::HandleAI(float timeStep) {
    auto *input = GetSubsystem<Input>();

    // Iterate through agent controllers and apply update
    std::vector<std::shared_ptr<Agent>> agents = EvolutionManager::getInstance()->getAgents();
    std::vector<std::shared_ptr<AgentController>> controllers = EvolutionManager::getInstance()->getAgentControllers();

    for (int i = 0; i < EvolutionManager::getInstance()->getAgents().size(); i++) {

        if (EvolutionManager::getInstance()->getNetworkActors().size() > 0) {
            bool dead = (!EvolutionManager::getInstance()->getNetworkActors()[i]);
            if (!dead) {
                if (EvolutionManager::getInstance()->getNetworkActors()[i]->IsAlive()) {

                    //String botName = EvolutionManager::getInstance()->getNetworkActors()[i]->GetUserName();
                    std::shared_ptr<AgentController> controller = EvolutionManager::getInstance()->getAgentControllers()[i];

                    if (controller->isAlive()) {
                        // Process sensor inputs through ffn and apply calculated inputs
                        controller->update(timeStep);

                        std::shared_ptr<NetworkActor> actor = EvolutionManager::getInstance()->getNetworkActors()[i];
                        if (actor && actor->vehicle_) {
                            float speed = actor->vehicle_->GetSpeedKmH();
                            float SPEED_KILL_LIMIT = 400.0f;
                            if (speed > SPEED_KILL_LIMIT) {

                                /*
                                controller->die();
                                actor->Kill();

                                // respawn?
                                EvolutionManager::getInstance()->getNetworkActors()[i]->Kill();
                                EvolutionManager::getInstance()->getNetworkActors()[i] = nullptr;
                                URHO3D_LOGDEBUGF("CreateAgents -> SpawnPlayer: %d", (i));
                                // Spawn ai bot and generate NetworkActor
                                EvolutionManager::getInstance()->getNetworkActors()[i] = SpawnPlayer(i);
                                agents[i]->setRespawnTime(0);
                                scene_->MarkNetworkUpdate();
                                */

                            }
                        }
                    }

                    if (!controller->isAlive()) {
                        if (EvolutionManager::getInstance()->getNetworkActors()[i]) {

                            EvolutionManager::getInstance()->getNetworkActors()[i]->Kill();
                            scene_->MarkNetworkUpdate();
                            EvolutionManager::getInstance()->getNetworkActors()[i] = nullptr;
                        }
                    }
                }
            }
        }

        /*
        if (dead) {
            // dead

            // Found a dead agent, respawn

            // Wait until time for respawn
            agents[i]->setRespawnTime(agents[i]->getRespawnTime() + timeStep);

            if (agents[i]->getRespawnTime() > 30.0f) {
                // respawn?
                EvolutionManager::getInstance()->getNetworkActors()[i]->Kill();
                EvolutionManager::getInstance()->getNetworkActors()[i] = nullptr;
                URHO3D_LOGDEBUGF("CreateAgents -> SpawnPlayer: %d", (i));
                // Spawn ai bot and generate NetworkActor
                EvolutionManager::getInstance()->getNetworkActors()[i] = SpawnPlayer(i);
                agents[i]->setRespawnTime(0);
            }
        }*/
    }

}

void AlgebraKart::HandlePhysicsPreStep(StringHash eventType, VariantMap &eventData) {

    // Skip on loading
    if (levelLoading_) return;
    if (clientLevelLoading_) return;

    const float MOVE_TORQUE = 3.0f;

    using namespace Update;
    // Take the frame time step, which is stored as a float
    float timeStep = eventData[P_TIMESTEP].GetFloat();
    // This function is different on the client and server. The client collects controls (WASD controls + yaw angle)
    // and sets them to its server connection object, so that they will be sent to the server automatically at a
    // fixed rate, by default 30 FPS. The server will actually apply the controls (authoritative simulation.)
    auto network = GetSubsystem<Network>();
    auto serverConnection = network->GetServerConnection();

    RigidBody *body;
    // Rigid Body change function for applying forces
    auto change_func = [&](Vector3 force) {
        body->ApplyForce(force);
        body->ApplyTorque(force);
    };

    // Handle AI bots
    HandleAI(timeStep);

    // CLIENT CODE
    if (serverConnection)
    {
            // Predict locally
            auto applyControls = [&](const Controls& controls) {

                // Retrieve based on username
                String name = serverConnection->identity_["UserName"].GetString();

                String actorName = String("actor-") + name;
                Node *actorNode = scene_->GetChild(actorName);

                String vehicleName = String("vehicle-") + name;
                Node *vehicleNode = scene_->GetChild(vehicleName);

                    if (actorNode) {

                        // On change of controls, send update
                        serverConnection->SetControls(ntwkControls_);

                        // On change of controls, send update
                        if (lastControls_.buttons_ != ntwkControls_.buttons_) {
                            URHO3D_LOGINFOF("CLIENT: SET controls -> %s",
                                            ToStringHex(ntwkControls_.buttons_).CString());
                        }

                        // Store last control for next iteration
                        lastControls_ = ntwkControls_;



                        // Apply local predictive physics until we get next update from server

                        // CLIENT SECTION
                        // Client: collect controls
                        // Torque is relative to the forward vector
                        Quaternion rotationF(0.0f, ntwkControls_.yaw_, 0.0f);

                        // Apply local physics change (Client-Side Prediction)
                        if (ntwkControls_.buttons_ & NTWK_CTRL_FORWARD)
                            change_func(rotationF * Vector3::RIGHT * MOVE_TORQUE);
                        if (ntwkControls_.buttons_ & NTWK_CTRL_BACK)
                            change_func(rotationF * Vector3::LEFT * MOVE_TORQUE);
                        if (ntwkControls_.buttons_ & NTWK_CTRL_LEFT)
                            change_func(rotationF * Vector3::FORWARD * MOVE_TORQUE);
                        if (ntwkControls_.buttons_ & NTWK_CTRL_RIGHT)
                            change_func(rotationF * Vector3::BACK * MOVE_TORQUE);

                    }
            };


        // In case the server wants to do position-based interest management using the NetworkPriority components,
        // we should also tell it our observer (camera) position.
        serverConnection->SetPosition(cameraNode_->GetPosition());

        if (cspClient_->currentControls == nullptr)
        {
            // Local client-side prediction
            auto controls = SampleCSPControls();

            // Predict locally
            applyControls(controls);

            // Sample controls when not running prediction
            cspClient_->addInput(controls);
        }
        else {
            // Not predicting
            applyControls(*cspClient_->currentControls);
        }
    }


    // SERVER CODE
    else if (network->IsServerRunning())
    {

        // Server: get local input and control server fly cam
        auto *input = GetSubsystem<Input>();

        float flySpeed = 6.0f;

        // Move Cam
        if (input->GetKeyDown(KEY_W)) {
            flyCam_ = flyCam_ - Vector3(flySpeed, 0, 0);
        }

        if (input->GetKeyDown(KEY_A)) {
            flyCam_ = flyCam_ - Vector3(0, 0, -flySpeed);
        }

        if (input->GetKeyDown(KEY_D)) {
            flyCam_ = flyCam_ - Vector3(0, 0, flySpeed);
        }

        if (input->GetKeyDown(KEY_S)) {
            flyCam_ = flyCam_ - Vector3(-flySpeed, 0, 0);
        }

        if (input->GetKeyDown(KEY_Q)) {
            flyCam_ = flyCam_ - Vector3(0, -flySpeed, 0);
        }

        if (input->GetKeyDown(KEY_E)) {
            flyCam_ = flyCam_ - Vector3(0, flySpeed, 0);
        }

        // Move Cam Target
        if (input->GetKeyDown(KEY_I)) {
            flyTargetCam_ = flyTargetCam_ - Vector3(flySpeed, 0, 0);
        }

        if (input->GetKeyDown(KEY_J)) {
            flyTargetCam_ = flyTargetCam_ - Vector3(0, 0, -flySpeed);
        }

        if (input->GetKeyDown(KEY_L)) {
            flyTargetCam_ = flyTargetCam_ - Vector3(0, 0, flySpeed);
        }

        if (input->GetKeyDown(KEY_K)) {
            flyTargetCam_ = flyTargetCam_ - Vector3(-flySpeed, 0, 0);
        }

        if (input->GetKeyDown(KEY_U)) {
            flyTargetCam_ = flyTargetCam_ - Vector3(0, -flySpeed, 0);
        }

        if (input->GetKeyDown(KEY_O)) {
            flyTargetCam_ = flyTargetCam_ - Vector3(0, flySpeed, 0);
        }

        // Server: apply controls to client objects
        const auto& connections = network->GetClientConnections();

        for (unsigned i = 0; i < connections.Size(); ++i)
        {
            Connection* connection = connections[i];

            //auto *actor = dynamic_cast<NetworkActor *>(actorMap_[connection].Get());

            Node* actorNode = serverObjects_[connection];
            if (!actorNode)
                continue;

            // Get the last controls sent by the client
            //const auto& controls = connection->GetControls();
            auto& controls = cspServer_->clientInputs[connection];

            // Server is direct controller of client objects

            // Server: apply controls to client objects
            const Controls &clientControls = connection->GetControls();

            // Get the object this connection is controlling
            ClientObj *actor = actorMap_[connection];

            if (!actor)
                continue;

            // Apply controls to network actor
            actor->SetControls(clientControls);

            // update last used ID
            cspServer_->clientLastIDs[connection] = controls.extraData_.Find("id")->second_.GetUInt();

            if (!headless_)
                GetSubsystem<DebugHud>()->SetAppStats("num_inputs: ", ++cspServer_->usedInputs);

            // reset controls
            controls = Controls();

        }
    }
}

// CLIENT CODE
void AlgebraKart::HandleLoginListRefresh(StringHash eventType, VariantMap &eventData) {

    // CLIENT CODE

    auto *network = GetSubsystem<Network>();
    Connection *serverConnection = network->GetServerConnection();

    // Handle Packed String -> contains Login List

    // Client identification packet
    if (eventType == E_LOGINLISTREFRESH) {
        URHO3D_LOGINFOF("HandleLoginListRefresh -> eventType: %l -> E_LOGINLISTREFRESH", eventType.Value());
        String packedStr = eventData[LoginListRefresh::P_LOGINLIST].GetString();
        Vector<String> rcvLoginList = packedStr.Split(';');

        // Store in local login list
        loginList_.Clear();

        loginList_.Push("PLAYER LIST:");
        loginList_.Push("============");

        int k = 0;
        for (String login: rcvLoginList) {
            // Store in local login list
            loginList_.Push(login.CString());
            k++;
        }


        // Reload login list if text exists
        for (int k = 0; k < loginList_.Size(); k++) {
            // Load login list
            if (loginListText_[k]) {
                loginListText_[k]->SetText(loginList_[k]);
            }
        }
    }

}

// CLIENT CODE
void AlgebraKart::HandleRaceRank(StringHash eventType, VariantMap &eventData) {

    // CLIENT CODE

    auto *network = GetSubsystem<Network>();
    Connection *serverConnection = network->GetServerConnection();

    // Race rankings
    if (eventType == E_RACERANK) {
        String playerIdStrList = eventData[RaceRank::P_PLAYERID_LIST].GetString();
        String rankStrList = eventData[RaceRank::P_RANK_LIST].GetString();
        String timeStrList = eventData[RaceRank::P_TIME_LIST].GetString();

        // Load string lists into local client attributes
        Vector<String> rcvPlayerIdList = playerIdStrList.Split(';');
        Vector<String> rcvRankList = rankStrList.Split(';');
        Vector<String> rcvTimeList = timeStrList.Split(';');

        // Refresh local lists
        rcvPlayerIdList_.Clear();
        rcvRankList_.Clear();
        rcvTimeList_.Clear();

        for (String playerId: rcvPlayerIdList) {
            // Store in local list
            rcvPlayerIdList_.Push(playerId.CString());
        }

        for (String rank: rcvRankList) {
            // Store in local list
            rcvRankList_.Push(rank.CString());
        }

        for (String time: rcvTimeList) {
            // Store in local list
            rcvTimeList_.Push(time.CString());
        }

        URHO3D_LOGINFOF("HandleRaceRank: E_RACERANK -> PLAYERID_LIST %s  TIME_LIST %s, RANK LIST %s -> ", playerIdStrList.CString(), timeStrList.CString(), rankStrList.CString());
    }
}

// CLIENT CODE
void AlgebraKart::HandleRaceVictory(StringHash eventType, VariantMap &eventData) {

    // CLIENT CODE

    auto *network = GetSubsystem<Network>();
    Connection *serverConnection = network->GetServerConnection();

    // Race rankings
    if (eventType == E_RACEVICTORY) {
        String playerIdStrList = eventData[RaceRank::P_PLAYERID_LIST].GetString();
        String rankStrList = eventData[RaceRank::P_RANK_LIST].GetString();
        String timeStrList = eventData[RaceRank::P_TIME_LIST].GetString();

        // Load string lists into local client attributes
        Vector<String> rcvPlayerIdList = playerIdStrList.Split(';');
        Vector<String> rcvRankList = rankStrList.Split(';');
        Vector<String> rcvTimeList = timeStrList.Split(';');

        // Refresh local lists
        rcvPlayerIdList_.Clear();
        rcvRankList_.Clear();
        rcvTimeList_.Clear();

        for (String playerId: rcvPlayerIdList) {
            // Store in local list
            rcvPlayerIdList_.Push(playerId.CString());
        }

        for (String rank: rcvRankList) {
            // Store in local list
            rcvRankList_.Push(rank.CString());
        }

        for (String time: rcvTimeList) {
            // Store in local list
            rcvTimeList_.Push(time.CString());
        }

        URHO3D_LOGINFOF("HandleRaceVictory: E_RACEVICTORY -> PLAYERID_LIST %s  TIME_LIST %s, RANK LIST %s -> ", playerIdStrList.CString(), timeStrList.CString(), rankStrList.CString());
    }
}

// CLIENT CODE
void AlgebraKart::HandleRadioTrackNext(StringHash eventType, VariantMap &eventData) {

    // CLIENT CODE

    auto *network = GetSubsystem<Network>();
    Connection *serverConnection = network->GetServerConnection();

    // Next track on the radio
    if (eventType == E_RADIOTRACKNEXT) {
        URHO3D_LOGINFOF("HandleRadioTrackNext -> eventType: %l -> E_RADIOTRACKNEXT", eventType.Value());
        String trackArtist = eventData[RadioTrackNext::P_TRACKARTIST].GetString();
        String trackArtistLink = eventData[RadioTrackNext::P_LINKARTIST].GetString();
        String trackName = eventData[RadioTrackNext::P_TRACKNAME].GetString();

        if (bkgMusic_) {

            RadioTrack track;
            track.trackName = trackName;
            track.artistName = trackArtist;
            track.artistLink = trackArtistLink;
            radioTrackInfoQueue_.Push(track);

            if (!bkgMusicPlaying_) {
                bkgMusicPlaying_ = true;
                PlayNextTrack();
            }
        }
    }
}


void AlgebraKart::HandleNetworkUpdateSent(StringHash eventType, VariantMap& eventData)
{
//    URHO3D_LOGINFO("HandleNetworkUpdateSent");

    Network* network = GetSubsystem<Network>();
    Connection* serverConnection = network->GetServerConnection();

    // Client: clear controls
    if (serverConnection) {

        // CLIENT CODE

        String name = serverConnection->identity_["UserName"].GetString();

        // Locate node
        Node *node = scene_->GetChild(name);

        if (node) {

            // Retrieve NetworkActor
            ClientObj *networkActor = node->GetDerivedComponent<ClientObj>();
            if (networkActor) {
                // Clear network actor controls
                //URHO3D_LOGINFO("networkActor -> Clear Controls");
                networkActor->ClearControls();
            }
        }
    }
}


void AlgebraKart::HandleNodeCollision(StringHash eventType, VariantMap& eventData) {

    /*
    using namespace PhysicsBeginContact2D;

    Node* p1Node = scene_->GetChild("Bear-P1", true);
    Node* p2Node = scene_->GetChild("Bear-P2", true);

    // Get colliding node
    auto* hitNodeA = static_cast<Node*>(eventData[PhysicsBeginContact2D::P_NODEA].GetPtr());
    auto* hitNodeB = static_cast<Node*>(eventData[PhysicsBeginContact2D::P_NODEB].GetPtr());

    URHO3D_LOGINFOF("hitNodeA=%d, hitNodeB=%d", hitNodeA, hitNodeB);
    URHO3D_LOGINFOF("hitNodeA id=%d, hitNodeB id=%d", hitNodeA->GetID(), hitNodeB->GetName());
        Vector2 contactPosition;

        MemoryBuffer contacts(eventData[P_CONTACTS].GetBuffer());
        while (!contacts.IsEof()) {
            contactPosition = contacts.ReadVector2();
            auto contactNormal = contacts.ReadVector2();
            auto contactDistance = contacts.ReadFloat();
            auto contactImpulse = contacts.ReadFloat();
       //     std::cout << "contact position " << contactPosition.ToString().CString() << std::endl;
       //     std::cout << "contact normal " << contactNormal.ToString().CString() << std::endl;
       //     std::cout << "contact distance " << contactDistance << std::endl;
       //     std::cout << "contact impulse " << contactImpulse << std::endl;
        }

        std::cout << std::endl;
*/
}

void AlgebraKart::HandlePlayerStateUpdate(StringHash eventType, VariantMap& eventData) {
    using namespace ClientPlayerState;

    // Player state update
    if (eventType == E_PLAYERSTATE) {

        //int id = eventData[P_ID].GetUInt();
        //int vehicleId = eventData[P_VEHICLE_ID].GetUInt();

//        int wheelContactsNum = eventData[P_WHEEL_CONTACTS].GetInt();
//        float noWheelContactTime = eventData[P_NO_WHEEL_CONTACT_TIME].GetFloat();
        //float distance = eventData[P_DISTANCE].GetFloat();
        int life = eventData[P_LIFE].GetInt();
        int prana = eventData[P_PRANA].GetInt();
        float rpm = eventData[P_RPM].GetFloat();
        float velocity = eventData[P_VELOCITY].GetFloat();
        float steer = eventData[P_STEER].GetFloat();
        int pickupItemState = eventData[P_PICKUP_ITEM].GetInt();

        currentSteeringValue_ = steer;
        hasValidSteeringData_ = true;
        UpdateSteeringWheelDisplay();

        URHO3D_LOGDEBUGF("Client -> HandlePlayerStateUpdate: life=%d, prana=%d, rpm=%f, velocity=%f, steer=%f, pickup=%d",
                         life, prana, rpm, velocity, steer, pickupItemState);

        // Sequencer data
        float currTime = 0;
        float beatTime = 0;
        int beat = 0;
//        float currTime = eventData[P_SEQ_CURR_TIME].GetFloat();
 //       float beatTime = eventData[P_SEQ_BEAT_TIME].GetFloat();
  //      int beat = eventData[P_SEQ_BEAT].GetInt();

        /*
        URHO3D_LOGINFOF("Client -> HandlePlayerStateUpdate: %d %f, %f, %d, %d, %f, %f, %f, %f, %f, %d, %d",
                        wheelContactsNum, noWheelContactTime, distance, life, prana, rpm, velocity, steer, currTime,
                        beatTime, beat, pickupItemState);
*/
        URHO3D_LOGINFOF("Client -> HandlePlayerStateUpdate: %d, %d, %f, %f, %f, %d",
                        life, prana, rpm, velocity, steer, pickupItemState);

        // Update sequencer view based on player update
        if (seqTimeCursorModel_) {
            auto *cache = GetSubsystem<ResourceCache>();
            //currTime, beatTimeStep, beat
            float beatSpaceSize = 6.7f;
            float subBeatSpaceSize = 6.5f;
            seqTimeCursorModel_->GetNode()->SetPosition(Vector3(seqTimeCursorModel_->GetNode()->GetPosition().x_,
                                                                seqTimeCursorModel_->GetNode()->GetPosition().y_,
                                                                25.6f - beat * beatSpaceSize));
            float beatSpace = 0.069f;

            for (int i = 0; i < NUM_DRUM_MACHINE_PADS; i++) {

                if (beatModelVec_[i]) {
                    beatModelVec_[i]->SetEnabled(true);


                    // Channel 1
                    if (i == (beat - 1) * 4) {
                        beatModelVec_[i]->SetMaterial(cache->GetResource<Material>("Materials/buttons.active.xml"));
                    } else {
                        beatModelVec_[i]->SetMaterial(cache->GetResource<Material>("Materials/buttons.xml"));
                    }

                    // Channel 2
                    if ((i >= 16) && (i < 32)) {
                        int subBeatIdx = ((beat - 1) * 4) + 16 + Round(beatTime * 4.0f);
                        if (i == subBeatIdx) {
                            beatModelVec_[i]->SetMaterial(cache->GetResource<Material>("Materials/buttons.active.xml"));
                        } else {
                            beatModelVec_[i]->SetMaterial(cache->GetResource<Material>("Materials/buttons.xml"));
                        }
                    }

                    // Channel 3
                    if ((i >= 32) && (i < 48)) {
                        int subBeatIdx = ((beat - 1) * 4) + 32 + Round(beatTime * 4.0f);
                        if (i == subBeatIdx) {
                            beatModelVec_[i]->SetMaterial(cache->GetResource<Material>("Materials/buttons.active.xml"));
                        } else {
                            beatModelVec_[i]->SetMaterial(cache->GetResource<Material>("Materials/buttons.xml"));
                        }
                    }

                    // Channel 4
                    if ((i >= 48) && (i < 64)) {
                        int subBeatIdx = ((beat - 1) * 4) + 48 + Round(beatTime * 4.0f);
                        if (i == subBeatIdx) {
                            beatModelVec_[i]->SetMaterial(cache->GetResource<Material>("Materials/buttons.active.xml"));
                        } else {
                            beatModelVec_[i]->SetMaterial(cache->GetResource<Material>("Materials/buttons.xml"));
                        }
                    }

                }
            }


            if ((beat % 2) == 0) {
                metronomeCursorModel_->SetMaterial(cache->GetResource<Material>("Materials/buttons.xml"));
            } else {
                metronomeCursorModel_->SetMaterial(cache->GetResource<Material>("Materials/buttons.active.xml"));
            }

            beatTimeCursorModel_->GetNode()->SetPosition(Vector3(beatTimeCursorModel_->GetNode()->GetPosition().x_,
                                                                 beatTimeCursorModel_->GetNode()->GetPosition().y_,
                                                                 25.6f - (beat * beatSpaceSize) -
                                                                 (beatTime * subBeatSpaceSize)));


            seqTimeCursorModel_->SetEnabled(false);
            beatTimeCursorModel_->SetEnabled(false);

            Vector3 v;
            v = seqCam_->GetNode()->GetPosition();
            /*
            Vector3 acc = Vector3(0,0,0.0011f);
            Vector3 v;
            if (seqCamOffset_ < -0.165f) {
                v = seqCam_->GetNode()->GetPosition();
            } else {
                seqCamOffset_ += acc.z_;
                v = Vector3(seqCam_->GetNode()->GetPosition().x_, seqCam_->GetNode()->GetPosition().y_, seqCamOffset_);
            }*/
            seqCam_->GetNode()->SetPosition(Vector3(v.x_, v.y_, v.z_));

            //URHO3D_LOGINFOF("seqCamOffset: %f", seqCamOffset_);
        }

        if (powerBarBkgP1Sprite_) {

            // Update player powerbar
            IntVector2 v = powerBarBkgP1Sprite_->GetSize();
            int power = int(((life) / 100.0f) * (float) v.x_);
            powerBarP1Sprite_->SetSize(power, v.y_);

            // Get vehicle attributes
            // Max engine force and rpm

            float maxRPM = 25000.0f;//9000.0f;
            float clampedRPM = rpm;
            if (clampedRPM > maxRPM) {
                clampedRPM = maxRPM;
            }

            v = rpmBarBkgP1Sprite_->GetSize();
            float rpm = (clampedRPM / maxRPM);

            rpmBarP1Sprite_->SetSize(v.x_, v.y_);
            //rpmBarP1Sprite_->SetFullImageRect()
            //rpmBarP1Sprite_->SetHotSpot(rpm*v.x_, 0);
            float rpmX = (rpmBarP1Sprite_->GetWidth() * 0.2f) + (rpmBarP1Sprite_->GetWidth() * rpm * 0.8f);

            rpmBarP1Sprite_->SetSize(rpmX, v.y_);
            //rpmBarP1Sprite_->SetImageRect(IntRect(0, 0, rpmX, rpmBarP1Sprite_->GetHeight()));



            //float maxSpeed = 400.0f; // 400 km
            float maxSpeed = 400.0f; // to limit properly
            float clampedSpeed = velocity; // velocity is storing skid
            if (clampedSpeed > maxSpeed) {
                clampedSpeed = maxSpeed;
            }

            v = velBarBkgP1Sprite_->GetSize();
            int velocity = (((maxSpeed - clampedSpeed) / maxSpeed) * v.x_);
            velBarP1Sprite_->SetSize(velocity, v.y_);

            // Set text progress bars
            int sticks = 0;
            String progressBar = "";

            sticks = Round((life / 100.0f) * (float) 25);
            progressBar = "";
            for (int i = 0; i < sticks; i++) {
                progressBar += "|";
            }
            powerBarProgBarText_->SetText(progressBar);


            sticks = Round(rpm * (float) 25);
            progressBar = "";
            for (int i = 0; i < sticks; i++) {
                progressBar += "|";
            }
            rpmBarProgBarText_->SetText(progressBar);


            float vel = (clampedSpeed / maxSpeed);
            sticks = Round(vel * (float) 25);
            progressBar = "";
            for (int i = 0; i < sticks; i++) {
                progressBar += "|";
            }
            velBarProgBarText_->SetText(progressBar);


            float maxPrana = 100.0f;
            float pranaFloat = (prana / maxPrana);
            sticks = Round(pranaFloat * (float) 25);
            progressBar = "";
            for (int i = 0; i < sticks; i++) {
                progressBar += "|";
            }
            pranaBarProgBarText_->SetText(progressBar);

        }

        // Update speedometer
        if (speedometerBkgSprite_ && speedometerNeedleSprite_ && speedometerText_) {
            // Show speedometer when in vehicle and moving
            bool showSpeedometer = true;
            if (auto* networkActor = actorMap_[GetSubsystem<Network>()->GetServerConnection()].Get()) {
                bool inVehicle = true; //networkActor->entered_;
                showSpeedometer = inVehicle && velocity > 1.0f; // Show when moving faster than 1 km/h
            }

            speedometerBkgSprite_->SetVisible(showSpeedometer);
            speedometerNeedleSprite_->SetVisible(showSpeedometer);
            speedometerText_->SetVisible(showSpeedometer);

            if (showSpeedometer) {
                // Calculate needle rotation based on speed
                // Assuming max speed display of 200 km/h, with needle rotating 180 degrees
                float maxSpeed = 200.0f;
                float speedRatio = Clamp(velocity / maxSpeed, 0.0f, 1.0f);

                // Needle rotates from -90 degrees (left) to +90 degrees (right)
                float needleRotation = -45.0f + (speedRatio * 180.0f);
                speedometerNeedleSprite_->SetRotation(needleRotation);

                // Update digital readout
                speedometerText_->SetText(String((int)velocity) + " KM/H");

                // Color coding for speed
                if (velocity < 60.0f) {
                    speedometerText_->SetColor(Color::WHITE);
                } else if (velocity < 120.0f) {
                    speedometerText_->SetColor(Color::YELLOW);
                } else {
                    speedometerText_->SetColor(Color::RED);
                }
            }
        }
    }
}


void AlgebraKart::HandleClientUIUpdate() {

    Network *network = GetSubsystem<Network>();
    Connection *serverConnection = network->GetServerConnection();

    int life;
    int prana;
    float rpm;
    float velocity;
    float steer;
    if (serverConnection) {
        int size;
        PODVector<Node *> vec;

        String actorName = String("actor-") + clientName_;
        Node *actorNode = scene_->GetChild(actorName);

        String vehicleName = String("vehicle-") + clientName_;
        Node *vehicleNode = scene_->GetChild(vehicleName);

        //BLUE-304-vehicle

        Vector3 pos;
        Quaternion rot;
        Vector3 lVel;

        if (actorNode) {
            // Retrieve Actor
            ClientObj *actor = actorNode->GetDerivedComponent<ClientObj>();

            if (actor) {
                NetworkActor *na = actorNode->GetDerivedComponent<NetworkActor>();
                Vehicle *v = vehicleNode->GetDerivedComponent<Vehicle>();


                if (na) {
                    auto *body = na->GetNode()->GetComponent<RigidBody>(true);
                    if (body) {
                        // CLIENT RIGID BODY RETRIEVED
                        pos = body->GetPosition();
                        rot = body->GetRotation();
                        lVel = body->GetLinearVelocity();

                        life = na->life_;
                        prana = na->prana_;
                        rpm = na->rpm_;
                        velocity = na->velocity_;
                        steer = na->steer_;

                    }
                }
            }
        }
    }

        // Sequencer data
        float currTime = 0;
        float beatTime = 0;
        int beat = 0;
//        float currTime = eventData[P_SEQ_CURR_TIME].GetFloat();
        //       float beatTime = eventData[P_SEQ_BEAT_TIME].GetFloat();
        //      int beat = eventData[P_SEQ_BEAT].GetInt();


        /*
        URHO3D_LOGINFOF("Client -> HandlePlayerStateUpdate: %d %f, %f, %d, %d, %f, %f, %f, %f, %f, %d, %d",
                        wheelContactsNum, noWheelContactTime, distance, life, prana, rpm, velocity, steer, currTime,
                        beatTime, beat, pickupItemState);
*/

        // Update sequencer view based on player update
        if (seqTimeCursorModel_) {
            auto *cache = GetSubsystem<ResourceCache>();
            //currTime, beatTimeStep, beat
            float beatSpaceSize = 6.7f;
            float subBeatSpaceSize = 6.5f;
            seqTimeCursorModel_->GetNode()->SetPosition(Vector3(seqTimeCursorModel_->GetNode()->GetPosition().x_,
                                                                seqTimeCursorModel_->GetNode()->GetPosition().y_,
                                                                25.6f - beat * beatSpaceSize));
            float beatSpace = 0.069f;

            for (int i = 0; i < NUM_DRUM_MACHINE_PADS; i++) {

                if (beatModelVec_[i]) {
                    beatModelVec_[i]->SetEnabled(true);


                    // Channel 1
                    if (i == (beat - 1) * 4) {
                        beatModelVec_[i]->SetMaterial(cache->GetResource<Material>("Materials/buttons.active.xml"));
                    } else {
                        beatModelVec_[i]->SetMaterial(cache->GetResource<Material>("Materials/buttons.xml"));
                    }

                    // Channel 2
                    if ((i >= 16) && (i < 32)) {
                        int subBeatIdx = ((beat - 1) * 4) + 16 + Round(beatTime * 4.0f);
                        if (i == subBeatIdx) {
                            beatModelVec_[i]->SetMaterial(cache->GetResource<Material>("Materials/buttons.active.xml"));
                        } else {
                            beatModelVec_[i]->SetMaterial(cache->GetResource<Material>("Materials/buttons.xml"));
                        }
                    }

                    // Channel 3
                    if ((i >= 32) && (i < 48)) {
                        int subBeatIdx = ((beat - 1) * 4) + 32 + Round(beatTime * 4.0f);
                        if (i == subBeatIdx) {
                            beatModelVec_[i]->SetMaterial(cache->GetResource<Material>("Materials/buttons.active.xml"));
                        } else {
                            beatModelVec_[i]->SetMaterial(cache->GetResource<Material>("Materials/buttons.xml"));
                        }
                    }

                    // Channel 4
                    if ((i >= 48) && (i < 64)) {
                        int subBeatIdx = ((beat - 1) * 4) + 48 + Round(beatTime * 4.0f);
                        if (i == subBeatIdx) {
                            beatModelVec_[i]->SetMaterial(cache->GetResource<Material>("Materials/buttons.active.xml"));
                        } else {
                            beatModelVec_[i]->SetMaterial(cache->GetResource<Material>("Materials/buttons.xml"));
                        }
                    }

                }
            }


            if ((beat % 2) == 0) {
                metronomeCursorModel_->SetMaterial(cache->GetResource<Material>("Materials/buttons.xml"));
            } else {
                metronomeCursorModel_->SetMaterial(cache->GetResource<Material>("Materials/buttons.active.xml"));
            }

            beatTimeCursorModel_->GetNode()->SetPosition(Vector3(beatTimeCursorModel_->GetNode()->GetPosition().x_,
                                                                 beatTimeCursorModel_->GetNode()->GetPosition().y_,
                                                                 25.6f - (beat * beatSpaceSize) -
                                                                 (beatTime * subBeatSpaceSize)));


            seqTimeCursorModel_->SetEnabled(false);
            beatTimeCursorModel_->SetEnabled(false);

            Vector3 v;
            v = seqCam_->GetNode()->GetPosition();
            /*
            Vector3 acc = Vector3(0,0,0.0011f);
            Vector3 v;
            if (seqCamOffset_ < -0.165f) {
                v = seqCam_->GetNode()->GetPosition();
            } else {
                seqCamOffset_ += acc.z_;
                v = Vector3(seqCam_->GetNode()->GetPosition().x_, seqCam_->GetNode()->GetPosition().y_, seqCamOffset_);
            }*/
            seqCam_->GetNode()->SetPosition(Vector3(v.x_, v.y_, v.z_));

            //URHO3D_LOGINFOF("seqCamOffset: %f", seqCamOffset_);
        }

        if (powerBarBkgP1Sprite_) {

            // Update player powerbar
            IntVector2 v = powerBarBkgP1Sprite_->GetSize();
            int power = int(((life) / 100.0f) * (float) v.x_);
            powerBarP1Sprite_->SetSize(power, v.y_);

            // Get vehicle attributes
            // Max engine force and rpm

            float maxRPM = 25000.0f;//9000.0f;
            float clampedRPM = rpm;
            if (clampedRPM > maxRPM) {
                clampedRPM = maxRPM;
            }

            v = rpmBarBkgP1Sprite_->GetSize();
            float rpm = (clampedRPM / maxRPM);

            rpmBarP1Sprite_->SetSize(v.x_, v.y_);
            //rpmBarP1Sprite_->SetFullImageRect()
            //rpmBarP1Sprite_->SetHotSpot(rpm*v.x_, 0);
            float rpmX = (rpmBarP1Sprite_->GetWidth() * 0.2f) + (rpmBarP1Sprite_->GetWidth() * rpm * 0.8f);

            rpmBarP1Sprite_->SetSize(rpmX, v.y_);
            //rpmBarP1Sprite_->SetImageRect(IntRect(0, 0, rpmX, rpmBarP1Sprite_->GetHeight()));



            //float maxSpeed = 400.0f; // 400 km
            float maxSpeed = 400.0f; // to limit properly
            float clampedSpeed = velocity; // velocity is storing skid
            if (clampedSpeed > maxSpeed) {
                clampedSpeed = maxSpeed;
            }

            v = velBarBkgP1Sprite_->GetSize();
            int velocity = (((maxSpeed - clampedSpeed) / maxSpeed) * v.x_);
            velBarP1Sprite_->SetSize(velocity, v.y_);

            // Set text progress bars
            int sticks = 0;
            String progressBar = "";

            sticks = Round((life / 100.0f) * (float) 25);
            progressBar = "";
            for (int i = 0; i < sticks; i++) {
                progressBar += "|";
            }
            powerBarProgBarText_->SetText(progressBar);


            sticks = Round(rpm * (float) 25);
            progressBar = "";
            for (int i = 0; i < sticks; i++) {
                progressBar += "|";
            }
            rpmBarProgBarText_->SetText(progressBar);


            float vel = (clampedSpeed / maxSpeed);
            sticks = Round(vel * (float) 25);
            progressBar = "";
            for (int i = 0; i < sticks; i++) {
                progressBar += "|";
            }
            velBarProgBarText_->SetText(progressBar);


            float maxPrana = 100.0f;
            float pranaFloat = (prana / maxPrana);
            sticks = Round(pranaFloat * (float) 25);
            progressBar = "";
            for (int i = 0; i < sticks; i++) {
                progressBar += "|";
            }
            pranaBarProgBarText_->SetText(progressBar);

        }
}



void AlgebraKart::HandlePlayerPickUpState(StringHash eventType, VariantMap& eventData) {
    using namespace ClientPlayerPickUpState;

    // Player pickup state
    if (eventType == E_PLAYERPICKUPSTATE) {

        int pickupItemState = eventData[P_PICKUP_ITEM].GetInt();

        URHO3D_LOGINFOF("Client -> HandlePlayerPickUpState: %d",
                        pickupItemState);


        /*
        // Update Client UI bars
        if (pickupSprite_) {
            // Update player powerbar
            IntVector2 v = pickupSprite_->GetSize();
            //int power = int(((life) / 100.0f) * (float) v.x_);
            pickupSprite_->SetSize(v.x_, v.y_);
        }

        if (pickupItemSprite_) {
            // Update item
            IntVector2 v = pickupItemSprite_->GetSize();
            //int power = int(((life) / 100.0f) * (float) v.x_);
            pickupItemSprite_->SetSize(v.x_, v.y_);

            if (pickupItemState == 0) {
                // No pick up, show nothing
                pickupItemSprite_->SetVisible(false);
            } else {
                pickupItemSprite_->SetVisible(true);

                // types
                // 1 -> balloon
                // 2 -> heart with wings
                // 3 -> battery
                // 4 -> N2O
                // 5 ->
                // 6 ->

                int index = 0;
                pickupItemState = index;
                switch (pickupItemState) {
                    // type
                    case 1:
                        // 1 -> balloon
                        break;
                    case 2:
                        // 2 -> heart with wings
                        break;
                    case 3:
                        // 3 -> battery
                        break;
                    case 4:
                        // 4 -> N2O
                        break;
                }

                pickupItemSprite_->SetTexture(hudPickupItemTextures_[index]);
            }
        }*/
    }
}


void AlgebraKart::HandleClientConnected(StringHash eventType, VariantMap& eventData)
{
    using namespace ClientConnected;

    // When client connects, assign to scene to begin scene replication
    auto* newConnection = static_cast<Connection*>(eventData[P_CONNECTION].GetPtr());
}



void AlgebraKart::HandleCollisionBegin(StringHash eventType, VariantMap &eventData) {
    // Get colliding node
    auto *hitNode = static_cast<Node *>(eventData[PhysicsBeginContact2D::P_NODEA].GetPtr());
    if (hitNode->GetName() == "Bear-P1")
        hitNode = static_cast<Node *>(eventData[PhysicsBeginContact2D::P_NODEB].GetPtr());
    String nodeName = hitNode->GetName();
    Node *character2DNode = scene_->GetChild("Bear-P1", true);
}


void AlgebraKart::HandleCollisionReplay(StringHash eventType, VariantMap &eventData) {
    using namespace NodeCollision;

    // Add throttling to prevent spam
    static float lastCollisionReplayTime = 0.0f;
    float currentTime = GetSubsystem<Time>()->GetElapsedTime();

    if (currentTime - lastCollisionReplayTime < 2.0f) {
        return; // Ignore collisions within 2 seconds of last replay
    }

    // Get collision information safely
    float maxImpulse = 0.0f;
    try {
        maxImpulse = GetMaxCollisionImpulse(eventData);
    } catch (...) {
        URHO3D_LOGERROR("Exception while calculating collision impulse");
        return;
    }

    // Only trigger replay for significant collisions
    if (maxImpulse > 15.0f) {
        Node* nodeA = static_cast<Node*>(eventData[P_OTHERNODE].GetPtr());
        if (!nodeA) {
            URHO3D_LOGWARNING("Invalid collision node");
            return;
        }

        // Try to find associated vehicle
        NetworkActor* actor = FindNetworkActorFromNode(nodeA);
        if (actor && actor->GetNode() && replaySystem_ && replaySystemEnabled_) {

            // Additional safety check for the actor
            String actorName = "Unknown";
            try {
                actorName = actor->GetUserName();
            } catch (...) {
                URHO3D_LOGWARNING("Could not get actor name for collision replay");
                return;
            }

            // Update throttling timer
            lastCollisionReplayTime = currentTime;

            // Trigger replay with error handling
            try {
                replaySystem_->TriggerReplay(actor, REPLAY_HIGH_SPEED_COLLISION);
                URHO3D_LOGINFOF("Collision replay triggered for vehicle: %s (impulse: %f)",
                                actorName.CString(), maxImpulse);
            } catch (...) {
                URHO3D_LOGERROR("Exception while triggering collision replay");
            }
        }
    }
}

float AlgebraKart::GetMaxCollisionImpulse(VariantMap& eventData) {
    using namespace NodeCollision;
    MemoryBuffer contacts(eventData[P_CONTACTS].GetBuffer());
    float maxImpulse = 0.0f;

    while (!contacts.IsEof()) {
        contacts.ReadVector3(); // position
        contacts.ReadVector3(); // normal
        contacts.ReadFloat();   // distance
        float impulse = contacts.ReadFloat();
        maxImpulse = Max(maxImpulse, impulse);
    }

    return maxImpulse;
}

Vehicle* AlgebraKart::FindVehicleFromNode(Node* node) {
    // Search through the node hierarchy to find associated vehicle
    while (node) {
        Vehicle* vehicle = node->GetComponent<Vehicle>();
        if (vehicle) return vehicle;

        node = node->GetParent();
    }
    return nullptr;
}

NetworkActor* AlgebraKart::FindNetworkActorFromNode(Node* node) {
    // Search through the node hierarchy to find associated network actor
    while (node) {
        NetworkActor* actor = node->GetComponent<NetworkActor>();
        if (actor) return actor;

        node = node->GetParent();
    }
    return nullptr;
}

void AlgebraKart::HandleUpdateParticlePool(float timeStep) {

    // FIX: Clean up particles in fixed-size array pool
    static float cleanupTimer = 0.0f;

    cleanupTimer += timeStep;

    // Clean up every 2 seconds for the fixed array
    if (cleanupTimer >= 2.0f) {
        cleanupTimer = 0.0f;

        // Clean up dead/expired particles in the fixed array
        for (int i = 0; i < PARTICLE_POOL_SIZE; ++i) {
            ParticlePool& particle = particlePool_[i];

            // Check if particle is expired or invalid
            bool shouldCleanup = false;

            if (!particle.node || !particle.node->GetScene()) {
                shouldCleanup = true;
            }
            else if (particle.timeToLive > 0) {
                particle.timeToLive -= timeStep;
                if (particle.timeToLive <= 0) {
                    shouldCleanup = true;
                }
            }

            // Clean up the particle
            if (shouldCleanup) {
                if (particle.node && particle.node->GetScene()) {
                    particle.node->Remove();
                }

                // Reset the particle slot
                particle.node.Reset();
                particle.timeToLive = 0.0f;
                particle.active = false;

                // Reset any other particle properties you have
                if (particle.emitter) {
                    particle.emitter.Reset();
                }
            }
        }
    }

    // Also update particle lifetimes each frame
    for (int i = 0; i < PARTICLE_POOL_SIZE; ++i) {
        ParticlePool& particle = particlePool_[i];
        if (particle.active && particle.timeToLive > 0) {
            particle.timeToLive -= timeStep;
        }
    }
}

void AlgebraKart::HandleCollisionEnd(StringHash eventType, VariantMap &eventData) {
    // Get colliding node
    auto *hitNode = static_cast<Node *>(eventData[PhysicsEndContact2D::P_NODEA].GetPtr());
    if (hitNode->GetName() == "Bear-P1")
        hitNode = static_cast<Node *>(eventData[PhysicsEndContact2D::P_NODEB].GetPtr());
    String nodeName = hitNode->GetName();
    Node *character2DNode = scene_->GetChild("Bear-P1", true);

}


///
// HandleRenderUpdate is called every frame during Urho3D's render phase
// This happens AFTER the main Update event but BEFORE actual rendering

void AlgebraKart::HandleRenderUpdate(StringHash eventType, VariantMap &eventData) {
    // In headless mode, we skip ALL rendering-related operations
    if (headless_) {
        return; // Exit early - no graphics processing needed
    }

    // == FRAME TIMING ==
    // Get the time step for this frame (usually 1/60th second at 60 FPS)
    using namespace RenderUpdate;
    float timeStep = eventData[P_TIMESTEP].GetFloat();

    //using namespace Update;
    auto *input = GetSubsystem<Input>();

    // Take the frame time step, which is stored as a float
    //float timeStep = eventData[P_TIMESTEP].GetFloat();
    HandleUpdateParticlePool(timeStep);

    DebugRenderer *dbgRenderer = scene_->GetComponent<DebugRenderer>(REPLICATED);

    float deltaSum;

    // == UI UPDATES THAT DEPEND ON CURRENT FRAME STATE ==

    if (!isServer_) {

        // CLIENT CODE

        // Found network player
        if (isSnapped_) {

            String actorName = String("actor-") + clientName_;
            Node *actorNode = scene_->GetChild(actorName);

            String vehicleName = String("vehicle-") + clientName_;
            Node *vehicleNode = scene_->GetChild(vehicleName);
            //BLUE-304-vehicle

            Vector3 bodyPos;
            Quaternion rotation;

            if (actorNode) {
                // Retrieve Actor
                ClientObj *actor = actorNode->GetDerivedComponent<ClientObj>();

                if (actor) {
                    NetworkActor *na = actorNode->GetDerivedComponent<NetworkActor>();
                    Vehicle *v = vehicleNode->GetDerivedComponent<Vehicle>();
                    if (na) {
                        auto *body = na->GetNode()->GetComponent<RigidBody>(true);
                        if (body) {
                            // CLIENT RIGID BODY RETRIEVED
                            bodyPos = body->GetPosition();
                            rotation = na->GetNode()->GetRotation();

                            if (hasValidSteeringData_) {
                                UpdateSteeringWheelDisplay();
                            }

                            /*
                            if (speedometerNeedleSprite_) {
                                if (v) {
                                        // Update speedometer display based on current vehicle speed
                                        if (speedometerNeedleSprite_ && speedometerBkgSprite_) {
                                            // Get current player's vehicle speed
                                            float currentSpeed = v->GetSpeedKmH();weew

                                            // Calculate needle rotation (0-180 degrees for 0-200 km/h)
                                            float maxDisplaySpeed = 200.0f;
                                            float speedRatio = Clamp(currentSpeed / maxDisplaySpeed, 0.0f, 1.0f);
                                            float needleAngle = -90.0f + (speedRatio * 180.0f); // -90° to +90°

                                            // Rotate the needle sprite
                                            speedometerNeedleSprite_->SetRotation(needleAngle);

                                            // Update digital speed text
                                            if (speedometerText_) {
                                                speedometerText_->SetText(String((int)currentSpeed) + " km/h");
                                            }
                                        }
                                }
                            }*/

                            ///

                            // TODO: Finish vehicle damage system
                            // Vehicle Damage
                            if (vDamageSprite_) {
                                vDamageSprite_->SetVisible(true);
                                float height = 3.0f;
                                for (int i = 0; i < VDAMAGE_BODY_NUM_LINES; i++) {
                                    // TODO ADD v damage list
                                    float centerX = 6.0f;
                                    float centerY = -142.0f;
                                    SharedPtr<Sprite> sprite_;
                                    sprite_ = vDamage_FL_LineSprites_.At(i);
                                    sprite_->SetAlignment(HA_RIGHT, VA_BOTTOM);
                                    sprite_->SetPosition(Vector2(centerX-130.0f, centerY-90.0f+(i*height)));
                                    sprite_->SetOpacity(0.9f);
                                    // Set a low priority so that other UI elements can be drawn on top
                                    sprite_->SetPriority(-100);
                                    sprite_->SetVisible(true);

                                    sprite_ = vDamage_FR_LineSprites_.At(i);
                                    sprite_->SetAlignment(HA_RIGHT, VA_BOTTOM);
                                    sprite_->SetPosition(Vector2(centerX-48.0f, centerY-90.0f+(i*height)));
                                    sprite_->SetOpacity(0.9f);
                                    // Set a low priority so that other UI elements can be drawn on top
                                    sprite_->SetPriority(-100);
                                    sprite_->SetVisible(true);

                                    sprite_ = vDamage_BL_LineSprites_.At(i);
                                    sprite_->SetAlignment(HA_RIGHT, VA_BOTTOM);
                                    sprite_->SetPosition(Vector2(centerX-130.0f, centerY-10.0f+(i*height)));
                                    sprite_->SetOpacity(0.9f);
                                    // Set a low priority so that other UI elements can be drawn on top
                                    sprite_->SetPriority(-100);
                                    sprite_->SetVisible(true);

                                    sprite_ = vDamage_BR_LineSprites_.At(i);
                                    sprite_->SetAlignment(HA_RIGHT, VA_BOTTOM);
                                    sprite_->SetPosition(Vector2(centerX-48.0f, centerY-10.0f+(i*height)));
                                    sprite_->SetOpacity(0.9f);
                                    // Set a low priority so that other UI elements can be drawn on top
                                    sprite_->SetPriority(-100);
                                    sprite_->SetVisible(true);

                                    sprite_ = vDamage_FA_LineSprites_.At(i);
                                    sprite_->SetAlignment(HA_RIGHT, VA_BOTTOM);
                                    sprite_->SetPosition(Vector2(centerX, centerY+110.0f+(i*height)));
                                    sprite_->SetOpacity(0.9f);
                                    // Set a low priority so that other UI elements can be drawn on top
                                    sprite_->SetPriority(-100);
                                    sprite_->SetVisible(true);

                                    sprite_ = vDamage_RA_LineSprites_.At(i);
                                    sprite_->SetAlignment(HA_RIGHT, VA_BOTTOM);
                                    sprite_->SetPosition(Vector2(centerX, centerY-110.0f+(i*height)));
                                    sprite_->SetOpacity(0.9f);
                                    // Set a low priority so that other UI elements can be drawn on top
                                    sprite_->SetPriority(-100);
                                    sprite_->SetVisible(true);

                                    // ? left
                                    // 90 middle
                                    // ? right


                                }
                            }

                            if (na->entered_) {
                                if (v) {
                                    float steering = v->GetSteering();


                                }
                            } else {
                                if (steerActorSprite_) {
                                    steerWheelSprite_->SetVisible(false);

                                    // Vehicle Damage
                                    if (vDamageSprite_) {
//                                         vDamageSprite_->SetVisible(false);
                                    }

                                    // Detect move vector and visible only on non-zero
                                    if (joySteer_.LengthSquared() > 0) {

                                        // Joy movement


                                        steerActorSprite_->SetVisible(true);
                                        steerActorSprite_->SetRotation(body->GetRotation().YawAngle());
                                        steerActorSprite_->SetPosition(Vector2(-90, -90));
                                    } else {
                                        steerActorSprite_->SetVisible(false);
                                    }
                                }
                                /// Alternate
                                /**
                                 *
                                 *
                                 */
                                // == STEERING WHEEL VISUALIZATION ==
/*
                                // Show steering input visually
                                if (steerWheelSprite_ && steerActorSprite_) {
                                    float steerInput = GetCurrentSteerInput(); // -1.0 to 1.0

                                    // Rotate steering wheel sprite
                                    float wheelRotation = steerInput * 45.0f; // Max 45 degrees rotation
                                    steerWheelSprite_->SetRotation(wheelRotation);

                                    // Position the actor indicator on the wheel rim
                                    float indicatorAngle = wheelRotation * M_PI / 180.0f;
                                    float wheelRadius = 50.0f; // Radius in pixels
                                    Vector2 wheelCenter = steerWheelSprite_->GetPosition();
                                    Vector2 indicatorPos = wheelCenter + Vector2(
                                            Sin(indicatorAngle) * wheelRadius,
                                            -Cos(indicatorAngle) * wheelRadius
                                    );
                                    steerActorSprite_->SetPosition(indicatorPos);
                                }
*/


                            }

/*
                            // == AUDIO SPECTRUM VISUALIZATION (if radio is playing) ==
                            if (radioSpectrumSprite_ && _radioStream && _radioStream->IsPlaying()) {
                                // Update audio spectrum visualization
                                UpdateAudioSpectrum(timeStep);

                                // Draw frequency spectrum on the radio display
                                DrawFFT(Vector2(0, radioSpectrumHeight_ / 2),
                                        radioSpectrumWidth_,
                                        radioSpectrumHeight_,
                                        Color(0.97f, 0.19f, 0.58f)); // Pink color
                            }
*/

                            // == VEHICLE DAMAGE VISUALIZATION ==

                            // Update damage indicators (only visible when damaged)
                            UpdateVehicleDamageDisplay(v, timeStep);

                            // == NETWORK LAG INDICATOR ==

                            // Show connection quality
                            auto* network = GetSubsystem<Network>();
                            if (network->GetServerConnection()) {
                                float ping = network->GetServerConnection()->GetRoundTripTime();
                                UpdateNetworkStatusDisplay(ping);
                            }


                            // == PICKUP ITEM DISPLAY ==

                            // Player pickup state
                            int pickupItemState = -1;
                            if (na->numPickups_ > 0) {
                                pickupItemState = na->activePickup_;
                            }

                            //URHO3D_LOGDEBUGF("pickupItemState=%d", pickupItemState);
                            // Update Client UI bars

                            // Pickups
                            if (pickupSprite_) {
                                // Update player powerbar
                                IntVector2 v = pickupSprite_->GetSize();
                                //int power = int(((life) / 100.0f) * (float) v.x_);
                                pickupSprite_->SetSize(v.x_, v.y_);
                            }

                            if (pickupItemSprite_) {
                                // Update item
                                IntVector2 v = pickupItemSprite_->GetSize();
                                //int power = int(((life) / 100.0f) * (float) v.x_);
                                pickupItemSprite_->SetSize(v.x_, v.y_);

                                if (pickupItemState <= 0) {
                                    // No pick up, show nothing
                                    pickupItemSprite_->SetVisible(false);
                                } else {
                                    pickupItemSprite_->SetVisible(true);

                                    // Show current pickup item with blinking animation
                                    int currentPickup = pickupItemState;

                                    // Blinking animation for ready-to-use items
                                    static float blinkTimer = 0.0f;
                                    blinkTimer += timeStep * 3.0f; // Blink speed
                                    float alpha = (Sin(blinkTimer) + 1.0f) * 0.5f; // 0.0 to 1.0
                                    pickupItemSprite_->SetOpacity(Lerp(0.7f, 1.0f, alpha));

                                    // Set appropriate pickup icon
                                    if (pickupItemState > 0) {
                                        // types
                                        // 1 -> balloon
                                        // 2 -> heart with wings
                                        // 3 -> battery
                                        // 4 -> N2O
                                        // 5 ->
                                        // 6 ->

                                        switch (pickupItemState) {
                                            // type
                                            case 1:
                                                // 1 -> balloon
                                                break;
                                            case 2:
                                                // 2 -> heart with wings
                                                break;
                                            case 3:
                                                // 3 -> battery
                                                break;
                                            case 4:
                                                // 4 -> N2O
                                                break;
                                        }
                                        pickupItemSprite_->SetTexture(hudPickupItemTextures_[pickupItemState-1]);
                                    }
                                }
                            }

                            // == MINIMAP UPDATES ==
/*
                            // Update player position on minimap
                            if (miniMapP1Sprite_) {
                                Vector3 playerPos = GetPlayerWorldPosition(); // Your method
                                Vector2 minimapPos = WorldToMinimapCoords(playerPos); // Convert to minimap space

                                miniMapP1Sprite_->SetPosition(minimapPos);

                                // Rotate minimap icon based on player facing direction
                                float playerYaw = GetPlayerYaw(); // Your method
                                miniMapP1Sprite_->SetRotation(playerYaw);
                            }
*/


                            // Update client UI
                            HandleClientUIUpdate();

                        }
                    }
                }
            }
        }
    }
    ///
}

void AlgebraKart::UpdateAudioSpectrum(float timeStep) {
    // Process audio data and update spectrum visualization
    if (analyzer_ && capturer_) {
        // Update spectrum analyzer with current audio data
        analyzer_->update(capturer_->getBuffer(),
                          capturer_->bufferHeadIndex(),
                          audioSpectrumOptions_.topLevel,
                          audioSpectrumOptions_.bottomLevel,
                          audioSpectrumOptions_.minFreq,
                          audioSpectrumOptions_.maxFreq,
                          audioSpectrumOptions_.axisLogBase);
    }
}

void AlgebraKart::UpdateVehicleDamageDisplay(Vehicle* v, float timeStep) {
    // Show damage indicators on different parts of the vehicle

    // Update damage line sprites for each wheel
    for (int i = 0; i < 4; i++) {
        if (v->GetWheelDamage()[i] > 0.1f) {
            // Show damage lines with intensity based on damage level
            //auto& damageSprites = GetWheelDamageSprites(i); // Your method
            //UpdateDamageLines(damageSprites, wheelDamage[i], timeStep);
        }
    }
}

void AlgebraKart::UpdateNetworkStatusDisplay(float ping) {
    // Color-code connection quality
    Color statusColor;
    if (ping < 50.0f) {
        statusColor = Color::GREEN;      // Excellent
    } else if (ping < 100.0f) {
        statusColor = Color::YELLOW;     // Good
    } else if (ping < 200.0f) {
        statusColor = Color::MAGENTA;     // Fair
    } else {
        statusColor = Color::RED;        // Poor
    }

    // Update network status indicator
    //if (networkStatusSprite_) {
    //    networkStatusSprite_->SetColor(statusColor);
    //}
}

///

float ticks = 0;


void print_message( const pair<int,string>& message ){
    cout << message.first << " - " << message.second << endl;

}

void AlgebraKart::DrawWave(const Vector2 &start, int length, int pixelAmplitude, float t, const Color &color) {
    Vector<Vector2> points;

    if (!capturer_->getBuffer()->Empty()) {
        // use smallest possible step so sound wave fills window
        int step = (int)ceil((double)length / (double)capturer_->getBuffer()->Size());

        //       int step = (int)ceil((double)length / (double)analyzer_->spectrum().Size());
      //  auto amplitude = analyzer_->spectrum().Begin();
        auto amplitude = capturer_->getBuffer()->Begin();
        for (int i = 0; i < length; i+=step) {
           // points.Push(Vector2( start.x_ + i, (int)(start.y_ + (*amplitude) * pixelAmplitude)));
            points.Push(Vector2( start.x_ + i, (int)(start.y_ + (*amplitude) * pixelAmplitude)));
            ++amplitude;
        }
    } else { // silence
        points.Push(Vector2( start.x_, start.y_ ));
        points.Push(Vector2( start.x_ + length - 1, start.y_ ));

    }

    // Clear spectrum
    radioVisualizerImg_->FillRectangle(0,0,length,pixelAmplitude,Color(0,0,0));

    for (int i = 0; i < points.Size()-1; i++) {
        Vector2 point1 = points[i];
        Vector2 point2 = points[i+1];
        radioVisualizerImg_->DrawLine(point1.x_, point1.y_, point2.x_, point2.y_, color);
    }

    // Time Marker
  //  radioVisualizerImg_->DrawLine(t+1, 0, t+1, pixelAmplitude, Color(1,1,1));
    radioVisualizerImg_->DrawLine(t, 0, t, pixelAmplitude, Color(1,1,0));
  //  radioVisualizerImg_->DrawLine(t-1, 0, t-1, pixelAmplitude, Color(1,1,1));

    // Load new image data into texture
    radioSpectrumTexture_->SetData(radioVisualizerImg_, false);
}

void AlgebraKart::DrawFFT(const Vector2 &start, int length, int pixelAmplitude, const Color &color) {
    Vector<Vector2> points;
    Vector<float> buffer1;
    Vector<float> buffer2;
    size_t width = length;

    float smoothing = 0.5f;
    int windowSize = 1;

    struct Rect {
        Vector2 topRight;
        Vector2 bottomLeft;
    };
    float rectWidth;

    Vector<Rect> rectList;

    /////
    String retStr = "";
    const std::string str("⠀⠁⠂⠃⠄⠅⠆⠇⡀⡁⡂⡃⡄⡅⡆⡇⠈⠉⠊⠋⠌⠍⠎⠏⡈⡉⡊⡋⡌⡍⡎⡏⠐⠑⠒⠓⠔⠕⠖⠗⡐⡑⡒⡓⡔⡕⡖⡗⠘⠙⠚⠛⠜⠝⠞⠟⡘⡙⡚⡛⡜⡝⡞⡟⠠⠡⠢⠣⠤⠥⠦⠧⡠⡡⡢⡣⡤⡥⡦⡧⠨⠩⠪⠫⠬⠭⠮⠯⡨⡩⡪⡫⡬⡭⡮⡯⠰⠱⠲⠳⠴⠵⠶⠷⡰⡱⡲⡳⡴⡵⡶⡷⠸⠹⠺⠻⠼⠽⠾⠿⡸⡹⡺⡻⡼⡽⡾⡿⢀⢁⢂⢃⢄⢅⢆⢇⣀⣁⣂⣃⣄⣅⣆⣇⢈⢉⢊⢋⢌⢍⢎⢏⣈⣉⣊⣋⣌⣍⣎⣏⢐⢑⢒⢓⢔⢕⢖⢗⣐⣑⣒⣓⣔⣕⣖⣗⢘⢙⢚⢛⢜⢝⢞⢟⣘⣙⣚⣛⣜⣝⣞⣟⢠⢡⢢⢣⢤⢥⢦⢧⣠⣡⣢⣣⣤⣥⣦⣧⢨⢩⢪⢫⢬⢭⢮⢯⣨⣩⣪⣫⣬⣭⣮⣯⢰⢱⢲⢳⢴⢵⢶⢷⣰⣱⣲⣳⣴⣵⣶⣷⢸⢹⢺⢻⢼⢽⢾⢿⣸⣹⣺⣻⣼⣽⣾⣿");

    const int bs[] = {0, 0x8, 0xc, 0xe, 0xf};
    //const int bs[] = {0, 0x8, 0x4, 0x2, 0x1};


    const size_t resolution = width * 2;
    const size_t unitBarWidth = analyzer_->spectrum().Size() / resolution;

    buffer1.Resize(resolution);
    for (size_t barIndex = 0; barIndex < resolution; ++barIndex)
    {
        const size_t beginIndex = unitBarWidth * barIndex;
        const size_t endIndex = unitBarWidth * (barIndex + 1);

        float maxValue = 0.0f;
        for (size_t i = beginIndex; i < endIndex; ++i)
        {
            maxValue = std::max(maxValue, analyzer_->spectrum()[i]);
        }

        buffer1.Buffer()[barIndex] += (maxValue - buffer1.At(barIndex)) * smoothing;
    }

    const auto gaussianWeights = [](int windowSize, float variance)
    {
        const float pi = 3.1415926535f;
        std::vector<float> result(windowSize);
        int centerIndex = windowSize / 2;
        for (int i = 0; i < windowSize; ++i)
        {
            int currentX = i - centerIndex;
            result[i] = (1.0f / std::sqrt(2.0f * pi * variance)) * std::exp(-currentX * currentX / (2.0f * variance));
        }

        float sum = std::accumulate(result.begin(), result.end(), 0.0f);
        for(int i = 0; i < windowSize; ++i)
        {
            result[i] /= sum;
        }

        return result;
    };

    buffer2.Resize(buffer1.Size());
    const auto weights = gaussianWeights(windowSize, 1.0f);
    for (size_t barIndex = 0; barIndex < resolution; ++barIndex)
    {
        float value = 0.0f;
        for (int i = 0; i < windowSize; ++i)
        {
            int offset = i - windowSize / 2;
            value += (0 <= barIndex + offset && barIndex + offset < resolution ? buffer1.At(barIndex + offset) : 0.0f) * weights[i];
        }

        buffer2.Buffer()[barIndex] = value;

        Rect rect;
        rectWidth = (resolution/length);

 //       rect.bottomLeft = Vector2(barIndex*(rectWidth), 0);
//        rect.topRight = Vector2((barIndex*(rectWidth))+(rectWidth-2), value*17.0f);

        rect.bottomLeft = Vector2(barIndex*(rectWidth), 0);
        rect.topRight = Vector2((barIndex*(rectWidth))+(rectWidth*0.98f), barIndex);

        rectList.Push(rect);
    }

    /*
    for (size_t charIndex = 0; charIndex < width; ++charIndex)
    {
        const size_t leftIndex = unitBarWidth * (charIndex * 2 + 0);
        const size_t rightIndex = unitBarWidth * (charIndex * 2 + 1);

        int index = 0;

        {
            const float v = buffer2->At(charIndex*2 + 0);

            const int xi = static_cast<int>(v / 0.2f);
            const int x = std::max(0, std::min(4, xi));
            index |= bs[x];
        }

        {
            const float v = buffer2->At(charIndex*2 + 1);

            const int xi = static_cast<int>(v / 0.2f);
            const int x = std::max(0, std::min(4, xi));
            index |= (bs[x] << 4);
        }

        // Populate return string based on index string look up
//        retStr += str.substr(index*3, 3).c_str();
    }*/


    for (Rect rect : rectList) {
            radioVisualizerImg_->FillRectangle(rect.bottomLeft.x_, rect.bottomLeft.y_, rect.topRight.x_, rect.topRight.y_, color);
    }

    ///////



    // Load new image data into texture
    radioSpectrumTexture_->SetData(radioVisualizerImg_, false);
}


void AlgebraKart::HandleDelayUpdate(StringHash eventType, VariantMap& eventData) {
    using namespace Update;
    auto *input = GetSubsystem<Input>();

    // Take the frame time step, which is stored as a float
    float timeStep = eventData[P_TIMESTEP].GetFloat();

    // Send delayed messages
    for (auto it = welcomeMsgMap_.Begin(); it != welcomeMsgMap_.End(); ++it) {
        Connection *connection = (*it).first_;
        float wait = (*it).second_;
        if (wait > 0.0f) wait = wait -= timeStep;

        if (wait < 0.250f && wait != -909) {
            //SendWelcomeMessage(connection);
            wait = -909; // stop signal
        }

        // Store updated wait
        welcomeMsgMap_[connection] = wait;
    }
}


void AlgebraKart::HandleUpdate(StringHash eventType, VariantMap &eventData) {
    using namespace Update;
    auto *input = GetSubsystem<Input>();

    // Take the frame time step, which is stored as a float
    float timeStep = eventData[P_TIMESTEP].GetFloat();

    ticks += timeStep;
    upTime_ += timeStep;

    // Update loading screen animation
    UpdateLoadingScreen(timeStep);

        // Skip on loading
    if (levelLoading_) return;
    if (clientLevelLoading_) return;

    // Update timers
    lastCamRaycast += timeStep;

    HandleUpdateParticlePool(timeStep);
    // Update minimap
    if (minimapCam_)
    {
        UpdateMinimap(timeStep);
    }

    auto *cache = GetSubsystem<ResourceCache>();

    UI *ui = GetSubsystem<UI>();
    Server *server = GetSubsystem<Server>();

    Vector<Urho3D::String> loginList;

    // On the client, use the local login list
    if (isServer_) {




        // Clear for recalculation of ranking
        rankList_.Clear();

        // Add connected clients
        HashMap<String, Connection *> map = server->GetLoginList();
        loginList += map.Keys();

        // Add ai bot clients
        Vector<String> botMap = server->GetAgents();
        if (!botMap.Empty()) {

            for (String bot: botMap) {
                loginList += "[BOT] " + bot;
            }
        }


        // Calculate uptime
        int seconds = Round(upTime_);
        int minutes = seconds / 60;
        seconds %= 60;
        int hours = minutes / 60;
        minutes %= 60;


        int len = aiActorMap_.Size() + actorMap_.Size();
        struct RaceRank {
            String playerId;
            int lapNum;
            float lapTime;
            float totalTime;
            float totalDistance;
            int wayptIdx;
            float lastWaypt;
            int rank;
        };
        RaceRank rankArr[len];

        // Calculate rank

        // Retrieve AI bot actors
        for (int i = 0; i < aiActorMap_.Size(); i++) {
            auto *actor = dynamic_cast<NetworkActor *>(aiActorMap_[i].Get());
            if (actor) {
                if (actor->vehicle_) {
                    Node *node = actor->vehicle_->GetNode();

                    if (!node) return;

                    String username = actor->GetUserName();
                    float botDistance = actor->vehicle_->GetRaycastVehicle()->GetDistanceOnGround();
                    int lapNum = actor->lapNum_;
                    float lapTime = actor->lapTime_;
                    float totalTime = actor->totalTime_;

                    rankArr[i].playerId = username;
                    rankArr[i].lapNum = lapNum;
                    rankArr[i].lapTime = lapTime;
                    rankArr[i].totalTime = totalTime;
                    rankArr[i].totalDistance = botDistance;
                    rankArr[i].wayptIdx = actor->vehicle_->getSteerIndex();
                    rankArr[i].lastWaypt = actor->lastWaypoint_;
                    rankArr[i].rank = 0;

                    //URHO3D_LOGDEBUGF("botDistance=%f,getSteerIndex()=%d, actor->lastWaypoint_=%f", botDistance, actor->vehicle_->getSteerIndex(), actor->lastWaypoint_);
                }
            }
        }

        // Start c index from last ai bot
        int c = aiActorMap_.Size();

        // Retrieve Client actors
            if (GetSubsystem<Network>()->GetClientConnections().Size()) {
                auto connections = GetSubsystem<Network>()->GetClientConnections();
                for (auto it = connections.Begin(); it != connections.End(); ++it) {

                    Connection *connection = (*it);

                    auto *actor = dynamic_cast<NetworkActor *>(actorMap_[connection].Get());

                    if (actor) {
                        if (actor->vehicle_) {
                            Node *node = actor->vehicle_->GetNode();

                            if (!node) return;

                            String username = actor->GetUserName();
                            float botDistance = actor->vehicle_->GetRaycastVehicle()->GetDistanceOnGround();
                            int lapNum = actor->lapNum_;
                            float lapTime = actor->lapTime_;
                            float totalTime = actor->totalTime_;

                            Vector3 closestAiAgent;
                            Vector<float> distances;
                            // Retrieve ai actors
                            for (int i = 0; i < aiActorMap_.Size(); i++) {
                                auto *aiActor = dynamic_cast<NetworkActor *>(aiActorMap_[i].Get());
                                if (aiActor) {
                                    if (aiActor->vehicle_) {
                                        Node * node = aiActor->vehicle_->GetNode();

                                        if (!node) return;

                                        String username = aiActor->GetUserName();
                                        float botDistance = aiActor->vehicle_->GetRaycastVehicle()->GetDistanceOnGround();

                                        Vector3 distToActor = aiActor->GetBody()->GetPosition()-actor->GetBody()->GetPosition();
                                        distances.Push(distToActor.Length());
                                        // Set the actor to the closest ai target
                                        actor->SetProjectileTarget(aiActor->GetBody()->GetPosition());

                                    }
                                }
                            }




                            rankArr[c].playerId = username;
                            rankArr[c].lapNum = lapNum;
                            rankArr[c].lapTime = lapTime;
                            rankArr[c].totalTime = totalTime;
                            rankArr[c].totalDistance = botDistance;
                            rankArr[c].wayptIdx = actor->vehicle_->getSteerIndex();
                            rankArr[c].lastWaypt = actor->lastWaypoint_;
                            rankArr[c].rank = 0;
                            c++;
                            //URHO3D_LOGDEBUGF("botDistance=%f,getSteerIndex()=%d, actor->lastWaypoint_=%f", botDistance, actor->vehicle_->getSteerIndex(), actor->lastWaypoint_);
                        }
                    }
                }
        }


        float DEF_FLOAT = -999999.0f;
        float minTotalTime = DEF_FLOAT;
        float minLapTime = 999999.0f;
        int maxLap = 0;
        float maxDistance = 0;
        float distanceRange = 14.0f;
        int maxWaypt = 0;
        float minLastWaypt = 0;
        Vector<int> top;
        Vector<int> rem;

        vector<pair<int,string>> sortList1;
        for (int i = 0; i < len; i++) {
            sortList1.push_back(make_pair(rankArr[i].wayptIdx , rankArr[i].playerId.CString()));
        }

        // sorts pairs in decreasing order of their first value
        std::sort(sortList1.begin(), sortList1.end(), std::greater<>());

        vector<pair<float,String>> rankList;
        float wayptTime = 0;
        String id = "";

        for (int i = 0; i < sortList1.size(); i++) {
            // Assign rank
 //           rankArr[top.At(i)].rank;
            int wayptIdx = sortList1[i].first;
            id = String(sortList1[i].second.c_str());
            //URHO3D_LOGDEBUGF("SORT BY WAYPOINT IDX -> id=%s, wayptIdx=%d", id.CString(), wayptIdx);

            for (int j = 0; j < len; j++) {
                if (id.Contains(String(rankArr[j].playerId))) {
                    // Match
                    float frac = 0;
                    if (rankArr[j].lastWaypt == 0) { frac = 0.0000001f; } else { frac = rankArr[j].lastWaypt; }
                    rankList.push_back(make_pair((wayptIdx*100.0f)+frac, rankArr[j].playerId));
                }
            }
        }

        // Increasing order
        std::sort(rankList.begin(), rankList.end(), std::greater<>());

        // SERVER SEND UPDATED RACE RANK

        using namespace RaceRank;

        // Send the event forward
        VariantMap &newEventData = GetEventDataMap();
        String playerIdPackedStr = "";
        String rankPackedStr = "";
        String timePackedStr = "";


        // Retrieve AI bot actors
        for (int i = 0; i < aiActorMap_.Size(); i++) {
            auto *actor = dynamic_cast<NetworkActor *>(aiActorMap_[i].Get());
            if (actor) {
                timePackedStr += String(actor->GetUserName())+String("|")+String(actor->lastWaypoint_);
                timePackedStr += ";"; // delimiter
            }
        }

        // Retrieve Client actors
        if (GetSubsystem<Network>()->GetClientConnections().Size()) {
            auto connections = GetSubsystem<Network>()->GetClientConnections();
            for (auto it = connections.Begin(); it != connections.End(); ++it) {
                Connection *connection = (*it);
                auto *actor = dynamic_cast<NetworkActor *>(actorMap_[connection].Get());
                if (actor) {
                    timePackedStr += String(actor->GetUserName())+String("|")+String(actor->lastWaypoint_);
                    timePackedStr += ";"; // delimiter
                }
            }
        }

         // Send to client

         // Update race rank send timer
        lastRaceRank_ += timeStep;

        for (int r = 0; r < rankList.size(); r++) {

            String username = String(rankList.at(r).second);
            String rankScore = String(rankList.at(r).first);

            playerIdPackedStr += username;
            playerIdPackedStr += ";"; // delimiter

            rankPackedStr += String(username)+String("|")+String(rankScore);
            rankPackedStr += ";"; // delimiter

        }

        // Load race rank event packet
        newEventData[P_PLAYERID_LIST] = playerIdPackedStr;
        newEventData[P_RANK_LIST] = rankPackedStr;
        newEventData[P_TIME_LIST] = timePackedStr;

        // Retrieve Client actors
        if (GetSubsystem<Network>()->GetClientConnections().Size()) {
            auto connections = GetSubsystem<Network>()->GetClientConnections();
            for (auto it = connections.Begin(); it != connections.End(); ++it) {

                Connection *connection = (*it);

                // Send race rank every 2500 ms
                if (lastRaceRank_ > 2.5f) {
                    connection->SendRemoteEvent(E_RACERANK, true, newEventData);
                    String clientId = connection->ToString().CString();

                    URHO3D_LOGINFOF("E_RACERANK -> %s -> %s", playerIdPackedStr.CString(), timePackedStr.CString());

                    // Reset timer
                    lastRaceRank_ = 0;
                }

                if (lastPlayerStateTime_ > 0.2f) {

                    // Send the event forward
                    VariantMap &playerStateData = GetEventDataMap();

                    //URHO3D_PARAM(P_LIFE, Life);         // unsigned
                    //URHO3D_PARAM(P_RPM, RPM);         // float
                    //URHO3D_PARAM(P_VELOCITY, Velocity);     // float
                    //URHO3D_PARAM(P_PRANA, Prana);           // float
                    //URHO3D_PARAM(P_STEER, Steering);         // float
                    //URHO3D_PARAM(P_PICKUP_ITEM, PickUp);            // int

                    auto *actor = dynamic_cast<NetworkActor *>(actorMap_[connection].Get());
                    if (actor) {
                        // Load player state event packet
                        playerStateData[ClientPlayerState::P_LIFE] = actor->GetLife();
                        playerStateData[ClientPlayerState::P_RPM] = actor->GetSpeed();
                        playerStateData[ClientPlayerState::P_VELOCITY] = actor->vehicle_->GetSpeedKmH();
                        playerStateData[ClientPlayerState::P_PRANA] = actor->GetPrana();
                        playerStateData[ClientPlayerState::P_STEER] = actor->GetVehicle()->GetSteering();
                        playerStateData[ClientPlayerState::P_PICKUP_ITEM] = actor->GetPickUpItemState();

                        connection->SendRemoteEvent(E_PLAYERSTATE, true, playerStateData);
                    }
                    lastPlayerStateTime_ = 0;
                }
            }

            lastPlayerStateTime_ += timeStep;

            // Send to RACE VICTORY to client

            // Update race rank send timer
            lastRaceVictory_ += timeStep;

            for (int r = 0; r < rankList.size(); r++) {

                String username = String(rankList.at(r).second);
                String rankScore = String(rankList.at(r).first);

                playerIdPackedStr += username;
                playerIdPackedStr += ";"; // delimiter

                rankPackedStr += String(username) + String("|") + String(rankScore);
                rankPackedStr += ";"; // delimiter

            }

            // Load race victory event packet
            newEventData[P_PLAYERID_LIST] = playerIdPackedStr;
            newEventData[P_RANK_LIST] = rankPackedStr;
            newEventData[P_TIME_LIST] = timePackedStr;

            // Retrieve Client actors
            if (GetSubsystem<Network>()->GetClientConnections().Size()) {
                auto connections = GetSubsystem<Network>()->GetClientConnections();
                for (auto it = connections.Begin(); it != connections.End(); ++it) {

                    Connection *connection = (*it);

                    // Send race rank every 2500 ms
                    if (lastRaceVictory_ > 2.5f) {
                        connection->SendRemoteEvent(E_RACEVICTORY, true, newEventData);
                        String clientId = connection->ToString().CString();

                        URHO3D_LOGINFOF("E_RACEVICTORY -> %s -> %s", playerIdPackedStr.CString(),
                                        timePackedStr.CString());

                        // Reset timer
                        lastRaceVictory_ = 0;
                    }

                }
            }

            // Show rankings
            int r = 0;
            float vertScale = 26.0f;
            float vAlignRank = 260.0f;
            for (int r = 0; r < rankList.size(); r++) {
                if (rankText_[r]) {
                    rankText_[r]->SetAlignment(HA_LEFT, VA_TOP);
                    rankText_[r]->SetPosition(10.0f, vAlignRank + (r * vertScale));
                    rankText_[r]->SetVisible(true);
                    rankText_[r]->SetText(
                            String("[") + String(r) + String("] ") + String(rankList.at(r).second) + String(": ") +
                            String(rankList.at(r).first));
                }
            }
        }

        int k = 0;
        float vAlign = 100.0f;
        debugText_[k]->SetAlignment(HA_LEFT, VA_TOP);
        debugText_[k]->SetPosition(10.0f, vAlign + (k * 12));
        debugText_[k]->SetVisible(true);
        debugText_[k]->SetText(String("Uptime: ") + String(hours) + String("h ") + String(minutes) + String("m ") + String(seconds) + String("s"));

        k++;
        debugText_[k]->SetAlignment(HA_LEFT, VA_TOP);
        debugText_[k]->SetPosition(10.0f, vAlign + (k * 12));
        debugText_[k]->SetVisible(true);
        debugText_[k]->SetText(String("Cam Mode: ") + String(camMode_));

        float botSpeedKm = 0;
        float botAvgSpeedKm = 0;
        float botDistance = 0;
        bool onGround = false;
        float reward = 0;
        Vector3 pos;
        Quaternion rot;

        int wheelsContactNum = 0;
        float noWheelContactTime = 0;
        float wheelContactTime = 0;
        Vector<int> wheelContact;
        float turrentAngle = 0;
        float desiredSteer = 0;
        int steerIndex = 0;
        int lapNum = 0;
        float lapTime = 0;
        int pickupItemState = -2;

        // In-bot view mode (cam mode > 0)
        if (camMode_ > 0) {


            // Camera Positioning
            if (aiActorMap_[camMode_-1]) {
                    auto *actor = dynamic_cast<NetworkActor *>(aiActorMap_[camMode_-1].Get());
                    if (actor) {
                        if (actor->vehicle_) {
                            Node *node = actor->vehicle_->GetNode();

                            if (!node) return;

                            String username = actor->GetUserName();
                            botSpeedKm = round(abs(actor->vehicle_->GetRaycastVehicle()->GetSpeedKm()));
                            botDistance = actor->vehicle_->GetRaycastVehicle()->GetDistanceOnGround();
                            botAvgSpeedKm = round(abs(EvolutionManager::getInstance()->getAgentControllers()[camMode_-1]->getLastAvgSpdKm()));
                            onGround = actor->vehicle_->GetRaycastVehicle()->OnGround();
                            reward = EvolutionManager::getInstance()->getAgentControllers()[camMode_-1]->getCurrentCompletionReward();

                            // No wheel contact time
                            noWheelContactTime = EvolutionManager::getInstance()->getAgentControllers()[camMode_-1]->getTimeSinceNoWheelContact();

                            pos = actor->vehicle_->GetNode()->GetPosition();
                            rot = actor->vehicle_->GetNode()->GetRotation();

                            wheelsContactNum = actor->vehicle_->GetRaycastVehicle()->getNumWheelsPrevContact();
                            wheelContactTime = actor->vehicle_->getWheelContactTime();

                            turrentAngle = actor->vehicle_->GetTurrentAngle();


                            desiredSteer = actor->vehicle_->getDesiredSteer();
                            steerIndex = actor->vehicle_->getSteerIndex();

                            lapNum = actor->lapNum_;
                            lapTime = actor->lapTime_;

                            pickupItemState = actor->GetPickUpItemState();

                        }
                    }

            }


            // Update debug text
            k++;
            debugText_[k]->SetAlignment(HA_LEFT, VA_TOP);
            debugText_[k]->SetPosition(10.0f, vAlign + (k * 12));
            debugText_[k]->SetVisible(true);
            debugText_[k]->SetText(String("Speed (km/h): ") + String(botSpeedKm));

            k++;
            debugText_[k]->SetAlignment(HA_LEFT, VA_TOP);
            debugText_[k]->SetPosition(10.0f, vAlign + (k * 12));
            debugText_[k]->SetVisible(true);
            debugText_[k]->SetText(String("Distance (km): ") + String(botDistance));


            k++;
            debugText_[k]->SetAlignment(HA_LEFT, VA_TOP);
            debugText_[k]->SetPosition(10.0f, vAlign + (k * 12));
            debugText_[k]->SetVisible(true);
            debugText_[k]->SetText(String("Waypoint: ") + String(steerIndex));

            k++;
            debugText_[k]->SetAlignment(HA_LEFT, VA_TOP);
            debugText_[k]->SetPosition(10.0f, vAlign + (k * 12));
            debugText_[k]->SetVisible(true);
            debugText_[k]->SetText(String("Lap: ") + String(lapNum));


            k++;
            debugText_[k]->SetAlignment(HA_LEFT, VA_TOP);
            debugText_[k]->SetPosition(10.0f, vAlign + (k * 12));
            debugText_[k]->SetVisible(true);
            debugText_[k]->SetText(String("Lap Time: ") + String(lapTime));


            k++;
            debugText_[k]->SetAlignment(HA_LEFT, VA_TOP);
            debugText_[k]->SetPosition(10.0f, vAlign + (k * 12));
            debugText_[k]->SetVisible(true);
            debugText_[k]->SetText(String("rankArr[top1]: "));

            k++;
            debugText_[k]->SetAlignment(HA_LEFT, VA_TOP);
            debugText_[k]->SetPosition(10.0f, vAlign + (k * 12));
            debugText_[k]->SetVisible(true);
            debugText_[k]->SetText(String("rankArr[top2]"));

            k++;
            debugText_[k]->SetAlignment(HA_LEFT, VA_TOP);
            debugText_[k]->SetPosition(10.0f, vAlign + (k * 12));
            debugText_[k]->SetVisible(true);
            debugText_[k]->SetText(String("desiredSteer -> ") + String(desiredSteer));

            k++;
            debugText_[k]->SetAlignment(HA_LEFT, VA_TOP);
            debugText_[k]->SetPosition(10.0f, vAlign + (k * 12));
            debugText_[k]->SetVisible(true);
            debugText_[k]->SetText(String("pickupItemState -> ") + String(pickupItemState));

            /*       k++;
                   debugText_[k]->SetAlignment(HA_LEFT, VA_TOP);
                   debugText_[k]->SetPosition(10.0f, 400 + (k * 20));
                   debugText_[k]->SetVisible(true);
                   debugText_[k]->SetText(String("Wheel Contact: ") + String(wheelsContactNum));
       */

        } // camMode > 0
        else {

            // SERVER: LAST CLIENT JOINED PLAYER CAM

            // camMode = 0 (last joined player cam)

            // Camera Positioning
            if (actorMap_[lastConnection_]) {
                auto *actor = dynamic_cast<NetworkActor *>(actorMap_[lastConnection_].Get());
                if (actor) {
                        if (actor->vehicle_) {
                            Node *node = actor->vehicle_->GetNode();
                            if (!node) return;

                            String username = actor->GetUserName();
                            botSpeedKm = round(abs(actor->vehicle_->GetRaycastVehicle()->GetSpeedKm()));
                            botDistance = actor->vehicle_->GetRaycastVehicle()->GetDistanceOnGround();
                            onGround = actor->vehicle_->GetRaycastVehicle()->OnGround();

                            pos = actor->vehicle_->GetNode()->GetPosition();
                            rot = actor->vehicle_->GetNode()->GetRotation();


                            noWheelContactTime = actor->vehicle_->GetRaycastVehicle()->getNoWheelContactTime();

//                            wheelsContactNum = actor->vehicle_->GetRaycastVehicle()->getNumWheelsContact();
                            wheelsContactNum = actor->vehicle_->GetRaycastVehicle()->getNumWheelsPrevContact();


                            turrentAngle = actor->vehicle_->GetTurrentAngle();

                            pickupItemState = actor->GetPickUpItemState();


                        }
                    }


                    // Update debug text
                    k++;
                    debugText_[k]->SetAlignment(HA_LEFT, VA_TOP);
                    debugText_[k]->SetPosition(10.0f, vAlign + (k * 12));
                    debugText_[k]->SetVisible(true);
                    debugText_[k]->SetText(String("Speed (km/h): ") + String(botSpeedKm));

                    k++;
                    debugText_[k]->SetAlignment(HA_LEFT, VA_TOP);
                    debugText_[k]->SetPosition(10.0f, vAlign + (k * 12));
                    debugText_[k]->SetVisible(true);
                    debugText_[k]->SetText(String("Distance (km): ") + String(botDistance));

/*                    k++;
                    debugText_[k]->SetAlignment(HA_LEFT, VA_TOP);
                    debugText_[k]->SetPosition(10.0f, 400 + (k * 20));
                    debugText_[k]->SetVisible(true);
                    debugText_[k]->SetText(String("Avg Speed (km/h): ") + String(botAvgSpeedKm));
*/

                    k++;
                    debugText_[k]->SetAlignment(HA_LEFT, VA_TOP);
                    debugText_[k]->SetPosition(10.0f, vAlign + (k * 12));
                    debugText_[k]->SetVisible(true);
                    debugText_[k]->SetText(String("On Ground: ") + String(onGround));

                    k++;
                    debugText_[k]->SetAlignment(HA_LEFT, VA_TOP);
                    debugText_[k]->SetPosition(10.0f, vAlign + (k * 12));
                    debugText_[k]->SetVisible(true);
                    debugText_[k]->SetText(String("Pos: ") + String(pos) + String(", Rot: ") + String(rot));

                    k++;
                    debugText_[k]->SetAlignment(HA_LEFT, VA_TOP);
                    debugText_[k]->SetPosition(10.0f, vAlign + (k * 12));
                    debugText_[k]->SetVisible(true);
                    debugText_[k]->SetText(String("wheelsContactNum -> ") + String(wheelsContactNum));

                   k++;
                   debugText_[k]->SetAlignment(HA_LEFT, VA_TOP);
                   debugText_[k]->SetPosition(10.0f, vAlign + (k * 12));
                   debugText_[k]->SetVisible(true);
                   debugText_[k]->SetText(String("noWheelContactTime: ") + String(noWheelContactTime));

                   k++;
                debugText_[k]->SetAlignment(HA_LEFT, VA_TOP);
                debugText_[k]->SetPosition(10.0f, vAlign + (k * 12));
                debugText_[k]->SetVisible(true);
                debugText_[k]->SetText(String("pickupItemState -> ") + String(pickupItemState));


            }

        }

            // Show sums

            // Genetic Algorithm statistics text
            int s = 0;
            genAlgStatsText_[s]->SetAlignment(HA_LEFT, VA_TOP);
            genAlgStatsText_[s]->SetPosition(10.0f, 10 + (s * 12));
            genAlgStatsText_[s]->SetVisible(true);
            genAlgStatsText_[s]->SetText(
                    String("Generation: ") + String(EvolutionManager::getInstance()->getGeneticAlgorithm()->generationCount_) + String(", ") + String("Population: ") + String(EvolutionManager::getInstance()->agentsAliveCount) + String(" out of ") + String(EvolutionManager::getInstance()->getGeneticAlgorithm()->populationSize_));

            s++;
            genAlgStatsText_[s]->SetAlignment(HA_LEFT, VA_TOP);
            genAlgStatsText_[s]->SetPosition(10.0f, 10 + (s * 12));
            genAlgStatsText_[s]->SetVisible(true);
            genAlgStatsText_[s]->SetText(
            String("Genotype Param Count: ") + String(EvolutionManager::getInstance()->getGeneticAlgorithm()->genotypeParamCount_));

            // Get best agents
            float maxFitness = 0.0f;
            int bestFitIdx = -1;
            float maxEval = 0.0f;
            int bestEvalIdx = -1;

        for (int i = 0; i < EvolutionManager::getInstance()->getAgentControllers().size(); i++) {
                float fitness = EvolutionManager::getInstance()->getAgents()[i]->genotype->fitness;
                float eval = EvolutionManager::getInstance()->getAgents()[i]->genotype->evaluation;

                if (fitness > maxFitness) {
                    maxFitness = fitness;
                    bestFitIdx = i;
                }

                if (eval > maxEval) {
                    maxEval = eval;
                    bestEvalIdx = i;
                }

        } // Calculated sums

        // This is not really necessary -> the sorted previous population by fitness will by in order of highest to low.
        float prevPopMaxEval = 0.0f;
        float prevPopMaxFitness = 0.0f;

        int prevBestEvalIdx = 0;
        int prevBestFitIdx = 0;
        for (int i = 0; i < EvolutionManager::getInstance()->getGeneticAlgorithm()->getPrevPopulation().size(); i++) {
            float eval = EvolutionManager::getInstance()->getGeneticAlgorithm()->getPrevPopulation()[i]->evaluation;
            float fitness = EvolutionManager::getInstance()->getGeneticAlgorithm()->getPrevPopulation()[i]->fitness;

            if (fitness > prevPopMaxFitness) {
                prevPopMaxFitness = fitness;
                prevBestFitIdx = i;
            }

            if (eval > prevPopMaxEval) {
                prevPopMaxEval = eval;
                prevBestEvalIdx = i;
            }
        }



            s++;
            genAlgStatsText_[s]->SetAlignment(HA_LEFT, VA_TOP);
            genAlgStatsText_[s]->SetPosition(10.0f, 10 + (s * 12));
            genAlgStatsText_[s]->SetVisible(true);
            genAlgStatsText_[s]->SetText(String("Best Evaluation (Current): Agent -> ") + String(bestEvalIdx) + String(", Evaluation -> ") + String(maxEval));

            s++;
            genAlgStatsText_[s]->SetAlignment(HA_LEFT, VA_TOP);
            genAlgStatsText_[s]->SetPosition(10.0f, 10 + (s * 12));
            genAlgStatsText_[s]->SetVisible(true);
            genAlgStatsText_[s]->SetText(String("Previous Population (1st best): Agent -> ") + String(prevBestFitIdx) + String(", Fitness -> ") + String(prevPopMaxFitness));

            s++;
            genAlgStatsText_[s]->SetAlignment(HA_LEFT, VA_TOP);
            genAlgStatsText_[s]->SetPosition(10.0f, 10 + (s * 12));
            genAlgStatsText_[s]->SetVisible(true);
            genAlgStatsText_[s]->SetText(String("Previous Population (2nd best): Agent -> ") + String(1) + String(", Fitness -> ") + String(EvolutionManager::getInstance()->getGeneticAlgorithm()->getPrevPopulation()[1]->fitness));

/*            s++;
            genAlgStatsText_[s]->SetAlignment(HA_LEFT, VA_TOP);
            genAlgStatsText_[s]->SetPosition(10.0f, 10 + (s * 12));
            genAlgStatsText_[s]->SetVisible(true);
            genAlgStatsText_[s]->SetText(String("Previous Population (3rd best): Agent -> ") + String(2) + String(", Fitness -> ") + String(EvolutionManager::getInstance()->getGeneticAlgorithm()->getPrevPopulation()[2]->fitness));
*/
            s++;
            genAlgStatsText_[s]->SetAlignment(HA_LEFT, VA_TOP);
            genAlgStatsText_[s]->SetPosition(10.0f, 10 + (s * 12));
            genAlgStatsText_[s]->SetVisible(true);
            genAlgStatsText_[s]->SetText(String("Previous Population: Agent -> ") + String(prevBestEvalIdx) + String(", Evaluation -> ") + String(prevPopMaxEval));

    } else {
        loginList = loginList_;
    }


    // On client
    if (!isServer_) {

        int k = 0;
        if (!radioTrackQueue_.Empty()) {

            // Add this near the end of the method where audio is processed
            UpdateVUMeterSmooth(timeStep);


            if (radioText_[k]) {
                RadioTrack currTrack = radioTrackInfoQueue_[radioTrack_];
                radioText_[k]->SetText(String("Artist: ") + String(currTrack.artistName));
                k++;

                radioText_[k]->SetText(String("") + String(currTrack.artistLink));
                k++;

                radioText_[k]->SetText(String("") + String(currTrack.trackName));
                k++;

                radioText_[k]->SetText(String(" ") + String(currTrack.artistName));
                k++;

                //radioText_[k]->SetText(String(" -------------------------------- "));

                // SoundMixRate (int) %Sound output frequency in Hz. Default 44100.
                if (_radioStream) {
                    float t = _radioStream->GetTimePosition();
                    float defaultMixRate = 44100;
                    // Next track
                    if (t == 0) {
                        if (!_radioStream->GetSound()) {
                            // No stream data
                            // Load next track
                            PlayNextTrack();

                        }
                    }

                    // This stays the same
                    float totalSamples = 0;
                    if (_radioStream->GetSound()) {
                        _radioStream->GetSound()->GetSampleSize() * _radioStream->GetSound()->GetFrequency();
                    }


                    //float totalSamples = _radioStream->GetSound()->GetSampleSize() * _radioStream->GetSound()->GetFrequency();
                    //_radioStream->GetPlayPosition()-_radioStream->GetSound()->GetStart();

                    //float t = (float) (int) (size_t) (_radioStream->GetPlayPosition()-_radioStream->GetSound()->GetStart())) /
//                                    (sound_->GetSampleSize() * sound_->GetFrequency())

                    // Update the time position. In stream mode, copy unused data back to the beginning of the stream buffer

                    //timePosition_ += ((float)samples / (float)mixRate) * frequency_ / soundStream_->GetFrequency();

                    //timePosition_ = ((float) (int) (size_t) (position_ - sound_->GetStart())) /
//                                    (sound_->GetSampleSize() * sound_->GetFrequency());

                    String fm, fs;
                    int m = int(_radioStream->GetTimePosition() / 60.0f);
                    if (m < 10) {
                        fm = "0";
                    }
                    int s = int(fmod(_radioStream->GetTimePosition(), 60.0f));
                    if (s < 10) {
                        fs = "0";
                    }

                    String progressBar = "[";
                    String playTime = " " + String(fm) + String(m) + ":" + String(fs) + String(s) + " ";
                    if (_radioStream->GetSound()) {
                        int ind = (t * totalSamples) / _radioStream->GetSound()->GetDataSize();

                        for (int i = 0; i < 11; i++) {
                            if (ind == i) {
                                progressBar += "|";
                            } else {
                                progressBar += "-";
                            }
                        }
                    }
                    progressBar += "]";

                    radioText_[k]->SetText(progressBar + playTime);
                    k++;
                } else {
                    radioText_[k]->SetText(String("Waiting for next track..."));
                }
            }

            /* NEXT QUEUE
            RadioTrack nextTrack = radioTrackInfoQueue_[nextRadioTrack_];
            */
            ProcessAudioStream();
        }
        else
            if (radioText_[k])
                radioText_[k]->SetText(String("Waiting for next track..."));
    }

    float deltaSum;


    // Toggle debug geometry with 'Z' key
    if (input->GetKeyPress(KEY_Z))
        drawDebug_ = !drawDebug_;

    // Toggle through focus objects
    if (input->GetKeyPress(KEY_T)) {

        //1448.78039550781,0,1507.31701660156
        //Vector3 trackMarkerPos = Vector3(724.390197753906,0,753.658508300781);
        // Place on at focus object
//        vehicle_->GetNode()->SetPosition(Vector3(focusObjects_[focusIndex_].x_, 500.0f, focusObjects_[focusIndex_].z_));

        // Increment focus object
        focusIndex_++;
    }


    // Toggle through radio tracks
    if (input->GetKeyPress(KEY_C)) {
        nextRadioTrack_++;
        if (!radioTrackInfoQueue_.Empty()) {
            // Clamp to track limit
            nextRadioTrack_ = nextRadioTrack_ % radioTrackInfoQueue_.Size();
        }
    }


    // Play track on radio
    if (input->GetKeyPress(KEY_V)) {
        //_radioStream->GetSound()->ReleaseRef();
        // Wait for current try to finish
        if (!_radioStream->GetSound()) {
            PlayNextTrack();
        }
    }

    if (input->GetKeyPress(KEY_K)) {
        if (camMode_ > 0) {
            // Switch the steer index
            int aIndex = camMode_ - 1;
            if (EvolutionManager::getInstance()->getNetworkActors()[aIndex]) {
                if (EvolutionManager::getInstance()->getNetworkActors()[aIndex]->vehicle_) {

                    if ((EvolutionManager::getInstance()->getNetworkActors()[aIndex]->vehicle_->getSteerIndex() - 1) < 0) {

                        EvolutionManager::getInstance()->getNetworkActors()[aIndex]->vehicle_->setSteerIndex(splineSize_);

                    } else {

                        EvolutionManager::getInstance()->getNetworkActors()[aIndex]->vehicle_->setSteerIndex(
                                (EvolutionManager::getInstance()->getNetworkActors()[aIndex]->vehicle_->getSteerIndex() -
                                 1) % splineSize_);
                        EvolutionManager::getInstance()->getNetworkActors()[aIndex]->setSteerSet(false);
                    }

                }
            }
        }
    }

    if (input->GetKeyPress(KEY_L)) {
        if (camMode_ > 0) {
            // Switch the steer index
            int aIndex = camMode_ - 1;
            if (EvolutionManager::getInstance()->getNetworkActors()[aIndex]) {
                if (EvolutionManager::getInstance()->getNetworkActors()[aIndex]->vehicle_) {
                    EvolutionManager::getInstance()->getNetworkActors()[aIndex]->vehicle_->setSteerIndex(
                            (EvolutionManager::getInstance()->getNetworkActors()[aIndex]->vehicle_->getSteerIndex() + 1)  % splineSize_);
                    EvolutionManager::getInstance()->getNetworkActors()[aIndex]->setSteerSet(false);
                }
            }
        }
    }


    if (input->GetKeyPress(KEY_N)) {
        camMode_ = camMode_ - 1;
    }

    if (input->GetKeyPress(KEY_M)) {
        camMode_ = camMode_ + 1;
    }

    if (input->GetKeyPress(KEY_F)) {
        if (isServer_) {
            if (camMode_ > 0) {
                // Flip the vehicle
                int aIndex = camMode_-1;
                if (EvolutionManager::getInstance()->getNetworkActors()[aIndex]) {
                    if (EvolutionManager::getInstance()->getNetworkActors()[aIndex]->vehicle_) {
                        EvolutionManager::getInstance()->getNetworkActors()[aIndex]->vehicle_->Flip(timeStep);
                    }
                }
            }
        }
    }

    if (input->GetKeyPress(KEY_J)) {
        // Increment focus object
        camSpin2_ = camSpin2_ - 10.0f;
    }

    if (input->GetKeyPress(KEY_K)) {
        // Increment focus object
        camSpin2_ = camSpin2_ + 10.0f;
    }


    if (!focusObjects_.Empty()) {
        // Clamp focus index
        focusIndex_ = focusIndex_ % focusObjects_.Size();
    }



    if (input->GetKeyPress(KEY_F7))
        ReloadScene(false);


    ui = GetSubsystem<UI>();

    // Toggle physics debug geometry with space
    if (input->GetKeyPress(KEY_F5)) {
//        drawDebug_ = !drawDebug_;
        SaveScene(false);
    }


    // Check for loading / saving the scene
    if (input->GetKeyPress(KEY_K)) {
        File saveFile(context_, GetSubsystem<FileSystem>()->GetProgramDir() + "Data/Scenes/AlgebraKart_Output.xml",
                      FILE_WRITE);
        scene_->SaveXML(saveFile);
    }

    // Update replay system
    if (replaySystem_ && replaySystemEnabled_) {
        replaySystem_->Update(timeStep);

        // Update replay UI if playing
        if (replaySystem_->IsPlayingReplay()) {
            // Handle replay UI updates here if needed
            replayUI_->SetVisible(true);
        } else {
            replayUI_->SetVisible(false);
        }
    }

    // Update audio UI system
    UpdateAudioUISystem(timeStep);

    // Call our render update
    HandleRenderUpdate(eventType, eventData);

    // Memory management calls
    MonitorMemoryUsage(timeStep);
    ManageResourceCache(timeStep);


    ///
    // Update missile manager
    if (globalMissileManager_) {
        float timeStep = eventData[Update::P_TIMESTEP].GetFloat();
        globalMissileManager_->Update(timeStep);
    }

}

void AlgebraKart::ProcessAudioStream() {
    MutexLock lock(audioMutex_);
    if (!audioProcessingEnabled_ || !_radioStream->GetSound()->GetData()) return;

    SharedArrayPtr stream = SharedArrayPtr(_radioStream->GetSound()->GetData());
    int streamSize = _radioStream->GetSound()->GetDataSize();
    capturer_->getBuffer()->Resize(streamSize);
    capturer_->data->readCount = 0;
    float t = _radioStream->GetTimePosition();

    for (size_t i = 0; i < streamSize; i++) {

        // bug: streamSize is longer than available?
        if (stream.Get()[(int) i]) {
            float v = stream.Get()[(int) i] / 128.0f;
            capturer_->getBuffer()->At(i) = v;
            capturer_->data->readCount++;
            //URHO3D_LOGDEBUGF("readCount=%d,v=%f",capturer_->data->readCount, v);
        }
    }

    //URHO3D_LOGDEBUGF("readCount=%d",capturer_->data->readCount);

    if (audioSpectrumOptions_.inputSize < capturer_->bufferReadCount()) {
        // Update analyzer
        analyzer_->update(capturer_->getBuffer(), capturer_->bufferHeadIndex(),
                          audioSpectrumOptions_.bottomLevel,
                          audioSpectrumOptions_.topLevel, audioSpectrumOptions_.minFreq,
                          audioSpectrumOptions_.maxFreq, audioSpectrumOptions_.axisLogBase);
        //String renderStr = renderer_->draw(analyzer_->spectrum(), audioSpectrumOptions_.windowSize, audioSpectrumOptions_.smoothing, true);
        //  URHO3D_LOGDEBUGF("renderStr -> %s", renderStr.CString());
    }

    // Draw wave
    /*DrawWave(Vector2(0, radioSpectrumHeight_ / 2), radioSpectrumWidth_, radioSpectrumHeight_,
             _radioStream->GetTimePosition(), Color(75.0f / 255.0f, 247.0f / 255.0f, 48.0f / 255.0f));*/
    // DrawFFT(Vector2(0, radioSpectrumHeight_ / 2), radioSpectrumWidth_, radioSpectrumHeight_, Color(247.0f/255.0f,48.0f/255.0f,148.0f/255.0f));
}

void AlgebraKart::HandlePostUpdate(StringHash eventType, VariantMap &eventData) {

    // SEND DELAYED LOAD MESSAGES
    if (ticks > 4.0f) {

        // Player spawn sent first
        if (!sendSpawnMsg_.Empty()) {

            for (int i = 0; i < sendSpawnMsg_.Size(); ++i) {
                // Send remote event to client (after delay)
                sendSpawnMsg_[i]->SendRemoteEvent(E_PLAYERSPAWNED, true); // Player spawn
            }
            for (int i = 0; i < sendSpawnMsg_.Size(); ++i) {
                sendSpawnMsg_.Erase(i);
            }

        }

        // Player loaded after spawn
        if (!sendLoadMsg_.Empty()) {
            for (int i = 0; i < sendLoadMsg_.Size(); ++i) {
                // Send remote event to client (after delay)
                sendLoadMsg_[i]->SendRemoteEvent(E_PLAYERLOADED, true);
                SendWelcomeMessage(sendLoadMsg_[i]);
            }
            for (int i = 0; i < sendLoadMsg_.Size(); ++i) {
                sendLoadMsg_.Erase(i);
            }

        }

        ticks = 0;
    }

    // Skip on loading
    if (levelLoading_) return;
    if (clientLevelLoading_) return;


    if (menuScene_) {
        if (menuScene_->GetChild("menuCam", LOCAL)) {
            // Get camera
            auto *menuCam = menuScene_->GetChild("menuCam", LOCAL)->GetComponent<Camera>();

            if (menuCam) {
                using namespace Update;
                // Take the frame time step, which is stored as a float
                float timeStep = eventData[P_TIMESTEP].GetFloat();

                Vector3 pos = menuCam->GetNode()->GetPosition();
                if (pos.x_ > -6.0f) {
                    menuCam->GetNode()->SetPosition(
                            Vector3(menuCam->GetNode()->GetPosition().x_ - 0.0001f * timeStep,
                                    menuCam->GetNode()->GetPosition().y_,
                                    menuCam->GetNode()->GetPosition().z_));
                }


                auto* progressBar = menuScene_->GetChild("ProgressBar",LOCAL)->GetComponent<StaticModel>();
                progressBar->GetNode()->SetScale(Vector3(1,1,loadProgress_*200.0f));
            }
        }

    }


    if (started_) {

        loadProgress_ = Random(0.0f, 1.0f);

        if (networkInfo_) {
            if (networkCounterTimer_.GetMSec(false) > 1000 && GetSubsystem<Network>()->GetServerConnection()) {
                float ping = GetSubsystem<Network>()->GetServerConnection()->GetRoundTripTime();
                networkInfo_->SetText(String("Ping: ") + String(ping) + String(" ms"));
                if ((ping > 0) && (ping < 100)) {
                    networkInfo_->SetColor(Color::WHITE);
                } else if ((ping >= 100) && (ping < 200)) {
                    networkInfo_->SetColor(Color::YELLOW);
                } else if ((ping >= 200) && (ping < 300)) {
                    networkInfo_->SetColor(Color::RED);
                }
                networkCounterTimer_.Reset();
            }
        }

        if (isServer_) {
            // SERVER
            Server *server = GetSubsystem<Server>();

            Vector<Vector3> camPosList;

            // CLIENT CONNECTIONS UPDATE

            // Update actor marker positions
            if (GetSubsystem<Network>()->GetClientConnections().Size()) {
                auto connections = GetSubsystem<Network>()->GetClientConnections();
                for (auto it = connections.Begin(); it != connections.End(); ++it) {

                    Connection *connection = (*it);

                    auto *actor = dynamic_cast<NetworkActor *>(actorMap_[connection].Get());
                            if (actor) {

                                using namespace Update;
                                // Take the frame time step, which is stored as a float
                                float timeStep = eventData[P_TIMESTEP].GetFloat();
                                float controlYawAngle = actor->controls_.yaw_ - 90.0f;
                                float slowdown = 0.07f;

                                float accelLevel = actor->GetControls().extraData_["accelLevel"].GetFloat();
                                unsigned int buttons = actor->GetControls().buttons_;
                                URHO3D_LOGDEBUGF("actor %d, yaw -> %f, buttons -> %d", actor->GetID(), actor->GetControls().yaw_, buttons);

                                // Increment timers
                                actor->lastWaypoint_ += timeStep;

                                // Update this angle from global axis (joystick) to local (actor)
                                Quaternion actorRot;
                                Vector3 actorPos;

                                // Smooth step
                                //const float rotLerpRate = 10.0f;
                                const float rotLerpRate = 0.08f;

                                // Physics update has completed. Position camera behind vehicle

                                // Body orientation
                                actorPos = actor->GetBody()->GetPosition();
                                actorRot = actor->GetBody()->GetRotation();
                                Quaternion dir(actorRot.YawAngle(), Vector3::UP);

                                // Control rotation
                                dir = dir * Quaternion(controlYawAngle*slowdown, Vector3::UP);

                                // Apply down force
                                if (!actor->entered_) {
                                    // Position dir around actor
                                    dir = SmoothStepAngle(dir, actorRot, timeStep * rotLerpRate);

                                    // On foot controls
                                    float z = controlYawAngle;

                                    // Apply rotation
                                    //actor->GetBody()->GetNode()->SetRotation(dir);
                                    // Align model and apply movement to body
                                    actor->ApplyMovement(timeStep);
                                    // Apply vertical force
                                    actor->ApplyThrust(timeStep);
                                }


                                        // Immediate update


                                        // Add to server cam calculation
                                        // camPos += startPos;
                                    }

//                    }
                } // End of connections loop

                // Get average position
                //camPos = camPos / GetSubsystem<Network>()->GetClientConnections().Size();
            }

            // AI BOT UPDATE

            // Update AI bots
            for (int i = 0; i < EvolutionManager::getInstance()->getAgents().size(); i++) {
                if (aiActorMap_[i]) {

                        auto *actor = dynamic_cast<NetworkActor *>(aiActorMap_[i].Get());
                        if (actor) {

                            using namespace Update;
                            // Take the frame time step, which is stored as a float
                            float timeStep = eventData[P_TIMESTEP].GetFloat();
                            //Vector3 camPos = actor->GetNode()->GetPosition();
                            Vector3 camPos = actor->GetBody()->GetPosition();

                            //Vector3 camPos = actor->GetNode()->GetPosition();
                            // Store ai actor vehicle position for server cam
                            camPosList.Push(camPos);

                            // Increment timers
                            actor->lastWaypoint_ += timeStep;

                            if (!actor->killed_) {
                                if (actor->vehicle_) {
                                    Node * node = actor->vehicle_->GetNode();

                                    // Apply time increment
                                    actor->totalTime_ += timeStep;
                                    actor->lapTime_ += timeStep;

                                    Vector3 center = Vector3(0,0,0);
                                    if (!steerMarks_.Empty()) {
                                        if (steerMarks_.Size() >= actor->vehicle_->getSteerIndex()) {
                                            center = steerMarks_[actor->vehicle_->getSteerIndex()]->GetWorldPosition();
                                        }
                                    }
                                    actor->SetTarget(center);
                                    actor->setSteerSet(true);

                                    // Apply auto steering
                                    if (actor->isSteerSet()) {
                                        // Towards steer waypoint
                                        Vector3 towardsWaypt = (
                                                actor->vehicle_->GetRaycastVehicle()->GetNode()->GetPosition() -
                                                actor->getToTarget()).Normalized();
                                        float distToWypt = (
                                                actor->vehicle_->GetRaycastVehicle()->GetNode()->GetPosition() -
                                                actor->getToTarget()).Length();
                                        //URHO3D_LOGDEBUGF("getSteerIndex()=%d,distToWypt=%f, actor->lastWaypoint_=%f", actor->vehicle_->getSteerIndex(), distToWypt, actor->lastWaypoint_);
                                        if (distToWypt > 1000.0f) {
                                            //actor->Restart();
                                            if (!actor->onGround_) {
                                                actor->Respawn();
                                            }
                                        }

                                        // Once vehicle hits waypoint, move to next waypoint after 1.5 secs
                                        if (distToWypt < 120.0f && actor->lastWaypoint_ > 1.5f) {
                                            // Check for last lap
                                            int nextWaypt = (actor->vehicle_->getSteerIndex() > 0) ? (
                                                    actor->vehicle_->getSteerIndex() - 1) : splineSize_ - 1;
                                            if (nextWaypt == 0) {
                                                // FINISH LAP
                                                actor->lapNum_++;
                                                // Store lap time
                                                actor->lapTimes_.Push(actor->lapTime_);
                                                actor->lapTime_ = 0;
                                            }
                                            actor->vehicle_->setSteerIndex((actor->vehicle_->getSteerIndex() > 0) ? (
                                                    actor->vehicle_->getSteerIndex() - 1) : splineSize_ - 1);
                                            actor->lastWaypoint_ = 0;
                                        }
                                        Vector3 vDir = (actor->vehicle_->GetRaycastVehicle()->GetNode()->GetPosition() +
                                                        actor->vehicle_->GetRaycastVehicle()->GetNode()->GetRotation() *
                                                        Vector3::RIGHT * 1.0f) -
                                                       actor->vehicle_->GetRaycastVehicle()->GetNode()->GetPosition();
                                        vDir = vDir.Normalized();
                                        float steerProj = towardsWaypt.DotProduct(vDir);
                                        float dist = (towardsWaypt - actor->GetPosition()).Length();
                                        float speed = dist / 0.3f;
                                        // steer projection represents delta from 0 steer
                                        //  steerProj = 1.0f; // Right
                                        //  steerProj = -1.0f; // Left
                                        float desiredSteer;
                                        Vector3 v1 = (actor->vehicle_->GetRaycastVehicle()->GetNode()->GetPosition() +
                                                      actor->vehicle_->GetRaycastVehicle()->GetNode()->GetRotation() *
                                                      Vector3::FORWARD * 1.0f) -
                                                     actor->vehicle_->GetRaycastVehicle()->GetNode()->GetPosition();
                                        Vector3 v2 = (actor->vehicle_->GetRaycastVehicle()->GetNode()->GetPosition() +
                                                      actor->vehicle_->GetRaycastVehicle()->GetNode()->GetRotation() *
                                                      Vector3::RIGHT * 1.0f * actor->vehicle_->getDesiredSteer()) -
                                                     actor->vehicle_->GetRaycastVehicle()->GetNode()->GetPosition();

                                        // v1 and v2 are perpendicular, the dot product is always zero
//                                                    float v12proj = v1.DotProduct(v2);

                                        // Amplify the steering projection in inverse
                                        desiredSteer = -steerProj * 360.0f;
                                        // Apply vehicle steering
                                        actor->vehicle_->setDesiredSteer(desiredSteer);

                                        DebugRenderer *dbgRenderer = scene_->GetComponent<DebugRenderer>(REPLICATED);

                                          //dbgRenderer->AddLine(actor->vehicle_->GetRaycastVehicle()->GetNode()->GetPosition(), actor->vehicle_->GetRaycastVehicle()->GetNode()->GetPosition()-towardsWaypt*15.0f,
                                            //                   Color(1.0f, 0.0, 0.0));


                                        dbgRenderer->AddLine(actor->vehicle_->GetRaycastVehicle()->GetNode()->GetPosition(), actor->getToTarget(),
                                                             Color(1.0f, 0.0, 1.0));



                                    }
                                    //if (!node) return;

                                    //  Vector3 camPos = actor->vehicle_->GetNode()->GetPosition();
                                    //Vector3 camPos = actor->GetNode()->GetPosition();
                                    // Store ai actor vehicle position for server cam
                                    //    camPosList.Push(camPos);
                                }
                            }
                        }

                }
            }

            int numCamModes = 1 + aiActorMap_.Size(); // One cam for each ai actor
            if (numCamModes > 0) {
                camMode_ = camMode_ % numCamModes;
                if (camMode_ < 0) camMode_ = numCamModes - 1;
                switch (camMode_) {
                    case 0: {

                        // SERVER: LAST CLIENT JOINED PLAYER CAM

                        // camMode = 0 (last joined player cam)

                        if (lastConnection_) {
                            // Camera Positioning
                            // Raycast camera collision
                            if (actorMap_[lastConnection_]) {
                                auto *actor = dynamic_cast<NetworkActor *>(actorMap_[lastConnection_].Get());
                                if (actor) {

                                    String username = actor->GetUserName();
                                    float botSpeedKm = 0;
                                    Quaternion forward;

                                        float velMult = 8.0f;

                                        Vector3 cameraTargetPos;
                                        if (actor->onVehicle_ && actor->entered_) {

                                            if (actor->vehicle_) {
                                                botSpeedKm = round(
                                                        abs(actor->vehicle_->GetRaycastVehicle()->GetSpeedKm()));
                                                // Back wheel points forward
                                                forward = actor->vehicle_->GetNode()->GetRotation();
                                            }

                                            //float bodyVel = EvolutionManager::getInstance()->getAgents()[camMode_ -1]->getActor()->vehicle_->GetBody()->GetLinearVelocity().Length();
                                            botSpeedKm = Clamp(botSpeedKm, 1.0f, 2000.0f);

                                            // On vehicle

                                            // Zoom up on body velocity increase
                                            cameraTargetPos =
                                                    actor->vehicle_->GetBody()->GetPosition() + forward *
                                                                                                Vector3(actor->vehicle_->GetBody()->GetLinearVelocity().Length() *
                                                                                                        velMult,
                                                                                                        (actor->vehicle_->GetNode()->GetPosition().y_ +
                                                                                                         CAMERA_RAY_DISTANCE_LIMIT +
                                                                                                         12.0f) +
                                                                                                        (botSpeedKm *
                                                                                                         2.7),
                                                                                                        actor->vehicle_->GetBody()->GetLinearVelocity().Length() *
                                                                                                        velMult) * 0.9f;
                                        } else {
                                            // On foot
                                            botSpeedKm = 0;
                                            // Back wheel points forward
                                            forward = actor->GetNode()->GetRotation();

                                            // Zoom up on body velocity increase
                                            cameraTargetPos =
                                                    actor->GetBody()->GetPosition() + forward *
                                                                                                Vector3(actor->GetBody()->GetLinearVelocity().Length() *
                                                                                                        velMult * 0.07,
                                                                                                        CAMERA_RAY_DISTANCE_LIMIT/9,
                                                                                                        actor->GetBody()->GetLinearVelocity().Length() *
                                                                                                        velMult) * 0.06f;

                                        }
                                        Vector3 cameraStartPos = serverCam_->GetNode()->GetPosition();

                                        // Camera ray cast limiter
                                        if (lastCamRaycast > CAM_RAYCAST_TIME_WAIT) {

                                            // Raycast camera against static objects (physics collision mask 2)
                                            // and move it closer to the vehicle if something in between
                                            Ray cameraRay(cameraStartPos, cameraTargetPos - cameraStartPos);
                                            float cameraRayLength = (cameraTargetPos - cameraStartPos).Length();
                                            PhysicsRaycastResult result;

                                            bool stopMove = false;
                                            // Adjust camera up to ray length
                                            if (cameraRayLength < CAMERA_RAY_DISTANCE_LIMIT) {
                                            }

                                            if (cameraRayLength < 10.0f) {
                                                scene_->GetComponent<PhysicsWorld>()->RaycastSingle(result, cameraRay,
                                                                                                    cameraRayLength,
                                                                                                    NETWORKACTOR_COL_LAYER);
                                            } else {
                                                scene_->GetComponent<PhysicsWorld>()->RaycastSingleSegmented(result,
                                                                                                             cameraRay,
                                                                                                             cameraRayLength,
                                                                                                             NETWORKACTOR_COL_LAYER);
                                            }
                                            if (result.body_)
                                                cameraTargetPos =
                                                        cameraStartPos +
                                                        cameraRay.direction_ * (result.distance_ - 0.5f);

                                            // Reset timer for recent ray cast
                                            lastCamRaycast = 0;

                                            Vector3 lookAtObject;

                                            if (actor->onVehicle_ && actor->entered_) {
                                                lookAtObject = actor->vehicle_->GetBody()->GetPosition();
                                            } else {
                                                lookAtObject = actor->GetBody()->GetPosition();
                                            }

                                            // Set camera position and orientation
                                            float w1;
                                            float w2;

                                            if (stopMove) {
                                                w1 = 1.0f; // hold position
                                                w2 = 0.0f;
                                            } else {
                                                w1 = 0.97;
                                                w2 = 0.03;
                                            }

                                            Vector3 weightedSum =
                                                    serverCam_->GetNode()->GetPosition() * w1 + cameraTargetPos * w2;

                                            // Calculate camera distance
                                            //serverCam_->GetNode()->SetPosition(weightedSum);
                                            serverCam_->GetNode()->LookAt(lookAtObject);
                                        }
                                    }
                                }
                            } else {
                                // Heli cam
                                serverCam_->GetNode()->SetRotation(Quaternion(90.0f, 0.0f, 90.0f));

                                Vector3 avgMid = Vector3(0, 0, 0);
                                Vector3 avgSum = Vector3(0, 0, 0);

//                                for (int b = 0; b < aiActorMap_.Size(); b++) {
                                    int b = Random(0, aiActorMap_.Size());
                                    b = 0;
                                    // Get ai cluster position
                                    if (aiActorMap_[b]) {
                                        auto *actor = dynamic_cast<NetworkActor *>(aiActorMap_[b].Get());
                                        if (actor) {
                                            if (actor->vehicle_) {
                                                String username = actor->GetUserName();
                                                float botSpeedKm = round(
                                                        abs(actor->vehicle_->GetRaycastVehicle()->GetSpeedKm()));
                                                // Back wheel points forward
                                                Quaternion forward = actor->vehicle_->GetNode()->GetRotation();

                                                avgSum += actor->vehicle_->GetBody()->GetPosition();
                                                heliCamView_ = actor->vehicle_->GetBody()->GetPosition() + (Vector3(0,380.0f,0));

                                            }
                                        }
                                    }
  //                              }
//                                avgMid = avgSum / aiActorMap_.Size() + (Vector3(0,380.0f,0));

                                ///
                                ///
                                ///
                                ///



///
///
//////


                                serverCam_->GetNode()->SetPosition(heliCamView_);

                            }
                } // End of Server: Move Camera (Player Cam)
                        break;
                    case 1 ... 1000: {
                        // SERVER VIEW MODE
                        // Server: Move Camera

                        // Camera ray cast limiter
                        if (lastCamRaycast > CAM_RAYCAST_TIME_WAIT) {

                            // AI bot cam

                            if (!camPosList.Empty()) {

                                // Raycast camera collision
                                if (aiActorMap_[camMode_ - 1]) {
                                    auto *actor = dynamic_cast<NetworkActor *>(aiActorMap_[camMode_ - 1].Get());
                                    if (actor) {
                                        if (actor->vehicle_) {
                                            String username = actor->GetUserName();
                                            float botSpeedKm = round(
                                                    abs(actor->vehicle_->GetRaycastVehicle()->GetSpeedKm()));
                                            // Back wheel points forward
                                            Quaternion forward = actor->vehicle_->GetNode()->GetRotation();


                                            //float bodyVel = EvolutionManager::getInstance()->getAgents()[camMode_ -1]->getActor()->vehicle_->GetBody()->GetLinearVelocity().Length();
                                            botSpeedKm = Clamp(botSpeedKm, 1.0f, 2000.0f);

                                            /// Update spline path
                                            SplinePath *steerSplinePath = scene_->GetComponent<SplinePath>(true);
                                            if (steerSplinePath) {
                                                // Found steer spline

                                                using namespace Update;
                                                // Take the frame time step, which is stored as a float
                                                float timeStep = eventData[P_TIMESTEP].GetFloat();
                                                // Loop back to start
                                                if (steerSplinePath->IsFinished()) { steerSplinePath->Reset(); }


                                                if (!steerSplinePath->GetControlledNode())
                                                    steerSplinePath->SetControlledNode(splineModel_->GetNode());


                                                DebugRenderer *dbgRenderer = scene_->GetComponent<DebugRenderer>(true);



                                                if (actor->isSteerSet()) {
                                                   /* actor->lastWaypoint_ += timeStep;

                                                    // Towards steer waypoint
                                                    Vector3 towardsWaypt = (actor->vehicle_->GetRaycastVehicle()->GetNode()->GetPosition() -
                                                                            actor->getToTarget()).Normalized();
                                                    float distToWypt = (actor->vehicle_->GetRaycastVehicle()->GetNode()->GetPosition() -
                                                                        actor->getToTarget()).Length();
                                                    URHO3D_LOGDEBUGF("distToWypt=%f, actor->lastWaypoint_=%f", distToWypt, actor->lastWaypoint_);

                                                    // Once vehicle hits waypoint, move to next waypoint after 5 secs
                                                    if (distToWypt < 120.0f && actor->lastWaypoint_ > 5.0f) {
                                                        actor->vehicle_->setSteerIndex(actor->vehicle_->getSteerIndex()+1 % splineSize_);
                                                        actor->lastWaypoint_ = 0;
                                                    }
                                                    Vector3 vDir = (actor->vehicle_->GetRaycastVehicle()->GetNode()->GetPosition()+actor->vehicle_->GetRaycastVehicle()->GetNode()->GetRotation() * Vector3::RIGHT*1.0f) - actor->vehicle_->GetRaycastVehicle()->GetNode()->GetPosition();
                                                    vDir = vDir.Normalized();
                                                    float steerProj = towardsWaypt.DotProduct(vDir);
                                                    // steer projection represents delta from 0 steer
                                                  //  steerProj = 1.0f; // Right
                                                  //  steerProj = -1.0f; // Left
                                                   float desiredSteer;
                                                    Vector3 v1 = (actor->vehicle_->GetRaycastVehicle()->GetNode()->GetPosition()+actor->vehicle_->GetRaycastVehicle()->GetNode()->GetRotation() * Vector3::FORWARD*1.0f)-actor->vehicle_->GetRaycastVehicle()->GetNode()->GetPosition();
                                                    Vector3 v2 = (actor->vehicle_->GetRaycastVehicle()->GetNode()->GetPosition()+actor->vehicle_->GetRaycastVehicle()->GetNode()->GetRotation() * Vector3::RIGHT*1.0f*actor->vehicle_->getDesiredSteer())-actor->vehicle_->GetRaycastVehicle()->GetNode()->GetPosition();

                                                    // v1 and v2 are perpendicular, the dot product is always zero
//                                                    float v12proj = v1.DotProduct(v2);

                                                    // Amplify the steering projection in inverse
                                                    desiredSteer = -steerProj*360.0f;

                                                    // Apply vehicle steering
                                                    actor->vehicle_->setDesiredSteer(desiredSteer);
*/
                                                    //dbgRenderer->AddLine(actor->vehicle_->GetRaycastVehicle()->GetNode()->GetPosition(), actor->vehicle_->GetRaycastVehicle()->GetNode()->GetPosition()+actor->vehicle_->GetRaycastVehicle()->GetNode()->GetRotation() * Vector3::FORWARD*15.0f,
                                                    //                     Color(0.0f, 1.0, 0.0));


/*                                                    dbgRenderer->AddLine(actor->vehicle_->GetRaycastVehicle()->GetNode()->GetPosition(), actor->vehicle_->GetRaycastVehicle()->GetNode()->GetPosition()+steerProj * Vector3::FORWARD*15.0f,
                                                                         Color(1.0f, 0.0, 0.0));
*/

                                                    //dbgRenderer->AddLine(actor->vehicle_->GetRaycastVehicle()->GetNode()->GetPosition(), actor->vehicle_->GetRaycastVehicle()->GetNode()->GetPosition()+actor->vehicle_->GetRaycastVehicle()->GetNode()->GetRotation() * Vector3::RIGHT*15.0f*actor->vehicle_->getDesiredSteer(),
                                                    //                     Color(0.0f, 0.0, 1.0));

                                                 /*   dbgRenderer->AddLine(actor->vehicle_->GetRaycastVehicle()->GetBody()->GetPosition(), Quaternion(90.0f, Vector3::UP) * actor->vehicle_->GetRaycastVehicle()->GetBody()->GetPosition() * Vector3::FORWARD * 35.0,
                                                                         Color(1.0f, 1.0, 1.0));
*/

                                      /*              dbgRenderer->AddLine(actor->vehicle_->GetRaycastVehicle()->GetNode()->GetPosition(), actor->vehicle_->GetRaycastVehicle()->GetNode()->GetPosition()-towardsWaypt*15.0f,
                                                                         Color(1.0f, 0.0, 0.0));
*/

                                                }

 //                                               steerPath->DrawDebugGeometry(dbgRenderer, true);
                                                  //steerSplinePath->DrawDebugGeometry(dbgRenderer, true);

                                                if (steerSplinePath->getControlPoints().Size() > 3) {

                                                    /// for (int i = 0; i < steerSplinePath->getControlPoints().Size() - 1; i++) {
                                                    //Vector3 firstMark =
                                                    //Vector3 secondMark = steerPath->getControlPoints()[i]->GetWorldPosition() + steerPath->getControlPoints()[i]->Get
                                                    //                                            StaticModel* model;
                                                    //                                          StaticModel* model2;
//                                                model = steerPath->getControlPoints()[i]->GetComponent<StaticModel>(true);
                                                    //                                              model2 = steerPath->getControlPoints()[i+1]->GetComponent<StaticModel>(true);
                                                    //Vector3 center = actor->vehicle_->GetRaycastVehicle()->GetNode()->GetPosition();
                                                    // Vector3 center2 = steerSplinePath->getControlPoints()[0]->GetWorldPosition();


                                                    //   dbgRenderer->AddLine(center, center2, Color(1.0f, 0.0, 0.0));
                                                    //dbgRenderer->AddLine(steerPath->GetPoint(0.1), steerPath->GetPoint(0.2), Color(0.0f, 1.0, 0.0));
                                                    //dbgRenderer->AddLine(steerPath->GetPoint(0.2), steerPath->GetPoint(0.3), Color(0.0f, 0.0, 1.0));





//                                            }
                                                }
                                            }

                                            /*
                                            // Move time segment
                                            if (steerPath->GetControlledNode())
                                                steerPath->Move(timeStep);*/



                                            float velMult = 1.0f;

                                            // Zoom up on body velocity increase
/*                                    Vector3 cameraTargetPos =
                                            camPosList[camMode_ - 1] + forward*Vector3(actor->vehicle_->GetBody()->GetLinearVelocity().Length()*velMult, (actor->vehicle_->GetNode()->GetPosition().y_+CAMERA_RAY_DISTANCE_LIMIT+12.0f)+(botSpeedKm*2.7), actor->vehicle_->GetBody()->GetLinearVelocity().Length()*velMult)*0.9f;
*/
                                            Vector3 cameraTargetPos =
                                                    camPosList[camMode_ - 1] + forward * Vector3(1, 20.0f, 1) * 0.9f;
                                            //actor->vehicle_->GetBody()->GetLinearVelocity().Length()*velMult, actor->vehicle_->GetNode()->GetPosition().y_+(botSpeedKm*1.0), actor->vehicle_->GetBody()->GetLinearVelocity().Length()*velMult
                                            Vector3 cameraStartPos = serverCam_->GetNode()->GetPosition();

                                            // Raycast camera against static objects (physics collision mask 2)
                                            // and move it closer to the vehicle if something in between
                                            Ray cameraRay(cameraStartPos, cameraTargetPos - cameraStartPos);
                                            float cameraRayLength = (cameraTargetPos - cameraStartPos).Length();
                                            PhysicsRaycastResult result;

                                            bool stopMove = false;

                                            if (actor->onVehicle_ && actor->entered_) {

                                                // Adjust camera up to ray length
                                                if (cameraRayLength < CAMERA_RAY_DISTANCE_LIMIT) {
                                                    stopMove = true;
                                                }
                                            } else {
                                                // On foot
                                                // Adjust camera up to ray length
                                                if (cameraRayLength < CAMERA_RAY_DISTANCE_LIMIT/8) {
                                                    stopMove = true;
                                                }
                                            }

                                            if (cameraRayLength < 10.0f) {
                                                scene_->GetComponent<PhysicsWorld>()->RaycastSingle(result, cameraRay,
                                                                                                    cameraRayLength,
                                                                                                    NETWORKACTOR_COL_LAYER);
                                            } else {
                                                // cameraRayLength == 240.0001404 crash
                                                scene_->GetComponent<PhysicsWorld>()->RaycastSingleSegmented(result,
                                                                                                             cameraRay,
                                                                                                             cameraRayLength,
                                                                                                             NETWORKACTOR_COL_LAYER);
                                            }
                                            if (result.body_)
                                                cameraTargetPos =
                                                        cameraStartPos +
                                                        cameraRay.direction_ * (result.distance_ - 0.5f);

                                            // Reset timer for recent ray cast
                                            lastCamRaycast = 0;


                                            // Set camera position and orientation
                                            float w1;
                                            float w2;

                                            if (stopMove) {
                                                w1 = 1.0f; // hold position
                                                w2 = 0.0f;
                                            } else {
                                                w1 = 0.97;
                                                w2 = 0.03;
                                            }

                                            Vector3 weightedSum =
                                                    serverCam_->GetNode()->GetPosition() * w1 + cameraTargetPos * w2;

                                            // Calculate camera distance
                                            //serverCam_->GetNode()->SetPosition(weightedSum);
                                            //serverCam_->GetNode()->LookAt(camPosList[camMode_ - 1]);

                                        }
                                    }
                                } // End of Server: Move Camera


                                // Update per-BOT info

                                // FFN
                                int aIndex = camMode_ - 1;

                                std::shared_ptr<Genotype> genotype = EvolutionManager::getInstance()->getAgents()[aIndex]->genotype;
                                // Retrieve parameters for genotype
                                std::vector<float> parameters = genotype->getParameterCopy();

                                // Set weights to parameter values
                                for (int p = 0; p < genotype->getParameterCount(); p++) {
                                    //EvolutionManager::getInstance()->getAgents()[i]->ffn->layers[k]->weights[i][j] = parameters[p];

                                    // Server-side view
                                    Sprite *sprite = ffnBotSprite_.Find(p)->second_;
                                    int width = 16;
                                    int height = 10;

                                    float ix = (p % width) * width;
                                    float iy = (p / width) * height;
                                    // Set logo sprite size
                                    sprite->SetAlignment(HA_LEFT, VA_TOP);
                                    sprite->SetPosition(Vector2(800.0f + ix, 10.0f + iy));
                                    sprite->SetScale(0.3f);
                                    sprite->SetOpacity(0.9f);
                                    // Set a low priority so that other UI elements can be drawn on top
                                    sprite->SetPriority(-100);
                                    sprite->SetVisible(false);

                                    sprite->SetColor(Color(Vector3(parameters[p], parameters[p], parameters[p])));


                                }


                                // Show BOT control inputs
                                float steerInput = EvolutionManager::getInstance()->getAgentControllers()[aIndex]->movement->getSteerInput();
                                float accInput = EvolutionManager::getInstance()->getAgentControllers()[aIndex]->movement->getAccelInput();
                                float action = EvolutionManager::getInstance()->getAgentControllers()[aIndex]->movement->getAction();
                                float accLimit = 1.0f;

                                steerWheelBotSprite_->SetPosition(Vector2(1000, 200));
                                steerWheelBotSprite_->SetScale(0.2f);
                                steerWheelBotSprite_->SetRotation((steerInput) * 360.0f);
                                steerWheelBotSprite_->SetVisible(false);

                                velBarBotSprite_->SetPosition(Vector2(900, 100));
                                velBarBotSprite_->SetVisible(false);

                                if (accInput > 0) {
                                    IntVector2 v = velBarBkgBotSprite_->GetSize();
                                    int power = int(((accInput) / accLimit) * (float) v.x_);
                                    velBarBotSprite_->SetSize(power, v.y_);
                                    velBarBotSprite_->SetScale(0.3f);
                                } else {
                                    IntVector2 v = velBarBkgBotSprite_->GetSize();
                                    int power = int(((-accInput) / -accLimit) * (float) v.x_);
                                    velBarBotSprite_->SetSize(power, v.y_);
                                    velBarBotSprite_->SetScale(0.3f);
                                }

                                velBarBkgBotSprite_->SetPosition(Vector2(900, 100));
                                velBarBkgBotSprite_->SetVisible(false);
                                velBarBkgBotSprite_->SetScale(0.3f);

                                /*
                                // Update player powerbar
                                IntVector2 v = powerBarBkgP1Sprite_->GetSize();
                                int power = int(((life) / 100.0f) * (float) v.x_);
                                powerBarP1Sprite_->SetSize(power, v.y_);
        */
                                /*
                                 *
                                 *  // Construct FFN from genotype
        for (int k = 0; k < NUM_NEURAL_LAYERS; k++) {

            for (int i = 0; i < ffn->layers[k]->neuronCount; i++) {
                for (int j = 0; j < ffn->layers[k]->outputCount; j++) {

                    // Retrieve parameters for genotype
                    std::vector<float> parameters = genotype->getParameterCopy();

                    // Set weights to parameter values
                    for (int p = 0; p < genotype->getParameterCount(); p++) {
                        ffn->layers[k]->weights[i][j] = parameters[p];
                    }
                }
            }
        }
                                 *
                                 */

                            }
                        } // End of Raycast limiter
                    }
                 break;
                }
            }


            // Special server input
            UI *ui = GetSubsystem<UI>();
            Input *input = GetSubsystem<Input>();

            if (input->GetMouseButtonDown(MOUSEB_LEFT)) {


                // Do aggregate based settings for server
                PlaceObject(serverCam_, 0);
            }


        } else {
                // CLIENT

                Node *actorNode = nullptr;
                Node *vehicleNode = nullptr;
                Network *network = GetSubsystem<Network>();
                auto serverConnection = network->GetServerConnection();
                using namespace Update;
                float timeStep = eventData[P_TIMESTEP].GetFloat();

                // MAIN CLIENT UPDATE


                // Apply transformations to camera
                MoveCamera(timeStep);


                instructionsText_->SetVisible(true);

                for (int i = 0; i < hudTextList_.Size(); i++) {
                    hudTextList_[i]->SetVisible(true);
                }
            }

        // Update missile camera system
        if (missileCameraActive_) {
            using namespace Update;
            float timeStep = eventData[P_TIMESTEP].GetFloat();
            UpdateMissileCamera(timeStep);
        }

    }
}


void AlgebraKart::HandlePostRenderUpdate(StringHash eventType, VariantMap &eventData) {

    // Skip on loading
    if (levelLoading_) return;
    if (clientLevelLoading_) return;

}

void AlgebraKart::HandleEndRendering(StringHash eventType, VariantMap &eventData) {

    Graphics *graphics = GetSubsystem<Graphics>();
    auto *cache = GetSubsystem<ResourceCache>();

    if (levelLoading_) {
        if (scene_->GetAsyncProgress() == 1.0f) {

            // ASYNC MAP LOAD COMPLETE
            levelLoading_  = false;

        }

        int sticks = Round(scene_->GetAsyncProgress() * (float) 20);
        String progressBar = "";
        for (int i = 0; i < sticks; i++) {
            progressBar += "|";
        }
        if (!headless_)
            progressText_->SetText(progressBar);
    }

    if (!clientLevelLoading_) {
        if (!headless_) {
            progressText_->SetVisible(levelLoading_);
            progressResText_->SetVisible(levelLoading_);
        }
    } else {
        int b = 0;

    }


    int a = 0;
}

void AlgebraKart::ReloadScene(bool reInit) {
  // LOAD MAP

 auto *cache = GetSubsystem<ResourceCache>();
 //String filename = "Map1_Start_T1";
/// String filename = "Map2_Start_T1";


// LOAD BASE

 //////////
        // sub
        SubscribeToEvent(E_ASYNCLOADPROGRESS, URHO3D_HANDLER(AlgebraKart, HandleLoadProgress));
        SubscribeToEvent(E_ASYNCLEVELOADFINISHED, URHO3D_HANDLER(AlgebraKart, HandleLevelLoaded));






    levelPathName_= "Scenes/Async/";
    // load scene
    XMLFile *xmlLevel = cache->GetResource<XMLFile>("Scenes/base.xml");


    //BackgroundLoadResource -> fails
    //XMLFile *xmlLevel;
    //cache->BackgroundLoadResource<XMLFile>("Scenes/base.xml", true, xmlLevel);


    /*XMLFile *xmlLevel = cache->GetResource<XMLFile>("Scenes/" + filename + ".xml");
    //loadFile(context_, cache + "Data/Scenes/" + filename + ".xml",
     */
    if (xmlLevel) {
        scene_->LoadXML(xmlLevel->GetRoot());
    }

    /////////


   }//Map1_Start_T1.xml


   void AlgebraKart::LoadLevel(int id) {
       auto *cache = GetSubsystem<ResourceCache>();
       levelPathName_= "Scenes/Async/";

       // remove any existing level
       if (nextLevel_)
       {
           nextLevel_->Remove();
           nextLevel_ = NULL;
       }



       //String filename = "Map3_Start_T1";
       String filename = "Map" + String(id) + "_Start_T1";

       // async load
//    String levelPathFile = levelPathName_ + loadLevel + String(".xml");
       String loadLevel = filename;
       String levelPathFile = levelPathName_ + loadLevel + String(".xml");

       URHO3D_LOGINFO("cache file " + levelPathFile);
       XMLFile *xmlLevel  = cache->GetResource<XMLFile>(levelPathFile);

       if (xmlLevel) {
           SceneResolver resolver;
           Node *tmpNode = scene_->CreateTemporaryChild();

           URHO3D_LOGINFO("LoadXML loading " + levelPathFile);
           if (tmpNode->LoadXML(xmlLevel->GetRoot(), resolver, false)) {
               URHO3D_LOGINFO("Async loading " + levelPathFile);
               if (scene_->InstantiateXMLAsync(xmlLevel->GetRoot(), tmpNode->GetWorldPosition(),
                                               tmpNode->GetWorldRotation())) {
                   // prevent the trigger to reload while already loading
                   levelLoadPending_ = levelPathFile;
                   if (!headless_)
                    progressText_->SetText("");
               } else {
                   URHO3D_LOGERROR("InstantiateXMLAsync failed to init, level=" + levelPathFile);
               }
           }

       }



       // setup for async loading
       //scene_->SetAsyncLoadingMs(3);
       levelLoading_ = true;

       scene_->SetUpdateEnabled(true);
   }

   void AlgebraKart::PlaySoundEffect(const String &soundName) {

   /*
       if (sndFxPlay_[soundName]) {
           float f = sndFxPlay_[soundName];
           if (f > 5.0f) {
               sndFxPlay_.Erase(soundName);
           } else {
               // Skip play
               return;
           }
       }

       // TOOD: Hash map based on soundname -> playing
       Pair<String, float> p = Pair<String, float>(soundName, 0);
       sndFxPlay_.Insert(p);

       auto *cache = GetSubsystem<ResourceCache>();
       auto *sound = cache->GetResource<Sound>("Sounds/" + soundName);

       auto *source = scene_->CreateComponent<SoundSource3D>(LOCAL);
       source->SetNearDistance(1);  // distance up to where the volume is 100%
       source->SetFarDistance(1550);  // distance from where the volume is at 0%
       source->SetSoundType(SOUND_EFFECT);
       if (sound != nullptr) {
           source->SetAutoRemoveMode(REMOVE_COMPONENT);
           source->Play(sound);
       }*/
}

void AlgebraKart::PlayDrumMachine() {
    // CLIENT CODE

    // Play music on client


    // Get sequencers and set play source
    // Found network player
    if (isSnapped_) {

        String actorName = String("actor-") + clientName_;
        Node * actorNode = scene_->GetChild(actorName);

        String vehicleName = String("vehicle-") + clientName_;
        Node * vehicleNode = scene_->GetChild(vehicleName);

        Vector3 bodyPos;
        Quaternion rotation;

        if (actorNode) {
            // Retrieve Actor
            ClientObj *actor = actorNode->GetDerivedComponent<ClientObj>();
            if (actor->GetSequencer()) {
                // Update play source location to actor
                actor->GetSequencer()->GetPlaySource()->GetNode()->SetPosition(actor->GetPosition());
            }
        }
    }
}


void AlgebraKart::PlayMusic(const String &soundName) {
    // CLIENT CODE
    auto *cache = GetSubsystem<ResourceCache>();

    // Safety: Clean up previous stream if it exists
    if (_radioStream && _radioStream->GetNode()) {
        _radioStream->Stop();
        _radioStream = nullptr;
    }

    // Create new stream with error checking
    Node* audioNode = scene_->CreateChild("RadioStreamNode", LOCAL);
    if (!audioNode) {
        URHO3D_LOGERROR("Failed to create audio node for radio stream");
        return;
    }

    _radioStream = audioNode->CreateComponent<SoundSource3D>(LOCAL);
    if (!_radioStream) {
        URHO3D_LOGERROR("Failed to create SoundSource3D component");
        audioNode->Remove();
        return;
    }

    // Configure audio source
    _radioStream->SetNearDistance(1);
    _radioStream->SetFarDistance(6000);
    _radioStream->SetSoundType(SOUND_MUSIC);

    // Load sound with error checking
    auto *sound = cache->GetResource<Sound>(soundName);
    if (!sound) {
        URHO3D_LOGERROR("Failed to load sound: " + soundName);
        audioNode->Remove();
        _radioStream = nullptr;
        return;
    }

    // Verify sound data before playing
    if (!sound->GetData() || sound->GetDataSize() == 0) {
        URHO3D_LOGERROR("Sound has no data: " + soundName);
        audioNode->Remove();
        _radioStream = nullptr;
        return;
    }

    // Store track and play
    radioTrackQueue_.Push(_radioStream);

    _radioStream->SetAutoRemoveMode(REMOVE_COMPONENT);
    _radioStream->Play(sound);
    _radioStream->SetGain(0.8f);

    URHO3D_LOGINFO("Successfully started playing: " + soundName);
}

void AlgebraKart::CleanupAudioResources() {

    // FIX: More thorough audio cleanup
    {
        MutexLock lock(audioMutex_);

        // Stop and clean radio stream
        if (_radioStream) {
            _radioStream->Stop();
            _radioStream->Remove();
        }

        // Clean up all sound sources
        if (scene_) {
            PODVector<SoundSource3D*> soundSources;
            scene_->GetComponents<SoundSource3D>(soundSources, true);

            for (SoundSource3D* source : soundSources) {
                if (source->GetAutoRemoveMode() != AutoRemoveMode::REMOVE_NODE) {
                    source->Stop();
                    source->GetNode()->RemoveComponent<SoundSource3D>();
                }
            }
        }

        // Clear audio spectrum data
        // Clean up audio analyzer components
        if (analyzer_) {
            delete analyzer_;
            analyzer_ = nullptr;
        }

        if (capturer_) {
            delete capturer_;
            capturer_ = nullptr;
        }

        if (renderer_) {
            delete renderer_;
            renderer_ = nullptr;
        }

        // Clear all audio UI components
        beatSequencerUI_.Reset();
        synthControlsUI_.Reset();
    }

    // Clear queue
    radioTrackQueue_.Clear();

    URHO3D_LOGINFO("Audio resources cleaned up");

}

// SERVER CODE
void AlgebraKart::PlaySoundEffectGlobal(const String &soundName) {

    auto *cache = GetSubsystem<ResourceCache>();
    auto *source = scene_->CreateComponent<SoundSource3D>(REPLICATED);

    source->SetNearDistance(1);  // distance up to where the volume is 100%
    source->SetFarDistance(6000);  // distance from where the volume is at 0%
    source->SetSoundType(SOUND_EFFECT);

    auto *sound = cache->GetResource<Sound>("Sounds/" + soundName);
    if (sound != nullptr) {
        source->SetAutoRemoveMode(REMOVE_COMPONENT);
        source->Play(sound);
    }
}

// CLIENT CODE
SoundSource3D *AlgebraKart::PlaySoundEffectLocal(const String &soundName) {

    auto *cache = GetSubsystem<ResourceCache>();
    auto *source = scene_->CreateComponent<SoundSource3D>(LOCAL);

    source->SetNearDistance(1);  // distance up to where the volume is 100%
    source->SetFarDistance(6000);  // distance from where the volume is at 0%
    source->SetSoundType(SOUND_EFFECT);

    auto *sound = cache->GetResource<Sound>("Sounds/" + soundName);
    if (sound != nullptr) {
        source->SetAutoRemoveMode(REMOVE_COMPONENT);
        source->Play(sound);
        return source;
    }
}

void AlgebraKart::DoPlay() {
    Graphics *graphics = GetSubsystem<Graphics>();
    auto* renderer = GetSubsystem<Renderer>();
    ResourceCache *cache = GetSubsystem<ResourceCache>();
    UI *ui = GetSubsystem<UI>();
    UIElement *root = ui->GetRoot();


    UIElement* chatContainer_ = root->CreateChild<UIElement>();
    chatContainer_->SetFixedSize(280, 24);
    chatContainer_->SetPosition(0,-170);
    chatContainer_->SetHorizontalAlignment(Urho3D::HA_RIGHT);
    chatContainer_->SetVerticalAlignment(Urho3D::VA_CENTER);

    chatContainer_->SetLayoutMode(Urho3D::LM_VERTICAL);
    chatContainer_->SetLayoutSpacing(10.0);
    textEdit_ = chatContainer_->CreateChild<LineEdit>();
    textEdit_->SetStyleAuto();
    textEdit_->SetVisible(true);


    // Clear menu
    renderer->SetViewport(0, nullptr);
    renderer->SetViewport(1, nullptr);
    renderer->SetViewport(2, nullptr);
    renderer->SetViewport(3, nullptr);

    // Run frame to refresh screen
    if (engine_)
        engine_->RunFrame();

    // Call on connect to server
    DoConnect();
}

void AlgebraKart::HandlePlayButton(StringHash eventType, VariantMap &eventData) {

    scene_->Clear(true,true);

    // Run frame to refresh screen
    if (engine_)
        engine_->RunFrame();

    Graphics *graphics = GetSubsystem<Graphics>();
    auto* renderer = GetSubsystem<Renderer>();

    //renderer->GetViewport(0


    // CLIENT CODE STARTS
    DoPlay();
}

// Network functions
void AlgebraKart::CreateServerSubsystem() {
    context_->RegisterSubsystem(new Server(context_));

    // Register base network client
    ClientObj::RegisterObject(context_);

    // Register two implementations of ClientObj, NetworkActor and Vehicle
    NetworkActor::RegisterObject(context_);
    Vehicle::RegisterObject(context_);

    Missile::RegisterObject(context_);
    MissileManager::RegisterObject(context_);

    // Vehicle & Projectiles
    RaycastVehicleBase::RegisterObject(context_);
    WheelTrackModel::RegisterObject(context_);

    // Client-side Prediction
    CSP_Server::RegisterObject(context_);
    CSP_Client::RegisterObject(context_);

    // Beat
    // Sequencer
    Sampler::RegisterObject(context_);
    Sequencer::RegisterObject(context_);
    Synthesizer::RegisterObject(context_);
    Recorder::RegisterObject(context_);
    BufferData::RegisterObject(context_);


    PixelImage::RegisterObject(context_);

}

void AlgebraKart::CreateUI() {
    if (headless_) {
        URHO3D_LOGDEBUG("Skipping UI creation in headless mode");
        return;
    }
    Graphics *graphics = GetSubsystem<Graphics>();
    auto* renderer = GetSubsystem<Renderer>();

    // Viewports:
    // 0 = front view
    // 1 = rear view
    // 2 = menu view
    // 3 = sequencer view
    renderer->SetNumViewports(4);

    // Setup menu viewport
    //SetupMenuViewport();


    ResourceCache *cache = GetSubsystem<ResourceCache>();
    UI *ui = GetSubsystem<UI>();
    UIElement *root = ui->GetRoot();
    XMLFile *uiStyle = cache->GetResource<XMLFile>("UI/DefaultStyle.xml");
    root->SetDefaultStyle(uiStyle);


    SharedPtr<Cursor> cursor(new Cursor(context_));
    cursor->SetStyleAuto(uiStyle);
    ui->SetCursor(cursor);

    cursor->SetPosition(graphics->GetWidth() / 2, graphics->GetHeight() / 2);

    int textureWidth;
    int textureHeight;

    // Get logo texture
    Texture2D *bkgTexture = cache->GetResource<Texture2D>("Textures/menu-bkg.png");
    if (!bkgTexture)
        return;

    // Create bkg sprite and add to the UI layout
    bkgSprite_ = ui->GetRoot()->CreateChild<Sprite>();
    bkgAngle_ = 0.0f;

    // Set logo sprite texture
    bkgSprite_->SetTexture(bkgTexture);

    textureWidth = bkgTexture->GetWidth();
    textureHeight = bkgTexture->GetHeight();

    // Set logo sprite scale
    bkgSprite_->SetScale((256.0f / textureWidth) * 9.2f);

    // Set logo sprite size
    bkgSprite_->SetSize(textureWidth, textureHeight);

    // Set logo sprite hot spot
    bkgSprite_->SetHotSpot(textureWidth / 2, textureHeight / 2);

    // Set logo sprite alignment
    bkgSprite_->SetAlignment(HA_CENTER, VA_CENTER);
    bkgSprite_->SetPosition(0, 0);

    // Make logo not fully opaque to show the scene underneath
    bkgSprite_->SetOpacity(0.9f);

    // Set a low priority for the logo so that other UI elements can be drawn on top
    bkgSprite_->SetPriority(-100);

    // Construct the instructions text element
    versionText_ = ui->GetRoot()->CreateChild<Text>();
    versionText_->SetText(STUDIO_VERSION);
    versionText_->SetFont(cache->GetResource<Font>(INGAME_FONT3), 16);
    versionText_->SetColor(Color::GRAY);

    auto versionText2_ = ui->GetRoot()->CreateChild<Text>();
    versionText2_->SetText(STUDIO_VERSION);
    versionText2_->SetFont(cache->GetResource<Font>(INGAME_FONT3), 16);
    versionText2_->SetColor(Color::YELLOW);
    versionText2_->SetParent(versionText_);

    // Position the text relative to the screen center
    //versionText_->SetHorizontalAlignment(HA_CENTER);
    versionText_->SetAlignment(HA_CENTER, VA_TOP);
    versionText_->SetPosition(0, 10);
    // Hide once connected
    versionText_->SetVisible(true);

    versionText2_->SetAlignment(HA_CENTER, VA_TOP);
    versionText2_->SetPosition(0, -2);
    // Hide once connected
    versionText2_->SetVisible(true);

    // Construct the instructions text element
    gameNameText_ = ui->GetRoot()->CreateChild<Text>();
    gameNameText_->SetText(GAME_NAME);
    gameNameText_->SetFont(cache->GetResource<Font>(INGAME_FONT2), 40);
    gameNameText_->SetColor(Color::GRAY);
    // Position the text relative to the screen center
    gameNameText_->SetAlignment(HA_CENTER, VA_TOP);
    gameNameText_->SetPosition(0, 70);
    gameNameText_->SetVisible(true);

    auto gameNameText2_ = ui->GetRoot()->CreateChild<Text>();
    gameNameText2_->SetText(GAME_NAME);
    gameNameText2_->SetFont(cache->GetResource<Font>(INGAME_FONT2), 40);
    gameNameText2_->SetColor(Color::WHITE);
    // Position the text relative to the screen center
    gameNameText2_->SetAlignment(HA_CENTER, VA_TOP);
    gameNameText2_->SetPosition(-3, -2);
    gameNameText2_->SetVisible(true);
    gameNameText2_->SetParent(gameNameText_);




    /*studioText_ = ui->GetRoot()->CreateChild<Text>();
    studioText_->SetText(STUDIO_VERSION);
    studioText_->SetFont(cache->GetResource<Font>("Fonts/CompassGold.ttf"), 26);
    studioText_->SetColor(Color::BLACK);
    // Position the text relative to the screen center
    studioText_->SetHorizontalAlignment(HA_CENTER);
    studioText_->SetPosition(0, 920);
    // Hide once connected
    studioText_->SetVisible(true);
*/


    // Construct the instructions text element
    instructionsText_ = ui->GetRoot()->CreateChild<Text>();
    instructionsText_->SetText(
            ""
            "An online open world combat racing game."
    );
    instructionsText_->SetFont(cache->GetResource<Font>(INGAME_FONT3), 32);
    instructionsText_->SetColor(Color::WHITE);
    // Position the text relative to the screen center
    instructionsText_->SetHorizontalAlignment(HA_CENTER);
    instructionsText_->SetPosition(0, 550);
    // Hide until connected
    instructionsText_->SetVisible(false);

    /*
    levelText_ = ui->GetRoot()->CreateChild<Text>();
    levelText_->SetFont(cache->GetResource<Font>("Fonts/Anonymous Pro.ttf"), 12);
    levelText_->SetTextAlignment(HA_CENTER);
    levelText_->SetColor(Color::CYAN);
    levelText_->SetPosition(graphics->GetWidth() - 100, 10);

    triggerText_ = ui->GetRoot()->CreateChild<Text>();
    triggerText_->SetFont(cache->GetResource<Font>("Fonts/Anonymous Pro.ttf"), 12);
    triggerText_->SetTextAlignment(HA_CENTER);
    triggerText_->SetColor(Color::CYAN);
    triggerText_->SetPosition(graphics->GetWidth() - 250, 10);
*/
    progressText_ = ui->GetRoot()->CreateChild<Text>();
    progressText_->SetFont(cache->GetResource<Font>(INGAME_FONT), 21);
    progressText_->SetTextAlignment(HA_CENTER);
    progressText_->SetColor(Color::GREEN);
    progressText_->SetVerticalAlignment(VA_CENTER);
    progressText_->SetHorizontalAlignment(HA_CENTER);
    progressText_->SetPosition(0, 0);
    progressText_->SetVisible(false);

    progressResText_ = ui->GetRoot()->CreateChild<Text>();
    progressResText_->SetFont(cache->GetResource<Font>(INGAME_FONT), 15);
    progressResText_->SetTextAlignment(HA_CENTER);
    progressResText_->SetColor(Color::GRAY);
    progressResText_->SetVerticalAlignment(VA_CENTER);
    progressResText_->SetHorizontalAlignment(HA_CENTER);
    progressResText_->SetPosition(0, 30);
    progressResText_->SetVisible(false);


    int hudTextCount = 4;
    for (int i = 0; i < hudTextCount; i++) {
        // Construct the text element
        SharedPtr<Text> hudText_ = static_cast<SharedPtr<Text>>(ui->GetRoot()->CreateChild<Text>());
        hudText_->SetText("");
        hudText_->SetFont(cache->GetResource<Font>(INGAME_FONT), 13);
        hudText_->SetColor(Color::WHITE);
        // Position the text relative to the screen center
        hudText_->SetHorizontalAlignment(HA_CENTER);
        hudText_->SetPosition(0, 610+(i*20.0f));
        // Hide until connected
        hudText_->SetVisible(false);

        hudTextList_.Push(hudText_);
    }

    if (showMenu_) {
        buttonContainer_ = root->CreateChild<UIElement>();
        //buttonContainer_->SetFixedSize(graphics->GetWidth(), graphics->GetHeight());

        //buttonContainer_->SetF
//        buttonContainer_->SetPosition(graphics->GetWidth() / 5, graphics->GetHeight() / 5);
        buttonContainer_->SetHorizontalAlignment(HA_CENTER);
        buttonContainer_->SetVerticalAlignment(VA_CENTER);

        buttonContainer_->SetLayoutMode(Urho3D::LM_VERTICAL);
        buttonContainer_->SetLayoutSpacing(10.0);
        //textEdit_ = buttonContainer_->CreateChild<LineEdit>();
        //textEdit_->SetStyleAuto();
        //textEdit_->SetVisible(false);



        gameServerDropDownList_ = static_cast<SharedPtr<DropDownList>>(ui->GetRoot()->CreateChild<DropDownList>());
        gameServerDropDownList_->SetStyleAuto();

        gameServerDropDownList_->SetFixedSize(580, 54);
        gameServerDropDownList_->SetMinWidth(580);
        for (int i = 0; i < gameServers.size(); i++) {
            Text *text = new Text(context_);
            text->SetFont(INGAME_FONT4, 24);
            text->SetText(gameServers[i].c_str());
            text->SetFixedSize(200, 50);
            text->SetColor(Color::WHITE);
            text->SetEffectStrokeThickness(2);
            gameServerDropDownList_->AddItem(text);

        }
        gameServerDropDownList_->SetSelection(0);

        buttonContainer_->AddChild(gameServerDropDownList_);
        playButton_ = CreateButton("PLAY", 580);
        startServerButton_ = CreateButton("START SERVER", 580);
        exitButton_ = CreateButton("EXIT", 580);
    }

    // Get logo texture
    Texture2D *logoTexture = cache->GetResource<Texture2D>("Textures/BZLogo.png");
    if (!logoTexture)
        return;

    // Create logo sprite and add to the UI layout
    logoSprite_ = ui->GetRoot()->CreateChild<Sprite>();

    // Set logo sprite texture
    logoSprite_->SetTexture(logoTexture);

    textureWidth = logoTexture->GetWidth();
    textureHeight = logoTexture->GetHeight();

    // Set logo sprite scale
    //logoSprite_->SetScale(256.0f / textureWidth);
    //logoSprite_->SetScale(256.0f / textureWidth);

    // Set logo sprite size
    logoSprite_->SetSize(textureWidth, textureHeight);

    // Set logo sprite hot spot
    logoSprite_->SetHotSpot(textureWidth, textureHeight);

    // Set logo sprite alignment
    logoSprite_->SetAlignment(HA_CENTER, VA_CENTER);
    logoSprite_->SetPosition(textureWidth-textureWidth/3.0f+68.0f, 41.0f);
    logoSprite_->SetScale(1.6);

    // Make logo not fully opaque to show the scene underneath
    logoSprite_->SetOpacity(0.9f);

    // Set a low priority for the logo so that other UI elements can be drawn on top
    logoSprite_->SetPriority(-100);

    UpdateButtons();

    // No viewports or scene is defined. However, the default zone's fog color controls the fill color
    GetSubsystem<Renderer>()->GetDefaultZone()->SetFogColor(Color(0.0f, 0.0f, 0.1f));

    // Start Client
    if (autoStartClient_) {
        // Auto-play
        DoPlay();
    }
}

void AlgebraKart::InitiateViewport(Context* context, Scene* scene, Camera* camera, int id)
{
    Renderer* renderer = GetSubsystem<Renderer>();
    SharedPtr<Viewport> viewport(new Viewport(context, scene, camera));
    renderer->SetViewport(id, viewport);
}


void AlgebraKart::SetupSequencerViewport() {
    auto *graphics = GetSubsystem<Graphics>();
    auto *renderer = GetSubsystem<Renderer>();
    auto *cache = GetSubsystem<ResourceCache>();

    // Load or create enhanced sequencer scene
    seqScene_ = MakeShared<Scene>(context_);
    seqScene_->SetName("SeqScene");

    // Create zone for lighting
    Node * zoneNode = seqScene_->CreateChild("Zone");
    Zone *zone = zoneNode->CreateComponent<Zone>();
    zone->SetBoundingBox(BoundingBox(-1000.0f, 1000.0f));
    zone->SetAmbientColor(Color(0.2f, 0.2f, 0.3f));
    zone->SetFogColor(Color(0.1f, 0.1f, 0.2f));

    // Create lighting
    Node * lightNode = seqScene_->CreateChild("DirectionalLight");
    lightNode->SetDirection(Vector3(0.6f, -1.0f, 0.8f));
    Light *light = lightNode->CreateComponent<Light>();
    light->SetLightType(LIGHT_DIRECTIONAL);
    light->SetColor(Color(0.8f, 0.9f, 1.0f));
    light->SetCastShadows(true);

    // Create camera
    Node * cameraNode = seqScene_->CreateChild("SeqCamera");
    seqCam_ = cameraNode->CreateComponent<Camera>();
    seqCam_->SetOrthographic(true);
    seqCam_->SetOrthoSize(50.0f);

    // Position camera for good view of the beat grid
    cameraNode->SetPosition(Vector3(0.0f, 30.0f, -20.0f));
    cameraNode->LookAt(Vector3(0.0f, 0.0f, 0.0f));

    // Create enhanced 3D beat pads with glowing effects
    beatModelVec_.Clear();
    beatModelVec_.Resize(NUM_DRUM_MACHINE_PADS);

    for (int channel = 0; channel < 4; channel++) {
        for (int step = 0; step < 16; step++) {
            int index = channel * 16 + step;

            // Create beat pad node
            Node * padNode = seqScene_->CreateChild("BeatPad3D");
            padNode->SetPosition(Vector3(
                    (step - 7.5f) * 2.0f,           // X: spread across 16 steps
                    0.0f,                           // Y: base level
                    (channel - 1.5f) * 3.0f        // Z: 4 channels deep
            ));

            // Create the visual model
            StaticModel * model = padNode->CreateComponent<StaticModel>();
            model->SetModel(cache->GetResource<Model>("Models/Box.mdl"));

            // Create material with emissive properties for glow effect
            Material *material = new Material(context_);
            material->SetTechnique(0, cache->GetResource<Technique>("Techniques/DiffUnlit.xml"));

            // Set channel-based colors
            Color channelColor;
            switch (channel) {
                case 0:
                    channelColor = Color::RED;
                    break;      // Kick
                case 1:
                    channelColor = Color::BLUE;
                    break;     // Snare
                case 2:
                    channelColor = Color::GREEN;
                    break;    // Hi-hat
                case 3:
                    channelColor = Color::MAGENTA;
                    break;  // Synth
            }

            material->SetShaderParameter("MatDiffColor", channelColor);
            material->SetShaderParameter("MatEmissiveColor", channelColor * 0.3f);
            model->SetMaterial(material);

            // Scale based on position importance (quarter notes larger)
            float scale = (step % 4 == 0) ? 1.2f : 1.0f;
            padNode->SetScale(Vector3(scale, 0.3f, scale));

            beatModelVec_[index] = model;
        }
    }

    // Create enhanced time cursor with trail effect
    Node * cursorNode = seqScene_->CreateChild("TimeCursor");
    seqTimeCursorModel_ = cursorNode->CreateComponent<StaticModel>();
    seqTimeCursorModel_->SetModel(cache->GetResource<Model>("Models/Cylinder.mdl"));

    Node * beatTimeNode = seqScene_->CreateChild("BeatTimeCursor");
    beatTimeCursorModel_ = beatTimeNode->CreateComponent<StaticModel>();
    beatTimeCursorModel_->SetModel(cache->GetResource<Model>("Models/Cylinder.mdl"));

    Node * metronomeNode = seqScene_->CreateChild("MetronomeCursor");
    metronomeCursorModel_ = metronomeNode->CreateComponent<StaticModel>();
    metronomeCursorModel_->SetModel(cache->GetResource<Model>("Models/Cylinder.mdl"));


    Material *cursorMaterial = new Material(context_);
    cursorMaterial->SetTechnique(0, cache->GetResource<Technique>("Techniques/DiffUnlit.xml"));
    cursorMaterial->SetShaderParameter("MatDiffColor", Color::YELLOW);
    cursorMaterial->SetShaderParameter("MatEmissiveColor", Color::YELLOW * 0.8f);
    seqTimeCursorModel_->SetMaterial(cursorMaterial);

    cursorNode->SetPosition(Vector3(0.0f, 2.0f, 0.0f));
    cursorNode->SetScale(Vector3(0.5f, 3.0f, 8.0f));

    // Setup viewport
    float seqWidth = 900.0f;
    float seqHeight = 200.0f;
    seqViewport_ = new Viewport(context_, seqScene_, seqCam_,
                                IntRect(graphics->GetWidth() + seqWidth, graphics->GetHeight() - seqHeight,
                                        graphics->GetWidth(), graphics->GetHeight()));
    renderer->SetViewport(3, seqViewport_);
}

void AlgebraKart::SetupMenuViewport() {
    auto* graphics = GetSubsystem<Graphics>();
    auto* renderer = GetSubsystem<Renderer>();

    // Load menu scene
    ResourceCache *cache = GetSubsystem<ResourceCache>();
    XMLFile *xmlLevel = cache->GetResource<XMLFile>("Scenes/AlgebraKartMenu.xml");
    menuScene_ = MakeShared<Scene>(context_);
    menuScene_->SetName("MenuScene");

    if (xmlLevel) {
        menuScene_->LoadXML(xmlLevel->GetRoot());
        menuScene_->SetEnabled(true);
        menuScene_->SetUpdateEnabled(true);
    }


    // Get camera
    auto* menuCam = menuScene_->GetChild("menuCam",LOCAL)->GetComponent<Camera>();
    auto* progressBar = menuScene_->GetChild("ProgressBar",LOCAL)->GetComponent<StaticModel>();
    progressBar->GetNode()->SetScale(Vector3(1,1,1));

    // The viewport index must be greater in that case, otherwise the view would be left behind
    menuViewport_ = new Viewport(context_, menuScene_, menuCam,
                                 IntRect(32, 32, graphics->GetWidth() - 32, graphics->GetHeight() - 32));
    renderer->SetViewport(2, menuViewport_);


}

void AlgebraKart::SetupGameViewports()
{
    if (headless_) {
        URHO3D_LOGDEBUG("Skipping viewport setup in headless mode");
        return;
    }

    auto* graphics = GetSubsystem<Graphics>();
    auto* renderer = GetSubsystem<Renderer>();

    // Viewports:
    // 0 = front view
    // 1 = rear view
    // 2 = menu view
    // 3 = sequencer view


    SharedPtr<Camera> rearCam;
    if (isServer_) {
        // SERVER CODE
        InitiateViewport(context_, scene_, serverCam_, 0);
        // Create camera opposite
        Node* node = serverCam_->GetNode()->CreateChild("RearCamera", LOCAL);
        rearCam = node->CreateComponent<Camera>();
        //rearCam->SetOrthographic(true);
        rearCam->GetNode()->SetRotation(Quaternion(0, 180, 0));
        rearCam->SetFarClip(48000.0f);
        rearCam->SetFillMode(Urho3D::FILL_SOLID);

    } else {
        // CLIENT CODE
        InitiateViewport(context_, scene_, clientCam_, 0);
        // Create camera opposite
        Node* node = clientCam_->GetNode()->CreateChild("RearCamera", LOCAL);
        rearCam = node->CreateComponent<Camera>();
        //rearCam->SetOrthographic(true);
        rearCam->GetNode()->SetRotation(Quaternion(0, 180, 0));
        rearCam->GetNode()->SetPosition(Vector3(0.0f,-0.3f,0.0f));
        rearCam->SetFarClip(48000.0f);
        rearCam->SetFillMode(Urho3D::FILL_SOLID);

    }

    // DEBUG -- disable views
//    renderer->SetViewport(0, nullptr);
//    renderer->SetViewport(1, nullptr);

    //menuCam->SetFarClip(48000.0f);
    //rearCam->SetFillMode(Urho3D::FILL_SOLID);

    // Set up the rear camera viewport on top of the front view ("rear view mirror")
    float widthP = graphics->GetWidth()/6;
    float heightP = graphics->GetHeight()/15;
    // The viewport index must be greater in that case, otherwise the view would be left behind
    SharedPtr<Viewport> rearViewport(new Viewport(context_, scene_, rearCam,
                                                     IntRect((graphics->GetWidth() / 2)-widthP, 2, (graphics->GetWidth()/2)+widthP - 16, 32+heightP)));
    renderer->SetViewport(1, rearViewport);

}


void AlgebraKart::ChangeDebugHudText() {
    // change profiler text
    if (GetSubsystem<DebugHud>()) {
        Text *dbgText = GetSubsystem<DebugHud>()->GetProfilerText();
        dbgText->SetColor(Color::CYAN);
        dbgText->SetTextEffect(TE_NONE);

        dbgText = GetSubsystem<DebugHud>()->GetStatsText();
        dbgText->SetColor(Color::CYAN);
        dbgText->SetTextEffect(TE_NONE);

        dbgText = GetSubsystem<DebugHud>()->GetMemoryText();
        dbgText->SetColor(Color::CYAN);
        dbgText->SetTextEffect(TE_NONE);

        dbgText = GetSubsystem<DebugHud>()->GetModeText();
        dbgText->SetColor(Color::CYAN);
        dbgText->SetTextEffect(TE_NONE);
    }
}

Button *AlgebraKart::CreateButton(const String &text, int width) {
    ResourceCache *cache = GetSubsystem<ResourceCache>();
    Font *font = cache->GetResource<Font>(INGAME_FONT4);

    Button *button = buttonContainer_->CreateChild<Button>();
    button->SetStyleAuto();
    button->SetFixedHeight(120);
    button->SetFixedWidth(width);
    //button->SetHeight(5);

    Text *buttonText = button->CreateChild<Text>();
    buttonText->SetName("text");
    buttonText->SetFont(INGAME_FONT4, 40);
    buttonText->SetAlignment(HA_CENTER, VA_CENTER);
    buttonText->SetText(text);

    return button;
}

void AlgebraKart::UpdateButtons() {
    Network *network = GetSubsystem<Network>();
    Connection *serverConnection = network->GetServerConnection();
    bool serverRunning = network->IsServerRunning();


    if (showMenu_) {
        playButton_->SetVisible(!serverConnection && !serverRunning);
        startServerButton_->SetVisible(!serverConnection && !serverRunning);
        exitButton_->SetVisible(!serverConnection && !serverRunning);

        //exitButton_->SetVisible(!serverConnection && !serverRunning);
        //  textEdit_->SetVisible(!serverConnection && !serverRunning);
    }
}

// Manage camera movement for Client
void AlgebraKart::MoveCamera(float timeStep) {
    // Skip camera movement in headless mode
    if (headless_) return;

    // Only move camera if we have a valid camera
    //if (!clientCam_ || !clientCam_->GetNode()) return;

    // CLIENT CODE

    // Right mouse button controls mouse cursor visibility: hide when pressed
    UI *ui = GetSubsystem<UI>();
    Input *input = GetSubsystem<Input>();
    ui->GetCursor()->SetVisible(!input->GetMouseButtonDown(MOUSEB_RIGHT));

    // Mouse sensitivity as degrees per pixel
    const float MOUSE_SENSITIVITY = 0.1f;


    // smooth step
    const float rotLerpRate = 10.0f;
    const float maxVel = 50.0f;
    const float damping = 0.2f;

    int k = 0;
    if (started_) {

        // For clients
        if (!isServer_) {


            // Use this frame's mouse motion to adjust camera node yaw and pitch. Clamp the pitch and only move the camera
            // when the cursor is hidden
            if (!ui->GetCursor()->IsVisible()) {
                IntVector2 mouseMove = input->GetMouseMove();
                yaw_ += MOUSE_SENSITIVITY * mouseMove.x_;
                pitch_ += MOUSE_SENSITIVITY * mouseMove.y_;
                pitch_ = Clamp(pitch_, 1.0f, 90.0f);
            }

            if (clientCam_) {

                // Only move camera if we have a controllable object

                Network *network = GetSubsystem<Network>();
                Connection *serverConnection = network->GetServerConnection();

                if (serverConnection) {
                    int size;
                    PODVector<Node *> vec;

                    String actorName = String("actor-") + clientName_;
                    Node *actorNode = scene_->GetChild(actorName);

                    String vehicleName = String("vehicle-") + clientName_;
                    Node *vehicleNode = scene_->GetChild(vehicleName);

                    //BLUE-304-vehicle

                    Vector3 pos;
                    Quaternion rot;
                    Vector3 lVel;

                    if (actorNode) {
                        // Retrieve Actor
                        ClientObj *actor = actorNode->GetDerivedComponent<ClientObj>();

                        if (actor) {
                            NetworkActor *na = actorNode->GetDerivedComponent<NetworkActor>();
                            Vehicle *v = vehicleNode->GetDerivedComponent<Vehicle>();


                            if (na) {
                                auto *body = na->GetNode()->GetComponent<RigidBody>(true);
                                if (body) {
                                    // CLIENT RIGID BODY RETRIEVED
                                    pos = body->GetPosition();
                                    rot = body->GetRotation();
                                    lVel = body->GetLinearVelocity();
                                }
                            }

                            // Client: Move Camera
                            // Camera Positioning
                            ////

                            // Update camera based on current mode
                            switch (currentCameraMode_) {
                                case CAMERA_FIRST_PERSON:
                                    UpdateCameraFirstPerson(timeStep);
                                    break;
                                case CAMERA_THIRD_PERSON:
                                    UpdateCameraThirdPerson(timeStep);
                                    break;
                                case CAMERA_ISOMETRIC:
                                    UpdateCameraIsometric(timeStep);
                                    break;

                                    ///
                                case CAMERA_FREEFLY: {
                                // Enhanced free-fly camera with obstruction detection
                                    Vector3 lookAtObject;

                                    if (na) {
                                        String username = actor->GetUserName();
                                        float botSpeedKm = 0;
                                        Quaternion forward;
                                        float velMult = 8.0f;

                                        // Update obstruction detection
                                        UpdateCameraObstruction(timeStep, na);

                                        Vector3 cameraTargetPos;
                                        auto *naBody = na->GetNode()->GetComponent<RigidBody>(true);

                                        if (na->onVehicle_ && na->entered_) {
                                            if (naBody) {
                                                pos = naBody->GetPosition();
                                                rot = naBody->GetRotation();
                                                lVel = naBody->GetLinearVelocity();
                                            }

                                            if (v) {
                                                auto *vBody = v->GetNode()->GetComponent<RigidBody>(true);
                                                if (vBody) {
                                                    pos = vBody->GetPosition();
                                                    rot = vBody->GetRotation();
                                                    lVel = vBody->GetLinearVelocity();
                                                    lookAtObject = vBody->GetPosition();
                                                    forward = -vBody->GetNode()->GetRotation();
                                                }
                                            }

                                            botSpeedKm = Clamp(botSpeedKm, 1.0f, 2000.0f);
                                            float velLen = lVel.Length();

                                            // Use obstruction-adjusted distance
                                            float adjustedDistance = currentCameraDistance_;

                                            // Calculate target position with obstruction awareness
                                            Vector3 baseDirection = forward * Vector3(velLen * velMult * 0.07f,
                                                                                      (CAMERA_RAY_DISTANCE_LIMIT / 22) * velLen,
                                                                                      -50.0f + -velLen * velMult);

                                            cameraTargetPos = pos + baseDirection * 0.6f;

                                            // Check for obstructions and adjust if needed
                                            float obstacleDistance;
                                            if (CheckCameraObstruction(cameraTargetPos, pos, na, obstacleDistance)) {
                                                Vector3 safeDirection = GetSafeDirectionFromObstruction(
                                                        baseDirection.Normalized(), pos, adjustedDistance);
                                                cameraTargetPos = pos + safeDirection * adjustedDistance;
                                            }

                                            lookAtObject = pos + naBody->GetRotation() * Vector3::FORWARD * 15.0f;
                                        } else {
                                            // On foot with obstruction handling
                                            botSpeedKm = 0;
                                            forward = rot;
                                            float velLen = lVel.Length();

                                            float adjustedDistance = Max(78.0f, currentCameraDistance_);
                                            Vector3 baseDirection = -Vector3::FORWARD;

                                            cameraTargetPos = pos + baseDirection * adjustedDistance + Vector3::UP * 57.3f;

                                            // Check for obstructions
                                            float obstacleDistance;
                                            if (CheckCameraObstruction(cameraTargetPos, pos, na, obstacleDistance)) {
                                                Vector3 safeDirection = GetSafeDirectionFromObstruction(
                                                        baseDirection, pos, adjustedDistance);
                                                cameraTargetPos = pos + safeDirection * adjustedDistance + Vector3::UP * 57.3f;
                                            }

                                            lookAtObject = naBody->GetPosition();
                                        }

                                        Vector3 cameraStartPos = clientCam_->GetNode()->GetPosition();
                                        bool stopMove = false;

                                        // Camera ray cast limiter (keep existing timing logic)
                                        if (lastCamRaycast > CAM_RAYCAST_TIME_WAIT) {
                                            lastCamRaycast = 0;

                                            float w1 = stopMove ? 1.0f : 0.94f;
                                            float w2 = stopMove ? 0.0f : 0.06f;

                                            Vector3 weightedSum = clientCam_->GetNode()->GetPosition() * w1 + cameraTargetPos * w2;

                                            clientCam_->GetNode()->SetPosition(weightedSum);

                                            Quaternion lastRot = clientCam_->GetNode()->GetRotation();
                                            clientCam_->GetNode()->LookAt(lookAtObject);
                                            Quaternion nextRot = clientCam_->GetNode()->GetRotation();

                                            float t = 0.8f;
                                            Quaternion q = lastRot * (1 - t) - nextRot * t;
                                            clientCam_->GetNode()->SetRotation(q);
                                        }
                                    }
                                    break;
                                }
                                    ///
                            }

                            // Update listener position for 3D audio
                            if (auto* audio = GetSubsystem<Audio>()) {
                                if (auto* listener = clientCam_->GetNode()->GetComponent<SoundListener>()) {
                                    audio->SetListener(listener);
                                }
                            }

                            ////

                            // CLIENT CODE

                            // Get rank and show
                            if (!rcvRankList_.Empty()) {

                                Vector<String> ranks;

                                for (int i = 0; i < rcvRankList_.Size(); i++) {
                                    String rankScore = rcvRankList_.At(i);

                                    // Show rankings
                                    if (rankText_[i]) {
                                        float vertScale = 33.0f;
                                        float vAlignRank = 340.0f;
                                        Vector<String> tokens = rankScore.Split('|');

                                        rankText_[i]->SetAlignment(HA_LEFT, VA_TOP);
                                        rankText_[i]->SetPosition(10.0f, vAlignRank + (i * vertScale));
                                        rankText_[i]->SetVisible(true);
                                        rankText_[i]->SetColor(Color(232.0f/255.0f, 239.0f/255.0f, 205.0f/255.0f));
                                        rankText_[i]->SetText(
                                                String("[") + String(i) + String("] ") + String(tokens[0]) +
                                                String(": ") + String(tokens[1]));
                                    }
                                }
                            }

                            if (debugText_[k]) {
                                debugText_[k]->SetAlignment(HA_LEFT, VA_TOP);
                                debugText_[k]->SetPosition(10.0f, 400 + (k * 12));
                                debugText_[k]->SetVisible(false);
                                debugText_[k]->SetText(String("Actor Pos -> ") + String(pos.ToString()));
                                k++;
                                debugText_[k]->SetAlignment(HA_LEFT, VA_TOP);
                                debugText_[k]->SetPosition(10.0f, 400 + (k * 12));
                                debugText_[k]->SetVisible(false);
                                debugText_[k]->SetText(String("Actor Rot -> ") + String(rot.ToString()));
                                k++;
                                debugText_[k]->SetAlignment(HA_LEFT, VA_TOP);
                                debugText_[k]->SetPosition(10.0f, 400 + (k * 12));
                                debugText_[k]->SetVisible(false);
                                debugText_[k]->SetText(String("Actor -> ") + String(actorName));
                                k++;

                                debugText_[k]->SetAlignment(HA_LEFT, VA_TOP);
                                debugText_[k]->SetPosition(10.0f, 400 + (k * 12));
                                debugText_[k]->SetVisible(false);
                                debugText_[k]->SetText(String("ntwkControls_.yaw_ -> ") + String(ntwkControls_.yaw_));
                                k++;


                                if (v) {
                                    // Snap camera to vehicle once available
                                    Vector3 startPos = v->GetNode()->GetNetPositionAttr();
                                    MemoryBuffer buf(v->GetNode()->GetNetRotationAttr());
                                    Quaternion rotation = buf.ReadPackedQuaternion();

                                    // Show message on first snap
                                    if (!isSnapped_) {
                                        isSnapped_ = true;

                                        URHO3D_LOGDEBUG(
                                                "--- Retrieved vehicle -> can snap camera -> Actor [" +
                                                String(startPos.x_) + "," +
                                                String(startPos.y_) + "," +
                                                String(startPos.z_) + "]");
                                    }


                                    debugText_[k]->SetAlignment(HA_LEFT, VA_TOP);
                                    debugText_[k]->SetPosition(10.0f, 400 + (k * 12));
                                    debugText_[k]->SetVisible(false);
                                    debugText_[k]->SetText(String("Vehicle Pos -> ") + String(startPos.ToString()));
                                    k++;
                                    debugText_[k]->SetAlignment(HA_LEFT, VA_TOP);
                                    debugText_[k]->SetPosition(10.0f, 400 + (k * 12));
                                    debugText_[k]->SetVisible(false);
                                    debugText_[k]->SetText(String("Vehicle Rot -> ") + String(rotation.ToString()));
                                    k++;
                                    debugText_[k]->SetAlignment(HA_LEFT, VA_TOP);
                                    debugText_[k]->SetPosition(10.0f, 400 + (k * 12));
                                    debugText_[k]->SetVisible(false);
                                    debugText_[k]->SetText(String("Vehicle -> ") + String(vehicleName));
                                    k++;



                                    // Object location for camera
                                    Vector3 snap = Vector3(startPos);
                                    snap.y_ += 230.0f;
                                    snap.x_ -= 120.0f;
                                    snap.z_ -= 120.0f;
                                    //Vector3 snap = server->GetFocusObjects().Empty() ? Vector3(0, 35.0, 0) : server->GetFocusObjects()[0];

                                }
                            }

                            for (int i = 0; i < k; i++) {
                                if (debugText_[k]) {
                                    debugText_[k]->SetVisible(false);
                                }
                            }


                        }
                    }
                }

            }
        }
    }  // End of started_
}


void AlgebraKart::NodeRegisterLoadTriggers(Node *node)
{
    if (node)
    {
        PODVector<Node*> result;
        node->GetChildrenWithTag(result, "levelLoadTrigger", true);

        for ( unsigned i = 0; i < result.Size(); ++i )
        {
            SubscribeToEvent(result[i], E_NODECOLLISIONSTART, URHO3D_HANDLER(AlgebraKart, HandleLoadTriggerEntered));
        }
    }
    else
    {
        URHO3D_LOGERROR("NodeRegisterLoadTriggers - node is NULL.");
    }
}

void AlgebraKart::HandleLoadTriggerEntered(StringHash eventType, VariantMap& eventData)
{
    using namespace NodeCollisionStart;

    // prevent the trigger to reload while already loading
    if (!levelLoadPending_.Empty())
    {
        return;
    }

    Node *node = ((RigidBody*)eventData[P_BODY].GetVoidPtr())->GetNode();
    StringVector tagVec = node->GetTags();
    String levelName;
    String loadLevel;

    // get trigger tags
    for ( unsigned i = 0; i < tagVec.Size(); ++i )
    {
        if (tagVec[i].StartsWith("levelName="))
        {
            const unsigned nameLen = String("levelName=").Length();
            levelName = tagVec[i].Substring(nameLen, tagVec[i].Length() - nameLen);
        }
        else if (tagVec[i].StartsWith("loadLevel="))
        {
            const unsigned loadLen = String("loadLevel=").Length();
            loadLevel = tagVec[i].Substring(loadLen, tagVec[i].Length() - loadLen);
        }
    }

    if (!levelName.Empty() && !loadLevel.Empty())
    {
        levelText_->SetText(String("cur level:\n") + levelName);
        triggerText_->SetText(String("trig info:\n") + "level=" + levelName + "\nload =" + loadLevel);

        String curLevelName = curLevel_?curLevel_->GetName():String::EMPTY;
        String loadLevelName = nextLevel_?nextLevel_->GetName():String::EMPTY;

        if (curLevelName != levelName)
        {
            // swap nodes
            if (curLevelName == loadLevel && loadLevelName == levelName)
            {
                Node *tmpNode = curLevel_;
                curLevel_ = nextLevel_;
                nextLevel_ = tmpNode;
            }
            else
            {
                URHO3D_LOGERROR("Trigger level and load names out of sequence.");
            }
        }
        else if (loadLevelName != loadLevel)
        {
            // remove any existing level
            if (nextLevel_)
            {
                nextLevel_->Remove();
                nextLevel_ = NULL;
            }

            // async load
            ResourceCache* cache = GetSubsystem<ResourceCache>();
            String levelPathFile = levelPathName_ + loadLevel + String(".xml");
            URHO3D_LOGINFO("cache file " + levelPathFile);
            XMLFile *xmlLevel = cache->GetResource<XMLFile>(levelPathFile);

            if (xmlLevel)
            {
                SceneResolver resolver;
                Node *tmpNode = scene_->CreateTemporaryChild();

                URHO3D_LOGINFO("LoadXML loading " + levelPathFile);
                if (tmpNode->LoadXML(xmlLevel->GetRoot(), resolver, false))
                {
                    URHO3D_LOGINFO("Async loading " + levelPathFile);
                    if (scene_->InstantiateXMLAsync(xmlLevel->GetRoot(), tmpNode->GetWorldPosition(), tmpNode->GetWorldRotation()))
                    {
                        // prevent the trigger to reload while already loading
                        levelLoadPending_ = levelPathFile;
                        progressText_->SetText("");
                    }
                    else
                    {
                        URHO3D_LOGERROR("InstantiateXMLAsync failed to init, level=" + levelPathFile);
                    }
                }
            }
            else
            {
                URHO3D_LOGERROR("Load level file= " + levelPathFile + " not found!");
            }
        }
    }
}

void AlgebraKart::HandleLoadProgress(StringHash eventType, VariantMap& eventData)
{
    using namespace AsyncLoadProgress;

    float progress = eventData[P_PROGRESS].GetFloat();
    int loadedNode = eventData[P_LOADEDNODES].GetInt();
    int totalNodes = eventData[P_TOTALNODES].GetInt();
    int loadedResources = eventData[P_LOADEDRESOURCES].GetInt();
    int totalResources = eventData[P_TOTALRESOURCES].GetInt();

    String progressStr = ToString("progress=%d%%", (int)(progress * 100.0f)) +
                         ToString("\nnodes: %d/%d", loadedNode, totalNodes) +
                         ToString("\nresources: %d/%d", loadedResources, totalResources);

    //progressText_->SetText(progressStr);
}

void AlgebraKart::HandleLevelLoaded(StringHash eventType, VariantMap& eventData) {
    ResourceCache *cache = GetSubsystem<ResourceCache>();
    using namespace AsyncLevelLoadFinished;
    nextLevel_ = (Node *) eventData[P_NODE].GetVoidPtr();

    // register triggers from new level node and clear loading flag
    NodeRegisterLoadTriggers(nextLevel_);
    levelLoadPending_.Clear();


    scene_->StopAsyncLoading();

    levelLoading_ = false;
    // Initiate game map for multiplayer game
    InitiateGameMap(scene_);
    // Spawn the player to server itself
    SpawnPlayer();

    if (!headless_)
        // Start in game mode
        UpdateUIState(true);

    // Attach agent bot list to server (for login list)
    Vector<String> botList;
    // Build bot list
    for (int i = 0; i < EvolutionManager::getInstance()->getAgents().size(); i++) {
        String botName = EvolutionManager::getInstance()->getNetworkActors()[i]->GetUserName();
        botList.Push(botName);
    }
    Server *server = GetSubsystem<Server>();
    server->SetAgents(botList);

    // Create sphere for camera target
    camTargetNode_ = scene_->CreateChild("CamTargetMarker");
    camTargetNode_->SetScale(38.0f);
    camTargetNode_->SetPosition(flyTargetCam_);

    // model
    StaticModel *ballModel = camTargetNode_->GetOrCreateComponent<StaticModel>();
    //ballModel->SetModel(cache->GetResource<Model>("Models/Sphere.mdl"));
    //ballModel->SetCastShadows(true);


    // spline model
    splineModel_ = scene_->GetOrCreateComponent<StaticModel>();
    //splineModel_->SetModel(cache->GetResource<Model>("Models/Sphere.mdl"));

    // Post map load

    /// Update steer spline path
    Node *steerSpline = scene_->GetChild("SteerSpline", "true");
    if (steerSpline) {
        steerSpline->GetChildren(steerMarks_);
        splineSize_ = steerMarks_.Size();
        for (int i = 0; i < aiActorMap_.Size(); i++) {
            auto *actor = dynamic_cast<NetworkActor *>(aiActorMap_[i].Get());
            if (actor) {
                if (actor->vehicle_) {
                    actor->vehicle_->setSteerIndex(splineSize_-2);
                }
            }
        }

    }

    /// Retrieve pickups
    Node *pickups = scene_->GetChild("Pickups", "true");
    if (pickups) {
        pickups->GetChildren(pickupMarks_);
    }

    if (!headless_) {
        // Clear menu
        Renderer *renderer = GetSubsystem<Renderer>();
        renderer->SetViewport(2, nullptr);
    }

}

void AlgebraKart::HandleConnectionFailed(StringHash eventType, VariantMap &eventData) {
    URHO3D_LOGINFO("Connection to server failed!");
    InitMsgWindow("Connection failure", "Connection to server failed!");
}

void AlgebraKart::CreateLoadingScreen() {
    auto* cache = GetSubsystem<ResourceCache>();
    auto* ui = GetSubsystem<UI>();
    auto* graphics = GetSubsystem<Graphics>();

    // Create the main loading screen container
    loadingScreen_ = ui->GetRoot()->CreateChild<UIElement>();
    loadingScreen_->SetName("LoadingScreen");
    loadingScreen_->SetSize(graphics->GetWidth(), graphics->GetHeight());
    loadingScreen_->SetPosition(0, 0);
    loadingScreen_->SetVisible(false);
    loadingScreen_->SetPriority(1000); // High priority to appear on top

    // Create background
    loadingBackgroundSprite_ = loadingScreen_->CreateChild<Sprite>();
    loadingBackgroundSprite_->SetTexture(cache->GetResource<Texture2D>("Textures/LoadingBackground.png"));
    loadingBackgroundSprite_->SetSize(graphics->GetWidth(), graphics->GetHeight());
    loadingBackgroundSprite_->SetAlignment(HA_LEFT, VA_TOP);
    loadingBackgroundSprite_->SetPosition(0, 0);

    // Create loading text
    loadingText_ = loadingScreen_->CreateChild<Text>();
    loadingText_->SetText("Preparing scene...");
    loadingText_->SetFont(cache->GetResource<Font>(INGAME_FONT2), 24);
    loadingText_->SetColor(Color::WHITE);
    loadingText_->SetAlignment(HA_CENTER, VA_CENTER);
    loadingText_->SetPosition(0, -120);
    loadingText_->SetEffectShadowOffset(IntVector2(4,5));
    loadingText_->SetTextEffect(TextEffect::TE_SHADOW);

    // Create progress bar background
    loadingProgressBarBg_ = loadingScreen_->CreateChild<Sprite>();
    loadingProgressBarBg_->SetTexture(cache->GetResource<Texture2D>("Textures/ProgressBarBg.png"));
    loadingProgressBarBg_->SetSize(700, 20);
    loadingProgressBarBg_->SetAlignment(HA_CENTER, VA_CENTER);
    loadingProgressBarBg_->SetPosition(-350, 0);
    loadingProgressBarBg_->SetColor(Color(0.3f, 0.3f, 0.3f, 0.8f));

    // Create progress bar
    loadingProgressBar_ = loadingProgressBarBg_->CreateChild<Sprite>();
    loadingProgressBar_->SetTexture(cache->GetResource<Texture2D>("Textures/ProgressBar.png"));
    loadingProgressBar_->SetSize(0, 20);
    loadingProgressBar_->SetAlignment(HA_CENTER, VA_CENTER);
    loadingProgressBar_->SetPosition(-349, -9);
    loadingProgressBar_->SetColor(Color(0.7f, 1.0f, 0.9f, 1.0f));

    // Create tip text
    loadingTipText_ = loadingScreen_->CreateChild<Text>();
    loadingTipText_->SetFont(cache->GetResource<Font>(INGAME_FONT3, 17));
    loadingTipText_->SetColor(Color(0.8f, 0.8f, 0.8f, 1.0f));
    loadingTipText_->SetAlignment(HA_CENTER, VA_CENTER);
    loadingTipText_->SetPosition(0, 420);

    // Initialize loading tips
    loadingTips_.Push("Tip: Use WASD keys to control your vehicle");
    loadingTips_.Push("Tip: Collect power-ups to boost your performance");
    loadingTips_.Push("Tip: AI bots are evolving using neural networks");
    loadingTips_.Push("Tip: Watch the mini-map to track your position");
    loadingTips_.Push("Tip: Each race helps the AI learn better driving");
    loadingTips_.Push("Tip: Press TAB to toggle camera views");

    loadingProgress_ = 0.0f;
    loadingTipTimer_ = 0.0f;
    currentTipIndex_ = 0;

    if (!loadingTips_.Empty()) {
        loadingTipText_->SetText(loadingTips_[currentTipIndex_]);
    }
}

void AlgebraKart::ShowLoadingScreen() {
    if (loadingScreen_) {
        loadingScreen_->SetVisible(true);
        loadingProgress_ = 0.0f;
        loadingTipTimer_ = 0.0f;
        UpdateLoadingProgress(0.0f, "Initializing connection...");
    }
}

void AlgebraKart::HideLoadingScreen() {
    if (loadingScreen_) {
        loadingScreen_->SetVisible(false);
    }
}

void AlgebraKart::UpdateLoadingProgress(float progress, const String& status) {
    loadingProgress_ = Clamp(progress, 0.0f, 1.0f);

    if (loadingProgressBar_) {
        float barWidth = 698.0f * loadingProgress_; // 698 = 700 - 2 for padding
        loadingProgressBar_->SetSize(barWidth, 18);
    }

    if (!status.Empty() && loadingText_) {
        loadingText_->SetText(status);
    }
}

void AlgebraKart::UpdateLoadingScreen(float timeStep) {
    if (!loadingScreen_ || !loadingScreen_->IsVisible()) {
        return;
    }

    // Update tip rotation
    loadingTipTimer_ += timeStep;
    if (loadingTipTimer_ >= 3.0f && !loadingTips_.Empty()) { // Change tip every 3 seconds
        loadingTipTimer_ = 0.0f;
        currentTipIndex_ = (currentTipIndex_ + 1) % loadingTips_.Size();
        loadingTipText_->SetText(loadingTips_[currentTipIndex_]);
    }

    // Animate progress bar color
    if (loadingProgressBar_) {
        float colorShift = 0.5f + 0.5f * Sin(GetSubsystem<Time>()->GetElapsedTime() * 3.0f);
        Color barColor = Color(0.2f + 0.3f * colorShift, 0.8f, 0.2f + 0.3f * colorShift, 1.0f);
        loadingProgressBar_->SetColor(barColor);
    }
}

void AlgebraKart::DoConnect() {

    // Show loading screen at the start of connection
    ShowLoadingScreen();

    static const int MAX_ARRAY_SIZE = 10;
    static String colorArray[MAX_ARRAY_SIZE] =
            {
                    "WHITE",
                    "GRAY",
                    "BLACK",
                    "RED",
                    "GREEN",
                    "BLUE",
                    "CYAN",
                    "MAGENTA",
                    "YELLOW",
                    "VEGAS GOLD"
            };

    // Client code
    isServer_ = false;
    Server *server = GetSubsystem<Server>();
//..    String address = gameServers[gameServerSelected_].c_str();
    String address;

    if (gameServerDropDownList_) {
        UIElement *ui = gameServerDropDownList_->GetSelectedItem();
        auto *text = static_cast<Text *>(ui);
        address = text->GetText();
    } else {
        address = gameServers[0].c_str();
    }

    // Force local host
    if (local_)
        address = gameServers[0].c_str();

    if (remote_)
        address = serverIP_;

//    String address = textEdit_->GetText().Trimmed();

    // Setup Client-Side Prediction
    cspClient_ = scene_->CreateComponent<CSP_Client>(LOCAL);
    cspClient_->SetScene(scene_);

    // randomize (or customize) client info/data
    int idx = Random(MAX_ARRAY_SIZE - 1);
    char buffer[200];
    String baseName = colorArray[idx];
    sprintf(buffer, "%s-%d", baseName.CString(), Random(1, 1000));
    String name = buffer;
    URHO3D_LOGINFOF("client idx=%i, username=%s", idx, name.CString());

    VariantMap identity;
    // Update loading progress
    UpdateLoadingProgress(0.2f, "Establishing connection...");

    identity["UserName"] = name;
    identity["ColorIdx"] = idx;
    identity["Position"] = Vector3(-2,-2,-2);

    Network* network = GetSubsystem<Network>();

    scene_->Clear(true, true);
    if (engine_)
        engine_->RunFrame();

    // Client connect to server
    bool success = network->Connect(address, SERVER_PORT, scene_, identity);
    if (success) {
        UpdateLoadingProgress(0.4f, "Connected! Loading resources...");
    } else {
        UpdateLoadingProgress(0.0f, "Connection failed. Retrying...");
        HideLoadingScreen();
        URHO3D_LOGINFOF("Connection to server failed =%s", address.CString());
        engine_->Exit();
        return;
    }

    // CLIENT CODE -> START CONNECT

    // Set camera node for viewport
    cameraNode_ = scene_->CreateChild("Camera", LOCAL);
    clientCam_ = cameraNode_->CreateComponent<Camera>();
    //clientCam_->SetOrthographic(true);
    clientCam_->SetFarClip(24000.0f);
    //clientCam_->SetFillMode(Urho3D::FILL_WIREFRAME);
    cameraNode_->SetPosition(Vector3(heliCamView_));
    clientCam_->GetNode()->SetRotation(Quaternion(90.0f, 0.0f, 0.0f));

    // Setup game viewport
    SetupGameViewports();

    URHO3D_LOGINFOF("client identity name=%s", name.CString());
    URHO3D_LOGINFOF("AlgebraKart::HandleConnect - data: [%s, %d]", name.CString(), idx);

    // Store in local login list
    loginList_.Push(name.CString());

    if (showMenu_) {
        // Change UI -> hide menu and show game
        UpdateButtons();
    }
    started_ = true;

    // Set logo sprite alignment
    logoSprite_->SetAlignment(HA_RIGHT, VA_BOTTOM);
    logoSprite_->SetPosition(3, 3);
    logoSprite_->SetScale(0.33f);
    // Make logo not fully opaque to show the scene underneath
    logoSprite_->SetOpacity(0.5f);

    // Client startup code

    // Store name
    clientName_ = name.CString();

    String playerText = "Logged in as: " + String(clientName_.CString());
    instructionsText_->SetText(playerText);
    //instructionsText_->SetPosition(0, 730);
    instructionsText_->SetAlignment(HA_CENTER, VA_BOTTOM);
    instructionsText_->SetPosition(0, -13);

    // TODO CAN use this to get user input later for name
//        String address = textEdit_->GetText().Trimmed();
    // Empty the text edit after reading the address to connect to
//        textEdit_->SetText(String::EMPTY);

    UpdateButtons();

    URHO3D_LOGINFOF("client idx=%i, username=%s", idx, name.CString());

    /// Subscribe the ServerConnected Event
    SubscribeToEvent(E_SERVERCONNECTED, URHO3D_HANDLER(AlgebraKart, HandleServerConnected));
}

void AlgebraKart::HandleConnect(StringHash eventType, VariantMap &eventData) {
   DoConnect();
}

void AlgebraKart::HandleDisconnect(StringHash eventType, VariantMap &eventData) {
    Server *server = GetSubsystem<Server>();
    server->Disconnect();

    UpdateButtons();
}


void AlgebraKart::DoStartServer() {
    Server *server = GetSubsystem<Server>();
    if (!server->StartServer(SERVER_PORT)) {
        engine_->Exit();
    }

    // Load base scene
    ReloadScene(true);


    // Setup Client-Side Prediction
    cspServer_ = scene_->CreateComponent<CSP_Server>(LOCAL);
    cspServer_->updateInterval_ = 1.f / 1.f;

    // Create Admin
//    CreateAdminPlayer();
    isServer_ = true;

    if (!headless_)
        UpdateButtons();

    // Switch to game mode
    //UpdateUIState(true);
    started_ = true;

    if (!headless_) {
        // Set logo sprite alignment
        logoSprite_->SetAlignment(HA_RIGHT, VA_TOP);
        logoSprite_->SetPosition(-15, 180);
        logoSprite_->SetScale(0.33f);


        // Make logo not fully opaque to show the scene underneath
        logoSprite_->SetOpacity(0.5f);
    }

    StartMultiplayerGameSession();

    /*
    //// DEBUG LOGGING
    int size;
    PODVector<Node*> vec;

    // Retrieve Network Actor (Client Object)
    vec = scene_->GetChildren(false);
    size = vec.Size();
    URHO3D_LOGDEBUGF(
            "Number of nodes -> %l", size);

    for (int i = 0; i < vec.Size(); i++) {
        Node *node = vec.At(i);
        if (node) {
            URHO3D_LOGDEBUGF(
                    "[SERVER] Scene: Node -> %s [%l] [%f,%f,%f] -> Is Replicated? %d", node->GetName().CString(), node->GetID(), node->GetPosition().x_, node->GetPosition().y_,
                    node->GetPosition().z_, node->IsReplicated());
        }
    }
*/
    /////

    Network *network = GetSubsystem<Network>();
    network->SetUpdateFps(30);

    // Server load level
    //LoadLevel(6);
    //LoadLevel(8);
    //LoadLevel(6); // big world track
    //LoadLevel(8); // sky track
    //LoadLevel(7);
    LoadLevel(8);
}

void AlgebraKart::HandleStartServer(StringHash eventType, VariantMap &eventData) {
    // SERVER CODE STARTS
    DoStartServer();
}

void AlgebraKart::HandleConnectionStatus(StringHash eventType, VariantMap &eventData) {
    using namespace ServerStatus;
    StringHash msg = eventData[P_STATUS].GetStringHash();

    if (msg == E_SERVERDISCONNECTED) {
        //scene_->RemoveAllChildren();
        //CreateScene();
        //SetupViewport();

        URHO3D_LOGINFO("server connection lost");
    }

    UpdateButtons();
}

void AlgebraKart::HandleClientObjectID(StringHash eventType, VariantMap &eventData) {
    // CLIENT CODE

    auto *network = GetSubsystem<Network>();
    Connection *serverConnection = network->GetServerConnection();



    // Handle Identification Packet (Client Object ID) -> contains Network Actor and Vehicle Node Ids

    using namespace ClientConnected;

    // Client identification packet
    if (eventType == E_CLIENTOBJECTID) {
        URHO3D_LOGINFOF("HandleClientObjectID -> eventType: %l -> E_CLIENTOBJECTID", eventType.Value());
        // SCENE: Read data and store client object id information
        networkActorNodeId_ = eventData[ClientObjectID::P_NETWORKACTOR].GetUInt();
        vehicleNodeId_ = eventData[ClientObjectID::P_VEHICLE].GetUInt();

        URHO3D_LOGINFOF(
                "Client -> AlgebraKart::HandleClientObjectID: networkActorNodeId_ = %l, vehicleNodeId_ = %l",
                networkActorNodeId_, vehicleNodeId_);

    }
}

void AlgebraKart::HandleExit(StringHash eventType, VariantMap &eventData) {
    engine_->Exit();
}

void AlgebraKart::StartMultiplayerGameSession()
{
    // Multiple session
    isMultiplayer_ = true;

    ResourceCache *cache = GetSubsystem<ResourceCache>();
    Network* network = GetSubsystem<Network>();
    // If we are running a server
    if (network->IsServerRunning())
    {
        if (!handlersSet) {
            // Subscribe for the newcomer recognition
            SubscribeToEvent(E_CLIENTIDENTITY, URHO3D_HANDLER(AlgebraKart, HandleClientIdentity));
            // Subscribe the update event for the whole logic
            SubscribeToEvent(E_UPDATE, URHO3D_HANDLER(AlgebraKart, HandleUpdate));
            // Subscribe for the client disconnection
            SubscribeToEvent(E_CLIENTDISCONNECTED, URHO3D_HANDLER(AlgebraKart, HandleClientDisconnected));

            // Register the remote event 'playerspawned'
            network->RegisterRemoteEvent(E_PLAYERSPAWNED);

            // Register the remote event
            network->RegisterRemoteEvent(E_RADIOTRACKNEXT);

            handlersSet = true;
        }
    }
        // if we are running a client
    else if (network->GetServerConnection())
    {

        UpdateLoadingProgress(0.6f, "Establishing connection...");

        // Register the remote event 'player spawned'
        network->RegisterRemoteEvent(E_PLAYERSPAWNED);
        // Subscribe for the remote event 'player spawned'
        SubscribeToEvent(E_PLAYERSPAWNED, URHO3D_HANDLER(AlgebraKart, HandlePlayerRespawned));

        // Register the remote event 'player loaded'
        network->RegisterRemoteEvent(E_PLAYERLOADED);
        // Subscribe for the remote event 'player loaded'
        SubscribeToEvent(E_PLAYERLOADED, URHO3D_HANDLER(AlgebraKart, HandlePlayerLoaded));

        // Register the remote event
        network->RegisterRemoteEvent(E_RACERANK);
        // Subscribe for the remote event 'race rank'
        SubscribeToEvent(E_RACERANK, URHO3D_HANDLER(AlgebraKart, HandleRaceRank));

        // Register the remote event
        network->RegisterRemoteEvent(E_RACEVICTORY);
        // Subscribe for the remote event 'race rank'
        SubscribeToEvent(E_RACEVICTORY, URHO3D_HANDLER(AlgebraKart, HandleRaceVictory));

        // Register the remote event
        network->RegisterRemoteEvent(E_RADIOTRACKNEXT);
        // Subscribe for the remote event 'player loaded'
        SubscribeToEvent(E_RADIOTRACKNEXT, URHO3D_HANDLER(AlgebraKart, HandleRadioTrackNext));

    }

}


void AlgebraKart::InitMsgWindow(String title, String message) {
    UI *ui = GetSubsystem<UI>();
    ResourceCache *cache = GetSubsystem<ResourceCache>();

    // Create the Window and add it to the UI's root node
    msgWindow_ = new Window(context_);
    ui->GetRoot()->AddChild(msgWindow_);

    // Set Window size and layout settings
    msgWindow_->SetMinWidth(384);
    msgWindow_->SetLayout(LM_VERTICAL, 6, IntRect(6, 6, 6, 6));
    msgWindow_->SetAlignment(HA_CENTER, VA_CENTER);
    msgWindow_->SetName(message);


    // Create Window 'titlebar' container
    auto *titleBar = new UIElement(context_);
    titleBar->SetMinSize(0, 24);
    titleBar->SetVerticalAlignment(VA_TOP);
    titleBar->SetLayoutMode(LM_HORIZONTAL);

    // Create the Window title Text
    auto *windowTitle = new Text(context_);
    windowTitle->SetName("WindowTitle");
    windowTitle->SetText(title);
//    windowTitle->SetFont(cache->GetResource<Font>("Fonts/SinsGold.ttf"), 15);

    // Create the Window's close button
    auto *buttonClose = new Button(context_);
    buttonClose->SetName("CloseButton");

    // Add the controls to the title bar
    titleBar->AddChild(windowTitle);
    titleBar->AddChild(buttonClose);

    // Add the title bar to the Window
    msgWindow_->AddChild(titleBar);

    // Apply styles
    msgWindow_->SetStyleAuto();
    windowTitle->SetStyleAuto();
    buttonClose->SetStyle("CloseButton");

    // Subscribe to buttonClose release (following a 'press') events
    SubscribeToEvent(buttonClose, E_RELEASED, URHO3D_HANDLER(AlgebraKart, HandleClosePressed));

    // Subscribe also to all UI mouse clicks just to see where we have clicked
    //  SubscribeToEvent(E_UIMOUSECLICK, URHO3D_HANDLER(HelloGUI, HandleControlClicked));
}

void AlgebraKart::HandleClosePressed(StringHash eventType, VariantMap &eventData) {
    // Shutdown
    engine_->Exit();
}


void AlgebraKart::OutputLoginListToConsole() {

    URHO3D_LOGINFO("**** CLIENT: CLIENT LIST *****************************************************");

    for (int i = 0; i < loginList_.Size(); i++) {
        URHO3D_LOGINFOF("**** LOGIN: %s", loginList_.At(i).CString());
    }

    URHO3D_LOGINFO("******************************************************************************");

}


void AlgebraKart::ShowChatText(const String &row) {
    chatHistory_.Resize(static_cast<unsigned int>(16));
    chatHistory_.Erase(0);
    chatHistory_.Push(row);

    if (!chatHistoryText_) {
        UI *ui = GetSubsystem<UI>();
        ResourceCache *cache = GetSubsystem<ResourceCache>();

        auto *font = cache->GetResource<Font>(INGAME_FONT3);
        chatHistoryText_ = ui->GetRoot()->CreateChild<Text>();
        chatHistoryText_->SetPosition(23,380);
        chatHistoryText_->SetFixedWidth(300);
        chatHistoryText_->SetHorizontalAlignment(Urho3D::HA_LEFT);
        chatHistoryText_->SetFont(font, 10);
        chatHistoryText_->SetColor(Color(0.8f,0.8f,0.7f));
        chatHistoryText_->SetVisible(true);
    }

    // Concatenate all the rows in history
    String allRows;
    for (unsigned i = 0; i < chatHistory_.Size(); ++i)
        allRows += chatHistory_[i] + "\n";

    chatHistoryText_->SetText(allRows);
}

void AlgebraKart::HandleLogMessage(StringHash /*eventType*/, VariantMap &eventData) {
    using namespace LogMessage;

/*
 *  REDIRECT OUTPUT TO CHAT
 * if (!headless_)*/
        //ShowChatText(eventData[P_MESSAGE].GetString());
}

void AlgebraKart::HandleSendChat() {
    String text = textEdit_->GetText();
    if (text.Empty())
        return; // Do not send an empty message

    auto *network = GetSubsystem<Network>();
    Connection *serverConnection = network->GetServerConnection();

    if (serverConnection) {
        // A VectorBuffer object is convenient for constructing a message to send
        VectorBuffer msg;
        msg.WriteString(text);
        // Send the chat message as in-order and reliable
        serverConnection->SendMessage(MSG_CHAT, true, true, msg);

        // Empty the text edit after sending
        textEdit_->SetText(String::EMPTY);
    }
}

void AlgebraKart::HandleNetworkMessage(StringHash /*eventType*/, VariantMap &eventData) {
    auto *network = GetSubsystem<Network>();

    using namespace NetworkMessage;

    int msgID = eventData[P_MESSAGEID].GetInt();

    //URHO3D_LOGINFOF("HandleNetworkMessage: msgID -> %d", msgID);

    if (msgID == MSG_CHAT) {
        const PODVector<unsigned char> &data = eventData[P_DATA].GetBuffer();
        // Use a MemoryBuffer to read the message data so that there is no unnecessary copying
        MemoryBuffer msg(data);
        String text = msg.ReadString();

        // If we are the server, prepend the sender's IP address and port and echo to everyone
        // If we are a client, just display the message
        if (network->IsServerRunning()) {
            auto *sender = static_cast<Connection *>(eventData[P_CONNECTION].GetPtr());

            /// Retrieve the username for the client
            VariantMap identity = sender->GetIdentity();
            String username = String(identity["UserName"]);

            VectorBuffer sendMsg;
            sendMsg.WriteString(text);

            // Server will take uncensored chat text

            // 1. Run through safety filter
            // 2. Temporary store chat log
            // 3. Broadcast to group

            // Broadcast using remote event
            using namespace ChatMessage;

            // Send the event forward
            VariantMap &newEventData = GetEventDataMap();

            // name: username
            // ip: sender->ToString();

            // Chat data
            newEventData[P_USER] = username;

            // Apply safety filter
            for (int i = 0; i < swearWords->Length(); i++) {
                if (text.Contains(swearWords[i], false)) {
                    String replaceWord = "";

                    int len = swearWords[i].Length();
                    for (int j = 0; j < len; j++) {
                        replaceWord += "*";
                    }
                    text.Replace(swearWords[i], replaceWord);
                }
            }

            // Send censored chat
            newEventData[P_MESSAGE] = text;

            // Broadcast to group
            for (SharedPtr<Connection> c : network->GetClientConnections()) {
                c->SendRemoteEvent(E_CHATMESSAGE, true, newEventData);
            }

            String textStr = "[" + newEventData[P_USER].GetString() + "]: " + newEventData[P_MESSAGE].GetString();
            ShowChatText(textStr);
        }
    }
}

String AlgebraKart::SaveScene(bool initial) {
    String filename = "AlgebraKart_sceneExport";
    if (!initial)
        filename += "InGame";
    File saveFile(context_, GetSubsystem<FileSystem>()->GetProgramDir() + "Data/Scenes/" + filename + ".xml",
                  FILE_WRITE);
    scene_->SaveXML(saveFile);
    return saveFile.GetName();
}

void AlgebraKart::SetupMinimapViewport()
{
    if (!scene_)
        return;

    auto* graphics = GetSubsystem<Graphics>();
    auto* renderer = GetSubsystem<Renderer>();

    // Create minimap camera node
    minimapCamNode_ = scene_->CreateChild("MinimapCamera", LOCAL);
    minimapCam_ = minimapCamNode_->CreateComponent<Camera>();

    // Set up orthographic projection for top-down view
    minimapCam_->SetOrthographic(true);
    minimapCam_->SetOrthoSize(minimapZoomLevel_);  // Start with default zoom
    minimapCam_->SetNearClip(1.0f);
    minimapCam_->SetFarClip(5000.0f);

    // Set aspect ratio to maintain square minimap
    minimapCam_->SetAspectRatio(1.0f);

    // Position camera above the track center looking down
    minimapCamNode_->SetPosition(Vector3(0.0f, 2000.0f, 0.0f));
    minimapCamNode_->SetRotation(Quaternion(90.0f, 0.0f, 0.0f));

    // Create minimap viewport (viewport 4)
    minimapViewport_ = new Viewport(context_, scene_, minimapCam_);

    // Position minimap in top-right corner with padding
    int minimapSizePixels = (int)minimapSize_;
    int viewportX = graphics->GetWidth() - minimapSizePixels - (int)minimapBorderPadding_ - 0.0f;
    int viewportY = (int)minimapBorderPadding_;
    IntRect minimapRect(viewportX, viewportY, viewportX + minimapSizePixels, viewportY + minimapSizePixels);
    minimapViewport_->SetRect(minimapRect);

    // Set render path for minimap (simpler rendering)
    minimapViewport_->SetRenderPath(renderer->GetViewport(0)->GetRenderPath());



    // Create a custom render path for minimap (optional - for better performance)
    SharedPtr<RenderPath> renderPath = SharedPtr<RenderPath>(new RenderPath());
    renderPath->Load(GetSubsystem<ResourceCache>()->GetResource<XMLFile>("RenderPaths/Forward.xml"));
    // Remove unnecessary render passes for minimap
    renderPath->RemoveCommands("BloomHDR");
    renderPath->RemoveCommands("Bloom");
    renderPath->RemoveCommands("FXAA2");
    minimapViewport_->SetRenderPath(renderPath);

    // Add the viewport
    renderer->SetViewport(4, minimapViewport_);



    // Initialize smooth following position
    minimapTargetPosition_ = Vector3::ZERO;

    URHO3D_LOGINFO("Enhanced minimap viewport created");
}

void AlgebraKart::UpdateMinimap(float timeStep)
{
    if (!minimapCam_ || !scene_)
        return;

    // Find the local player's position
    Vector3 playerPos = Vector3::ZERO;
    float playerYaw = 0.0f;
    float playerSpeed = 0.0f;
    bool playerFound = false;

    // Check if we're on server or client
    if (isServer_)
    {
        // Server: Follow the last connected player or a selected AI bot
        if (camMode_ > 0 && camMode_ <= aiActorMap_.Size())
        {
            // Following an AI bot
            auto* actor = aiActorMap_[camMode_ - 1].Get();
            if (actor && actor->GetNode())
            {
                playerPos = actor->GetNode()->GetWorldPosition();
                playerYaw = actor->GetNode()->GetRotation().YawAngle();
                playerFound = true;
            }
        }
        else if (lastConnection_ && actorMap_.Contains(lastConnection_))
        {
            // Following the last connected player
            ClientObj* actor = actorMap_[lastConnection_];
            if (actor && actor->GetNode())
            {
                playerPos = actor->GetNode()->GetWorldPosition();
                playerYaw = actor->GetNode()->GetRotation().YawAngle();
                playerFound = true;
            }
        }
    }
    else
    {
        // Client: Follow the local player
        Network* network = GetSubsystem<Network>();
        Connection* serverConnection = network->GetServerConnection();

        if (serverConnection)
        {
            String actorName = String("actor-") + clientName_;
            Node* actorNode = scene_->GetChild(actorName);

            if (actorNode)
            {
                ClientObj* actor = actorNode->GetDerivedComponent<ClientObj>();
                if (actor)
                {
                    NetworkActor* na = actorNode->GetDerivedComponent<NetworkActor>();
                    if (na)
                    {
                        // Get position from vehicle if in one, otherwise from actor
                        if (na->onVehicle_ && na->vehicle_)
                        {
                            playerPos = na->vehicle_->GetNode()->GetWorldPosition();
                            playerYaw = na->vehicle_->GetNode()->GetRotation().YawAngle();
                            playerSpeed = na->vehicle_->GetSpeedKmH();
                        }
                        else
                        {

                            playerPos = na->GetBody()->GetPosition();
                            playerYaw = na->GetNode()->GetRotation().YawAngle();
                            playerSpeed = na->GetBody()->GetLinearVelocity().Length();
                            //URHO3D_LOGDEBUGF("Player found at position: %s", playerPos.ToString().CString());
                        }
                        playerFound = true;
                    }
                }
            }
        }
    }

    if (playerFound)
    {
        float dynamicZoom = minimapZoomLevel_ + (playerSpeed * 0.2f); // Zoom out slightly at high speed
        minimapCam_->SetOrthoSize(Clamp(dynamicZoom, minimapMinZoom_, minimapMaxZoom_));

        // Smooth camera following
        minimapTargetPosition_ = minimapTargetPosition_.Lerp(playerPos, minimapSmoothSpeed_ * timeStep);

        // Update camera position (keep Y high above)
        Vector3 camPos = minimapTargetPosition_;
        camPos.y_ = 2800.0f;  // Keep camera high above
        minimapCamNode_->SetPosition(camPos);

        // Handle minimap rotation
        if (minimapRotateWithPlayer_)
        {
            // Rotate minimap with player heading (car-centric view)
            Quaternion rotation(90.0f, 0.0f, -playerYaw);
            minimapCamNode_->SetRotation(rotation);
        }
        else
        {
            // Keep minimap north-oriented
            minimapCamNode_->SetRotation(Quaternion(90.0f, 0.0f, 0.0f));
        }

        // Handle zoom controls (optional - you can bind these to keys)
        Input* input = GetSubsystem<Input>();
        if (input->GetKeyDown(KEY_KP_PLUS))
        {
            minimapZoomLevel_ = Max(minimapMinZoom_, minimapZoomLevel_ - 10.0f);
            minimapCam_->SetOrthoSize(minimapZoomLevel_);
        }
        if (input->GetKeyDown(KEY_KP_MINUS))
        {
            minimapZoomLevel_ = Min(minimapMaxZoom_, minimapZoomLevel_ + 10.0f);
            minimapCam_->SetOrthoSize(minimapZoomLevel_);
        }
    }

    // Update player/bot markers
    UpdateMinimapMarkers();

    // Optional: Draw minimap border and player direction indicator
    DrawMinimapOverlay();
}

void AlgebraKart::DrawMinimapOverlay()
{
    auto* graphics = GetSubsystem<Graphics>();
    auto* ui = GetSubsystem<UI>();

    if (!minimapBorderSprite_)
    {
        // Create border sprite if it doesn't exist
        auto* cache = GetSubsystem<ResourceCache>();

        // Create minimap border
        minimapBorderSprite_ = ui->GetRoot()->CreateChild<Sprite>();
        Texture2D* borderTexture = cache->GetResource<Texture2D>("Textures/hud/MinimapBorder.png");
        if (borderTexture)
        {
            minimapBorderSprite_->SetTexture(borderTexture);
            minimapBorderSprite_->SetSize((int)minimapSize_ + 4, (int)minimapSize_ + 4);
            minimapBorderSprite_->SetAlignment(HA_RIGHT, VA_TOP);
            minimapBorderSprite_->SetPosition(
                    -((int)minimapBorderPadding_ - 2 + minimapSize_),
                    (int)minimapBorderPadding_ - 2
            );
            minimapBorderSprite_->SetPriority(100);  // On top
        }

        // Create direction indicator (shows which way is forward when rotating)
        if (!minimapRotateWithPlayer_)
        {
            minimapDirectionSprite_ = ui->GetRoot()->CreateChild<Sprite>();
            Texture2D* arrowTexture = cache->GetResource<Texture2D>("Textures/hud/MinimapArrow.png");
            if (arrowTexture)
            {
                minimapDirectionSprite_->SetTexture(arrowTexture);
                minimapDirectionSprite_->SetSize(32, 32);
                minimapDirectionSprite_->SetAlignment(HA_RIGHT, VA_TOP);
                minimapDirectionSprite_->SetPosition(
                        -(int)(minimapBorderPadding_ + minimapSize_ / 2 - 16),
                        (int)(minimapBorderPadding_ + 10)
                );
                minimapDirectionSprite_->SetPriority(101);
            }
        }
    }
}

Vector3 AlgebraKart::CalculatePlayersCenterPosition()
{
    Vector3 center = Vector3::ZERO;
    int playerCount = 0;

    // Add connected players
    for (auto& pair : actorMap_)
    {
        if (pair.second_)
        {
            center += pair.second_->GetNode()->GetPosition();
            playerCount++;
        }
    }

    // Add AI bots
    for (auto& pair : aiActorMap_)
    {
        if (pair.second_)
        {
            center += pair.second_->GetNode()->GetPosition();
            playerCount++;
        }
    }

    if (playerCount > 0)
        center /= (float)playerCount;

    return center;
}

void AlgebraKart::CreateMinimapPlayerMarker(Connection* connection, const Color& color)
{
    if (!scene_)
        return;

    // Create marker node
    Node* markerNode = scene_->CreateChild("MinimapPlayerMarker", LOCAL);
    markerNode->SetScale(Vector3(20.0f, 20.0f, 20.0f));

    // Create visual representation (a simple colored box)
    StaticModel* model = markerNode->CreateComponent<StaticModel>();
    model->SetModel(GetSubsystem<ResourceCache>()->GetResource<Model>("Models/Box.mdl"));

    // Create or get material
    auto* cache = GetSubsystem<ResourceCache>();
    Material* material = cache->GetResource<Material>("Materials/VColUnlit.xml");
    if (!material)
    {
        // Create a simple unlit material if it doesn't exist
        material = new Material(context_);
        //material->SetTechnique(0, cache->GetResource<Technique>("Techniques/NoTexture.xml"));
        material->SetShaderParameter("MatDiffColor", color);
    }
    else
    {
        material = material->Clone();
        material->SetShaderParameter("MatDiffColor", color);
    }

    model->SetMaterial(material);

    // Store the marker
    minimapPlayerMap_[connection] = SharedPtr<Node>(markerNode);
}

void AlgebraKart::CreateMinimapBotMarker(unsigned int botId, const Color& color)
{
    if (!scene_)
        return;

    // Create marker node
    Node* markerNode = scene_->CreateChild("MinimapBotMarker", LOCAL);
    markerNode->SetScale(Vector3(15.0f, 15.0f, 15.0f));

    // Create visual representation (a simple colored sphere for bots)
    StaticModel* model = markerNode->CreateComponent<StaticModel>();
    model->SetModel(GetSubsystem<ResourceCache>()->GetResource<Model>("Models/Sphere.mdl"));

    // Create material
    auto* cache = GetSubsystem<ResourceCache>();
    Material* material = cache->GetResource<Material>("Materials/VColUnlit.xml");
    if (!material)
    {
        material = new Material(context_);
        //material->SetTechnique(0, cache->GetResource<Technique>("Techniques/NoTexture.xml"));
        material->SetShaderParameter("MatDiffColor", color);
    }
    else
    {
        material = material->Clone();
        material->SetShaderParameter("MatDiffColor", color);
    }

    model->SetMaterial(material);

    // Store the marker
    minimapBotMap_[botId] = SharedPtr<Node>(markerNode);
}

void AlgebraKart::UpdateMinimapMarkers()
{
    // Get minimap camera position for visibility culling
    Vector3 minimapCamPos = minimapCamNode_->GetWorldPosition();
    float visibleRange = minimapZoomLevel_ * 1.5f;  // Show markers slightly beyond visible area

    // Update player markers
    for (auto& pair : minimapPlayerMap_)
    {
        Connection* connection = pair.first_;
        Node* marker = pair.second_;

        if (marker && actorMap_.Contains(connection))
        {
            ClientObj* actor = actorMap_[connection];
            if (actor && actor->GetNode())
            {
                Vector3 worldPos = actor->GetNode()->GetWorldPosition();

                // Check if marker is within visible range
                float distance = (worldPos - minimapCamPos).Length();
                if (distance < visibleRange)
                {
                    marker->SetEnabled(true);
                    marker->SetPosition(Vector3(worldPos.x_, worldPos.y_ + 50.0f, worldPos.z_));
                    marker->SetRotation(actor->GetNode()->GetRotation());

                    // Scale marker based on distance (optional)
                    float scale = 1.0f - (distance / visibleRange) * 0.3f;
                    marker->SetScale(Vector3(20.0f * scale, 20.0f * scale, 20.0f * scale));
                }
                else
                {
                    marker->SetEnabled(false);  // Hide markers outside visible range
                }
            }
        }
    }

    // Update bot markers
    for (auto& pair : minimapBotMap_)
    {
        unsigned int botId = pair.first_;
        Node* marker = pair.second_;

        if (marker && aiActorMap_.Contains(botId))
        {
            ClientObj* actor = aiActorMap_[botId];
            if (actor && actor->GetNode())
            {
                Vector3 worldPos = actor->GetNode()->GetWorldPosition();

                // Check if marker is within visible range
                float distance = (worldPos - minimapCamPos).Length();
                if (distance < visibleRange)
                {
                    marker->SetEnabled(true);
                    marker->SetPosition(Vector3(worldPos.x_, worldPos.y_ + 50.0f, worldPos.z_));
                    marker->SetRotation(actor->GetNode()->GetRotation());

                    // Scale marker based on distance
                    float scale = 1.0f - (distance / visibleRange) * 0.3f;
                    marker->SetScale(Vector3(15.0f * scale, 15.0f * scale, 15.0f * scale));
                }
                else
                {
                    marker->SetEnabled(false);
                }
            }
        }
    }
}

void AlgebraKart::CalculateTrackBounds()
{
    // Calculate track bounds based on scene content
    // This is a simple implementation - you might want to make it more sophisticated

    BoundingBox sceneBounds;

    // Include all track elements in bounds calculation
    PODVector<Node*> trackNodes;
    scene_->GetChildrenWithComponent<StaticModel>(trackNodes, true);

    for (Node* node : trackNodes)
    {
        if (node->GetName().Contains("Track") || node->GetName().Contains("Ground") ||
            node->GetName().Contains("RaceTrack") || node->GetName().Contains("Earth"))
        {
            // Get the StaticModel component to access the bounding box
            StaticModel* model = node->GetComponent<StaticModel>();
            if (model && model->GetModel())
            {
                // Get the world bounding box from the drawable component
                BoundingBox worldBounds = model->GetWorldBoundingBox();
                sceneBounds.Merge(worldBounds);
            }

            /*else
            {
                // Fallback: use node position with estimated size
                Vector3 nodePos = node->GetWorldPosition();
                Vector3 nodeScale = node->GetWorldScale();
                float estimatedSize = Max(nodeScale.x_, Max(nodeScale.y_, nodeScale.z_)) * 100.0f;

                BoundingBox nodeBounds(nodePos - Vector3(estimatedSize), nodePos + Vector3(estimatedSize));
                sceneBounds.Merge(nodeBounds);
            }*/
        }
    }

    if (sceneBounds.Defined())
    {
        trackCenter_ = sceneBounds.Center();
        Vector3 size = sceneBounds.Size();
        trackBounds_ = Max(size.x_, size.z_) * 0.6f; // Use 60% of the largest dimension

        // Ensure minimum bounds
        if (trackBounds_ < 500.0f)
            trackBounds_ = 1000.0f;
    }
    else
    {
        // Default values if no track found - look for existing markers
        if (starAMarkerSet_ && starBMarkerSet_)
        {
            trackCenter_ = (starAMarker_ + starBMarker_) * 0.5f;
            trackBounds_ = (starBMarker_ - starAMarker_).Length() * 2.0f;
        }
        else
        {
            trackCenter_ = Vector3::ZERO;
            trackBounds_ = 1500.0f; // Larger default for better visibility
        }
    }

    URHO3D_LOGINFOF("Track bounds calculated: center=(%f,%f,%f), bounds=%f",
                    trackCenter_.x_, trackCenter_.y_, trackCenter_.z_, trackBounds_);
}

Controls AlgebraKart::sample_input() {
    auto ui = GetSubsystem<UI>();
    auto input = GetSubsystem<Input>();

    Controls controls;

    // Copy mouse yaw
    controls.yaw_ = yaw_;

    // Only apply WASD controls if there is no focused UI element
    if (!ui->GetFocusElement()) {
        controls.Set(CTRL_FORWARD, input->GetKeyDown(KEY_W));
        controls.Set(CTRL_BACK, input->GetKeyDown(KEY_S));
        controls.Set(CTRL_LEFT, input->GetKeyDown(KEY_A));
        controls.Set(CTRL_RIGHT, input->GetKeyDown(KEY_D));
    }

    if (input->GetKeyDown(Urho3D::KEY_KP_ENTER)) {
        if (!textEdit_->GetText().Empty()) {
            // Send chat
            HandleSendChat();
        }
    }

    return controls;
}


void AlgebraKart::apply_input(Node *actorNode, const Controls &controls) {


}

void AlgebraKart::apply_input(Connection *connection, const Controls &controls) {
/*
    auto ballNode = serverObjects_[connection];
    if (!ballNode)
        return;

    apply_input(ballNode, controls);*/
}

void AlgebraKart::HandleClientIdentity(StringHash eventType, VariantMap& eventData) {
    // SERVER CODE
    using namespace ClientIdentity;
    // Get connection from the client
    Connection* connection = static_cast<Connection*>(eventData[P_CONNECTION].GetPtr());
    // Spawn actor
    SpawnPlayer(connection);
    // Send server update
    //connection->SendServerUpdate();
    // Wait 3 seconds
    // Store connect with time wait
    welcomeMsgMap_.Populate(connection, 1.5f);
    // Server: send a delayed loaded message to client
    sendSpawnMsg_.Push(connection);

    // Send scene to the client
    connection->SetScene(scene_);
}

void AlgebraKart::SendWelcomeMessage(Connection* connection) {
        Server *server = GetSubsystem<Server>();

        /// Retrieve the username for the client
        VariantMap identity = connection->GetIdentity();
        String username = String(identity["UserName"]);

        // Store in server local login list
        loginList_.Push(username);

        for (int i = 0; i < bzRadioTracksArtistName.size(); i++) {
            // Shuffle to next track
            currTrack_ = Random(0, bzRadioTracksTrackName.size());
            server->SendRadioTrackNextMsg(connection, bzRadioTracksArtistName[currTrack_].c_str(),
                                          bzRadioTracksArtistLink[currTrack_].c_str(),
                                          bzRadioTracksTrackName[currTrack_].c_str()); // Radio track
        }


        // Send Server welcome message
        // Broadcast using remote event
        using namespace ChatMessage;
        // Send the event forward
        VariantMap &newEventData = GetEventDataMap();
        // name: username
        // ip: sender->ToString();

        // Chat data
        newEventData[P_USER] = "SERVER";
        String text = "Welcome " + username + " to AlgebraKart world!";
        // Send censored chat
        newEventData[P_MESSAGE] = text;

        auto network = GetSubsystem<Network>();
        // Broadcast to group
        for (SharedPtr<Connection> c: network->GetClientConnections()) {
            c->SendRemoteEvent(E_CHATMESSAGE, true, newEventData);
        }

        connection->SendServerUpdate();
}

void AlgebraKart::HandleSceneUpdate(StringHash eventType, VariantMap &eventData) {
    // Move the camera by touch, if the camera node is initialized by descendant sample class
    if (cameraNode_) {
    }

    URHO3D_LOGINFOF("HandleSceneUpdate");

}

// Spawn Player (Admin on Server)
SharedPtr<Node> AlgebraKart::SpawnPlayer() {
    // SERVER CODE (ADMIN PLAYER)NetworkActor>

    // Create a new network actor for the player
    SharedPtr<Node> networkActorNode(scene_->CreateChild("actor-SERVER"));
    NetworkActor *actor = networkActorNode->CreateComponent<NetworkActor>();
    actor->Init(networkActorNode);
    String name = String(String("actor-SERVER"));
    actor->SetClientInfo(name, 99, Vector3(0,-6,0));
    actor->SetProjectileManager(globalMissileManager_);

    // Assign server recorder to each client sequencer
    Server *server = GetSubsystem<Server>();
    SharedPtr<Recorder> serverRec_ = server->GetRecorder();
    actor->GetSequencer()->SetServerRec(serverRec_);

    // 2. Create a new camera following the actor
    Graphics* graphics = GetSubsystem<Graphics>();
    String cameraname = "camera_actor-server";
    SharedPtr<Node>cameraNode(networkActorNode->CreateChild(cameraname));
    cameraNode->SetPosition(Vector3(0.0f, 3000.0f, 0.0f));

    // Define server cam views
    //heliCamView_ = Vector3(0.0f,2200.0f,0.0f);
    heliCamView_ = Vector3(0.0f,50.0f,0.0f);


    // Set camera node
    cameraNode_ = scene_->CreateChild("Camera", LOCAL);
    serverCam_ = cameraNode_->CreateComponent<Camera>();
    serverCam_->GetNode()->SetRotation(Quaternion(0.0f, 30.0f, 0.0f));
    serverCam_->SetOrthographic(true);
    serverCam_->SetOrthoSize(230.0);
//    serverCam_->SetFarClip(48000.0f);
    serverCam_->SetFarClip(960000.0f);

    serverCam_->SetFillMode(Urho3D::FILL_SOLID);
    //serverCam_->SetFillMode(Urho3D::FILL_WIREFRAME);
    serverCam_->GetNode()->SetPosition(heliCamView_);
//    serverCam_->GetNode()->SetRotation(Quaternion(90.0f, 0.0f, 90.0f));
//    serverCam_->GetNode()->SetRotation(Quaternion(0.0f, 30.0f, 0.0f));

    // Enable for 3D sounds to work (attach to camera node)
    //SoundListener *listener = serverCam_->GetNode()->CreateComponent<SoundListener>();
    //GetSubsystem<Audio>()->SetListener(listener);

    // Setup game viewport
    if (!headless_)
    SetupGameViewports();


    return networkActorNode;
}

void AlgebraKart::InitializeAudioUISystem()
{
    URHO3D_LOGINFO("Initializing Enhanced Audio UI System...");

    // Create beat sequencer UI
    beatSequencerUI_ = new BeatSequencerUI(context_);
    beatSequencerUI_->Initialize();
    beatSequencerUI_->SetVisible(false); // Start hidden

    // Create synthesizer controls UI
    synthControlsUI_ = new SynthesizerControlsUI(context_);
    synthControlsUI_->Initialize();
    synthControlsUI_->SetVisible(false); // Start hidden

    // Create master audio control panel
    CreateMasterAudioPanel();

    // Subscribe to UI events
    SubscribeToEvent("BeatToggled", URHO3D_HANDLER(AlgebraKart, HandleBeatToggled));
    SubscribeToEvent("SequencerPlayToggle", URHO3D_HANDLER(AlgebraKart, HandleSequencerPlayToggle));
    SubscribeToEvent("SequencerStop", URHO3D_HANDLER(AlgebraKart, HandleSequencerStop));
    SubscribeToEvent("SequencerRecordToggle", URHO3D_HANDLER(AlgebraKart, HandleSequencerRecordToggle));

    // Synthesizer events
    SubscribeToEvent("SynthOscillatorChanged", URHO3D_HANDLER(AlgebraKart, HandleSynthParameterChanged));
    SubscribeToEvent("SynthFilterChanged", URHO3D_HANDLER(AlgebraKart, HandleSynthParameterChanged));
    SubscribeToEvent("SynthEnvelopeChanged", URHO3D_HANDLER(AlgebraKart, HandleSynthParameterChanged));
    SubscribeToEvent("SynthEffectsChanged", URHO3D_HANDLER(AlgebraKart, HandleSynthParameterChanged));

    URHO3D_LOGINFO("Audio UI System initialized successfully");
}

void AlgebraKart::CreateMasterAudioPanel()
{
    auto* ui = GetSubsystem<UI>();
    auto* graphics = GetSubsystem<Graphics>();
    auto* cache = GetSubsystem<ResourceCache>();

    // Create master audio panel
    audioMasterPanel_ = ui->GetRoot()->CreateChild<UIElement>();
    audioMasterPanel_->SetName("MasterAudioPanel");
    audioMasterPanel_->SetSize(300, 80);
    audioMasterPanel_->SetPosition(graphics->GetWidth() - 320, graphics->GetHeight() - 100);
    audioMasterPanel_->SetOpacity(0.9f);
    audioMasterPanel_->SetLayoutMode(LM_VERTICAL);
    audioMasterPanel_->SetLayoutSpacing(5);

    // Background
    auto* background = audioMasterPanel_->CreateChild<BorderImage>();
    background->SetSize(300, 80);
    background->SetColor(Color(0.1f, 0.1f, 0.15f, 0.8f));
    background->SetPriority(-1);

    // Title row
    auto* titleRow = audioMasterPanel_->CreateChild<UIElement>();
    titleRow->SetLayoutMode(LM_HORIZONTAL);
    titleRow->SetLayoutSpacing(10);
    titleRow->SetMinHeight(25);

    auto* titleText = titleRow->CreateChild<Text>();
    titleText->SetText("AUDIO MASTER");
    titleText->SetFont(cache->GetResource<Font>("Fonts/Anonymous Pro.ttf"), 12);
    titleText->SetColor(Color::WHITE);

    // Status indicator
    audioStatusText_ = titleRow->CreateChild<Text>();
    audioStatusText_->SetText("● ACTIVE");
    audioStatusText_->SetFont(cache->GetResource<Font>("Fonts/Anonymous Pro.ttf"), 10);
    audioStatusText_->SetColor(Color::GREEN);
    audioStatusText_->SetAlignment(HA_RIGHT, VA_CENTER);

    // Control row
    auto* controlRow = audioMasterPanel_->CreateChild<UIElement>();
    controlRow->SetLayoutMode(LM_HORIZONTAL);
    controlRow->SetLayoutSpacing(10);
    controlRow->SetMinHeight(25);

    // Sequencer toggle button
    auto* seqButton = controlRow->CreateChild<Button>();
    seqButton->SetName("SequencerToggleButton");
    seqButton->SetMinSize(60, 25);
    seqButton->SetMaxSize(60, 25);

    auto* seqButtonText = seqButton->CreateChild<Text>();
    seqButtonText->SetText("SEQ");
    seqButtonText->SetFont(cache->GetResource<Font>("Fonts/Anonymous Pro.ttf"), 10);
    seqButtonText->SetAlignment(HA_CENTER, VA_CENTER);
    seqButtonText->SetColor(Color::WHITE);
    seqButton->SetStyleAuto();

    SubscribeToEvent(seqButton, E_RELEASED, URHO3D_HANDLER(AlgebraKart, HandleBeatSequencerToggle));

    // Synthesizer toggle button
    auto* synthButton = controlRow->CreateChild<Button>();
    synthButton->SetName("SynthToggleButton");
    synthButton->SetMinSize(60, 25);
    synthButton->SetMaxSize(60, 25);

    auto* synthButtonText = synthButton->CreateChild<Text>();
    synthButtonText->SetText("SYNTH");
    synthButtonText->SetFont(cache->GetResource<Font>("Fonts/Anonymous Pro.ttf"), 10);
    synthButtonText->SetAlignment(HA_CENTER, VA_CENTER);
    synthButtonText->SetColor(Color::WHITE);
    synthButton->SetStyleAuto();

    SubscribeToEvent(synthButton, E_RELEASED, URHO3D_HANDLER(AlgebraKart, HandleSynthControlsToggle));

    // Master volume control
    auto* volumeContainer = controlRow->CreateChild<UIElement>();
    volumeContainer->SetLayoutMode(LM_VERTICAL);
    volumeContainer->SetMinWidth(100);

    auto* volumeLabel = volumeContainer->CreateChild<Text>();
    volumeLabel->SetText("MASTER");
    volumeLabel->SetFont(cache->GetResource<Font>("Fonts/Anonymous Pro.ttf"), 8);
    volumeLabel->SetColor(Color::GRAY);
    volumeLabel->SetTextAlignment(HA_CENTER);

    masterVolumeSlider_ = volumeContainer->CreateChild<Slider>();
    masterVolumeSlider_->SetName("MasterVolumeSlider");
    masterVolumeSlider_->SetMinHeight(15);
    masterVolumeSlider_->SetRange(1.0f);
    masterVolumeSlider_->SetValue(0.8f);
    masterVolumeSlider_->SetStyleAuto();

    SubscribeToEvent(masterVolumeSlider_, E_SLIDERCHANGED, URHO3D_HANDLER(AlgebraKart, HandleMasterVolumeChanged));
}

void AlgebraKart::UpdateAudioUISystem(float timeStep)
{
    if (!beatSequencerUI_ || !synthControlsUI_) return;

    // Update beat sequencer if visible
    if (beatSequencerUI_->GetMainWindow()->IsVisible()) {
        UpdateUIFromSequencerState(timeStep);
    }

    // Update synthesizer oscilloscope if visible
    if (synthControlsUI_->GetMainWindow()->IsVisible()) {
        UpdateSynthesizerWaveformDisplay();
    }

    // Update master panel status
    UpdateMasterPanelStatus();

    // Process real-time audio analysis
    ProcessRealtimeAudioAnalysis();
}

void AlgebraKart::UpdateUIFromSequencerState(float timeStep)
{
    // Get current sequencer state from the active player
    if (!isSnapped_) return;

    String actorName = String("actor-") + clientName_;
    Node *actorNode = scene_->GetChild(actorName);

    if (actorNode) {
        ClientObj *actor = actorNode->GetDerivedComponent<ClientObj>();
        if (actor && actor->GetSequencer()) {
            Sequencer* sequencer = actor->GetSequencer();

            // Get current beat and timing info
            int currentBeat = sequencer->GetBeat();
            float beatTime = sequencer->GetBeatTime();
            bool isPlaying = !sequencer->IsMute();

            // Update the sequencer UI
            beatSequencerUI_->UpdateSequencer(timeStep, currentBeat, beatTime, isPlaying);

            // Update spectrum analyzer
            if (analyzer_ && !analyzer_->spectrum().Empty()) {
                beatSequencerUI_->UpdateSpectrum(analyzer_->spectrum());
            }

            // Update VU meters using captured audio
            if (capturer_ && !capturer_->getBuffer()->Empty()) {
                float leftLevel = CalculateChannelLevel(0, *capturer_->getBuffer());
                float rightLevel = CalculateChannelLevel(1, *capturer_->getBuffer());
                beatSequencerUI_->UpdateVUMeters(leftLevel, rightLevel);
            }
        }
    }
}

void AlgebraKart::UpdateSynthesizerWaveformDisplay()
{
    // Generate synthetic waveform for oscilloscope display
    // In a real implementation, you'd capture this from your synthesizer output

    const int waveformSize = 200;
    realtimeWaveform_.Resize(waveformSize);

    static float phase = 0.0f;
    float frequency = 440.0f; // A4 note
    float sampleRate = 44100.0f;

    // Generate a simple sine wave for demonstration
    for (int i = 0; i < waveformSize; i++) {
        float t = phase + (i / sampleRate);
        realtimeWaveform_[i] = Sin(2.0f * M_PI * frequency * t) * 0.5f;
    }

    phase += waveformSize / sampleRate;
    if (phase > 1.0f) phase -= 1.0f;

    // Update the oscilloscope
    synthControlsUI_->UpdateOscilloscope(realtimeWaveform_);
}

void AlgebraKart::UpdateMasterPanelStatus()
{
    if (!audioStatusText_) return;

    // Update audio system status
    bool audioActive = false;
    String statusText = "● INACTIVE";
    Color statusColor = Color::RED;

    if (isSnapped_) {
        String actorName = String("actor-") + clientName_;
        Node *actorNode = scene_->GetChild(actorName);

        if (actorNode) {
            ClientObj *actor = actorNode->GetDerivedComponent<ClientObj>();
            if (actor && actor->GetSequencer() && !actor->GetSequencer()->IsMute()) {
                audioActive = true;
                statusText = "● PLAYING";
                statusColor = Color::GREEN;
            }
        }
    }

    // Check if radio is playing
    if (_radioStream && _radioStream->IsPlaying()) {
        statusText = "● RADIO";
        statusColor = Color::YELLOW;
        audioActive = true;
    }

    audioStatusText_->SetText(statusText);
    audioStatusText_->SetColor(statusColor);
}

void AlgebraKart::ProcessRealtimeAudioAnalysis()
{
    // This method processes audio in real-time for UI visualization
    if (!analyzer_ || !capturer_) return;

    // Update spectrum analysis
    if (audioSpectrumOptions_.inputSize < capturer_->bufferReadCount()) {
        analyzer_->update(capturer_->getBuffer(), capturer_->bufferHeadIndex(),
                          audioSpectrumOptions_.bottomLevel,
                          audioSpectrumOptions_.topLevel, audioSpectrumOptions_.minFreq,
                          audioSpectrumOptions_.maxFreq, audioSpectrumOptions_.axisLogBase);
    }

    // Store current spectrum for UI updates
    if (!analyzer_->spectrum().Empty()) {
        realtimeSpectrum_ = analyzer_->spectrum();
    }
}

// Event Handlers

void AlgebraKart::HandleBeatSequencerToggle(StringHash eventType, VariantMap& eventData)
{
    if (beatSequencerUI_) {
        bool visible = beatSequencerUI_->GetMainWindow()->IsVisible();
        beatSequencerUI_->SetVisible(!visible);

        URHO3D_LOGINFOF("Beat sequencer %s", visible ? "hidden" : "shown");

        // Update button appearance
        auto* button = static_cast<Button*>(eventData[Released::P_ELEMENT].GetPtr());
        if (button) {
            auto* text = button->GetChild("Text", true);
            if (text) {
                static_cast<Text*>(text)->SetColor(visible ? Color::WHITE : Color::GREEN);
            }
        }
    }
}

void AlgebraKart::HandleSynthControlsToggle(StringHash eventType, VariantMap& eventData)
{
    if (synthControlsUI_) {
        bool visible = synthControlsUI_->GetMainWindow()->IsVisible();
        synthControlsUI_->SetVisible(!visible);

        URHO3D_LOGINFOF("Synthesizer controls %s", visible ? "hidden" : "shown");

        // Update button appearance
        auto* button = static_cast<Button*>(eventData[Released::P_ELEMENT].GetPtr());
        if (button) {
            auto* text = button->GetChild("Text", true);
            if (text) {
                static_cast<Text*>(text)->SetColor(visible ? Color::WHITE : Color::GREEN);
            }
        }
    }
}

void AlgebraKart::HandleMasterVolumeChanged(StringHash eventType, VariantMap& eventData)
{
    float volume = masterVolumeSlider_->GetValue();

    // Apply to Urho3D audio system
    auto* audio = GetSubsystem<Audio>();
    audio->SetMasterGain(SOUND_MUSIC, volume * 0.3f);
    audio->SetMasterGain(SOUND_EFFECT, volume * 0.3f);

    URHO3D_LOGINFOF("Master volume set to: %.2f", volume);
}

void AlgebraKart::HandleSynthParameterChanged(StringHash eventType, VariantMap& eventData)
{
    // Apply synthesizer parameter changes to your actual Synthesizer component
    ApplySynthParametersToSynthesizer(eventData);
}

// Integration Methods

void AlgebraKart::ApplyBeatPatternToSequencer(int channel, int step, bool active)
{
    // Apply beat pattern changes to your existing Sequencer
    if (!isSnapped_) return;

    String actorName = String("actor-") + clientName_;
    Node *actorNode = scene_->GetChild(actorName);

    if (actorNode) {
        ClientObj *actor = actorNode->GetDerivedComponent<ClientObj>();
        if (actor && actor->GetSequencer()) {
            // Here you would modify your sequencer's pattern
            // This requires extending your Sequencer class to support dynamic patterns

            URHO3D_LOGINFOF("Applied beat change: Channel %d, Step %d, Active %s",
                            channel, step, active ? "true" : "false");

            // Example implementation (you'd need to add these methods to Sequencer):
            // actor->GetSequencer()->SetBeatPattern(channel, step, active);
        }
    }
}

void AlgebraKart::ApplySynthParametersToSynthesizer(const VariantMap& params)
{
    // Apply synthesizer parameters to your existing Synthesizer component
    if (!isSnapped_) return;

    String actorName = String("actor-") + clientName_;
    Node *actorNode = scene_->GetChild(actorName);

    if (actorNode) {
        ClientObj *actor = actorNode->GetDerivedComponent<ClientObj>();
        // You would need to add a GetSynthesizer() method to your actor
        // if (actor && actor->GetSynthesizer()) {
        //     Synthesizer* synth = actor->GetSynthesizer();
        //
        //     // Apply oscillator parameters
        //     if (params.Contains("Osc1Freq")) {
        //         synth->SetOscillator1Frequency(params["Osc1Freq"].GetFloat());
        //     }
        //
        //     // Apply filter parameters
        //     if (params.Contains("FilterCutoff")) {
        //         synth->SetFilterCutoff(params["FilterCutoff"].GetFloat());
        //     }
        //
        //     // Apply envelope parameters
        //     if (params.Contains("Attack")) {
        //         synth->SetAttack(params["Attack"].GetFloat());
        //     }
        //
        //     // Apply effects
        //     if (params.Contains("ReverbEnabled")) {
        //         synth->SetReverbEnabled(params["ReverbEnabled"].GetBool());
        //     }
        // }

        URHO3D_LOGINFO("Applied synthesizer parameter changes");
    }
}


// CLIENT CODE
void AlgebraKart::CreateClientUI() {
    if (headless_) {
        URHO3D_LOGDEBUG("Skipping client UI creation in headless mode");
        return;
    }

    clientLevelLoading_ = true;

    ResourceCache *cache = GetSubsystem<ResourceCache>();
    auto *graphics = GetSubsystem<Graphics>();

    auto* ui = GetSubsystem<UI>();

    /*
    // Create steering wheel sprites with proper positioning
    auto* steerWheelTexture = cache->GetResource<Texture2D>("Textures/steer-wheel.png");
    auto* steerActorTexture = cache->GetResource<Texture2D>("Textures/steer-actor.png");

    if (steerWheelTexture) {
        steerWheelSprite_ = ui->GetRoot()->CreateChild<Sprite>();
        steerWheelSprite_->SetTexture(steerWheelTexture);

        int textureWidth = steerWheelTexture->GetWidth();
        int textureHeight = steerWheelTexture->GetHeight();

        // Position steering wheel in bottom-left, avoiding other UI elements
        float wheelSize = 120.0f;
        float marginFromEdge = 30.0f;

        steerWheelSprite_->SetSize(wheelSize, wheelSize);
        steerWheelSprite_->SetScale(1.0f);
        steerWheelSprite_->SetHotSpot(wheelSize / 2, wheelSize / 2); // Center rotation point
        steerWheelSprite_->SetAlignment(HA_LEFT, VA_BOTTOM);
        steerWheelSprite_->SetPosition(Vector2(marginFromEdge + wheelSize/2,
                                               -(marginFromEdge + wheelSize/2)));
        steerWheelSprite_->SetOpacity(0.8f);
        steerWheelSprite_->SetPriority(-90);
        steerWheelSprite_->SetVisible(true); // Hidden by default

        URHO3D_LOGINFO("Steering wheel sprite created and positioned");
    }

    if (steerActorTexture) {
        steerActorSprite_ = ui->GetRoot()->CreateChild<Sprite>();
        steerActorSprite_->SetTexture(steerActorTexture);

        int textureWidth = steerActorTexture->GetWidth();
        int textureHeight = steerActorTexture->GetHeight();

        // Position actor indicator relative to steering wheel
        float actorSize = 80.0f;
        float marginFromEdge = 30.0f;

        steerActorSprite_->SetSize(actorSize, actorSize);
        steerActorSprite_->SetScale(1.0f);
        steerActorSprite_->SetHotSpot(actorSize / 2, actorSize / 2);
        steerActorSprite_->SetAlignment(HA_LEFT, VA_BOTTOM);
        steerActorSprite_->SetPosition(Vector2(marginFromEdge + 120.0f / 2,
                                               -(marginFromEdge + 120.0f / 2)));
        steerActorSprite_->SetOpacity(0.9f);
        steerActorSprite_->SetPriority(-85);
        steerActorSprite_->SetVisible(false); // Hidden by default

        URHO3D_LOGINFO("Steering actor sprite created and positioned");
    }*/


    /*
    DebugRenderer *dbgRenderer = scene_->CreateComponent<DebugRenderer>();
    // Client will non-authoratively managed physics locally based on server copy
    clientPhysicsWorld_ = scene_->CreateComponent<PhysicsWorld>(LOCAL);
    clientPhysicsWorld_->SetFps(60);
    clientPhysicsWorld_->DrawDebugGeometry(false);
    clientPhysicsWorld_->SetDebugRenderer(dbgRenderer);
    // Disable interpolation (need determinism)
    clientPhysicsWorld_->SetInterpolation(false);

    clientPhysicsWorld_->SetEnabled(false);
    clientPhysicsWorld_->SetUpdateEnabled(false);

    packetsIn_ = ui->GetRoot()->CreateChild<Text>();
    packetsIn_->SetText("Packets in : 0");
    packetsIn_->SetColor(Color(0.7, 0.5, 0.9));
    packetsIn_->SetFont(cache->GetResource<Font>(INGAME_FONT), 9);
    packetsIn_->SetAlignment(HA_RIGHT, VA_BOTTOM);
    packetsIn_->SetPosition(-100, -32);

    packetsOut_ = ui->GetRoot()->CreateChild<Text>();
    packetsOut_->SetText("Packets out: 0");
    packetsOut_->SetColor(Color(0.7, 0.5, 0.9));
    packetsOut_->SetFont(cache->GetResource<Font>(INGAME_FONT), 9);
    packetsOut_->SetAlignment(HA_RIGHT, VA_BOTTOM);
    packetsOut_->SetPosition(-100, -18);


    // Set the default UI style and font
    //ui->GetRoot()->SetDefaultStyle(cache->GetResource<XMLFile>("UI/DefaultStyle.xml"));
    auto *font = cache->GetResource<Font>("INGAME_FONT2");
    auto *font2 = cache->GetResource<Font>(INGAME_FONT);
    auto *font3 = cache->GetResource<Font>(INGAME_FONT3);
    auto *font4 = cache->GetResource<Font>(INGAME_FONT4);

    // Get powerbar texture
    Texture2D *powerBarTexture = cache->GetResource<Texture2D>("Textures/powerbar.png");
    if (!powerBarTexture)
        return;

    // Get powerbar background texture
    Texture2D *powerBarBkgTexture = cache->GetResource<Texture2D>("Textures/powerbar-bk.png");
    if (!powerBarBkgTexture)
        return;

    // Get RPM bar texture
    Texture2D *rpmBarTexture = cache->GetResource<Texture2D>("Textures/rpm.png");
    if (!rpmBarTexture)
        return;

    // Get RPM bar background texture
    Texture2D *rpmBarBkgTexture = cache->GetResource<Texture2D>("Textures/powerbar-bk.png");
    if (!rpmBarBkgTexture)
        return;

    // Get velocity bar texture
    Texture2D *velBarTexture = cache->GetResource<Texture2D>("Textures/velocity.png");
    if (!rpmBarTexture)
        return;

    // Get velocity bar background texture
    Texture2D *velBarBkgTexture = cache->GetResource<Texture2D>("Textures/powerbar-bk.png");
    if (!rpmBarBkgTexture)
        return;

    // Get HUD vehicle damage background texture
    Texture2D *hudVDamageTexture = cache->GetResource<Texture2D>("Textures/hud_v-damage.png");
    if (!hudVDamageTexture)
        return;

    // Get HUD vehicle damage indicator line texture
    Texture2D *hudVDamageLineTexture = cache->GetResource<Texture2D>("Textures/hud_vdamage-line.png");
    if (!hudVDamageLineTexture)
        return;

    // Get HUD pickup background texture
    Texture2D *hudPickupTexture = cache->GetResource<Texture2D>("Textures/hud_pickup.png");
    if (!hudPickupTexture)
        return;

    for (int id = 1; id < 7; id++) {
        // Get HUD pickup item texture
        Texture2D *hudPickupItemTexture = cache->GetResource<Texture2D>("Textures/pickups/pickup-0" + String(id) + ".png");
        if (!hudPickupItemTexture)
            continue;
        hudPickupItemTextures_.Push(hudPickupItemTexture);
    }

    // Get mini map p1 texture
    Texture2D *miniMapP1Texture = cache->GetResource<Texture2D>("Textures/minimap-p1.png");
    if (!miniMapP1Texture)
        URHO3D_LOGERROR("Missing texture!");

    // Get mini map waypoint texture
    Texture2D *miniMapWPTexture = cache->GetResource<Texture2D>("Textures/minimap-wp.png");
    if (!miniMapWPTexture)
        URHO3D_LOGERROR("Missing texture!");

    // Get mini map background texture
    Texture2D *miniMapBkgTexture = cache->GetResource<Texture2D>("Textures/minimap-bk.png");
    if (!miniMapBkgTexture)
        URHO3D_LOGERROR("Missing texture!");

    // Get marker map background texture
    Texture2D *markerMapBkgTexture = cache->GetResource<Texture2D>("Textures/MarkerMap.png");
    if (!markerMapBkgTexture)
        URHO3D_LOGERROR("Missing texture!");

    // Get genotype texture
    Texture2D *genotypeTexture = cache->GetResource<Texture2D>("Textures/genotype.png");
    if (!genotypeTexture)
        URHO3D_LOGERROR("Missing texture!");

    // Get genotype background texture
    Texture2D *genotypeBkgTexture = cache->GetResource<Texture2D>("Textures/genotype-bk.png");
    if (!genotypeBkgTexture)
        URHO3D_LOGERROR("Missing texture!");


    // Radio stream audio spectrum sprites
    radioVisualizerImg_ = new PixelImage(context_);
    radioSpectrumTexture_ = (new Texture2D(context_));
    radioVisualizerImg_->SetSize(radioSpectrumWidth_, radioSpectrumHeight_);
    radioSpectrumTexture_->SetData(radioVisualizerImg_, false);

    // Create VU meter UI
    CreateVUMeter();

    // Create sprite and add to the UI layout
    powerBarP1Sprite_ = ui->GetRoot()->CreateChild<Sprite>();
    powerBarBkgP1Sprite_ = ui->GetRoot()->CreateChild<Sprite>();
    rpmBarP1Sprite_ = ui->GetRoot()->CreateChild<Sprite>();
    rpmBarBkgP1Sprite_ = ui->GetRoot()->CreateChild<Sprite>();
    velBarP1Sprite_ = ui->GetRoot()->CreateChild<Sprite>();
    velBarBkgP1Sprite_ = ui->GetRoot()->CreateChild<Sprite>();

    // Vehicle damage HUD
    vDamageSprite_ = ui->GetRoot()->CreateChild<Sprite>();
    // Indicator Lines
    SharedPtr<Sprite> sprite_;
    for (int i = 0; i < VDAMAGE_BODY_NUM_LINES; i++) {
        sprite_ = ui->GetRoot()->CreateChild<Sprite>();
        sprite_->SetTexture(hudVDamageLineTexture);
        sprite_->SetScale(1.0f);
        sprite_->SetSize(hudVDamageLineTexture->GetWidth(), hudVDamageLineTexture->GetHeight());
        sprite_->SetHotSpot(hudVDamageLineTexture->GetWidth(), hudVDamageLineTexture->GetHeight());
        sprite_->SetPosition(Vector2(0, 0));
        sprite_->SetOpacity(0.9f);
        // Set a low priority so that other UI elements can be drawn on top
        sprite_->SetPriority(-100);
        sprite_->SetVisible(false);
        vDamage_FL_LineSprites_.Push(sprite_);

        sprite_ = ui->GetRoot()->CreateChild<Sprite>();
        sprite_->SetTexture(hudVDamageLineTexture);
        sprite_->SetScale(1.0f);
        sprite_->SetSize(hudVDamageLineTexture->GetWidth(), hudVDamageLineTexture->GetHeight());
        sprite_->SetHotSpot(hudVDamageLineTexture->GetWidth(), hudVDamageLineTexture->GetHeight());
        sprite_->SetPosition(Vector2(0, 0));
        sprite_->SetOpacity(0.9f);
        // Set a low priority so that other UI elements can be drawn on top
        sprite_->SetPriority(-100);
        sprite_->SetVisible(false);
        vDamage_FR_LineSprites_.Push(sprite_);

        sprite_ = ui->GetRoot()->CreateChild<Sprite>();
        sprite_->SetTexture(hudVDamageLineTexture);
        sprite_->SetScale(1.0f);
        sprite_->SetSize(hudVDamageLineTexture->GetWidth(), hudVDamageLineTexture->GetHeight());
        sprite_->SetHotSpot(hudVDamageLineTexture->GetWidth(), hudVDamageLineTexture->GetHeight());
        sprite_->SetPosition(Vector2(0, 0));
        sprite_->SetOpacity(0.9f);
        // Set a low priority so that other UI elements can be drawn on top
        sprite_->SetPriority(-100);
        sprite_->SetVisible(false);
        vDamage_BL_LineSprites_.Push(sprite_);

        sprite_ = ui->GetRoot()->CreateChild<Sprite>();
        sprite_->SetTexture(hudVDamageLineTexture);
        sprite_->SetScale(1.0f);
        sprite_->SetSize(hudVDamageLineTexture->GetWidth(), hudVDamageLineTexture->GetHeight());
        sprite_->SetHotSpot(hudVDamageLineTexture->GetWidth(), hudVDamageLineTexture->GetHeight());
        sprite_->SetPosition(Vector2(0, 0));
        sprite_->SetOpacity(0.9f);
        // Set a low priority so that other UI elements can be drawn on top
        sprite_->SetPriority(-100);
        sprite_->SetVisible(false);
        vDamage_BR_LineSprites_.Push(sprite_);

        sprite_ = ui->GetRoot()->CreateChild<Sprite>();
        sprite_->SetTexture(hudVDamageLineTexture);
        sprite_->SetScale(1.0f);
        sprite_->SetSize(hudVDamageLineTexture->GetWidth(), hudVDamageLineTexture->GetHeight());
        sprite_->SetHotSpot(hudVDamageLineTexture->GetWidth(), hudVDamageLineTexture->GetHeight());
        sprite_->SetPosition(Vector2(0, 0));
        sprite_->SetOpacity(0.9f);
        // Set a low priority so that other UI elements can be drawn on top
        sprite_->SetPriority(-100);
        sprite_->SetVisible(false);
        vDamage_FA_LineSprites_.Push(sprite_);

        sprite_ = ui->GetRoot()->CreateChild<Sprite>();
        sprite_->SetTexture(hudVDamageLineTexture);
        sprite_->SetScale(1.0f);
        sprite_->SetSize(hudVDamageLineTexture->GetWidth(), hudVDamageLineTexture->GetHeight());
        sprite_->SetHotSpot(hudVDamageLineTexture->GetWidth(), hudVDamageLineTexture->GetHeight());
        sprite_->SetPosition(Vector2(0, 0));
        sprite_->SetOpacity(0.9f);
        // Set a low priority so that other UI elements can be drawn on top
        sprite_->SetPriority(-100);
        sprite_->SetVisible(false);
        vDamage_RA_LineSprites_.Push(sprite_);
    }

    pickupSprite_ = ui->GetRoot()->CreateChild<Sprite>();
    pickupItemSprite_ = ui->GetRoot()->CreateChild<Sprite>();
    //miniMapP1Sprite_ = ui->GetRoot()->CreateChild<Sprite>();
    //miniMapWPSprite_ = ui->GetRoot()->CreateChild<Sprite>();
    //miniMapBkgSprite_ = ui->GetRoot()->CreateChild<Sprite>();
    //markerMapBkgSprite_ = ui->GetRoot()->CreateChild<Sprite>();
    //steerWheelSprite_ = ui->GetRoot()->CreateChild<Sprite>();
    //steerActorSprite_ = ui->GetRoot()->CreateChild<Sprite>();
    radioSpectrumSprite_ = ui->GetRoot()->CreateChild<Sprite>();

    // Create analog speedometer
    auto* speedometerBkgTexture = cache->GetResource<Texture2D>("Textures/hud/speedometer_bg.png");
    auto* speedometerNeedleTexture = cache->GetResource<Texture2D>("Textures/hud/speedometer_needle.png");

    if (speedometerBkgTexture) {
        speedometerBkgSprite_ = ui->GetRoot()->CreateChild<Sprite>();
        speedometerBkgSprite_->SetTexture(speedometerBkgTexture);

        int textureWidth = speedometerBkgTexture->GetWidth();
        int textureHeight = speedometerBkgTexture->GetHeight();

        // Position speedometer in bottom-right corner, but avoid overlap with other elements
        speedometerBkgSprite_->SetSize(textureWidth, textureHeight);
        speedometerBkgSprite_->SetScale(1.0f);
        speedometerBkgSprite_->SetHotSpot(textureWidth / 2, textureHeight / 2);
        speedometerBkgSprite_->SetAlignment(HA_CENTER, VA_CENTER);

        // Position away from pickup items and other HUD elements
        float speedometerX = (graphics->GetWidth()/2)-85.0f;//700.0f;
        float speedometerY = (graphics->GetHeight()/2)-320.0f;//450.0f;
        speedometerBkgSprite_->SetPosition(Vector2(speedometerX, speedometerY));
        speedometerBkgSprite_->SetOpacity(0.8f);
        speedometerBkgSprite_->SetPriority(-100);
        speedometerBkgSprite_->SetVisible(true); // Hidden by default
    }

    if (speedometerNeedleTexture) {
        speedometerNeedleSprite_ = ui->GetRoot()->CreateChild<Sprite>();
        speedometerNeedleSprite_->SetTexture(speedometerNeedleTexture);

        int textureWidth = speedometerNeedleTexture->GetWidth();
        int textureHeight = speedometerNeedleTexture->GetHeight();

        // Position needle at center of speedometer
        speedometerNeedleSprite_->SetSize(textureWidth, textureHeight);
        speedometerNeedleSprite_->SetScale(1.0f);
        speedometerNeedleSprite_->SetHotSpot(textureWidth / 2, textureHeight - 5); // Rotate around bottom center
        speedometerNeedleSprite_->SetAlignment(HA_CENTER, VA_CENTER);

        float speedometerX = (graphics->GetWidth()/2) - 85.0f;
        float speedometerY = (graphics->GetHeight()/2) - 327.0f;
        speedometerNeedleSprite_->SetPosition(Vector2(speedometerX, speedometerY));
        speedometerNeedleSprite_->SetOpacity(0.9f);
        speedometerNeedleSprite_->SetPriority(-85);
        speedometerNeedleSprite_->SetVisible(true); // Hidden by default
    }

    // Create digital speed text overlay
    speedometerText_ = ui->GetRoot()->CreateChild<Text>();
    speedometerText_->SetFont(cache->GetResource<Font>(INGAME_FONT3), 16);
    speedometerText_->SetTextAlignment(HA_CENTER);
    speedometerText_->SetColor(Color::WHITE);
    speedometerText_->SetAlignment(HA_CENTER, VA_CENTER);

    // Position text below the speedometer
    float speedometerX = (graphics->GetWidth()/2) - 85.0f;
    float speedometerY = (graphics->GetHeight()/2) - 240.0f;
    speedometerText_->SetPosition(IntVector2(speedometerX, speedometerY));
    speedometerText_->SetText("0 KM/H");
    speedometerText_->SetVisible(false);
    speedometerText_->SetPriority(-80);

    // Create enhanced beat sequencer UI
    beatSequencerUI_ = new BeatSequencerUI(context_);
    beatSequencerUI_->Initialize();

    // Subscribe to sequencer events
    SubscribeToEvent("BeatToggled", URHO3D_HANDLER(AlgebraKart, HandleBeatToggled));
    SubscribeToEvent("SequencerPlayToggle", URHO3D_HANDLER(AlgebraKart, HandleSequencerPlayToggle));
    SubscribeToEvent("SequencerStop", URHO3D_HANDLER(AlgebraKart, HandleSequencerStop));
    SubscribeToEvent("SequencerRecordToggle", URHO3D_HANDLER(AlgebraKart, HandleSequencerRecordToggle));

    // Initially hide the sequencer UI
    beatSequencerUI_->SetVisible(false);


  /*  sprite_ = new Sprite2D(context_);
    sprite_->SetTexture(texture);
    sprite_->SetRectangle(IntRect(info.x, info.y, info.x + info.image_->GetWidth(), info.y + info.image_->GetHeight()));
    sprite_->SetHotSpot(Vector2(info.file_->pivotX_, info.file_->pivotY_));
*/
/*
    int progBarSize = 15;

    // Set sprite texture
    powerBarP1Sprite_->SetTexture(powerBarTexture);
    powerBarBkgP1Sprite_->SetTexture(powerBarBkgTexture);
    rpmBarP1Sprite_->SetTexture(rpmBarTexture);
    rpmBarBkgP1Sprite_->SetTexture(rpmBarBkgTexture);
    velBarP1Sprite_->SetTexture(velBarTexture);
    velBarBkgP1Sprite_->SetTexture(rpmBarBkgTexture);
    vDamageSprite_->SetTexture(hudVDamageTexture);
    pickupSprite_->SetTexture(hudPickupTexture);
    pickupItemSprite_->SetTexture(hudPickupItemTextures_[0]);
    //miniMapP1Sprite_->SetTexture(miniMapP1Texture);
    //miniMapWPSprite_->SetTexture(miniMapWPTexture);
    //miniMapBkgSprite_->SetTexture(miniMapBkgTexture);
    //markerMapBkgSprite_->SetTexture(markerMapBkgTexture);
    steerWheelSprite_->SetTexture(steerWheelTexture);
    steerActorSprite_->SetTexture(steerActorTexture);
    radioSpectrumSprite_->SetTexture(radioSpectrumTexture_);



    float textOverlap = 245.0f;

    powerBarText_ = ui->GetRoot()->CreateChild<Text>("powerBarText");
    powerBarText_->SetAlignment(HA_LEFT, VA_TOP);
    powerBarText_->SetPosition(10.0f, 23.0);
    powerBarText_->SetFont(cache->GetResource<Font>(INGAME_FONT), 16);
    powerBarText_->SetTextEffect(TE_SHADOW);
    powerBarText_->SetText(String("HEALTH"));
    powerBarText_->SetVisible(true);


    powerBarProgBarText_ = ui->GetRoot()->CreateChild<Text>("powerBarText");
    powerBarProgBarText_->SetAlignment(HA_LEFT, VA_TOP);
    powerBarProgBarText_->SetPosition(120.0f, 24.0);
    powerBarProgBarText_->SetFont(cache->GetResource<Font>(INGAME_FONT4), progBarSize);
    powerBarProgBarText_->SetTextEffect(TE_SHADOW);
    powerBarProgBarText_->SetText(String(""));
    powerBarProgBarText_->SetVisible(true);
    powerBarProgBarText_->SetColor(Color(1,1,0.4));

    int textureWidth;
    int textureHeight;

    textureWidth = powerBarTexture->GetWidth();
    textureHeight = powerBarTexture->GetHeight();

    powerBarP1Sprite_->SetScale(256.0f / textureWidth);
    powerBarP1Sprite_->SetSize(textureWidth, textureHeight);
    powerBarP1Sprite_->SetHotSpot(textureWidth, textureHeight);
    powerBarP1Sprite_->SetAlignment(HA_LEFT, VA_TOP);
    powerBarP1Sprite_->SetPosition(Vector2(280.0f, 50.0f));
    powerBarP1Sprite_->SetOpacity(1.0f);
    // Set a low priority so that other UI elements can be drawn on top
    powerBarP1Sprite_->SetPriority(-100);

    powerBarBkgP1Sprite_->SetScale(256.0f / textureWidth);
    powerBarBkgP1Sprite_->SetSize(textureWidth, textureHeight);
    powerBarBkgP1Sprite_->SetHotSpot(textureWidth, textureHeight);
    powerBarBkgP1Sprite_->SetAlignment(HA_LEFT, VA_TOP);
    powerBarBkgP1Sprite_->SetPosition(Vector2(280.0f, 50.0f));
    powerBarBkgP1Sprite_->SetOpacity(0.2f);
    // Set a low priority so that other UI elements can be drawn on top
    powerBarBkgP1Sprite_->SetPriority(-100);

    powerBarP1Sprite_->SetVisible(false);
    powerBarBkgP1Sprite_->SetVisible(false);

    rpmBarText_ = ui->GetRoot()->CreateChild<Text>("rpmBarText");
    rpmBarText_->SetAlignment(HA_LEFT, VA_TOP);
    rpmBarText_->SetPosition(10.0f, 122.5);
    rpmBarText_->SetFont(cache->GetResource<Font>(INGAME_FONT), 16);
    rpmBarText_->SetTextEffect(TE_SHADOW);
    rpmBarText_->SetText(String("RPM"));
    rpmBarText_->SetVisible(true);

    rpmBarProgBarText_ = ui->GetRoot()->CreateChild<Text>("rpmBarText");
    rpmBarProgBarText_->SetAlignment(HA_LEFT, VA_TOP);
    rpmBarProgBarText_->SetPosition(120.0f, 123);
    rpmBarProgBarText_->SetFont(cache->GetResource<Font>(INGAME_FONT4), progBarSize);
    rpmBarProgBarText_->SetTextEffect(TE_SHADOW);
    rpmBarProgBarText_->SetText(String(""));
    rpmBarProgBarText_->SetVisible(true);
    rpmBarProgBarText_->SetColor(Color(5/255.0f,173/255.0f,219/255.0f));
//    5, 173, 219


    textureWidth = rpmBarTexture->GetWidth();
    textureHeight = rpmBarTexture->GetHeight();

    rpmBarP1Sprite_->SetScale(256.0f / textureWidth);
    rpmBarP1Sprite_->SetSize(textureWidth, textureHeight);
    rpmBarP1Sprite_->SetHotSpot(textureWidth, textureHeight);
    rpmBarP1Sprite_->SetAlignment(HA_LEFT, VA_TOP);
    rpmBarP1Sprite_->SetPosition(Vector2(280.0f, 150.0f));
    rpmBarP1Sprite_->SetOpacity(1.0f);
    // Set a low priority so that other UI elements can be drawn on top
    rpmBarP1Sprite_->SetPriority(-100);

    rpmBarBkgP1Sprite_->SetScale(256.0f / textureWidth);
    rpmBarBkgP1Sprite_->SetSize(textureWidth, textureHeight);
    rpmBarBkgP1Sprite_->SetHotSpot(textureWidth, textureHeight);
    rpmBarBkgP1Sprite_->SetAlignment(HA_LEFT, VA_TOP);
    rpmBarBkgP1Sprite_->SetPosition(Vector2(280.0f, 150.0f));
    rpmBarBkgP1Sprite_->SetOpacity(0.2f);
    // Set a low priority so that other UI elements can be drawn on top
    rpmBarBkgP1Sprite_->SetPriority(-100);

    rpmBarP1Sprite_->SetVisible(false);
    rpmBarBkgP1Sprite_->SetVisible(false);

    velBarText_ = ui->GetRoot()->CreateChild<Text>("velBarText");
    velBarText_->SetAlignment(HA_LEFT, VA_TOP);
    velBarText_->SetPosition(10.0f, 72.5);
    velBarText_->SetFont(cache->GetResource<Font>(INGAME_FONT), 16);
    velBarText_->SetTextEffect(TE_SHADOW);
    velBarText_->SetText(String("SPEED"));
    velBarText_->SetVisible(true);

    velBarProgBarText_ = ui->GetRoot()->CreateChild<Text>("velBarText");
    velBarProgBarText_->SetAlignment(HA_LEFT, VA_TOP);
    velBarProgBarText_->SetPosition(120.0f, 75);
    velBarProgBarText_->SetFont(cache->GetResource<Font>(INGAME_FONT4), progBarSize);
    velBarProgBarText_->SetTextEffect(TE_SHADOW);
    velBarProgBarText_->SetText(String(""));
    velBarProgBarText_->SetVisible(true);
    velBarProgBarText_->SetColor(Color(0.7,0.1,1));

    pranaBarText_ = ui->GetRoot()->CreateChild<Text>("pranaBarText");
    pranaBarText_->SetAlignment(HA_LEFT, VA_TOP);
    pranaBarText_->SetPosition(10.0f, 171.5);
    pranaBarText_->SetFont(cache->GetResource<Font>(INGAME_FONT), 16);
    pranaBarText_->SetTextEffect(TE_SHADOW);
    pranaBarText_->SetText(String("ENERGY"));
    pranaBarText_->SetVisible(true);

    pranaBarProgBarText_ = ui->GetRoot()->CreateChild<Text>("pranaBarText");
    pranaBarProgBarText_->SetAlignment(HA_LEFT, VA_TOP);
    pranaBarProgBarText_->SetPosition(120.0f, 174);
    pranaBarProgBarText_->SetFont(cache->GetResource<Font>(INGAME_FONT4), progBarSize);
    pranaBarProgBarText_->SetTextEffect(TE_SHADOW);
    pranaBarProgBarText_->SetText(String(""));
    pranaBarProgBarText_->SetVisible(true);
    pranaBarProgBarText_->SetColor(Color(0.1, 0.7,0.1));



    textureWidth = rpmBarTexture->GetWidth();
    textureHeight = rpmBarTexture->GetHeight();

    velBarP1Sprite_->SetScale(256.0f / textureWidth);
    velBarP1Sprite_->SetSize(textureWidth, textureHeight);
    velBarP1Sprite_->SetHotSpot(textureWidth, textureHeight);
    velBarP1Sprite_->SetAlignment(HA_LEFT, VA_TOP);
    velBarP1Sprite_->SetPosition(Vector2(280.0f, 100.0f));
    velBarP1Sprite_->SetOpacity(1.0f);
    // Set a low priority so that other UI elements can be drawn on top
    velBarP1Sprite_->SetPriority(-100);

    velBarBkgP1Sprite_->SetScale(256.0f / textureWidth);
    velBarBkgP1Sprite_->SetSize(textureWidth, textureHeight);
    velBarBkgP1Sprite_->SetHotSpot(textureWidth, textureHeight);
    velBarBkgP1Sprite_->SetAlignment(HA_LEFT, VA_TOP);
    velBarBkgP1Sprite_->SetPosition(Vector2(280.0f, 100.0f));
    velBarBkgP1Sprite_->SetOpacity(0.2f);
    // Set a low priority so that other UI elements can be drawn on top
    velBarBkgP1Sprite_->SetPriority(-100);

    velBarP1Sprite_->SetVisible(false);
    velBarBkgP1Sprite_->SetVisible(false);

    vDamageSprite_->SetScale(1.0f);
    vDamageSprite_->SetSize(hudVDamageTexture->GetWidth(), hudVDamageTexture->GetHeight());
    vDamageSprite_->SetHotSpot(hudVDamageTexture->GetWidth(), hudVDamageTexture->GetHeight());
    vDamageSprite_->SetAlignment(HA_RIGHT, VA_BOTTOM);
    vDamageSprite_->SetPosition(Vector2(-24.0f, -123.0f));
    vDamageSprite_->SetOpacity(0.9f);
    // Set a low priority so that other UI elements can be drawn on top
    vDamageSprite_->SetPriority(-100);
    vDamageSprite_->SetVisible(false);

    pickupSprite_->SetScale(1.0f);
    pickupSprite_->SetSize(hudPickupTexture->GetWidth(), hudPickupTexture->GetHeight());
    pickupSprite_->SetHotSpot(hudPickupTexture->GetWidth(), hudPickupTexture->GetHeight());
    pickupSprite_->SetAlignment(HA_LEFT, VA_TOP);
    pickupSprite_->SetPosition(Vector2(625.0f, 133.0f));
    pickupSprite_->SetOpacity(0.9f);
    // Set a low priority so that other UI elements can be drawn on top
    pickupSprite_->SetPriority(-100);
    pickupSprite_->SetVisible(true);


    pickupItemSprite_->SetScale(1.0f);
    pickupItemSprite_->SetSize(hudPickupTexture->GetWidth(), hudPickupTexture->GetHeight());
    pickupItemSprite_->SetHotSpot(hudPickupTexture->GetWidth(), hudPickupTexture->GetHeight());
    pickupItemSprite_->SetAlignment(HA_LEFT, VA_TOP);
    pickupItemSprite_->SetPosition(Vector2(628.0f, 143.0f));
    pickupItemSprite_->SetOpacity(0.9f);
    // Set a low priority so that other UI elements can be drawn on top
    pickupItemSprite_->SetPriority(-100);
    pickupItemSprite_->SetVisible(true);

    textureWidth = miniMapP1Texture->GetWidth();
    textureHeight = miniMapP1Texture->GetHeight();

       float miniMapP1X = 776.0f+45.0f;
       float miniMapP1Y = 300.0f-15.0f;

       /*
    miniMapP1Sprite_->SetScale(0.4);//256.0f / textureWidth);
    miniMapP1Sprite_->SetSize(textureWidth, textureHeight);
    miniMapP1Sprite_->SetHotSpot(textureWidth / 2, textureHeight / 2);
    miniMapP1Sprite_->SetAlignment(HA_LEFT, VA_TOP);
    miniMapP1Sprite_->SetPosition(Vector2(miniMapP1X-16.0f, miniMapP1Y));
//    miniMapP1Sprite_->SetPosition(Vector2(miniMapP1X+256.0f-16.0f, miniMapP1Y));

    miniMapP1Sprite_->SetOpacity(0.9f);
    // Set a low priority so that other UI elements can be drawn on top
    miniMapP1Sprite_->SetPriority(-100);


    textureWidth = miniMapWPTexture->GetWidth();
    textureHeight = miniMapWPTexture->GetHeight();

    //   float miniMapP1X = 776.0f+45.0f;
    //   float miniMapP1Y = 300.0f-15.0f;

    miniMapWPSprite_->SetScale(0.4);//256.0f / textureWidth);
    miniMapWPSprite_->SetSize(textureWidth, textureHeight);
    miniMapWPSprite_->SetHotSpot(textureWidth / 2, textureHeight / 2);
    miniMapWPSprite_->SetAlignment(HA_LEFT, VA_TOP);
//    miniMapP1Sprite_->SetPosition(Vector2(miniMapP1X-16.0f, miniMapP1Y));c
//    miniMapP1Sprite_->SetPosition(Vector2(miniMapP1X+256.0f-16.0f, miniMapP1Y));

    miniMapWPSprite_->SetOpacity(0.9f);
    // Set a low priority so that other UI elements can be drawn on top
    miniMapWPSprite_->SetPriority(-100);


    textureWidth = miniMapBkgTexture->GetWidth();
    textureHeight = miniMapBkgTexture->GetHeight();

    float miniMapX = 1000.0f + 45.0f;
    float miniMapY = 300.0f - 15.0f;

    miniMapBkgSprite_->SetScale(256.0f / textureWidth);
    miniMapBkgSprite_->SetSize(textureWidth, textureHeight);
    miniMapBkgSprite_->SetHotSpot(textureWidth, textureHeight);
    miniMapBkgSprite_->SetAlignment(HA_LEFT, VA_TOP);
    miniMapBkgSprite_->SetPosition(Vector2(miniMapX, miniMapY));
    miniMapBkgSprite_->SetOpacity(0.2f);
    // Set a low priority so that other UI elements can be drawn on top
    miniMapBkgSprite_->SetPriority(-100);

    //miniMapP1Sprite_->SetVisible(true);
    //miniMapWPSprite_->SetVisible(true);
    //miniMapBkgSprite_->SetVisible(true);
    miniMapP1Sprite_->SetVisible(false);
    miniMapWPSprite_->SetVisible(false);
    miniMapBkgSprite_->SetVisible(false);

    textureWidth = markerMapBkgTexture->GetWidth();
    textureHeight = markerMapBkgTexture->GetHeight();

    markerMapBkgSprite_->SetScale(256.0f / textureWidth);
    markerMapBkgSprite_->SetSize(textureWidth, textureHeight);
    markerMapBkgSprite_->SetHotSpot(textureWidth, textureHeight);
    markerMapBkgSprite_->SetAlignment(HA_LEFT, VA_TOP);
    markerMapBkgSprite_->SetPosition(Vector2(miniMapX, miniMapY));
    markerMapBkgSprite_->SetOpacity(0.5f);
    // Set a low priority so that other UI elements can be drawn on top
    miniMapBkgSprite_->SetPriority(-100);
    markerMapBkgSprite_->SetVisible(true);

*/
       /*
    radioSpectrumSprite_->SetScale(256.0f / radioSpectrumTexture_->GetWidth());
    radioSpectrumSprite_->SetSize(radioSpectrumTexture_->GetWidth(), radioSpectrumTexture_->GetHeight());
    radioSpectrumSprite_->SetHotSpot(radioSpectrumTexture_->GetWidth() / 2, radioSpectrumTexture_->GetHeight() / 2);
//    radioSpectrumSprite_->SetAlignment(HA_RIGHT, VA_TOP);
    radioSpectrumSprite_->SetAlignment(HA_RIGHT, VA_CENTER);

    radioSpectrumSprite_->SetPosition(Vector2(-170.0f, -360.0f));
    radioSpectrumSprite_->SetOpacity(0.5f);
    // Set a low priority so that other UI elements can be drawn on top
    radioSpectrumSprite_->SetPriority(-100);
    radioSpectrumSprite_->SetVisible(true);

    // Rank text
    for (int i = 0; i < NUM_RANK_FIELDS; i++) {
        rankText_[i] = ui->GetRoot()->CreateChild<Text>("RankText");
        rankText_[i]->SetAlignment(HA_RIGHT, VA_CENTER);
        rankText_[i]->SetPosition(-10.0f, 10.0 + (i * 50));
        rankText_[i]->SetFont(cache->GetResource<Font>(INGAME_FONT3), 8);
        rankText_[i]->SetTextEffect(TE_SHADOW);
        rankText_[i]->SetVisible(false);
        std::string debugData1;
        debugData1.append("-");
        rankText_[i]->SetText(debugData1.c_str());
    }

    // Debug text
    for (int i = 0; i < NUM_DEBUG_FIELDS; i++) {
        debugText_[i] = ui->GetRoot()->CreateChild<Text>("DebugText");
        debugText_[i]->SetAlignment(HA_RIGHT, VA_CENTER);
        debugText_[i]->SetPosition(10.0f, 500.0 + (i * 40));
        debugText_[i]->SetFont(font, 8);
        debugText_[i]->SetTextEffect(TE_SHADOW);
        debugText_[i]->SetVisible(false);
        std::string debugData1;
        debugData1.append("-");
        debugText_[i]->SetText(debugData1.c_str());
    }

    //loginList_
    for (int i = 0; i < NUM_LOGIN_FIELDS; i++) {
        loginListText_[i] = ui->GetRoot()->CreateChild<Text>("LoginListText");
        loginListText_[i]->SetAlignment(HA_LEFT, VA_TOP);
        loginListText_[i]->SetPosition(11.0f, 340.0 + (i * 24));
        loginListText_[i]->SetFont(font3, 15);
        loginListText_[i]->SetTextEffect(TE_SHADOW);
        loginListText_[i]->SetVisible(false);
        std::string blnkData1;
        blnkData1.append("");
        loginListText_[i]->SetText(blnkData1.c_str());
    }



    // Music radio track text
    for (int i = 0; i < NUM_RADIO_TRACK_FIELDS; i++) {
        radioText_[i] = ui->GetRoot()->CreateChild<Text>("RadioTrackListText");

        radioText_[i]->SetAlignment(HA_RIGHT, VA_BOTTOM);
        radioText_[i]->SetPosition(-138.0f, -100 + (i * 20));
        radioText_[i]->SetVisible(true);
        radioText_[i]->SetColor(Color(235/255.0f, 217/255.0f, 255/255));
        radioText_[i]->SetFont(INGAME_FONT2, 9);
        radioText_[i]->SetTextEffect(TE_SHADOW);
        radioText_[i]->SetVisible(true);
        std::string debugData1;
        debugData1.append("");
        radioText_[i]->SetText(debugData1.c_str());
    }
    */
    ////

    // Calculate UI layout zones
    uiLayout_.CalculateZones(graphics->GetWidth(), graphics->GetHeight());

    // =============================================================================
    // STEERING WHEEL - Bottom Left Zone
    // =============================================================================
    auto* steerWheelTexture = cache->GetResource<Texture2D>("Textures/steer-wheel.png");
    auto* steerActorTexture = cache->GetResource<Texture2D>("Textures/steer-actor.png");

    if (steerWheelTexture) {
        steerWheelSprite_ = ui->GetRoot()->CreateChild<Sprite>();
        steerWheelSprite_->SetTexture(steerWheelTexture);

        float wheelSize = 120.0f;
        steerWheelSprite_->SetSize(wheelSize, wheelSize);
        steerWheelSprite_->SetScale(1.0f);
        steerWheelSprite_->SetHotSpot(wheelSize / 2, wheelSize / 2);
        steerWheelSprite_->SetAlignment(HA_LEFT, VA_TOP);

        // Position in bottom-left zone
        steerWheelSprite_->SetPosition(Vector2(
                uiLayout_.bottomLeft.x + wheelSize/2,
                uiLayout_.bottomLeft.y + wheelSize/2
        ));

        steerWheelSprite_->SetOpacity(0.8f);
        steerWheelSprite_->SetPriority(-90);
        steerWheelSprite_->SetVisible(true);
    }

    if (steerActorTexture) {
        steerActorSprite_ = ui->GetRoot()->CreateChild<Sprite>();
        steerActorSprite_->SetTexture(steerActorTexture);

        float actorSize = 80.0f;
        steerActorSprite_->SetSize(actorSize, actorSize);
        steerActorSprite_->SetScale(1.0f);
        steerActorSprite_->SetHotSpot(actorSize / 2, actorSize / 2);
        steerActorSprite_->SetAlignment(HA_LEFT, VA_TOP);

        // Position relative to steering wheel
        steerActorSprite_->SetPosition(Vector2(
                uiLayout_.bottomLeft.x + 120.0f/2,
                uiLayout_.bottomLeft.y + 120.0f/2
        ));

        steerActorSprite_->SetOpacity(0.9f);
        steerActorSprite_->SetPriority(-85);
        steerActorSprite_->SetVisible(false);
    }

    // =============================================================================
    // POWER BARS - Top Left Zone
    // =============================================================================
    auto *font = cache->GetResource<Font>(INGAME_FONT);
    auto *font3 = cache->GetResource<Font>(INGAME_FONT3);
    auto *font4 = cache->GetResource<Font>(INGAME_FONT4);

    // Power Bar Textures
    Texture2D *powerBarTexture = cache->GetResource<Texture2D>("Textures/powerbar.png");
    Texture2D *powerBarBkgTexture = cache->GetResource<Texture2D>("Textures/powerbar-bk.png");
    Texture2D *rpmBarTexture = cache->GetResource<Texture2D>("Textures/rpm.png");
    Texture2D *rpmBarBkgTexture = cache->GetResource<Texture2D>("Textures/powerbar-bk.png");
    Texture2D *velBarTexture = cache->GetResource<Texture2D>("Textures/velocity.png");
    Texture2D *velBarBkgTexture = cache->GetResource<Texture2D>("Textures/powerbar-bk.png");

    if (powerBarTexture && powerBarBkgTexture) {
        // Create power bar sprites
        powerBarP1Sprite_ = ui->GetRoot()->CreateChild<Sprite>();
        powerBarBkgP1Sprite_ = ui->GetRoot()->CreateChild<Sprite>();
        rpmBarP1Sprite_ = ui->GetRoot()->CreateChild<Sprite>();
        rpmBarBkgP1Sprite_ = ui->GetRoot()->CreateChild<Sprite>();
        velBarP1Sprite_ = ui->GetRoot()->CreateChild<Sprite>();
        velBarBkgP1Sprite_ = ui->GetRoot()->CreateChild<Sprite>();

        // Power bar text labels
        powerBarText_ = ui->GetRoot()->CreateChild<Text>("powerBarText");
        powerBarText_->SetAlignment(HA_LEFT, VA_TOP);
        powerBarText_->SetPosition(uiLayout_.topLeft.x, uiLayout_.topLeft.y + 20.0f);
        powerBarText_->SetFont(cache->GetResource<Font>(INGAME_FONT), 16);
        powerBarText_->SetTextEffect(TE_SHADOW);
        powerBarText_->SetText(String("HEALTH"));
        powerBarText_->SetVisible(true);

        rpmBarText_ = ui->GetRoot()->CreateChild<Text>("rpmBarText");
        rpmBarText_->SetAlignment(HA_LEFT, VA_TOP);
        rpmBarText_->SetPosition(uiLayout_.topLeft.x, uiLayout_.topLeft.y + 70.0f);
        rpmBarText_->SetFont(cache->GetResource<Font>(INGAME_FONT), 16);
        rpmBarText_->SetTextEffect(TE_SHADOW);
        rpmBarText_->SetText(String("RPM"));
        rpmBarText_->SetVisible(true);

        velBarText_ = ui->GetRoot()->CreateChild<Text>("velBarText");
        velBarText_->SetAlignment(HA_LEFT, VA_TOP);
        velBarText_->SetPosition(uiLayout_.topLeft.x, uiLayout_.topLeft.y + 120.0f);
        velBarText_->SetFont(cache->GetResource<Font>(INGAME_FONT), 16);
        velBarText_->SetTextEffect(TE_SHADOW);
        velBarText_->SetText(String("SPEED"));
        velBarText_->SetVisible(true);

        pranaBarText_ = ui->GetRoot()->CreateChild<Text>("pranaBarText");
        pranaBarText_->SetAlignment(HA_LEFT, VA_TOP);
        pranaBarText_->SetPosition(uiLayout_.topLeft.x, uiLayout_.topLeft.y + 170.0f);
        pranaBarText_->SetFont(cache->GetResource<Font>(INGAME_FONT), 16);
        pranaBarText_->SetTextEffect(TE_SHADOW);
        pranaBarText_->SetText(String("ENERGY"));
        pranaBarText_->SetVisible(true);

        // Progress bar text indicators
        int progBarSize = 15;
        powerBarProgBarText_ = ui->GetRoot()->CreateChild<Text>("powerBarProgText");
        powerBarProgBarText_->SetAlignment(HA_LEFT, VA_TOP);
        powerBarProgBarText_->SetPosition(uiLayout_.topLeft.x + 120.0f, uiLayout_.topLeft.y + 21.0f);
        powerBarProgBarText_->SetFont(cache->GetResource<Font>(INGAME_FONT4), progBarSize);
        powerBarProgBarText_->SetTextEffect(TE_SHADOW);
        powerBarProgBarText_->SetColor(Color(1,1,0.4));
        powerBarProgBarText_->SetVisible(true);

        rpmBarProgBarText_ = ui->GetRoot()->CreateChild<Text>("rpmBarProgText");
        rpmBarProgBarText_->SetAlignment(HA_LEFT, VA_TOP);
        rpmBarProgBarText_->SetPosition(uiLayout_.topLeft.x + 120.0f, uiLayout_.topLeft.y + 71.0f);
        rpmBarProgBarText_->SetFont(cache->GetResource<Font>(INGAME_FONT4), progBarSize);
        rpmBarProgBarText_->SetTextEffect(TE_SHADOW);
        rpmBarProgBarText_->SetColor(Color(5/255.0f,173/255.0f,219/255.0f));
        rpmBarProgBarText_->SetVisible(true);

        velBarProgBarText_ = ui->GetRoot()->CreateChild<Text>("velBarProgText");
        velBarProgBarText_->SetAlignment(HA_LEFT, VA_TOP);
        velBarProgBarText_->SetPosition(uiLayout_.topLeft.x + 120.0f, uiLayout_.topLeft.y + 121.0f);
        velBarProgBarText_->SetFont(cache->GetResource<Font>(INGAME_FONT4), progBarSize);
        velBarProgBarText_->SetTextEffect(TE_SHADOW);
        velBarProgBarText_->SetColor(Color(0.7,0.1,1));
        velBarProgBarText_->SetVisible(true);

        pranaBarProgBarText_ = ui->GetRoot()->CreateChild<Text>("pranaBarProgText");
        pranaBarProgBarText_->SetAlignment(HA_LEFT, VA_TOP);
        pranaBarProgBarText_->SetPosition(uiLayout_.topLeft.x + 120.0f, uiLayout_.topLeft.y + 171.0f);
        pranaBarProgBarText_->SetFont(cache->GetResource<Font>(INGAME_FONT4), progBarSize);
        pranaBarProgBarText_->SetTextEffect(TE_SHADOW);
        pranaBarProgBarText_->SetColor(Color(0.1, 0.7,0.1));
        pranaBarProgBarText_->SetVisible(true);

        // Configure power bar sprites
        int textureWidth = powerBarTexture->GetWidth();
        int textureHeight = powerBarTexture->GetHeight();

        // Health bar
        powerBarP1Sprite_->SetTexture(powerBarTexture);
        powerBarP1Sprite_->SetScale(256.0f / textureWidth);
        powerBarP1Sprite_->SetSize(textureWidth, textureHeight);
        powerBarP1Sprite_->SetHotSpot(textureWidth, textureHeight);
        powerBarP1Sprite_->SetAlignment(HA_LEFT, VA_TOP);
        powerBarP1Sprite_->SetPosition(Vector2(uiLayout_.topLeft.x + 280.0f, uiLayout_.topLeft.y + 47.0f));
        powerBarP1Sprite_->SetOpacity(1.0f);
        powerBarP1Sprite_->SetPriority(-100);
        powerBarP1Sprite_->SetVisible(false);

        powerBarBkgP1Sprite_->SetTexture(powerBarBkgTexture);
        powerBarBkgP1Sprite_->SetScale(256.0f / textureWidth);
        powerBarBkgP1Sprite_->SetSize(textureWidth, textureHeight);
        powerBarBkgP1Sprite_->SetHotSpot(textureWidth, textureHeight);
        powerBarBkgP1Sprite_->SetAlignment(HA_LEFT, VA_TOP);
        powerBarBkgP1Sprite_->SetPosition(Vector2(uiLayout_.topLeft.x + 280.0f, uiLayout_.topLeft.y + 47.0f));
        powerBarBkgP1Sprite_->SetOpacity(0.2f);
        powerBarBkgP1Sprite_->SetPriority(-100);
        powerBarBkgP1Sprite_->SetVisible(false);

        // RPM bar
        rpmBarP1Sprite_->SetTexture(rpmBarTexture);
        rpmBarP1Sprite_->SetScale(256.0f / textureWidth);
        rpmBarP1Sprite_->SetSize(textureWidth, textureHeight);
        rpmBarP1Sprite_->SetHotSpot(textureWidth, textureHeight);
        rpmBarP1Sprite_->SetAlignment(HA_LEFT, VA_TOP);
        rpmBarP1Sprite_->SetPosition(Vector2(uiLayout_.topLeft.x + 280.0f, uiLayout_.topLeft.y + 97.0f));
        rpmBarP1Sprite_->SetOpacity(1.0f);
        rpmBarP1Sprite_->SetPriority(-100);
        rpmBarP1Sprite_->SetVisible(false);

        rpmBarBkgP1Sprite_->SetTexture(rpmBarBkgTexture);
        rpmBarBkgP1Sprite_->SetScale(256.0f / textureWidth);
        rpmBarBkgP1Sprite_->SetSize(textureWidth, textureHeight);
        rpmBarBkgP1Sprite_->SetHotSpot(textureWidth, textureHeight);
        rpmBarBkgP1Sprite_->SetAlignment(HA_LEFT, VA_TOP);
        rpmBarBkgP1Sprite_->SetPosition(Vector2(uiLayout_.topLeft.x + 280.0f, uiLayout_.topLeft.y + 97.0f));
        rpmBarBkgP1Sprite_->SetOpacity(0.2f);
        rpmBarBkgP1Sprite_->SetPriority(-100);
        rpmBarBkgP1Sprite_->SetVisible(false);

        // Velocity bar
        velBarP1Sprite_->SetTexture(velBarTexture);
        velBarP1Sprite_->SetScale(256.0f / textureWidth);
        velBarP1Sprite_->SetSize(textureWidth, textureHeight);
        velBarP1Sprite_->SetHotSpot(textureWidth, textureHeight);
        velBarP1Sprite_->SetAlignment(HA_LEFT, VA_TOP);
        velBarP1Sprite_->SetPosition(Vector2(uiLayout_.topLeft.x + 280.0f, uiLayout_.topLeft.y + 147.0f));
        velBarP1Sprite_->SetOpacity(1.0f);
        velBarP1Sprite_->SetPriority(-100);
        velBarP1Sprite_->SetVisible(false);

        velBarBkgP1Sprite_->SetTexture(velBarBkgTexture);
        velBarBkgP1Sprite_->SetScale(256.0f / textureWidth);
        velBarBkgP1Sprite_->SetSize(textureWidth, textureHeight);
        velBarBkgP1Sprite_->SetHotSpot(textureWidth, textureHeight);
        velBarBkgP1Sprite_->SetAlignment(HA_LEFT, VA_TOP);
        velBarBkgP1Sprite_->SetPosition(Vector2(uiLayout_.topLeft.x + 280.0f, uiLayout_.topLeft.y + 147.0f));
        velBarBkgP1Sprite_->SetOpacity(0.2f);
        velBarBkgP1Sprite_->SetPriority(-100);
        velBarBkgP1Sprite_->SetVisible(false);
    }

    // =============================================================================
    // VEHICLE DAMAGE SYSTEM - Bottom Right Zone
    // =============================================================================
    Texture2D *hudVDamageTexture = cache->GetResource<Texture2D>("Textures/hud_v-damage.png");
    Texture2D *hudVDamageLineTexture = cache->GetResource<Texture2D>("Textures/hud_vdamage-line.png");

    if (hudVDamageTexture) {
        vDamageSprite_ = ui->GetRoot()->CreateChild<Sprite>();
        vDamageSprite_->SetTexture(hudVDamageTexture);
        vDamageSprite_->SetScale(1.0f);
        vDamageSprite_->SetSize(hudVDamageTexture->GetWidth(), hudVDamageTexture->GetHeight());
        vDamageSprite_->SetHotSpot(0, 0);
        vDamageSprite_->SetAlignment(HA_LEFT, VA_TOP);

        // Position in bottom-right zone
        vDamageSprite_->SetPosition(Vector2(
                uiLayout_.bottomRight.x + 10.0f,
                uiLayout_.bottomRight.y + 10.0f
        ));

        vDamageSprite_->SetOpacity(0.9f);
        vDamageSprite_->SetPriority(-100);
        vDamageSprite_->SetVisible(false);

        // Create damage indicator lines
        if (hudVDamageLineTexture) {
            SharedPtr<Sprite> sprite_;
            for (int i = 0; i < VDAMAGE_BODY_NUM_LINES; i++) {
                // Front left wheel lines
                sprite_ = SharedPtr<Sprite>(ui->GetRoot()->CreateChild<Sprite>());
                sprite_->SetTexture(hudVDamageLineTexture);
                sprite_->SetScale(1.0f);
                sprite_->SetSize(hudVDamageLineTexture->GetWidth(), hudVDamageLineTexture->GetHeight());
                sprite_->SetAlignment(HA_LEFT, VA_TOP);
                sprite_->SetPosition(Vector2(
                        uiLayout_.bottomRight.x + 15.0f,
                        uiLayout_.bottomRight.y + 30.0f + (i * 3.0f)
                ));
                sprite_->SetOpacity(0.9f);
                sprite_->SetPriority(-95);
                sprite_->SetVisible(false);
                vDamage_FL_LineSprites_.Push(sprite_);

                // Front right wheel lines
                sprite_ = SharedPtr<Sprite>(ui->GetRoot()->CreateChild<Sprite>());
                sprite_->SetTexture(hudVDamageLineTexture);
                sprite_->SetScale(1.0f);
                sprite_->SetSize(hudVDamageLineTexture->GetWidth(), hudVDamageLineTexture->GetHeight());
                sprite_->SetAlignment(HA_LEFT, VA_TOP);
                sprite_->SetPosition(Vector2(
                        uiLayout_.bottomRight.x + 15.0f,
                        uiLayout_.bottomRight.y + 30.0f + (i * 3.0f)
                ));
                sprite_->SetOpacity(0.9f);
                sprite_->SetPriority(-95);
                sprite_->SetVisible(false);
                vDamage_FR_LineSprites_.Push(sprite_);

                // Back left wheel lines
                sprite_ = SharedPtr<Sprite>(ui->GetRoot()->CreateChild<Sprite>());
                sprite_->SetTexture(hudVDamageLineTexture);
                sprite_->SetScale(1.0f);
                sprite_->SetSize(hudVDamageLineTexture->GetWidth(), hudVDamageLineTexture->GetHeight());
                sprite_->SetAlignment(HA_LEFT, VA_TOP);
                sprite_->SetPosition(Vector2(
                        uiLayout_.bottomRight.x + 15.0f,
                        uiLayout_.bottomRight.y + 30.0f + (i * 3.0f)
                ));
                sprite_->SetOpacity(0.9f);
                sprite_->SetPriority(-95);
                sprite_->SetVisible(false);
                vDamage_BL_LineSprites_.Push(sprite_);

                // Back right wheel lines
                sprite_ = SharedPtr<Sprite>(ui->GetRoot()->CreateChild<Sprite>());
                sprite_->SetTexture(hudVDamageLineTexture);
                sprite_->SetScale(1.0f);
                sprite_->SetSize(hudVDamageLineTexture->GetWidth(), hudVDamageLineTexture->GetHeight());
                sprite_->SetAlignment(HA_LEFT, VA_TOP);
                sprite_->SetPosition(Vector2(
                        uiLayout_.bottomRight.x + 15.0f,
                        uiLayout_.bottomRight.y + 30.0f + (i * 3.0f)
                ));
                sprite_->SetOpacity(0.9f);
                sprite_->SetPriority(-95);
                sprite_->SetVisible(false);
                vDamage_BR_LineSprites_.Push(sprite_);

                // Front axle wheel lines
                sprite_ = SharedPtr<Sprite>(ui->GetRoot()->CreateChild<Sprite>());
                sprite_->SetTexture(hudVDamageLineTexture);
                sprite_->SetScale(1.0f);
                sprite_->SetSize(hudVDamageLineTexture->GetWidth(), hudVDamageLineTexture->GetHeight());
                sprite_->SetAlignment(HA_LEFT, VA_TOP);
                sprite_->SetPosition(Vector2(
                        uiLayout_.bottomRight.x + 15.0f,
                        uiLayout_.bottomRight.y + 30.0f + (i * 3.0f)
                ));
                sprite_->SetOpacity(0.9f);
                sprite_->SetPriority(-95);
                sprite_->SetVisible(false);
                vDamage_FA_LineSprites_.Push(sprite_);

                // Rear axle wheel lines
                sprite_ = SharedPtr<Sprite>(ui->GetRoot()->CreateChild<Sprite>());
                sprite_->SetTexture(hudVDamageLineTexture);
                sprite_->SetScale(1.0f);
                sprite_->SetSize(hudVDamageLineTexture->GetWidth(), hudVDamageLineTexture->GetHeight());
                sprite_->SetAlignment(HA_LEFT, VA_TOP);
                sprite_->SetPosition(Vector2(
                        uiLayout_.bottomRight.x + 15.0f,
                        uiLayout_.bottomRight.y + 30.0f + (i * 3.0f)
                ));
                sprite_->SetOpacity(0.9f);
                sprite_->SetPriority(-95);
                sprite_->SetVisible(false);
                vDamage_RA_LineSprites_.Push(sprite_);
            }
        }
    }


    // Vehicle damage HUD
    vDamageSprite_ = ui->GetRoot()->CreateChild<Sprite>();
    // Indicator Lines
    SharedPtr<Sprite> sprite_;
    for (int i = 0; i < VDAMAGE_BODY_NUM_LINES; i++) {
        sprite_ = ui->GetRoot()->CreateChild<Sprite>();
        sprite_->SetTexture(hudVDamageLineTexture);
        sprite_->SetScale(1.0f);
        sprite_->SetSize(hudVDamageLineTexture->GetWidth(), hudVDamageLineTexture->GetHeight());
        sprite_->SetHotSpot(hudVDamageLineTexture->GetWidth(), hudVDamageLineTexture->GetHeight());
        sprite_->SetPosition(Vector2(0, 0));
        sprite_->SetOpacity(0.9f);
        // Set a low priority so that other UI elements can be drawn on top
        sprite_->SetPriority(-100);
        sprite_->SetVisible(false);
        vDamage_FL_LineSprites_.Push(sprite_);

        sprite_ = ui->GetRoot()->CreateChild<Sprite>();
        sprite_->SetTexture(hudVDamageLineTexture);
        sprite_->SetScale(1.0f);
        sprite_->SetSize(hudVDamageLineTexture->GetWidth(), hudVDamageLineTexture->GetHeight());
        sprite_->SetHotSpot(hudVDamageLineTexture->GetWidth(), hudVDamageLineTexture->GetHeight());
        sprite_->SetPosition(Vector2(0, 0));
        sprite_->SetOpacity(0.9f);
        // Set a low priority so that other UI elements can be drawn on top
        sprite_->SetPriority(-100);
        sprite_->SetVisible(false);
        vDamage_FR_LineSprites_.Push(sprite_);

        sprite_ = ui->GetRoot()->CreateChild<Sprite>();
        sprite_->SetTexture(hudVDamageLineTexture);
        sprite_->SetScale(1.0f);
        sprite_->SetSize(hudVDamageLineTexture->GetWidth(), hudVDamageLineTexture->GetHeight());
        sprite_->SetHotSpot(hudVDamageLineTexture->GetWidth(), hudVDamageLineTexture->GetHeight());
        sprite_->SetPosition(Vector2(0, 0));
        sprite_->SetOpacity(0.9f);
        // Set a low priority so that other UI elements can be drawn on top
        sprite_->SetPriority(-100);
        sprite_->SetVisible(false);
        vDamage_BL_LineSprites_.Push(sprite_);

        sprite_ = ui->GetRoot()->CreateChild<Sprite>();
        sprite_->SetTexture(hudVDamageLineTexture);
        sprite_->SetScale(1.0f);
        sprite_->SetSize(hudVDamageLineTexture->GetWidth(), hudVDamageLineTexture->GetHeight());
        sprite_->SetHotSpot(hudVDamageLineTexture->GetWidth(), hudVDamageLineTexture->GetHeight());
        sprite_->SetPosition(Vector2(0, 0));
        sprite_->SetOpacity(0.9f);
        // Set a low priority so that other UI elements can be drawn on top
        sprite_->SetPriority(-100);
        sprite_->SetVisible(false);
        vDamage_BR_LineSprites_.Push(sprite_);

        sprite_ = ui->GetRoot()->CreateChild<Sprite>();
        sprite_->SetTexture(hudVDamageLineTexture);
        sprite_->SetScale(1.0f);
        sprite_->SetSize(hudVDamageLineTexture->GetWidth(), hudVDamageLineTexture->GetHeight());
        sprite_->SetHotSpot(hudVDamageLineTexture->GetWidth(), hudVDamageLineTexture->GetHeight());
        sprite_->SetPosition(Vector2(0, 0));
        sprite_->SetOpacity(0.9f);
        // Set a low priority so that other UI elements can be drawn on top
        sprite_->SetPriority(-100);
        sprite_->SetVisible(false);
        vDamage_FA_LineSprites_.Push(sprite_);

        sprite_ = ui->GetRoot()->CreateChild<Sprite>();
        sprite_->SetTexture(hudVDamageLineTexture);
        sprite_->SetScale(1.0f);
        sprite_->SetSize(hudVDamageLineTexture->GetWidth(), hudVDamageLineTexture->GetHeight());
        sprite_->SetHotSpot(hudVDamageLineTexture->GetWidth(), hudVDamageLineTexture->GetHeight());
        sprite_->SetPosition(Vector2(0, 0));
        sprite_->SetOpacity(0.9f);
        // Set a low priority so that other UI elements can be drawn on top
        sprite_->SetPriority(-100);
        sprite_->SetVisible(false);
        vDamage_RA_LineSprites_.Push(sprite_);
    }

    // =============================================================================
    // PICKUP SYSTEM - Bottom Right Zone (below vehicle damage)
    // =============================================================================
    Texture2D *hudPickupTexture = cache->GetResource<Texture2D>("Textures/hud_pickup.png");

    // Load pickup item textures
    for (int id = 1; id < 7; id++) {
        Texture2D *hudPickupItemTexture = cache->GetResource<Texture2D>(
                "Textures/pickups/pickup-0" + String(id) + ".png");
        if (hudPickupItemTexture) {
            hudPickupItemTextures_.Push(hudPickupItemTexture);
        }
    }

    if (hudPickupTexture) {
        pickupSprite_ = ui->GetRoot()->CreateChild<Sprite>();
        pickupSprite_->SetTexture(hudPickupTexture);
        pickupSprite_->SetScale(1.0f);
        pickupSprite_->SetSize(hudPickupTexture->GetWidth(), hudPickupTexture->GetHeight());
        pickupSprite_->SetAlignment(HA_LEFT, VA_TOP);

        // Position below vehicle damage in bottom-right zone
        pickupSprite_->SetPosition(Vector2(
                uiLayout_.bottomRight.x + 20.0f,
                uiLayout_.bottomRight.y + 120.0f
        ));

        pickupSprite_->SetOpacity(0.9f);
        pickupSprite_->SetPriority(-100);
        pickupSprite_->SetVisible(true);

        pickupItemSprite_ = ui->GetRoot()->CreateChild<Sprite>();
        if (!hudPickupItemTextures_.Empty()) {
            pickupItemSprite_->SetTexture(hudPickupItemTextures_[0]);
        }
        pickupItemSprite_->SetScale(1.0f);
        pickupItemSprite_->SetSize(hudPickupTexture->GetWidth(), hudPickupTexture->GetHeight());
        pickupItemSprite_->SetAlignment(HA_LEFT, VA_TOP);

        // Position relative to pickup background
        pickupItemSprite_->SetPosition(Vector2(
                uiLayout_.bottomRight.x + 23.0f,
                uiLayout_.bottomRight.y + 123.0f
        ));

        pickupItemSprite_->SetOpacity(0.9f);
        pickupItemSprite_->SetPriority(-95);
        pickupItemSprite_->SetVisible(true);
    }

    // =============================================================================
    // SPEEDOMETER - Bottom Center Zone
    // =============================================================================
    auto* speedometerBkgTexture = cache->GetResource<Texture2D>("Textures/hud/speedometer_bg.png");
    auto* speedometerNeedleTexture = cache->GetResource<Texture2D>("Textures/hud/speedometer_needle.png");

    if (speedometerBkgTexture) {
        speedometerBkgSprite_ = ui->GetRoot()->CreateChild<Sprite>();
        speedometerBkgSprite_->SetTexture(speedometerBkgTexture);

        int textureWidth = speedometerBkgTexture->GetWidth();
        int textureHeight = speedometerBkgTexture->GetHeight();

        speedometerBkgSprite_->SetSize(textureWidth, textureHeight);
        speedometerBkgSprite_->SetScale(1.0f);
        speedometerBkgSprite_->SetHotSpot(textureWidth / 2, textureHeight / 2);
        speedometerBkgSprite_->SetAlignment(HA_LEFT, VA_TOP);

        // Position in bottom-center zone
        speedometerBkgSprite_->SetPosition(Vector2(
                uiLayout_.bottomCenter.x + uiLayout_.bottomCenter.width/2,
                uiLayout_.bottomCenter.y + uiLayout_.bottomCenter.height/2
        ));

        speedometerBkgSprite_->SetOpacity(0.8f);
        speedometerBkgSprite_->SetPriority(-100);
        speedometerBkgSprite_->SetVisible(true);
    }

    if (speedometerNeedleTexture) {
        speedometerNeedleSprite_ = ui->GetRoot()->CreateChild<Sprite>();
        speedometerNeedleSprite_->SetTexture(speedometerNeedleTexture);

        int textureWidth = speedometerNeedleTexture->GetWidth();
        int textureHeight = speedometerNeedleTexture->GetHeight();

        speedometerNeedleSprite_->SetSize(textureWidth, textureHeight);
        speedometerNeedleSprite_->SetScale(1.0f);
        speedometerNeedleSprite_->SetHotSpot(textureWidth / 2, textureHeight - 5);
        speedometerNeedleSprite_->SetAlignment(HA_LEFT, VA_TOP);

        // Position relative to speedometer background
        speedometerNeedleSprite_->SetPosition(Vector2(
                uiLayout_.bottomCenter.x + uiLayout_.bottomCenter.width/2,
                uiLayout_.bottomCenter.y + uiLayout_.bottomCenter.height/2 - 7.0f
        ));

        speedometerNeedleSprite_->SetOpacity(0.9f);
        speedometerNeedleSprite_->SetPriority(-85);
        speedometerNeedleSprite_->SetVisible(true);
    }

    // Digital speed text
    speedometerText_ = ui->GetRoot()->CreateChild<Text>();
    speedometerText_->SetFont(cache->GetResource<Font>(INGAME_FONT3), 16);
    speedometerText_->SetTextAlignment(HA_CENTER);
    speedometerText_->SetColor(Color::WHITE);
    speedometerText_->SetAlignment(HA_LEFT, VA_TOP);
    speedometerText_->SetPosition(IntVector2(
            uiLayout_.bottomCenter.x + uiLayout_.bottomCenter.width/2 - 30,
            uiLayout_.bottomCenter.y + uiLayout_.bottomCenter.height/2 + 60
    ));
    speedometerText_->SetText("0 KM/H");
    speedometerText_->SetVisible(false);
    speedometerText_->SetPriority(-80);

    // =============================================================================
    // RADIO SYSTEM - Center Right Zone
    // =============================================================================

    // Radio spectrum visualization
    radioVisualizerImg_ = new PixelImage(context_);
    radioSpectrumTexture_ = (new Texture2D(context_));
    radioVisualizerImg_->SetSize(radioSpectrumWidth_, radioSpectrumHeight_);
    radioSpectrumTexture_->SetData(radioVisualizerImg_, false);

    radioSpectrumSprite_ = ui->GetRoot()->CreateChild<Sprite>();
    radioSpectrumSprite_->SetTexture(radioSpectrumTexture_);
    radioSpectrumSprite_->SetScale(256.0f / radioSpectrumTexture_->GetWidth());
    radioSpectrumSprite_->SetSize(radioSpectrumTexture_->GetWidth(), radioSpectrumTexture_->GetHeight());
    radioSpectrumSprite_->SetHotSpot(radioSpectrumTexture_->GetWidth() / 2, radioSpectrumTexture_->GetHeight() / 2);
    radioSpectrumSprite_->SetAlignment(HA_LEFT, VA_TOP);

    // Position in center-right zone
    radioSpectrumSprite_->SetPosition(Vector2(
            uiLayout_.centerRight.x + 20.0f,
            uiLayout_.centerRight.y + 30.0f
    ));

    radioSpectrumSprite_->SetOpacity(0.5f);
    radioSpectrumSprite_->SetPriority(-100);
    radioSpectrumSprite_->SetVisible(true);

    // Create VU meter
    CreateVUMeter();

    // Radio track text information
    for (int i = 0; i < NUM_RADIO_TRACK_FIELDS; i++) {
        radioText_[i] = ui->GetRoot()->CreateChild<Text>("RadioTrackListText");
        radioText_[i]->SetAlignment(HA_LEFT, VA_TOP);
        radioText_[i]->SetPosition(
                uiLayout_.centerRight.x + 10.0f,
                uiLayout_.centerRight.y + 80.0f + (i * 15.0f)
        );
        radioText_[i]->SetVisible(true);
        radioText_[i]->SetColor(Color(235/255.0f, 217/255.0f, 255/255));
        radioText_[i]->SetFont(INGAME_FONT2, 9);
        radioText_[i]->SetTextEffect(TE_SHADOW);
        radioText_[i]->SetText("");
    }

    // =============================================================================
    // NETWORK STATUS - Top Right Zone
    // =============================================================================
    networkInfo_ = ui->GetRoot()->CreateChild<Text>();
    networkInfo_->SetText("Ping: - ms");
    networkInfo_->SetColor(Color::WHITE);
    networkInfo_->SetFont(cache->GetResource<Font>(INGAME_FONT), 10);
    networkInfo_->SetAlignment(HA_LEFT, VA_TOP);
    networkInfo_->SetPosition(uiLayout_.topRight.x + 200.0f, uiLayout_.topRight.y + 200.0f);

    // =============================================================================
    // DEBUG AND RANK TEXT - Use remaining spaces
    // =============================================================================

    // Debug text
    for (int i = 0; i < NUM_DEBUG_FIELDS; i++) {
        debugText_[i] = ui->GetRoot()->CreateChild<Text>("DebugText");
        debugText_[i]->SetAlignment(HA_LEFT, VA_TOP);
        debugText_[i]->SetPosition(10.0f, 300.0f + (i * 15));
        debugText_[i]->SetFont(font, 8);
        debugText_[i]->SetTextEffect(TE_SHADOW);
        debugText_[i]->SetVisible(false);
        debugText_[i]->SetText("-");
    }

    // Rank text
    for (int i = 0; i < NUM_RANK_FIELDS; i++) {
        rankText_[i] = ui->GetRoot()->CreateChild<Text>("RankText");
        rankText_[i]->SetAlignment(HA_RIGHT, VA_TOP);
        rankText_[i]->SetPosition(graphics->GetWidth() - 200.0f, 300.0f + (i * 25));
        rankText_[i]->SetFont(cache->GetResource<Font>(INGAME_FONT3), 8);
        rankText_[i]->SetTextEffect(TE_SHADOW);
        rankText_[i]->SetVisible(false);
        rankText_[i]->SetText("-");
    }

    // Login list text
    for (int i = 0; i < NUM_LOGIN_FIELDS; i++) {
        loginListText_[i] = ui->GetRoot()->CreateChild<Text>("LoginListText");
        loginListText_[i]->SetAlignment(HA_LEFT, VA_TOP);
        loginListText_[i]->SetPosition(uiLayout_.topRight.x, uiLayout_.topRight.y + (i * 20));
        loginListText_[i]->SetFont(font3, 12);
        loginListText_[i]->SetTextEffect(TE_SHADOW);
        loginListText_[i]->SetVisible(false);
        loginListText_[i]->SetText("");
    }

    // Initialize physics and other client systems...
    DebugRenderer *dbgRenderer = scene_->CreateComponent<DebugRenderer>();
    clientPhysicsWorld_ = scene_->CreateComponent<PhysicsWorld>(LOCAL);
    clientPhysicsWorld_->SetFps(60);
    clientPhysicsWorld_->DrawDebugGeometry(false);
    clientPhysicsWorld_->SetDebugRenderer(dbgRenderer);
    clientPhysicsWorld_->SetInterpolation(false);
    clientPhysicsWorld_->SetEnabled(false);
    clientPhysicsWorld_->SetUpdateEnabled(false);
    /////

    InitializeAudioUISystem();
    // Setup sequencer viewport
    SetupSequencerViewport();

    // Missile lock-on indicator
    auto *missileUIContainer_ = ui->GetRoot()->CreateChild<UIElement>();
    missileUIContainer_->SetSize(200, 50);
    missileUIContainer_->SetPosition(graphics->GetWidth() / 2 - 100, 100);

    auto *missileProgressBar_ = missileUIContainer_->CreateChild<ProgressBar>();
    missileProgressBar_->SetSize(200, 20);
    missileProgressBar_->SetRange(1.0f);
    missileProgressBar_->SetValue(0.0f);
    missileProgressBar_->SetVisible(false);

    auto *missileStatusText_ = missileUIContainer_->CreateChild<Text>();
    missileStatusText_->SetFont(cache->GetResource<Font>(INGAME_FONT), 14);
    missileStatusText_->SetText("NO TARGET");
    missileStatusText_->SetAlignment(HA_CENTER, VA_TOP);
    missileStatusText_->SetPosition(0, 25);

}


void AlgebraKart::CreateVUMeter() {
    auto* ui = GetSubsystem<UI>();
    auto* graphics = GetSubsystem<Graphics>();
    auto* cache = GetSubsystem<ResourceCache>();

    // Create main radio container
    radioContainer_ = ui->GetRoot()->CreateChild<UIElement>("RadioContainer");
    radioContainer_->SetSize(300, 120);
    radioContainer_->SetPosition(graphics->GetWidth() - 320, graphics->GetHeight()-100); // Top-right corner
    radioContainer_->SetOpacity(0.9f);
    radioContainer_->SetVisible(false); // Hidden until radio plays

    // Create background panel
    auto* radioBg = radioContainer_->CreateChild<BorderImage>("RadioBackground");
    radioBg->SetTexture(cache->GetResource<Texture2D>("Textures/UI/UIElement.png"));
    radioBg->SetSize(300, 180);
    radioBg->SetImageBorder(IntRect(4, 4, 4, 4));
    radioBg->SetColor(Color(0.1f, 0.1f, 0.1f, 0.8f));

    // Create radio title
    auto* radioTitle = radioContainer_->CreateChild<Text>("RadioTitle");
    radioTitle->SetText("Bad Zindagi Radio");
    radioTitle->SetFont(cache->GetResource<Font>(INGAME_FONT4), 10);
    radioTitle->SetColor(Color::WHITE);
    radioTitle->SetPosition(10, 5);

    // Create track info text (existing functionality)
    auto* trackInfo = radioContainer_->CreateChild<Text>("TrackInfo");
    trackInfo->SetText("No track playing");
    trackInfo->SetFont(cache->GetResource<Font>(INGAME_FONT3), 9);
    trackInfo->SetColor(Color::YELLOW);
    trackInfo->SetPosition(10, 25);
    trackInfo->SetSize(280, 20);

    // Create VU meter container
    vuMeterContainer_ = radioContainer_->CreateChild<UIElement>("VUMeterContainer");
    vuMeterContainer_->SetSize(280, 60);
    vuMeterContainer_->SetPosition(10, 50);

    // Initialize VU level arrays
    vuLevels_.Resize(VU_METER_CHANNELS);
    vuPeakLevels_.Resize(VU_METER_CHANNELS);
    vuPeakHoldTime_.Resize(VU_METER_CHANNELS);

    for (int i = 0; i < VU_METER_CHANNELS; i++) {
        vuLevels_[i] = 0.0f;
        vuPeakLevels_[i] = 0.0f;
        vuPeakHoldTime_[i] = 0.0f;
    }

    // Create VU meter bars for each channel
    vuMeterBars_.Clear();
    vuMeterPeakBars_.Clear();

    for (int channel = 0; channel < VU_METER_CHANNELS; channel++) {
        // Channel label
        auto* channelLabel = vuMeterContainer_->CreateChild<Text>();
        channelLabel->SetText(channel == 0 ? "L" : "R");
        channelLabel->SetFont(cache->GetResource<Font>("Fonts/Anonymous Pro.ttf"), 10);
        channelLabel->SetColor(Color::WHITE);
        channelLabel->SetPosition(0, channel * 25 + 5);

        for (int bar = 0; bar < VU_METER_BARS; bar++) {
            // Main VU bar
            auto* vuBar = vuMeterContainer_->CreateChild<Sprite>();
            vuBar->SetSize(10, 15);
            vuBar->SetPosition(20 + bar * 12, channel * 25);

            // Create or set texture for the bar
            // Option 1: Use existing UI texture (recommended)
            vuBar->SetTexture(cache->GetResource<Texture2D>("Textures/Palette.png"));
            vuBar->SetImageRect(IntRect(0, 0, 16, 16)); // Use a solid section

            // Option 2: Create procedural texture (if needed)
            // CreateVUBarTexture(vuBar); // See helper method below

            vuBar->SetColor(GetVUBarColor(bar));
            vuBar->SetVisible(false);
            vuMeterBars_.Push(Urho3D::SharedPtr<Sprite>(vuBar));

            // Peak indicator bar
            auto* peakBar = vuMeterContainer_->CreateChild<Sprite>();
            peakBar->SetSize(10, 3);
            peakBar->SetPosition(20 + bar * 12, channel * 25 - 5);
            peakBar->SetTexture(cache->GetResource<Texture2D>("Textures/Palette.png"));
            peakBar->SetImageRect(IntRect(0, 0, 16, 16));
            peakBar->SetColor(Color::YELLOW);
            peakBar->SetVisible(false);
            vuMeterPeakBars_.Push(Urho3D::SharedPtr<Sprite>(peakBar));
        }
    }
}

Color AlgebraKart::GetVUBarColor(int barIndex) {
    float ratio = (float)barIndex / (float)VU_METER_BARS;

    if (ratio < 0.6f) {
        // Green range (low levels)
        return Color::GREEN;
    } else if (ratio < 0.8f) {
        // Yellow range (medium levels)
        return Color::YELLOW;
    } else {
        // Red range (high levels)
        return Color::RED;
    }
}

void AlgebraKart::UpdateVUMeter(float timeStep) {
    if (!radioContainer_ || !_radioStream || !_radioStream->IsPlaying()) {
        if (radioContainer_) {
            radioContainer_->SetVisible(false);
        }
        return;
    }

    radioContainer_->SetVisible(true);

    // Update track info
    auto* trackInfo = radioContainer_->GetChild("TrackInfo", true);
    if (trackInfo && currTrack_ < bzRadioTracksTrackName.size()) {
        String trackText = String(bzRadioTracksArtistName[currTrack_].c_str()) + " - " +
                           String(bzRadioTracksTrackName[currTrack_].c_str());
        static_cast<Text*>(trackInfo)->SetText(trackText);
    }

    if (!analyzer_ || !capturer_) {
        return;
    }

    // Get spectrum data
    const Vector<float>& spectrum = analyzer_->spectrum();

    if (spectrum.Empty()) {
        return;
    }

    // Calculate VU levels for each channel
    // For simplicity, we'll split the frequency spectrum between left and right
    int spectrumSize = spectrum.Size();
    int halfSpectrum = spectrumSize / 2;

    // Left channel (lower frequencies)
    vuLevels_[0] = CalculateVULevel(spectrum, 0, halfSpectrum);

    // Right channel (higher frequencies)
    vuLevels_[1] = CalculateVULevel(spectrum, halfSpectrum, spectrumSize);

    // Update peak levels and peak hold
    for (int channel = 0; channel < VU_METER_CHANNELS; channel++) {
        if (vuLevels_[channel] > vuPeakLevels_[channel]) {
            vuPeakLevels_[channel] = vuLevels_[channel];
            vuPeakHoldTime_[channel] = VU_PEAK_HOLD_TIME;
        } else {
            vuPeakHoldTime_[channel] -= timeStep;
            if (vuPeakHoldTime_[channel] <= 0.0f) {
                vuPeakLevels_[channel] *= VU_PEAK_DECAY_RATE;
                vuPeakHoldTime_[channel] = 0.0f;
            }
        }

        // Clamp levels
        vuLevels_[channel] = Clamp(vuLevels_[channel], 0.0f, 1.0f);
        vuPeakLevels_[channel] = Clamp(vuPeakLevels_[channel], 0.0f, 1.0f);
    }

    // Update visual bars
    for (int channel = 0; channel < VU_METER_CHANNELS; channel++) {
        int activeBars = (int)(vuLevels_[channel] * VU_METER_BARS);
        int peakBar = (int)(vuPeakLevels_[channel] * VU_METER_BARS);

        for (int bar = 0; bar < VU_METER_BARS; bar++) {
            int barIndex = channel * VU_METER_BARS + bar;

            // Show/hide main bars
            vuMeterBars_[barIndex]->SetVisible(bar < activeBars);

            // Show/hide peak bars
            vuMeterPeakBars_[barIndex]->SetVisible(bar == peakBar && peakBar > 0);
        }
    }
}

float AlgebraKart::CalculateVULevel(const Vector<float>& spectrum, int startFreq, int endFreq) {
    if (spectrum.Empty() || startFreq >= endFreq) {
        return 0.0f;
    }

    float totalLevel = 0.0f;
    int sampleCount = 0;

    // Calculate RMS level for the frequency range
    for (int i = startFreq; i < endFreq && i < spectrum.Size(); i++) {
        totalLevel += spectrum[i] * spectrum[i];
        sampleCount++;
    }

    if (sampleCount == 0) {
        return 0.0f;
    }

    // RMS calculation
    float rmsLevel = Sqrt(totalLevel / sampleCount);

    // Convert to dB scale and normalize
    float dbLevel = 20.0f * log10(Max(rmsLevel, 0.001f)); // Avoid log(0)

    // Normalize from typical range (-60dB to 0dB) to (0.0 to 1.0)
    float normalizedLevel = (dbLevel + 60.0f) / 60.0f;

    return Clamp(normalizedLevel, 0.0f, 1.0f);
}

// Optional: Enhanced version with better stereo separation
float AlgebraKart::CalculateChannelLevel(int channel, const Vector<float>& spectrum) {
    if (spectrum.Empty()) {
        return 0.0f;
    }

    int spectrumSize = spectrum.Size();
    float totalLevel = 0.0f;
    int sampleCount = 0;

    if (channel == 0) {
        // Left channel: emphasize lower frequencies (bass, mids)
        int endIdx = spectrumSize * 0.7f; // Use first 70% of spectrum
        for (int i = 0; i < endIdx; i++) {
            float weight = 1.0f - (float)i / endIdx * 0.5f; // Weight lower frequencies more
            totalLevel += spectrum[i] * spectrum[i] * weight;
            sampleCount++;
        }
    } else {
        // Right channel: emphasize higher frequencies (treble)
        int startIdx = spectrumSize * 0.3f; // Use last 70% of spectrum
        for (int i = startIdx; i < spectrumSize; i++) {
            float weight = (float)(i - startIdx) / (spectrumSize - startIdx) * 0.5f + 0.5f;
            totalLevel += spectrum[i] * spectrum[i] * weight;
            sampleCount++;
        }
    }

    if (sampleCount == 0) return 0.0f;

    float rmsLevel = Sqrt(totalLevel / sampleCount);
    float dbLevel = 20.0f * log10(Max(rmsLevel, 0.001f));
    return Clamp((dbLevel + 60.0f) / 60.0f, 0.0f, 1.0f);
}

// Add smooth transitions and better visual effects
void AlgebraKart::UpdateVUMeterSmooth(float timeStep) {
    if (!radioContainer_ || !_radioStream || !_radioStream->IsPlaying()) {
        if (radioContainer_) {
            radioContainer_->SetVisible(false);
        }
        return;
    }

    radioContainer_->SetVisible(true);

    if (!analyzer_ || !capturer_) {
        return;
    }

    if (audioSpectrumOptions_.inputSize < capturer_->bufferReadCount()) {
        // Update analyzer
        analyzer_->update(capturer_->getBuffer(), capturer_->bufferHeadIndex(),
                          audioSpectrumOptions_.bottomLevel,
                          audioSpectrumOptions_.topLevel, audioSpectrumOptions_.minFreq,
                          audioSpectrumOptions_.maxFreq, audioSpectrumOptions_.axisLogBase);
    }

    const Vector<float>& spectrum = *capturer_->getBuffer();//analyzer_->spectrum();

    if (spectrum.Empty()) return;

    // Smooth level transitions
    static Vector<float> smoothedLevels(VU_METER_CHANNELS, 0.0f);
    const float SMOOTH_FACTOR = 0.3f; // Adjust for responsiveness

    for (int channel = 0; channel < VU_METER_CHANNELS; channel++) {
        float targetLevel = CalculateChannelLevel(channel, spectrum);

        // Smooth the level changes
        if (targetLevel > smoothedLevels[channel]) {
            smoothedLevels[channel] = targetLevel; // Fast attack
        } else {
            smoothedLevels[channel] = Lerp(smoothedLevels[channel], targetLevel, SMOOTH_FACTOR * timeStep * 10.0f); // Slow decay
        }

        vuLevels_[channel] = smoothedLevels[channel];

        // Update peaks
        if (vuLevels_[channel] > vuPeakLevels_[channel]) {
            vuPeakLevels_[channel] = vuLevels_[channel];
            vuPeakHoldTime_[channel] = VU_PEAK_HOLD_TIME;
        } else {
            vuPeakHoldTime_[channel] -= timeStep;
            if (vuPeakHoldTime_[channel] <= 0.0f) {
                vuPeakLevels_[channel] *= VU_PEAK_DECAY_RATE;
            }
        }
    }

    // Update visual representation with smooth animations
    for (int channel = 0; channel < VU_METER_CHANNELS; channel++) {
        int activeBars = (int)(vuLevels_[channel] * VU_METER_BARS);
        int peakBar = (int)(vuPeakLevels_[channel] * VU_METER_BARS);

        for (int bar = 0; bar < VU_METER_BARS; bar++) {
            int barIndex = channel * VU_METER_BARS + bar;
            auto* vuBarSprite = vuMeterBars_[barIndex].Get();
            auto* peakBarSprite = vuMeterPeakBars_[barIndex].Get();

            // Animate bar visibility with alpha
            if (bar < activeBars) {
                vuBarSprite->SetVisible(true);
                float alpha = 1.0f - (float)bar / VU_METER_BARS * 0.3f; // Slight fade for higher bars
                Color barColor = GetVUBarColor(bar);
                barColor.a_ = alpha;
                vuBarSprite->SetColor(barColor);
            } else {
                vuBarSprite->SetVisible(false);
            }

            // Peak indicators
            if (bar == peakBar && peakBar > 0) {
                peakBarSprite->SetVisible(true);
                float peakAlpha = vuPeakHoldTime_[channel] / VU_PEAK_HOLD_TIME;
                Color peakColor = Color::YELLOW;
                peakColor.a_ = peakAlpha;
                peakBarSprite->SetColor(peakColor);
            } else {
                peakBarSprite->SetVisible(false);
            }
        }
    }
}

void AlgebraKart::DestroyPlayer(Connection *connection) {
    ////
    ResourceCache *cache = GetSubsystem<ResourceCache>();

    /// Retrieve the username for the client
    VariantMap identity = connection->GetIdentity();
    String username = String(identity["UserName"]);

    if (!actorMap_[connection].Expired() && actorMap_[connection].NotNull()) {
        // Remove network actor from csp server
        cspServer_->snapshot.removeNode(actorMap_[connection]->GetNode());

        String playerName = actorMap_[connection]->GetUserName();
        // Store in local login list
        loginList_.Remove(playerName);

        ClientObj *actor = actorMap_[connection];
        if (actor) {
            // Clean up sequencer and recorder references
            if (actor && actor->GetSequencer()) {
                actor->GetSequencer()->Reset();
                actor->GetSequencer().Reset(); // Clear shared pointer
            }

            // Remove from all collections
            actorMap_.Erase(connection);

            // FIX: Clean up any remaining network actors associated with this connection
            // This is critical - actors might hold references preventing cleanup
            Scene *scene = scene_;
            if (scene) {
                PODVector<Node *> actorNodes;
                scene->GetChildrenWithComponent<NetworkActor>(actorNodes, true);

                for (Node *node: actorNodes) {
                    NetworkActor *actor = node->GetComponent<NetworkActor>();
                    if (actor && actor->GetConnection() == connection) {
                        // Properly cleanup the actor
                        actor->Kill();
                        // Clear maps
                        actorMap_.Erase(connection);
                        actorTextMap_.Erase(connection);
                        serverObjects_.Erase(connection);
                        // Clean up connection
                        actor->CleanupConnection(connection);
                        actor->SetConnection(nullptr);
                        node->Remove(); // Remove the entire node
                    }
                }
            }
        }

        // Remove minimap marker
        if (minimapPlayerMap_.Contains(connection)) {
            Node * marker = minimapPlayerMap_[connection];
            if (marker)
                marker->Remove();
            minimapPlayerMap_.Erase(connection);
        }


        URHO3D_LOGINFOF("Client disconnected and cleaned up: %s", connection->GetAddress().CString());

        // Push update to clients
        connection->SendServerUpdate();

        // Send server login list refresh
        Server *server = GetSubsystem<Server>();
        // Send refreshed login list to clients
        server->SendLoginListRefreshToClients();

        // Cleanup any remaining references
        if (welcomeMsgMap_.Contains(connection)) {
            welcomeMsgMap_.Erase(connection);
        }
    }
}

// Spawn Player (NetworkActor on Server for Client)
Node *AlgebraKart::SpawnPlayer(Connection *connection) {

    UpdateLoadingProgress(0.6f, "Spawning player...");

    // Store connection as most recent client
    lastConnection_ = connection;

    ResourceCache *cache = GetSubsystem<ResourceCache>();

    /// Retrieve the username for the client
    VariantMap identity = connection->GetIdentity();
    String username = String(identity["UserName"]);
    String name = String(String("actor-") + username);
    nextGridPosition_ += 2;
    // Get next grid position
    Vector3 actorPos = GetGridPosition(nextGridPosition_);
    nextGridPosition_++;
    // Create a new network actor for the player
    SharedPtr<Node> networkActorNode(scene_->CreateChild(name, REPLICATED));
    networkActorNode->SetPosition(actorPos);

    // Face the racing direction
    Quaternion racingOrientation;
    racingOrientation.FromLookRotation(RACING_DIRECTION, Vector3::UP);
    networkActorNode->SetRotation(racingOrientation);

    NetworkActor *actor = networkActorNode->CreateComponent<NetworkActor>(REPLICATED);
    actor->Init(networkActorNode);
    actor->SetProjectileManager(globalMissileManager_);

    // Assign server recorder to each client sequencer
    Server *server = GetSubsystem<Server>();
    SharedPtr<Recorder> serverRec_ = server->GetRecorder();
    actor->GetSequencer()->SetServerRec(serverRec_);

    //actor->SetClientInfo(name, Random(1,100), Vector3(Random(-400.0f,400.0f),Random(20.0f,80.0f),Random(-400.0f,400.0f)));

    auto* body = networkActorNode->GetComponent<RigidBody>(true);

    // Set calculated network actor position
/*
    Vector3 actorPos = Vector3(0,0,0);
    if (starAMarkerSet_) {
        Vector3 spawnDir = (starBMarker_-starAMarker_);
        Vector3 startPos = starAMarker_;

        float w = 1.2;

        //actorPos = Quaternion(0,90,0) * (startPos+spawnDir*w);
        actorPos = Quaternion(0,0,0) * (startPos+spawnDir*w);
        actor->GetBody()->SetPosition(actorPos);

        // Clamp y to start marker
        //actorPos.y_ = starAMarker_.y_;

    } else {
        actor->GetBody()->SetPosition(randLoc);
    }*/

    actor->SetClientInfo(username, Random(1,100), Vector3(actor->GetBody()->GetPosition()));
    // Unmute sequencer -> turn to true to hear drum machine
    actor->GetSequencer()->SetMute(false);


    // Generate vehicle

    String vehicleName = "vehicle-" + username;
    // Create a new vehicle for the player
    SharedPtr<Node> vehicleNode(scene_->CreateChild(vehicleName, REPLICATED));
    // Create vehicle VERY close to the actor (almost same position)
    //Vector3 vehiclePos = actorPos + Vector3(1.0f, 0.0f, -1.0f);  // Slightly behind and to the right
    //vehicleNode->SetPosition(vehiclePos);
    //vehicleNode->SetRotation(Quaternion(0.0f, 180.0f, 0.0f));
    Vehicle *vehicle = vehicleNode->CreateComponent<Vehicle>(REPLICATED);
    vehicle->Init(vehicleNode);
    vehicleNode->SetPosition(Vector3(actor->GetBody()->GetPosition())+Vector3(0,-5,-2));
    //vehicle->GetRaycastVehicle()->GetBody()->SetPosition(Vector3(actor->GetPosition())-Vector3::UP*50.0f);
    vehicleNode->SetRotation(Quaternion(0.0f, Random(0.0f, 360.0f), 0.0f));
    // Attach vehicle to actor
    actor->vehicle_ = vehicle;

    // Create text3d client info node
    Node *plyFltTextNode = vehicleNode->CreateChild("Actor FloatText", REPLICATED);
    Text3D *plyFltText_ = plyFltTextNode->CreateComponent<Text3D>();
    actorTextMap_[connection] = plyFltText_;
    plyFltText_->SetColor(Color(77.0f/255.0f,0.0f,1.0f));
    plyFltText_->SetEffectColor(Color::BLACK);
    plyFltText_->GetNode()->SetScale(18.0f);
    plyFltText_->SetFont(cache->GetResource<Font>(INGAME_FONT), 16);
    plyFltText_->SetFaceCameraMode(FC_ROTATE_XYZ);
    plyFltText_->GetNode()->SetPosition(Vector3(0,19.0f,-CHASSIS_WIDTH / 2.0f)*0.9f);

    //plyFltText_->GetNode()->SetPosition(actor->GetPosition() + Vector3(0,20.0f,0));
    plyFltText_->SetText(username);

    // Push the new comer into the map
    clientMap_[username] = networkActorNode;
    actorMap_[connection] = actor;
    serverObjects_[connection] = networkActorNode;

    // Add to focus map
    focusObjects_.Push(Vector3(actor->GetPosition()));
    // Increment focus object
    focusIndex_++;


    // Give the ownership to the client
    networkActorNode->SetOwner(connection);
    vehicleNode->SetOwner(connection);

    // 2. Create a new camera following the actor
    // ** THIS APPEARS TO BE CRITICAL FOR CLIENT SCENE REPLICATION TO START
    Graphics* graphics = GetSubsystem<Graphics>();
    String cameraName = "camera_" + username;
    Node* cameraNode = networkActorNode->CreateChild(cameraName, REPLICATED);
    // Set position to force update
    cameraNode->SetPosition(Vector3(0.0f, 2.0f, 0.0f));

    // Give the ownership to the client
    cameraNode->SetOwner(connection);

    // Push update to clients
    connection->SendServerUpdate();

    cspServer_->snapshot.addNode(networkActorNode);


    // smooth step
    vehicleRot_ = vehicleNode->GetRotation();
    Quaternion dir(vehicleRot_.YawAngle(), Vector3::UP);
    dir = dir * Quaternion(0, Vector3::UP);
    dir = dir * Quaternion(0, Vector3::RIGHT);
    targetCameraPos_ = vehicleNode->GetPosition() - dir * Vector3(0.0f, 0.0f, CLIENT_CAMERA_DISTANCE);


    // Create minimap marker for this player
    if (minimapCam_)
    {
        Color playerColor = Color::WHITE;
        // You can assign different colors based on player ID or other criteria
        switch (actorMap_.Size() % 6)
        {
            case 0: playerColor = Color::RED; break;
            case 1: playerColor = Color::BLUE; break;
            case 2: playerColor = Color::GREEN; break;
            case 3: playerColor = Color::YELLOW; break;
            case 4: playerColor = Color::MAGENTA; break;
            case 5: playerColor = Color::CYAN; break;
        }
        CreateMinimapPlayerMarker(connection, playerColor);
    }


    actor->EnterVehicle();
    return networkActorNode;
}

// Spawn Player (NetworkActor on Server for AI agent)
std::shared_ptr<NetworkActor> AlgebraKart::SpawnPlayer(unsigned int id) {

    ResourceCache *cache = GetSubsystem<ResourceCache>();

    /// Retrieve the username for the ai
    String username = String(String("ai-") + String(id));

    // Locate an previous actor node
    SharedPtr<Node> prevActorNode(scene_->GetChild(username));

    // Remove previous actor
    if (prevActorNode) {
        prevActorNode->Remove();
    }

    // Get next grid position for AI bot
    Vector3 actorPos = GetGridPosition(nextGridPosition_);
    nextGridPosition_++;

    // Create a new network actor for the player
    SharedPtr<Node> networkActorNode(scene_->CreateChild(username, REPLICATED));
    networkActorNode->SetPosition(actorPos);

    // Face the racing direction
    Quaternion racingOrientation;
    racingOrientation.FromLookRotation(RACING_DIRECTION, Vector3::UP);
    networkActorNode->SetRotation(racingOrientation);

    NetworkActor *actor = networkActorNode->CreateComponent<NetworkActor>(REPLICATED);
    actor->Init(networkActorNode);
    String name = String(String("actor-") + username);

    // Create vehicle close to the AI actor
    Vector3 vehiclePos = actorPos + Vector3(0.0f, -5.0f, 0.0f);  // Slightly behind and to the right

    // Assign server recorder to each client sequencer
    Server *server = GetSubsystem<Server>();
    SharedPtr<Recorder> serverRec_ = server->GetRecorder();
    actor->GetSequencer()->SetServerRec(serverRec_);

    if (starAMarkerSet_) {
        Vector3 spawnDir = (starBMarker_-starAMarker_);
        Vector3 startPos = starAMarker_;

        float w = 0.14f*id;

        //float w = 1.2;

        //actorPos = Quaternion(0,90,0) * (startPos+spawnDir*w);
//        actorPos = Quaternion(0,0,0) * (startPos+spawnDir*w);


        //actorPos = Quaternion(0,90,0) * (startPos+spawnDir*w);
        actorPos = Quaternion(0,0,0) * (startPos+spawnDir*w);
        //actor->GetNode()->SetPosition(actorPos);
        // Clamp y to start marker
        //actorPos.y_ = starAMarker_.y_;

    }

    actor->SetClientInfo(name, Random(1,100), actorPos);


    String vehicleName = "vehicle-" + username;
    // Create a new vehicle for the player
    SharedPtr<Node> vehicleNode(scene_->CreateChild(vehicleName, REPLICATED));
    //vehicleNode->SetRotation(Quaternion(0.0f, 180.0f, 0.0f));
    Vehicle *vehicle = vehicleNode->CreateComponent<Vehicle>(REPLICATED);
    vehicle->Init(vehicleNode);
    actor->SetClientInfo(username, Random(1,100), Vector3(actor->GetPosition()));
    vehicleNode->SetPosition(vehiclePos);
    vehicleNode->SetRotation(racingOrientation);
    // Attach vehicle to actor
    actor->vehicle_ = vehicle;

    // Create text3d client info node
    Node *plyFltTextNode = vehicleNode->CreateChild("Actor FloatText", REPLICATED);
    Text3D *plyFltText_ = plyFltTextNode->CreateComponent<Text3D>();
    aiActorTextMap_[id] = plyFltText_;
    plyFltText_->SetColor(Color(77.0f/255.0f,0.73f,1.0f));
    plyFltText_->SetEffectColor(Color::BLACK);
    plyFltText_->GetNode()->SetScale(18.0f);
    plyFltText_->SetFont(cache->GetResource<Font>(INGAME_FONT4), 24);
    plyFltText_->SetFaceCameraMode(FC_ROTATE_XYZ);
    plyFltText_->GetNode()->SetPosition(Vector3(-6.0f,19.0f,0.0f-CHASSIS_WIDTH / 2.0f)*0.9f);
    plyFltText_->SetText(username);
    //Vector3(0, 50.0f, CHASSIS_WIDTH / 2.0f));

    // Push the new comer into the map
    clientMap_[username] = networkActorNode;
    aiActorMap_[id] = actor;

    // Add to focus map
    focusObjects_.Push(Vector3(actor->GetPosition()));
    // Increment focus object
    focusIndex_++;

    // Snap to latest bot
    flyCam_ = vehicleNode->GetPosition();

    // Create minimap marker for this bot
    if (minimapCam_)
    {
        Color botColor = Color::GRAY;
        // Different shades for different bots
        float hue = (float)(id % 360);
        botColor.FromHSV(hue, 0.7f, 0.8f);
        CreateMinimapBotMarker(id, botColor);
    }

    if (actor && actor->GetNode())
    {
        // Add ViewCone component to AI bots
//        ViewCone* viewCone = actor->GetNode()->CreateComponent<ViewCone>();

        URHO3D_LOGINFO("About to create ViewCone");
        ViewCone* viewCone = actor->GetNode()->CreateComponent<ViewCone>(REPLICATED);
        viewCone->SetSegments(8); // Start with minimal segments
        URHO3D_LOGINFO("ViewCone created successfully");
        viewCone->DebugConeState();

        // Configure view cone
        viewCone->SetViewDistance(40.0f);
        viewCone->SetViewAngle(75.0f);
        viewCone->SetViewHeight(12.0f);

        // Color coding for different AI types
        Color aiColor;
        switch (id % 5) {
            case 0: aiColor = Color(0.2f, 1.0f, 0.2f, 0.3f); break; // Green
            case 1: aiColor = Color(1.0f, 0.2f, 0.2f, 0.3f); break; // Red
            case 2: aiColor = Color(0.2f, 0.2f, 1.0f, 0.3f); break; // Blue
            case 3: aiColor = Color(1.0f, 1.0f, 0.2f, 0.3f); break; // Yellow
            case 4: aiColor = Color(1.0f, 0.2f, 1.0f, 0.3f); break; // Magenta
        }
        viewCone->SetConeColor(aiColor);
        URHO3D_LOGINFO("ViewCone added to AI bot " + String(id));
    }

    // Auto-enter the vehicle for AI bots (they should start in their vehicles)
    actor->EnterVehicle();

    // Give the ownership to the client
    //networkActorNode->SetOwner(connection);
    //vehicleNode->SetOwner(connection);
    std::shared_ptr<NetworkActor> ptrActor(actor);
    return ptrActor;
}

void AlgebraKart::InitiateGameMap(Scene *scene) {

    ResourceCache *cache = GetSubsystem<ResourceCache>();
    auto *graphics = GetSubsystem<Graphics>();

    UI *ui = GetSubsystem<UI>();

    // Debug textwww.algebrakart.com
    for (int i = 0; i < NUM_DEBUG_FIELDS; i++) {
        debugText_[i] = ui->GetRoot()->CreateChild<Text>("DebugText");
        debugText_[i]->SetAlignment(HA_LEFT, VA_CENTER);
        debugText_[i]->SetPosition(10.0f, 10.0 + (i * 40));
        debugText_[i]->SetFont(cache->GetResource<Font>(INGAME_FONT2), 10);
        debugText_[i]->SetTextEffect(TE_SHADOW);
        debugText_[i]->SetVisible(false);
        std::string debugData1;
        debugData1.append("-");
        debugText_[i]->SetText(debugData1.c_str());
    }

    // Rank text
    for (int i = 0; i < NUM_RANK_FIELDS; i++) {
        rankText_[i] = ui->GetRoot()->CreateChild<Text>("RankText");
        rankText_[i]->SetAlignment(HA_LEFT, VA_CENTER);
        rankText_[i]->SetPosition(10.0f, 10.0 + (i * 50));
        rankText_[i]->SetFont(cache->GetResource<Font>(INGAME_FONT3), 10);
        rankText_[i]->SetTextEffect(TE_SHADOW);
        rankText_[i]->SetVisible(false);
        std::string debugData1;
        debugData1.append("-");
        rankText_[i]->SetText(debugData1.c_str());
    }

    // Genetic Algorithm statistics text
    for (int i = 0; i < NUM_STAT_FIELDS; i++) {
        genAlgStatsText_[i] = ui->GetRoot()->CreateChild<Text>("GenAlgStatsText");
        genAlgStatsText_[i]->SetAlignment(HA_LEFT, VA_TOP);
        genAlgStatsText_[i]->SetPosition(10.0f, 1.0 + (i * 12));
        genAlgStatsText_[i]->SetFont(cache->GetResource<Font>(INGAME_FONT2), 10);
        genAlgStatsText_[i]->SetTextEffect(TE_SHADOW);
        genAlgStatsText_[i]->SetVisible(false);
        std::string statData1;
        statData1.append("-");
        genAlgStatsText_[i]->SetText(statData1.c_str());
    }

    // Set the default UI style and font
    //ui->GetRoot()->SetDefaultStyle(cache->GetResource<XMLFile>("UI/DefaultStyle.xml"));
    auto *font = cache->GetResource<Font>(INGAME_FONT2);
    auto *font2 = cache->GetResource<Font>(INGAME_FONT);
    auto *font3 = cache->GetResource<Font>(INGAME_FONT3);
    auto *font4 = cache->GetResource<Font>(INGAME_FONT4);


    UpdateLoadingProgress(0.2f, "Creating map...");

/*
    // Create a directional light with shadows
        Node *lightNode = scene_->CreateChild("DirectionalLight", REPLICATED);
        lightNode->SetDirection(Vector3(0.3f, -0.5f, 0.425f));
        Light *light = lightNode->CreateComponent<Light>();
        light->SetLightType(LIGHT_DIRECTIONAL);
        light->SetCastShadows(true);
        light->SetShadowBias(BiasParameters(0.00025f, 0.5f));
        light->SetShadowCascade(CascadeParameters(10.0f, 50.0f, 200.0f, 0.0f, 0.8f));
        light->SetSpecularIntensity(0.5f);
        //lightNode->SetNetPositionAttr(Vector3(20, 30, 40));
*/

        // All static scene content and the camera are also created as local,
        // so that they are unaffected by scene replication and are
        // not removed from the client upon connection.
        // Create a Zone component first for ambient lighting & fog control.

        // Create static scene content. First create a zone for ambient lighting and fog control
        Node *zoneNode = scene_->CreateChild("Zone", REPLICATED);
        Zone *zone = zoneNode->CreateComponent<Zone>();
//        zone->SetAmbientColor(Color(0.15f, 0.15f, 0.15f));
//        zone->SetFogColor(Color(0.5f, 0.5f, 0.7f));


    float r1 = Random(0.01f,0.1f);
    float r2 = Random(0.01f,0.1f);
    float r3 = Random(0.01f,0.3f);
    zone->SetAmbientColor(Color(r1, r2, r3));
    zone->SetFogColor(Color(r1*2.0f, r2*2.0f, r3*2.0f));

        zone->SetFogStart(700.0f);
        zone->SetFogEnd(58000.0f);
        zone->SetBoundingBox(BoundingBox(-60000.0f, 60000.0f));
        //zoneNode->SetNetPositionAttr(Vector3(40, 50, 70));




    using namespace std;

    // Create heightmap terrain with collision
    Node *terrainNode = scene_->CreateChild("Terrain", REPLICATED);
    terrainNode->SetPosition(Vector3(0.0f,-720.0f,0.0f));

    terrain_ = terrainNode->CreateComponent<Terrain>(REPLICATED);
    terrain_->SetPatchSize(64);
    terrain_->SetSpacing(Vector3(2.8f, 0.28f, 2.8f));
//    terrain->SetSpacing(Vector3(3.0f, 0.1f, 3.0f)); // Spacing between vertices and vertical resolution of the height map
   // terrainNode->SetNetPositionAttr(Vector3::ZERO);

        //terrain_->SetHeightMap(cache->GetResource<Image>("Offroad/Terrain/HeightMapRace-257.png"));
        //terrain_->SetMaterial(cache->GetResource<Material>("Offroad/Terrain/TerrainRace-256.xml"));
    //terrain_->SetMarkerMap(cache->GetResource<Image>("Textures/MarkerMap.png"));
    terrain_->SetHeightMap(cache->GetResource<Image>("Textures/HeightMap.png"));
    terrain_->SetMaterial(cache->GetResource<Material>("Materials/Terrain.xml"));
    terrain_->GetNode()->SetScale(12.0f);

    terrain_->SetOccluder(true);


    RigidBody* body = terrainNode->CreateComponent<RigidBody>();
    body->SetCollisionLayer(2); // Use layer bitmask 2 for static geometry
    CollisionShape* shape = terrainNode->CreateComponent<CollisionShape>();
    shape->SetTerrain();



    // TRACK MARKER
    // HSL -> 0.500000059604645,1,0.643137276172638
// NOT SUPPORTED -> Without legacy TileMap3D updates
    /*
    // Search for track marker
    int trackX = 0;
    int trackY = 0;
    for (int k = 0; k < terrain_->GetMarkerMap()->GetHeight(); k++) {
        for (int j = 0; j < terrain_->GetMarkerMap()->GetWidth(); j++) {
            Vector3 hsl_ = terrain_->GetMarkerMap()->GetPixel(k, j).ToHSL();
            //URHO3D_LOGINFOF("terrain marker map[x,y]=[%f,%f,%f]", j, k, hsl_.x_, hsl_.y_, hsl_.z_);


            if (hsl_ == bkgMarkerToken)
                continue;

            if (hsl_ == treeMarkerToken) {
                trees_.Push(Vector3((float) j, 0.0f, (float) k));
            } else if (hsl_ == trackMarkerToken) {
                trackX = j;
                trackY = k;
            } else if (hsl_ == waypointToken) {
                waypoints_.Push((Vector3((float) j, 0.0f, (float) k)));
            } else {
                // Store track marker
                URHO3D_LOGINFOF("***** UNKNOWN terrain marker map[%d,%d]=[%f,%f,%f]", j, k, hsl_.x_, hsl_.y_,
                                hsl_.z_);
            }

        }
    }
*/

/*
    int reduceFactor = ((float) trees_.Size() * 1.99f);
    // Drop trees to reduce saturation of trees
    int reduceSize = Min(trees_.Size(), reduceFactor);

    for (int j = 0; j < reduceSize; j++) {
        trees_.Erase(Random(0, trees_.Size()), 1);
    }

    URHO3D_LOGINFOF("***** TREE COUNT: [%d]", trees_.Size());
    URHO3D_LOGINFOF("***** WAYPOINT COUNT: [%d]", waypoints_.Size());

    RigidBody *body = terrainNode->CreateComponent<RigidBody>(REPLICATED);
    body->SetCollisionLayer(NETWORKACTOR_COL_LAYER); // Use layer bitmask 2 for static geometry
    terrainShape_ = terrainNode->CreateComponent<CollisionShape>(REPLICATED);

    // Assigns terrain collision map (calculated based on heightmap)
    terrainShape_->SetTerrain();
*/
    // Load race track at track marker

    // Convert marker position to world position for track
    int w = 64;//terrain_->GetMarkerMap()->GetWidth();
    int h = 64;//terrain_->GetMarkerMap()->GetHeight();
    int trackX = 0;
    int trackY = 0;

    float trackOffsetX = -mapSize / 2;
    float trackOffsetY = -mapSize / 2;
    float trackPosX = (((float) trackX / (float) w) * mapSize) + trackOffsetX;
    float trackPosZ = (((float) trackY / (float) h) * mapSize) + trackOffsetY;

    // Convert from mini map to world position
//    Vector3 shiftedRange = Vector3(trackPosX, 0, trackPosZ) - Vector3(mapSize/2, mapSize/2, mapSize/2);

  //  URHO3D_LOGINFOF("-----> SET RACE TRACK TO location in world space [%f,%f,%f]", trackPosX, 0.0f, trackPosZ);

    /*
    // 1600+1600
    float xRange = (shiftedRange.x_*mapSize) / miniMapWidth;
    float zRange = (shiftedRange.z_*mapSize) / miniMapHeight;
*/


    ///code/dev/MonkeyMaya_com/bin/Data/Models/logo/Models/

    Node *adjNode;
/*
    // Monkey Maya logo ////
    std::string mmlogoMdlPath = "Models/logo/Models/mm_logo.mdl";
    auto* nodeLG = scene_->CreateChild("MM Logo", REPLICATED);
    nodeLG->SetPosition(Vector3(0, 0, 0));
    nodeLG->SetScale(30.0f);
    adjNode = nodeLG->CreateChild("AdjNode");


    auto *objectLG = adjNode->CreateComponent<StaticModel>(REPLICATED);
    //std::string matPath = "Models/Tracks/Models/trackA.txt";
    auto *modelLG = cache->GetResource<Model>(mmlogoMdlPath.c_str());
    objectLG->SetModel(modelLG);
    //object->ApplyMaterialList(matPath.c_str());
    objectLG->SetCastShadows(true);
    adjNode->SetRotation(Quaternion(0.0, 0.0, 0.0f));
*/

    ////

/*
    Vector3 trackMarkPosA = Vector3(53.5f, 4.2f, -214.0f);

    nodeLG = scene_->CreateChild("MM Logo", REPLICATED);
    nodeLG->SetPosition(trackMarkPosA + Vector3(-240.0f, 0.0f, 50.0f));
    nodeLG->SetRotation(Quaternion(0.0f, 23.0f, -90.0f));
    nodeLG->SetScale(180.0f);

    auto *objectLG2 = nodeLG->CreateComponent<StaticModel>(REPLICATED);
    //std::string matPath = "Models/Tracks/Models/trackA.txt";
    auto *modelLG2 = cache->GetResource<Model>(mmlogoMdlPath.c_str());
    objectLG2->SetModel(modelLG2);
    //object->ApplyMaterialList(matPath.c_str());
    objectLG2->SetCastShadows(true);

*/


    trackPosX = -4800.0f;
    trackPosZ = 700.0f;
/*
    // RACE TRACK ////
    raceTrack_ = scene_->CreateChild("RaceTrack", REPLICATED);

    int trackNum = 1;
    StaticModel *objectRT;

    switch (trackNum) {
        case 0: {
            objectRT = adjNode->CreateComponent<StaticModel>(REPLICATED);
            std::string mdlPath0 = "Models/Tracks/Models/trackA.mdl";
            std::string matPath0 = "Models/Tracks/Models/trackA.txt";
            auto *modelRT0 = cache->GetResource<Model>(mdlPath0.c_str());
            objectRT->SetModel(modelRT0);
            objectRT->ApplyMaterialList(matPath0.c_str());
            objectRT->SetCastShadows(true);
        }
        break;

        case 1: {
            Vector3 position(trackPosX, 80.0f, trackPosZ);
            //position.y_ = terrain_->GetHeight(position) + 2.0f;
            raceTrack_->SetPosition(position);
            // Create a rotation quaternion from up vector to terrain normal
            //raceTrack_->SetRotation(Quaternion(Vector3::UP, terrain_->GetNormal(position)));
            adjNode = raceTrack_->CreateChild("AdjNode");
            adjNode->SetRotation(Quaternion(0.0, 0.0, 0.0f));

            raceTrack_->SetScale(500.0f);

            objectRT = adjNode->CreateComponent<StaticModel>(REPLICATED);
            std::string mdlPath1 = "Models/Tracks/Models/A_trk.mdl";
            std::string matPath1 = "Models/Tracks/Models/A_trk.txt";
            auto *modelRT1 = cache->GetResource<Model>(mdlPath1.c_str());
            objectRT->SetModel(modelRT1);
            objectRT->ApplyMaterialList(matPath1.c_str());
            objectRT->SetCastShadows(true);

        }
            break;


        case 2: {
            Vector3 position(200, 3000.0f, 200);
            //position.y_ = terrain_->GetHeight(position) + 2.0f;
            raceTrack_->SetPosition(position);
            // Create a rotation quaternion from up vector to terrain normal
            //raceTrack_->SetRotation(Quaternion(Vector3::UP, terrain_->GetNormal(position)));
            adjNode = raceTrack_->CreateChild("AdjNode");
            adjNode->SetRotation(Quaternion(0.0, 0.0, 0.0f));
            raceTrack_->SetScale(240.0f);

            objectRT = adjNode->CreateComponent<StaticModel>(REPLICATED);
            std::string mdlPath1 = "Models/Tracks/Models/SpinTrack.mdl";
            std::string matPath1 = "Models/Tracks/Models/SpinTrack.txt";
            auto *modelRT1 = cache->GetResource<Model>(mdlPath1.c_str());
            objectRT->SetModel(modelRT1);
            objectRT->ApplyMaterialList(matPath1.c_str());
            objectRT->SetCastShadows(true);




        }


            break;

    }


    body = adjNode->CreateComponent<RigidBody>(REPLICATED);
    body->SetCollisionLayer(2);
    trackColShape_ = adjNode->CreateComponent<CollisionShape>(REPLICATED);
    trackColShape_->SetTriangleMesh(objectRT->GetModel(), 0);

//        trackColShape_->SetConvexHull(model);

*/
    // Place trees based on markers



    // Convert from mini map to world position
//    Vector3 shiftedRange = Vector3(trackPosX, 0, trackPosZ) - Vector3(mapSize/2, mapSize/2, mapSize/2);

    //URHO3D_LOGINFOF("-----> SET RACE TRACK TO location in world space [%f,%f,%f]", trackPosX, 0.0f, trackPosZ);
/*
    // Place trees
    for (unsigned i = 0; i < trees_.Size(); ++i) {
        float treeOffsetX = -mapSize / 2;
        float treeOffsetY = -mapSize / 2;
        // Convert marker position to world position for track
        float treePosX =
                (((float) trees_[i].x_ / (float) terrain_->GetMarkerMap()->GetWidth()) * mapSize) + treeOffsetX;
        float treePosZ =
                (((float) trees_[i].z_ / (float) terrain_->GetMarkerMap()->GetHeight()) * mapSize) + treeOffsetY;

        Node *objectNode = scene_->CreateChild("Tree", REPLICATED);
        Vector3 position(treePosX, 0.0f, treePosZ);
        position.y_ = terrain_->GetHeight(position) - 0.1f;
        objectNode->SetPosition(position);
        //objectNode->SetEnabled(false);


        // Create a rotation quaternion from up vector to terrain normal
        objectNode->SetRotation(Quaternion(Vector3::UP, terrain_->GetNormal(position)));
        Node *adjNode = objectNode->CreateChild("AdjNode", REPLICATED);
        adjNode->SetRotation(Quaternion(0.0, 0.0, -90.0f));

        objectNode->SetScale(20.0f);

        auto *object = adjNode->CreateComponent<StaticModel>(REPLICATED);

        // Random
        int r = std::round(Random(0.0f, 5.0f));
        switch (r) {
            case 0:
                object->SetModel(cache->GetResource<Model>("Models/AssetPack/tree-baobab_orange.mdl"));
                break;
            case 1:
                object->SetModel(cache->GetResource<Model>("Models/AssetPack/tree-birch02.mdl"));
                break;
            case 2:
                object->SetModel(cache->GetResource<Model>("Models/AssetPack/tree-elipse.mdl"));
                break;
            case 3:
                object->SetModel(cache->GetResource<Model>("Models/AssetPack/tree-fir.mdl"));
                break;
            case 4:
                object->SetModel(cache->GetResource<Model>("Models/AssetPack/tree-oak.mdl"));
                break;
            case 5:
                object->SetModel(cache->GetResource<Model>("Models/AssetPack/tree-lime.mdl"));
                break;
        }

        //       object->SetMaterial(cache->GetResource<Material>("Materials/LOWPOLY_COLORS.xml")
        object->SetMaterial(cache->GetResource<Material>("Materials/LOWPOLY-COLORS.xml"));
        object->SetCastShadows(true);

        auto *body = adjNode->CreateComponent<RigidBody>(REPLICATED);
        body->SetCollisionLayer(2);
        auto *shape = objectNode->CreateComponent<CollisionShape>(REPLICATED);
        shape->SetTriangleMesh(object->GetModel(), 0);
    }

    // Place waypoints

    // Store focusObjects index for waypoint reference
    wpStartIndex_ = focusObjects_.Size() + 1;
    for (unsigned i = 0; i < waypoints_.Size(); ++i) {
        float wpOffsetX = -mapSize / 2;
        float wpOffsetY = -mapSize / 2;
        // Convert marker position to world position for track
        float wpPosX =
                (((float) waypoints_[i].x_ / (float) terrain_->GetMarkerMap()->GetWidth()) * mapSize) + wpOffsetX;
        float wpPosZ =
                (((float) waypoints_[i].z_ / (float) terrain_->GetMarkerMap()->GetHeight()) * mapSize) + wpOffsetY;
//terrain_->GetHeight(Vector3(wpPosX, 0.0f, wpPosZ))
        waypointsWorld_.Push(Vector3(wpPosX, 0, wpPosZ));

        Node *objectNode = scene_->CreateChild("Waypoint", LOCAL);
        Vector3 position(wpPosX, 0.0f, wpPosZ);
        position.y_ = terrain_->GetHeight(position) - 0.1f;
        objectNode->SetPosition(position);

        // Store tree position as focus
        //focusObjects_.Push(position);

        // Create a rotation quaternion from up vector to terrain normal
        objectNode->SetRotation(Quaternion(Vector3::UP, terrain_->GetNormal(position)));
        Node *adjNode = objectNode->CreateChild("AdjNode", LOCAL);
        adjNode->SetRotation(Quaternion(0.0, 0.0, -90.0f));

        objectNode->SetScale(20.0f);

        auto *object = adjNode->CreateComponent<StaticModel>(LOCAL);
        object->SetModel(cache->GetResource<Model>("Models/AssetPack/castle-flag.mdl"));

        //       object->SetMaterial(cache->GetResource<Material>("Materials/LOWPOLY_COLORS.xml")
        object->SetMaterial(cache->GetResource<Material>("Materials/LOWPOLY-COLORS.xml"));
        object->SetCastShadows(true);
        object->SetEnabled(false);

        auto *body = adjNode->CreateComponent<RigidBody>(LOCAL);
        body->SetCollisionLayer(2);
        auto *shape = objectNode->CreateComponent<CollisionShape>(LOCAL);
        shape->SetTriangleMesh(object->GetModel(), 0);
    }

*/


    //
/*
    // Create 1000 mushrooms in the terrain. Always face outward along the terrain normal
    const unsigned NUM_MUSHROOMS = 1000;
    for (unsigned i = 0; i < NUM_MUSHROOMS; ++i)
    {
        Node* objectNode = scene_->CreateChild("Mushroom");
        Vector3 position(Random(2000.0f) - 1000.0f, 0.0f, Random(2000.0f) - 1000.0f);
        position.y_ = terrain_->GetHeight(position) - 0.1f;
        objectNode->SetPosition(position);
        // Create a rotation quaternion from up vector to terrain normal
        objectNode->SetRotation(Quaternion(Vector3::UP, terrain_->GetNormal(position)));
        Node* adjNode = objectNode->CreateChild("AdjNode");
        adjNode->SetRotation(Quaternion(0.0, 0.0, -90.0f));

        objectNode->SetScale(3.5f);

        auto* object = adjNode->CreateComponent<StaticModel>();

        // Random
        int r = std::round(Random(0.0f, 5.0f));
        switch (r) {
            case 0:
                object->SetModel(cache->GetResource<Model>("Models/AssetPack/tree-baobab_orange.mdl"));
                break;
            case 1:
                object->SetModel(cache->GetResource<Model>("Models/AssetPack/tree-birch02.mdl"));
                break;
            case 2:
                object->SetModel(cache->GetResource<Model>("Models/AssetPack/tree-elipse.mdl"));
                break;
            case 3:
                object->SetModel(cache->GetResource<Model>("Models/AssetPack/tree-fir.mdl"));
                break;
            case 4:
                object->SetModel(cache->GetResource<Model>("Models/AssetPack/tree-oak.mdl"));
                break;
            case 5:
                object->SetModel(cache->GetResource<Model>("Models/AssetPack/tree-lime.mdl"));
                break;
        }

 //       object->SetMaterial(cache->GetResource<Material>("Materials/LOWPOLY_COLORS.xml")
        object->SetMaterial(cache->GetResource<Material>("Materials/LOWPOLY-COLORS.xml"));
        object->SetCastShadows(true);

        auto* body = adjNode->CreateComponent<RigidBody>();
        body->SetCollisionLayer(2);
        auto* shape = objectNode->CreateComponent<CollisionShape>();
        shape->SetTriangleMesh(object->GetModel(), 0);
    }*/



    //  }

    // Setup AI
    // Initialize evolution manager
    AlgebraKart::InitEvolutionManager();

    // Attempt to retrieve player spawn line segment marker from pre-loaded scene
    if (!starAMarkerSet_) {
        // Locate start star marker A
        Node *starAStartNode = scene_->GetChild("start_starA", true);
        if (starAStartNode) {
            URHO3D_LOGINFOF("start_starA -> located at -> [%f,%f,%f]", starAStartNode->GetWorldPosition().x_,
                            starAStartNode->GetWorldPosition().y_, starAStartNode->GetWorldPosition().z_);
            starAMarker_ = starAStartNode->GetWorldPosition();
        }

        starAMarkerSet_ = true;
    }

    if (!starBMarkerSet_) {
        // Locate start star marker B
        Node *starBStartNode = scene_->GetChild("start_starB", true);

        if (starBStartNode) {
            URHO3D_LOGINFOF("start_starB -> located at -> [%f,%f,%f]", starBStartNode->GetWorldPosition().x_,
                            starBStartNode->GetWorldPosition().y_, starBStartNode->GetWorldPosition().z_);
            starBMarker_ = starBStartNode->GetWorldPosition();
        }
        starBMarkerSet_ = true;
    }



    // Locate tent A
    if (!buildTentAMarkerSet_) {

        Node *starNode = scene_->GetChild("build_tentA", true);
        if (starNode) {
            URHO3D_LOGINFOF("build tent a -> located at -> [%f,%f,%f]", starNode->GetWorldPosition().x_,
                            starNode->GetWorldPosition().y_, starNode->GetWorldPosition().z_);
            tentAMarker_ = starNode->GetWorldPosition();
        }

        buildTentAMarkerSet_ = true;

        // Build prefabs
        XMLFile *f = cache->GetResource<XMLFile>("Objects/Tent_Building.xml");
        Vector3 pos = Vector3(tentAMarker_);
        Quaternion q = Quaternion(0, 90, 0);
        Node *buildPrefab_ = scene_->InstantiateXML(f->GetRoot(), pos, q, REPLICATED);
        buildPrefab_->SetParent(starNode);
    }


    // Locate track
    Node *track1Node = scene_->GetChild("track1", true);

    // Set collision map for track
    if (track1Node) {
        StaticModel *model = track1Node->GetComponent<StaticModel>(REPLICATED);
        RigidBody *body = track1Node->GetComponent<RigidBody>(REPLICATED);
       // body->SetCollisionLayer(2);
        CollisionShape *trackColShape_ = track1Node->GetComponent<CollisionShape>(REPLICATED);
       // trackColShape_->SetTriangleMesh(model->GetModel(), 0);

//                trackColShape_->SetConvexHull(model->GetModel());
    }

    // Locate Earth
    Node *earth1Node = scene_->GetChild("earth-land", true);

    if (earth1Node) {
        StaticModel *model = earth1Node->GetComponent<StaticModel>(REPLICATED);
        RigidBody *body = earth1Node->GetComponent<RigidBody>(REPLICATED);
        //body->SetCollisionLayer(2);
        CollisionShape *colShape_ = earth1Node->GetComponent<CollisionShape>(REPLICATED);
        // trackColShape_->SetTriangleMesh(model->GetModel(), 0);

//                trackColShape_->SetConvexHull(model->GetModel());
    }


    UpdateLoadingProgress(0.3f, "Creating ai bots...");

    // Spawn the ai bot players
    CreateAgents();

    //for (int i = 0; i < EvolutionManager::getInstance()->getAgents().size(); i++) {
    // Grab ffn count from agent 1

    Sprite *sprite;
    Texture2D *texture;
    int i = 0;
    int numWeights = EvolutionManager::getInstance()->getAgents()[i]->ffn->weightCount;

        for (int j = 0; j < numWeights; j++) {
            // Create sprite for each node of ffn
            sprite = ui->GetRoot()->CreateChild<Sprite>();
            // Get neural net node texture
            texture = cache->GetResource<Texture2D>("Textures/nn-node.png");
            if (!texture)
                return;
            // Set sprite texture
            sprite->SetTexture(texture);
            // Set logo sprite scale
            //logoSprite_->SetScale(256.0f / textureWidth);
            // Set logo sprite size
            sprite->SetSize(texture->GetWidth(), texture->GetHeight());
            sprite->SetAlignment(HA_LEFT, VA_TOP);
            sprite->SetPosition(Vector2(700.0f, 30.0f));
            sprite->SetOpacity(0.8f);
            // Set a low priority so that other UI elements can be drawn on top
            sprite->SetPriority(-100);
            sprite->SetVisible(false);

            // Server-side view
            ffnBotSprite_.Insert(Pair<int, Sprite *>(j, sprite));
        }

    // Create sprite
    sprite = ui->GetRoot()->CreateChild<Sprite>();
    // Get steering wheel texture
    texture = cache->GetResource<Texture2D>("Textures/steer-wheel.png");
    if (!texture)
        return;
    // Set sprite texture
    sprite->SetTexture(texture);
    //sprite->SetScale(256.0f / texture->GetWidth());
    // Set logo sprite size
    sprite->SetSize(texture->GetWidth(), texture->GetHeight());
    sprite->SetAlignment(HA_LEFT, VA_TOP);
    sprite->SetHotSpot(texture->GetWidth()/2, texture->GetHeight()/2);
    sprite->SetPosition(Vector2(700.0f, 500.0f));
    sprite->SetOpacity(0.8f);
    // Set a low priority so that other UI elements can be drawn on top
    sprite->SetPriority(-100);
    sprite->SetVisible(false);
    steerWheelBotSprite_ = sprite;


    // Create sprite
    sprite = ui->GetRoot()->CreateChild<Sprite>();
    // Get velocity bar texture
    texture = cache->GetResource<Texture2D>("Textures/velocity.png");
    if (!texture)
        return;
    // Set sprite texture
    sprite->SetTexture(texture);
    sprite->SetScale(256.0f / texture->GetWidth());
    // Set logo sprite size
    sprite->SetSize(texture->GetWidth(), texture->GetHeight());
    sprite->SetAlignment(HA_LEFT, VA_TOP);
    sprite->SetPosition(Vector2(700.0f, 700.0f));
    sprite->SetOpacity(0.8f);
    // Set a low priority so that other UI elements can be drawn on top
    sprite->SetPriority(-100);
    sprite->SetVisible(false);
    velBarBotSprite_ = sprite;


    // Create sprite
    sprite = ui->GetRoot()->CreateChild<Sprite>();
    // Get velocity bar background texture
    texture = cache->GetResource<Texture2D>("Textures/powerbar-bk.png");
    if (!texture)
        return;

    // Set sprite texture
    sprite->SetTexture(texture);
    // Set logo sprite size
    sprite->SetSize(texture->GetWidth(), texture->GetHeight());
    sprite->SetScale(256.0f / texture->GetWidth());
    sprite->SetAlignment(HA_LEFT, VA_TOP);
    sprite->SetPosition(Vector2(700.0f, 700.0f));
    sprite->SetOpacity(0.8f);
    // Set a low priority so that other UI elements can be drawn on top
    sprite->SetPriority(-100);
    sprite->SetVisible(false);
    velBarBkgBotSprite_ = sprite;

    // Create missile manager as a standalone object
    globalMissileManager_ = SharedPtr<MissileManager>(new MissileManager(context_));
    // Store reference in scene for easy access
    scene_->SetVar("MissileManager", globalMissileManager_.Get());
    URHO3D_LOGINFO("Missile system initialized");

}

void AlgebraKart::HandleKeyDown(StringHash eventType, VariantMap& eventData) {
    using namespace KeyDown;
    int key = eventData[P_KEY].GetInt();

    // Camera mode switching
    if (key == KEY_1) {
        SetCameraMode(CAMERA_FIRST_PERSON);
        URHO3D_LOGINFO("Switched to First Person Camera");
    }
    else if (key == KEY_2) {
        SetCameraMode(CAMERA_THIRD_PERSON);
        URHO3D_LOGINFO("Switched to Third Person Camera");
    }
    else if (key == KEY_3) {
        SetCameraMode(CAMERA_ISOMETRIC);
        URHO3D_LOGINFO("Switched to Isometric Camera");
    }
    else if (key == KEY_4) {
        SetCameraMode(CAMERA_FREEFLY);
        URHO3D_LOGINFO("Switched to Free-fly Camera");
    }

    // Existing key handling code...
    if (key == KEY_TAB) {
        GetSubsystem<Input>()->SetMouseVisible(!GetSubsystem<Input>()->IsMouseVisible());
        GetSubsystem<Input>()->SetMouseGrabbed(!GetSubsystem<Input>()->IsMouseGrabbed());
    }
    else if (key == KEY_F1) {
        drawDebug_ = !drawDebug_;
    }

    // Audio UI hotkeys
    if (key == KEY_B) {
        // Toggle beat sequencer
        if (beatSequencerUI_) {
            bool visible = beatSequencerUI_->GetMainWindow()->IsVisible();
            beatSequencerUI_->SetVisible(!visible);
        }
    }
    else if (key == KEY_N) {
        // Toggle synthesizer controls
        if (synthControlsUI_) {
            bool visible = synthControlsUI_->GetMainWindow()->IsVisible();
            synthControlsUI_->SetVisible(!visible);
        }
    }
    else if (key == KEY_M) {
        // Toggle master audio panel
        if (audioMasterPanel_) {
            bool visible = audioMasterPanel_->IsVisible();
            audioMasterPanel_->SetVisible(!visible);
        }
    } else if (key == KEY_0) {

    }


    // Missile firing with camera (example: F key)
    if (key == KEY_F) {
        if (!isServer_) { // Client only
            NetworkActor* localActor = GetLocalNetworkActor();
            if (localActor) {
                // Simple targeting - fire forward from player
                Vector3 fireDirection = localActor->GetBody()->GetRotation() * Vector3::FORWARD;
                Vector3 targetPos = localActor->GetBody()->GetPosition() + fireDirection * 500.0f;

                FireMissileWithCamera(localActor, targetPos);
            }
        }
    }

    // Replay system controls
    if (replaySystem_) {
        if (key == KEY_R && GetSubsystem<Input>()->GetKeyDown(KEY_CTRL)) {
            // Ctrl+R: Manually trigger replay for local player
            NetworkActor* localActor = GetLocalNetworkActor();
            if (localActor) {
                replaySystem_->TriggerReplay(localActor, REPLAY_MANUAL);
                URHO3D_LOGINFO("Manual replay triggered");
            }
        }
        else if (key == KEY_ESCAPE && replaySystem_->IsPlayingReplay()) {
            // ESC: Skip current replay
            replaySystem_->SkipReplay();
            URHO3D_LOGINFO("Replay skipped");
        }
        else if (key == KEY_F9) {
            // F9: Toggle replay system on/off
            replaySystemEnabled_ = !replaySystemEnabled_;
            URHO3D_LOGINFOF("Replay system %s", replaySystemEnabled_ ? "enabled" : "disabled");
        }

        if (key == KEY_F10) {
            // F10: Trigger cinematic replay of last 10 seconds
            NetworkActor* actor = GetLocalNetworkActor();
            if (actor && actor->vehicle_) {
                replaySystem_->SetRecordingDuration(10.0f);
                replaySystem_->TriggerReplay(actor, REPLAY_MANUAL);
            }
        }
        else if (key == KEY_F11) {
            // F11: Cycle through replay camera modes during playback
            if (replaySystem_->IsPlayingReplay()) {
                // Implementation for cycling camera modes during replay
                URHO3D_LOGINFO("Cycling replay camera mode");
            }
        }
    }
}

// Add this method to set camera mode
void AlgebraKart::SetCameraMode(CameraMode mode) {
    currentCameraMode_ = mode;
    cameraInitialized_ = false; // Force re-initialization for smooth transition

    // Reset smooth camera values
    if (clientCam_ && clientCam_->GetNode()) {
        smoothCameraPosition_ = clientCam_->GetNode()->GetPosition();
        smoothCameraRotation_ = clientCam_->GetNode()->GetRotation();
    }
}

// Get local network actor
NetworkActor* AlgebraKart::GetLocalNetworkActor() {

    // Only move camera if we have a controllable object
    Network *network = GetSubsystem<Network>();
    Connection *serverConnection = network->GetServerConnection();

    if (serverConnection) {
        int size;
        PODVector<Node *> vec;

        String actorName = String("actor-") + clientName_;
        Node * actorNode = scene_->GetChild(actorName);

        String vehicleName = String("vehicle-") + clientName_;
        Node * vehicleNode = scene_->GetChild(vehicleName);

        //BLUE-304-vehicle

        Vector3 pos;
        Quaternion rot;
        Vector3 lVel;

        if (actorNode) {
            // Retrieve Actor
            ClientObj *actor = actorNode->GetDerivedComponent<ClientObj>();

            if (actor) {
                NetworkActor *na = actorNode->GetDerivedComponent<NetworkActor>();
                Vehicle *v = vehicleNode->GetDerivedComponent<Vehicle>();


                if (na) {
                    auto *body = na->GetNode()->GetComponent<RigidBody>(true);
                    if (body) {
                        // CLIENT RIGID BODY RETRIEVED
                        pos = body->GetPosition();
                        rot = body->GetRotation();
                        lVel = body->GetLinearVelocity();
                        // Only link vehicle if it's valid and has a RaycastVehicle
                        if (v && v->GetNode() && v->GetRaycastVehicle()) {
                            na->vehicle_ = v;
                        }
                        return na;
                    }
                }

            }
        }
    }
}

void AlgebraKart::HandleClientDisconnected(StringHash eventType, VariantMap &eventData) {
    using namespace ClientConnected;
    Connection* client = static_cast<Connection*>(eventData[P_CONNECTION].GetPtr());
    String username = String(client->GetIdentity()["UserName"]);

    // Clear client on disconnect
    DestroyPlayer(client);
}


void AlgebraKart::HandleServerConnected(StringHash eventType, VariantMap& eventData)
{
    UpdateLoadingProgress(0.55f, "Connected to server.");
    StartMultiplayerGameSession();
    // Start in game mode
    UpdateUIState(true);

    // For safe
    UnsubscribeFromEvent(E_SERVERCONNECTED);
}

void AlgebraKart::HandleServerDisconnected(StringHash eventType, VariantMap& eventData)
{

}

void AlgebraKart::HandlePlayerRespawned(StringHash eventType, VariantMap& eventData)
{
    UpdateLoadingProgress(1.0f, "Get ready!");

    // Small delay before hiding to show completion
    auto* time = GetSubsystem<Time>();
    float currentTime = time->GetElapsedTime();

    // For now, hide immediately after a frame
    GetSubsystem<Engine>()->RunFrame();
    HideLoadingScreen();

    // CLIENT CODE

    Network* network = GetSubsystem<Network>();
    Connection* connection = network->GetServerConnection();

    // Create client UI once we have spawned
    CreateClientUI();

    // Subscribe the update event for the whole logic
    SubscribeToEvent(E_UPDATE, URHO3D_HANDLER(AlgebraKart, HandleUpdate));

    // UnScribeRemoteEvents
    UnsubscribeFromEvent(E_PLAYERSPAWNED);

    // Calculate track bounds and setup minimap
    CalculateTrackBounds();
    SetupMinimapViewport();

    // Send remote event
    //network->GetServerConnection()->SendRemoteEvent(E_PLAYERSPAWNED, true);
    // Create camera and define viewport. We will be doing load / save, so it's convenient to create the camera outside the scene,
    // so that it won't be destroyed and recreated, and we don't have to redefine the viewport on load

    // Enable for 3D sounds to work (attach to camera node)
    SoundListener *listener = clientCam_->GetNode()->CreateComponent<SoundListener>();
    GetSubsystem<Audio>()->SetListener(listener);


    clientLevelLoading_ = false;

    // Start engine
    PlaySoundEffectLocal(driveAudioEffect[SOUND_FX_ENGINE_START].c_str());

    // Start engine loop
    PlaySoundEffectLocal(driveAudioEffect[SOUND_FX_ENGINE_LOOP].c_str());


    // Initial refresh of login list
    for (int k = 0; k < loginList_.Size(); k++) {
        // Load login list
        if (loginListText_[k]) {
            loginListText_[k]->SetText(loginList_[k]);
        }
    }

    // Load audio spectrum analyzer

    // Default values
    audioSpectrumOptions_.characterSize = 32;
    audioSpectrumOptions_.bottomLevel = -30.0f;
    audioSpectrumOptions_.topLevel = -6.0f;
    audioSpectrumOptions_.minFreq = 30.0f;
    audioSpectrumOptions_.maxFreq = 5000.0f;
    audioSpectrumOptions_.axisLogBase = 10;
    audioSpectrumOptions_.fftSize = 8192;
    audioSpectrumOptions_.inputSize = 2048;
    audioSpectrumOptions_.windowSize = 1;
    audioSpectrumOptions_.smoothing = 0.5f;
    audioSpectrumOptions_.displayAxis = true;
    audioSpectrumOptions_.lineFeed = "CR";
    audioSpectrumOptions_.samplingFrequency = 48000;

    analyzer_ = new AudioSpectrumAnalyzer(audioSpectrumOptions_.inputSize, audioSpectrumOptions_.fftSize, audioSpectrumOptions_.samplingFrequency);
    analyzer_->init(audioSpectrumOptions_.inputSize, audioSpectrumOptions_.fftSize, audioSpectrumOptions_.samplingFrequency);



    /*
("c,chars", "draw the spectrum using N characters.", cxxopts::value<int>()->default_value("32"), "N")
("t,top_db", "the maximum intensity(dB) of the spectrum to be displayed.", cxxopts::value<float>()->default_value("-6"), "x")
("b,bottom_db", "the minimum intensity(dB) of the spectrum to be displayed.", cxxopts::value<float>()->default_value("-30"), "x")
("l,lower_freq", "minimum cutoff frequency(Hz).", cxxopts::value<float>()->default_value("30"), "x")
("u,upper_freq", "maximum cutoff frequency(Hz).", cxxopts::value<float>()->default_value("5000"), "x")
("f,fft_size", "FFT sample size. N must be power of two.", cxxopts::value<int>()->default_value("8192"), "N")
("i,input_size", "N <= fft_size is input sample size.", cxxopts::value<int>()->default_value("2048"), "N")
("g,gaussian_diameter", "display each spectrum bar with a Gaussian blur with the surrounding N bars.", cxxopts::value<int>()->default_value("1"), "N")
("s,smoothing", "x in (0.0, 1.0] is linear interpolation parameter for the previous frame. if 1.0, always display the latest value.", cxxopts::value<float>()->default_value("0.5"), "x")
("a,axis", "display axis if 'on'.", cxxopts::value<std::string>()->default_value("on"), "{\'on\'|\'off\'}")
("axis_log_base", "logarithm base of the horizontal axis.", cxxopts::value<float>()->default_value("10"), "x")
("line_feed", "line feed character.", cxxopts::value<std::string>()->default_value("CR"), "{\'CR\'|\'LF\'|\'CRLF\'}")
*/


    if (audioSpectrumOptions_.displayAxis)
    {


        //std::cout << "_/> " << audioSpectrumOptions_.topLevel << " [dB]\n";
    }

    renderer_ = new AudioSpectrumRenderer(audioSpectrumOptions_.characterSize);
    capturer_ = new AudioSpectrumCapture();
    capturer_->init(audioSpectrumOptions_.inputSize);

}

void AlgebraKart::HandlePlayerLoaded(StringHash eventType, VariantMap &eventData) {

    // CLIENT CODE

    clientPhysicsWorld_->SetEnabled(true);
    clientPhysicsWorld_->SetUpdateEnabled(true);
    scene_->SetUpdateEnabled(true);
    scene_->SetEnabled(true);

    // Clear menu
    Renderer *renderer = GetSubsystem<Renderer>();
    renderer->SetViewport(2, nullptr);

    URHO3D_LOGINFOF("HandlePlayerLoaded");

    // Play first dialogue
    PlaySoundEffectLocal(dialogueAudioEffect[SOUND_FX_DIALOGUE_START_GAME].c_str());

    if (replaySystem_) {
        // CLIENT CODE

        String actorName = String("actor-") + clientName_;
        Node * actorNode = scene_->GetChild(actorName);

        String vehicleName = String("vehicle-") + clientName_;
        Node * vehicleNode = scene_->GetChild(vehicleName);

        Vector3 bodyPos;
        Quaternion rotation;

        if (actorNode) {
            // Retrieve Actor
            ClientObj *actor = actorNode->GetDerivedComponent<ClientObj>();

            if (actor) {
                NetworkActor *na = actorNode->GetDerivedComponent<NetworkActor>();
                Vehicle *v = vehicleNode->GetDerivedComponent<Vehicle>();
                if (na) {
                    auto *body = na->GetNode()->GetComponent<RigidBody>(true);
                    if (body) {
                        // CLIENT RIGID BODY RETRIEVED
                        bodyPos = body->GetPosition();
                        rotation = na->GetNode()->GetRotation();

                        // If this runs automatically, the memory will reach out of memory
                        //replaySystem_->StartRecording(na);
                        /*URHO3D_LOGINFOF("Started replay recording for player: %s",
                                        na->GetNode()->GetName().CString());
                                        */

                    }
                }
            }
        }
    }
}

void AlgebraKart::HandleChatMessage(StringHash eventType, VariantMap &eventData) {
    using namespace ChatMessage;
    String textStr = "[" + eventData[P_USER].GetString() + "]: " + eventData[P_MESSAGE].GetString();
    ShowChatText(textStr);
}

void AlgebraKart::PlayNextTrack() {
    // Go to next track
    radioTrack_++;
    // Clamp tracks to queue size
    radioTrack_ = radioTrack_ % radioTrackInfoQueue_.Size();
    PlayMusic("Sounds/" + radioTrackInfoQueue_.At(radioTrack_).trackName);
}

Vector3 AlgebraKart::GetGridPosition(int gridIndex) {
    int row = gridIndex / GRID_COLUMNS;
    int col = gridIndex % GRID_COLUMNS;

    // Calculate position in grid
    Vector3 gridPos = GRID_START_POSITION;
    if (starAMarkerSet_) {
        Vector3 spawnDir = (starBMarker_ - starAMarker_);
        gridPos = starAMarker_;
        gridPos.y_ += 34.0f;
    }
    gridPos.x_ += (col - (GRID_COLUMNS - 1) * 0.5f) * GRID_SPACING_X;  // Center the grid
    gridPos.z_ += row * GRID_SPACING_Z;

    return gridPos;
}


void AlgebraKart::StartRace() {
    // Reset grid positions for new race
    ResetGridPositions();

    // You can call this when starting a new race or restarting
    // Place any additional race start logic here
}

// Optional: Method to arrange existing players in formation
void AlgebraKart::ArrangePlayersInFormation() {
    ResetGridPositions();

    // Rearrange all connected clients
    for (auto& pair : actorMap_) {
        Connection* connection = pair.first_;
        WeakPtr<ClientObj> clientObj = pair.second_;

        if (clientObj && clientObj->GetComponent<NetworkActor>()) {
            Vector3 newPos = GetGridPosition(nextGridPosition_);
            nextGridPosition_++;

            clientObj->GetNode()->SetPosition(newPos);

            // Move vehicle close to actor
            auto* actor = clientObj->GetNode()->GetComponent<NetworkActor>();
            if (actor->vehicle_) {
                Vector3 vehiclePos = newPos + Vector3(2.0f, 0.0f, -3.0f);
                actor->vehicle_->GetNode()->SetPosition(vehiclePos);

                // Face racing direction
                Quaternion racingOrientation;
                racingOrientation.FromLookRotation(RACING_DIRECTION, Vector3::UP);
                clientObj->GetNode()->SetRotation(racingOrientation);
                //actor->vehicle_->GetNode()->SetRotation(racingOrientation);
            }
        }
    }

    // Rearrange all AI bots
    for (auto& pair : aiActorMap_) {
        unsigned int id = pair.first_;
        ClientObj* clientObj = pair.second_.Get();

        if (clientObj) {
            NetworkActor *na = clientObj->GetComponent<NetworkActor>();
            if (clientObj && clientObj->GetNode()) {
                Vector3 newPos = GetGridPosition(nextGridPosition_);
                nextGridPosition_++;

                clientObj->GetNode()->SetPosition(newPos);

                // Move vehicle close to actor
                if (na->vehicle_) {
                    Vector3 vehiclePos = newPos + Vector3(2.0f, 0.0f, -3.0f);
                    na->vehicle_->GetNode()->SetPosition(vehiclePos);

                    // Face racing direction
                    Quaternion racingOrientation;
                    racingOrientation.FromLookRotation(RACING_DIRECTION, Vector3::UP);
                    na->GetNode()->SetRotation(racingOrientation);
                    na->vehicle_->GetNode()->SetRotation(racingOrientation);
                }
            }
        }
    }
}

// Event handlers - Add to AlgebraKart.cpp:
void AlgebraKart::HandleBeatToggled(StringHash eventType, VariantMap& eventData)
{
    int channel = eventData["Channel"].GetInt();
    int step = eventData["Step"].GetInt();
    bool active = eventData["Active"].GetBool();

    URHO3D_LOGINFOF("Beat toggled - Channel: %d, Step: %d, Active: %s",
                    channel, step, active ? "true" : "false");

    // TODO: Apply the beat change to your actual sequencer
    // You could modify the sequence patterns here
    if (isSnapped_) {
        String actorName = String("actor-") + clientName_;
        Node *actorNode = scene_->GetChild(actorName);

        if (actorNode) {
            ClientObj *actor = actorNode->GetDerivedComponent<ClientObj>();
            if (actor && actor->GetSequencer()) {
                // Here you could modify the sequencer pattern
                // This would require extending your Sequencer class to support
                // dynamic pattern modification

                URHO3D_LOGINFO("Applied beat change to sequencer");
            }
        }
    }
}

void AlgebraKart::HandleSequencerPlayToggle(StringHash eventType, VariantMap& eventData)
{
    bool playing = eventData["Playing"].GetBool();

    URHO3D_LOGINFOF("Sequencer play toggle: %s", playing ? "Playing" : "Stopped");

    if (isSnapped_) {
        String actorName = String("actor-") + clientName_;
        Node *actorNode = scene_->GetChild(actorName);

        if (actorNode) {
            ClientObj *actor = actorNode->GetDerivedComponent<ClientObj>();
            if (actor && actor->GetSequencer()) {
                // Toggle sequencer mute (your current play/stop mechanism)
                actor->GetSequencer()->SetMute(!playing);

                if (playing && !playDrumMachine_) {
                    PlayDrumMachine();
                }
            }
        }
    }
}

void AlgebraKart::HandleSequencerStop(StringHash eventType, VariantMap& eventData)
{
    URHO3D_LOGINFO("Sequencer stopped");

    if (isSnapped_) {
        String actorName = String("actor-") + clientName_;
        Node *actorNode = scene_->GetChild(actorName);

        if (actorNode) {
            ClientObj *actor = actorNode->GetDerivedComponent<ClientObj>();
            if (actor && actor->GetSequencer()) {
                // Stop the sequencer
                actor->GetSequencer()->SetMute(true);

                // Reset to beginning - you might need to add this method to Sequencer
                // actor->GetSequencer()->Reset();
            }
        }
    }
}

void AlgebraKart::HandleSequencerRecordToggle(StringHash eventType, VariantMap& eventData)
{
    bool recording = eventData["Recording"].GetBool();

    URHO3D_LOGINFOF("Sequencer record toggle: %s", recording ? "Recording" : "Not Recording");

    if (isSnapped_) {
        String actorName = String("actor-") + clientName_;
        Node *actorNode = scene_->GetChild(actorName);

        if (actorNode) {
            ClientObj *actor = actorNode->GetDerivedComponent<ClientObj>();
            if (actor && actor->GetSequencer()) {
                // Enable/disable recording mode
                // You might need to add recording functionality to your Sequencer class

                URHO3D_LOGINFO("Set sequencer recording mode");
            }
        }
    }
}

void AlgebraKart::UpdateEnhanced3DSequencer(float timeStep, int currentBeat, bool isPlaying)
{
    if (!seqTimeCursorModel_) return;

    // Animate the time cursor
    float cursorX = (currentBeat - 7.5f) * 2.0f;
    seqTimeCursorModel_->GetNode()->SetPosition(Vector3(cursorX, 2.0f, 0.0f));

    // Update beat pad states with glow effects
    for (int channel = 0; channel < 4; channel++) {
        for (int step = 0; step < 16; step++) {
            int index = channel * 16 + step;
            if (index < beatModelVec_.Size() && beatModelVec_[index]) {

                StaticModel* model = beatModelVec_[index];
                Material* material = model->GetMaterial();

                bool isActive = false; // Disable until implemented GetBeatState(channel, step); // Your beat pattern check
                bool isCurrent = (step == currentBeat) && isPlaying;

                if (isCurrent) {
                    // Current beat - bright glow
                    Color glowColor = Color::WHITE;
                    material->SetShaderParameter("MatEmissiveColor", glowColor);
                    model->GetNode()->SetScale(Vector3(1.5f, 1.0f, 1.5f));
                } else if (isActive) {
                    // Active beat - medium glow
                    Color channelColor = Color::BLUE;//GetChannelColor(channel);
                    material->SetShaderParameter("MatEmissiveColor", channelColor * 0.6f);
                    model->GetNode()->SetScale(Vector3(1.0f, 0.8f, 1.0f));
                } else {
                    // Inactive beat - dim
                    material->SetShaderParameter("MatEmissiveColor", Color::BLACK);
                    model->GetNode()->SetScale(Vector3(1.0f, 0.3f, 1.0f));
                }
            }
        }
    }
}

void AlgebraKart::InitializeMissileSystem() {
    // Create global missile manager for the scene
    Node* missileManagerNode = scene_->CreateChild("MissileManager", LOCAL);
    MissileManager* missileManager = missileManagerNode->CreateComponent<MissileManager>();

    // Store reference for global access
    scene_->SetVar("MissileManager", missileManager);

    URHO3D_LOGINFO("Missile system initialized");
}

int AlgebraKart::FindFreeParticleSlot() {
    for (int i = 0; i < PARTICLE_POOL_SIZE; ++i) {
        if (!particlePool_[i].active || !particlePool_[i].node) {
            return i;
        }
    }

    // If no free slot, reuse oldest slot
    static int nextSlot = 0;
    int oldestSlot = nextSlot;
    nextSlot = (nextSlot + 1) % PARTICLE_POOL_SIZE;

    // Clean up the slot we're about to reuse
    if (particlePool_[oldestSlot].node) {
        particlePool_[oldestSlot].node->Remove();
        particlePool_[oldestSlot].node.Reset();
    }
    if (particlePool_[oldestSlot].emitter) {
        particlePool_[oldestSlot].emitter.Reset();
    }

    return oldestSlot;
}

// When creating particles, use the helper method to find free slots
void AlgebraKart::SetParticleEmitter(int hitId, float contactX, float contactY, int type, float timeStep) {
    // Find a free particle slot
    int slotIndex = FindFreeParticleSlot();

    if (slotIndex >= 0) {
        ParticlePool& particle = particlePool_[slotIndex];

        // Clean up previous particle in this slot
        if (particle.node) {
            particle.node->Remove();
        }

        // Create new particle
        Node* particleNode = scene_->CreateChild("Particle");
        ParticleEmitter* emitter = particleNode->CreateComponent<ParticleEmitter>();

        // Set up the particle
        particle.node = particleNode;
        particle.emitter = emitter;
        particle.active = true;
        particle.timeToLive = 5.0f; // 5 second lifetime
        particle.creationTime = scene_->GetElapsedTime();

        // Configure emitter based on type
        switch (type) {
            case 0: // Explosion
                emitter->SetEffect(GetSubsystem<ResourceCache>()->GetResource<ParticleEffect>("Particle/Explosion.xml"));
                particle.timeToLive = 2.0f;
                break;
            case 1: // Smoke
                emitter->SetEffect(GetSubsystem<ResourceCache>()->GetResource<ParticleEffect>("Particle/Smoke.xml"));
                particle.timeToLive = 8.0f;
                break;
                // Add more types as needed
        }

        // Position the particle
        particleNode->SetPosition(Vector3(contactX, 0.1f, contactY));

        URHO3D_LOGINFOF("Created particle in slot %d, type %d", slotIndex, type);
    } else {
        URHO3D_LOGWARNING("No free particle slots available");
    }
}

bool AlgebraKart::CheckCameraObstruction(const Vector3& cameraPos, const Vector3& targetPos,
                                         NetworkActor* playerActor, float& obstacleDistance) {
    if (!scene_ || !playerActor) {
        obstacleDistance = 0.0f;
        return false;
    }

    Vector3 rayDirection = (cameraPos - targetPos).Normalized();
    float rayDistance = (cameraPos - targetPos).Length();

    if (rayDistance < 0.1f) {
        obstacleDistance = 0.0f;
        return false;
    }

    // Perform raycast from player to camera position
    PhysicsRaycastResult result;
    Ray cameraRay(targetPos, rayDirection);

    // Use physics world raycast with collision filtering
    PODVector<PhysicsRaycastResult> results;
    scene_->GetComponent<PhysicsWorld>()->Raycast(results, cameraRay, rayDistance,
                                                  NETWORKACTOR_COL_LAYER);

    // Filter out player-related objects
    for (const auto& hit : results) {
        if (!hit.body_) continue;

        Node* hitNode = hit.body_->GetNode();
        if (!hitNode) continue;

        // Skip if this is the player's actor node
        if (hitNode == playerActor->GetNode()) continue;

        // Skip if this is the player's vehicle node
        if (playerActor->vehicle_ && hitNode == playerActor->vehicle_->GetNode()) continue;

        // Skip if this is a child of the player's actor or vehicle
        Node* parent = hitNode->GetParent();
        bool isPlayerRelated = false;
        while (parent) {
            if (parent == playerActor->GetNode() ||
                (playerActor->vehicle_ && parent == playerActor->vehicle_->GetNode())) {
                isPlayerRelated = true;
                break;
            }
            parent = parent->GetParent();
        }

        if (isPlayerRelated) continue;

        // Skip if this is a pickup, particle, or other non-obstructing object
        String nodeName = hitNode->GetName();
        if (nodeName.Contains("Pickup") || nodeName.Contains("Particle") ||
            nodeName.Contains("Effect") || nodeName.Contains("FloatText")) {
            continue;
        }

        // We found a valid obstruction
        obstacleDistance = hit.distance_;
        return true;
    }

    obstacleDistance = rayDistance;
    return false;
}

float AlgebraKart::CalculateObstructionZoomOut(float baseDistance, float obstructionDistance) {
    // Calculate how much to zoom out based on obstruction
    float obstructionRatio = obstructionDistance / baseDistance;

    if (obstructionRatio >= 0.9f) {
        // No significant obstruction
        return baseDistance;
    }

    // Calculate zoom out factor - closer obstructions require more zoom
    float zoomOutFactor = 1.0f + (1.0f - obstructionRatio) * 2.0f;
    float adjustedDistance = baseDistance * zoomOutFactor;

    // Clamp to reasonable limits
    return Clamp(adjustedDistance, minCameraDistance_, maxCameraDistance_);
}

void AlgebraKart::UpdateCameraObstruction(float timeStep, NetworkActor* actor) {
    if (!actor || !clientCam_) return;

    // Update check timer
    obstructionCheckTimer_ += timeStep;

    if (obstructionCheckTimer_ >= obstructionCheckInterval_) {
        obstructionCheckTimer_ = 0.0f;

        Vector3 currentCameraPos = clientCam_->GetNode()->GetWorldPosition();
        Vector3 playerPos = actor->GetBody()->GetPosition();

        // Add slight offset to player position for better line of sight
        Vector3 targetPos = playerPos + Vector3::UP * 5.0f;

        float obstacleDistance;
        bool isObstructed = CheckCameraObstruction(currentCameraPos, targetPos, actor, obstacleDistance);

        if (isObstructed) {
            cameraObstructed_ = true;

            // Calculate required distance to clear obstruction
            float clearanceDistance = obstacleDistance * 0.8f; // 80% of obstruction distance for safety
            float currentDistance = (currentCameraPos - targetPos).Length();

            // Determine if we need to zoom out
            if (clearanceDistance < currentDistance) {
                targetCameraDistance_ = CalculateObstructionZoomOut(currentDistance, clearanceDistance);
            }
        } else {
            // Gradually return to normal distance when no obstruction
            if (cameraObstructed_) {
                targetCameraDistance_ = currentCameraDistance_ * 0.95f; // Slowly zoom back in
                if (targetCameraDistance_ <= cameraDistance_ * 1.1f) {
                    cameraObstructed_ = false;
                    targetCameraDistance_ = cameraDistance_;
                }
            }
        }
    }

    // Smooth interpolation of camera distance
    currentCameraDistance_ = Lerp(currentCameraDistance_, targetCameraDistance_,
                                  cameraObstructionSmoothSpeed_ * timeStep);
}

Vector3 AlgebraKart::GetSafeDirectionFromObstruction(const Vector3& obstructedDirection,
                                                     const Vector3& playerPos, float distance) {
    // Try multiple directions around the obstruction
    Vector<Vector3> testDirections;

    // Create test directions in a hemisphere behind the player
    Vector3 up = Vector3::UP;
    Vector3 right = obstructedDirection.CrossProduct(up).Normalized();
    Vector3 forward = up.CrossProduct(right).Normalized();

    // Add test directions with different angles
    float angles[] = {0.0f, 30.0f, -30.0f, 60.0f, -60.0f, 90.0f, -90.0f, 120.0f, -120.0f};

    for (float angle : angles) {
        Quaternion rotation(angle, up);
        Vector3 testDir = rotation * obstructedDirection;
        testDirections.Push(testDir);

        // Also test elevated positions
        Quaternion elevationRot(20.0f, right);
        testDirections.Push(elevationRot * testDir);
    }

    // Test each direction for obstructions
    for (const Vector3& testDir : testDirections) {
        Vector3 testPos = playerPos + testDir * distance;
        float obstacleDistance;

        if (!CheckCameraObstruction(testPos, playerPos, GetLocalNetworkActor(), obstacleDistance)) {
            return testDir;
        }
    }

    // If all directions are obstructed, return elevated position
    return (obstructedDirection + Vector3::UP * 0.5f).Normalized();
}

// Updated camera mode implementations:

void AlgebraKart::UpdateCameraFirstPerson(float timeStep) {
    NetworkActor* actor = GetLocalNetworkActor();
    if (!actor || !actor->GetNode() || !clientCam_) return;

    Node* actorNode = actor->GetNode();
    Node* cameraNode = clientCam_->GetNode();

    // Initialize camera position if needed
    if (!cameraInitialized_) {
        smoothCameraPosition_ = actorNode->GetWorldPosition();
        smoothCameraRotation_ = actorNode->GetWorldRotation();
        targetCameraDistance_ = 2.0f; // Very close for first person
        currentCameraDistance_ = 2.0f;
        cameraInitialized_ = true;
    }

    Vector3 targetPosition;
    Quaternion targetRotation;

    if (actor->onVehicle_ && actor->vehicle_) {
        // In vehicle - position camera at driver's eye level
        Node* vehicleNode = actor->vehicle_->GetNode();
        targetPosition = vehicleNode->GetWorldPosition() +
                         vehicleNode->GetWorldRotation() * Vector3(0.0f, 8.0f, 10.0f) +
                         actor->GetBody()->GetLinearVelocity() / 4.0f;
        targetRotation = vehicleNode->GetWorldRotation();
    } else {
        // On foot - position camera at head level
        targetPosition = actor->GetBody()->GetPosition() +
                         Vector3(0.0f, 2.6f, 0.0f) +
                         actor->GetBody()->GetLinearVelocity() / 4.0f;
        targetRotation = actor->GetBody()->GetRotation();
    }

    // Apply mouse look if right mouse button is held
    if (GetSubsystem<Input>()->GetMouseButtonDown(MOUSEB_RIGHT)) {
        IntVector2 mouseMove = GetSubsystem<Input>()->GetMouseMove();
        const float MOUSE_SENSITIVITY = 0.1f;

        float yawDelta = -mouseMove.x_ * MOUSE_SENSITIVITY;
        float pitchDelta = -mouseMove.y_ * MOUSE_SENSITIVITY;

        Quaternion yawRotation(yawDelta, Vector3::UP);
        Quaternion pitchRotation(pitchDelta, targetRotation * Vector3::RIGHT);

        targetRotation = yawRotation * pitchRotation * targetRotation;
    }

    // First person doesn't need obstruction checking since camera is inside player
    smoothCameraPosition_ = smoothCameraPosition_.Lerp(targetPosition, cameraSmoothSpeed_ * timeStep);
    smoothCameraRotation_ = smoothCameraRotation_.Slerp(targetRotation, cameraSmoothSpeed_ * timeStep);

    cameraNode->SetWorldPosition(smoothCameraPosition_);
    cameraNode->SetRotation(smoothCameraRotation_);
}

void AlgebraKart::UpdateCameraThirdPerson(float timeStep) {
    NetworkActor* actor = GetLocalNetworkActor();
    if (!actor || !actor->GetNode() || !clientCam_) return;
    if (!actor->GetBody()) return;

    Node* actorNode = actor->GetNode();
    Node* cameraNode = clientCam_->GetNode();

    // Initialize camera position if needed
    if (!cameraInitialized_) {
        smoothCameraPosition_ = actor->GetBody()->GetPosition();
        smoothCameraRotation_ = actorNode->GetWorldRotation();
        targetCameraDistance_ = cameraDistance_;
        currentCameraDistance_ = cameraDistance_;
        cameraInitialized_ = true;
    }

    // Update obstruction detection
    UpdateCameraObstruction(timeStep, actor);

    Vector3 targetPosition;
    Vector3 lookAtPosition;
    float dynamicDistance = currentCameraDistance_; // Use obstruction-adjusted distance

    if (actor->onVehicle_ && actor->vehicle_ && actor->vehicle_->GetNode() && actor->vehicle_->GetRaycastVehicle()) {
        // In vehicle - dynamic camera based on speed
        Vector3 vPos = actor->vehicle_->GetBody()->GetPosition();
        float speed = actor->vehicle_->GetSpeedKmH();

        // Adjust camera distance based on speed and obstructions
        float speedDistance = dynamicDistance + (speed * 0.3f);
        float finalDistance = Max(speedDistance, currentCameraDistance_);
        float dynamicHeight = cameraHeight_ + (speed * 0.1f);

        // Calculate camera offset
        Vector3 forward = actor->vehicle_->GetBody()->GetRotation() * Vector3::FORWARD;
        Vector3 baseDirection = -forward; // Behind the vehicle

        // Check if base direction is obstructed
        Vector3 testPos = vPos + baseDirection * finalDistance + Vector3::UP * dynamicHeight;
        float obstacleDistance;

        if (CheckCameraObstruction(testPos, vPos, actor, obstacleDistance)) {
            // Find alternative direction
            baseDirection = GetSafeDirectionFromObstruction(baseDirection, vPos, finalDistance);
        }

        targetPosition = vPos + baseDirection * finalDistance + Vector3::UP * dynamicHeight;
        lookAtPosition = vPos + forward * cameraLookAhead_ + Vector3::UP * 5.0f;
    } else {
        // On foot - standard third person with obstruction handling
        Vector3 forward = actor->GetBody()->GetRotation() * Vector3::FORWARD;
        Vector3 baseDirection = -forward;

        Vector3 testPos = actor->GetBody()->GetPosition() + baseDirection * dynamicDistance + Vector3::UP * 15.0f;
        float obstacleDistance;

        if (CheckCameraObstruction(testPos, actor->GetBody()->GetPosition(), actor, obstacleDistance)) {
            baseDirection = GetSafeDirectionFromObstruction(baseDirection, actor->GetBody()->GetPosition(), dynamicDistance);
        }

        targetPosition = actor->GetBody()->GetPosition() + baseDirection * dynamicDistance + Vector3::UP * 15.0f;
        lookAtPosition = actor->GetBody()->GetPosition() + Vector3::UP * 5.0f;
    }

    // Final obstruction check for the calculated position
    float finalObstacleDistance;
    if (CheckCameraObstruction(targetPosition, lookAtPosition, actor, finalObstacleDistance)) {
        // Move camera closer to clear obstruction
        Vector3 direction = (targetPosition - lookAtPosition).Normalized();
        targetPosition = lookAtPosition + direction * (finalObstacleDistance * 0.8f);
    }

    // Store valid position for fallback
    if (!cameraObstructed_) {
        lastValidCameraPosition_ = targetPosition;
        hasValidCameraPosition_ = true;
    } else if (hasValidCameraPosition_) {
        // Blend towards last valid position if current position is problematic
        targetPosition = targetPosition.Lerp(lastValidCameraPosition_, 0.3f);
    }

    // Smooth camera movement
    smoothCameraPosition_ = smoothCameraPosition_.Lerp(targetPosition, cameraSmoothSpeed_ * timeStep);

    cameraNode->SetWorldPosition(smoothCameraPosition_);
    cameraNode->LookAt(lookAtPosition, Vector3::UP);
}

void AlgebraKart::UpdateCameraIsometric(float timeStep) {
    NetworkActor* actor = GetLocalNetworkActor();
    if (!clientCam_) return;

    Node* cameraNode = clientCam_->GetNode();
    Vector3 targetPosition;
    Vector3 lookAtPosition;

    // Initialize camera for isometric view
    if (!cameraInitialized_) {
        clientCam_->SetOrthographic(false);
        clientCam_->SetFov(30.0f);
        targetCameraDistance_ = isometricCameraHeight_;
        currentCameraDistance_ = isometricCameraHeight_;
        cameraInitialized_ = true;
    }

    float currentSpeed = 0.0f;
    if (actor && actor->GetNode()) {
        lookAtPosition = actor->GetBody()->GetPosition();

        if (actor->onVehicle_ && actor->vehicle_ && actor->vehicle_->GetRaycastVehicle()) {
            currentSpeed = actor->vehicle_->GetSpeedKmH();
        } else {
            Vector3 velocity = actor->GetBody()->GetLinearVelocity();
            currentSpeed = velocity.Length() * 3.6f;
        }

        // Update obstruction detection for isometric view
        UpdateCameraObstruction(timeStep, actor);
    } else {
        lookAtPosition = Vector3::ZERO;
    }

    // Speed-based zoom calculation with obstruction adjustment
    static float smoothedSpeed = 0.0f;
    smoothedSpeed = Lerp(smoothedSpeed, currentSpeed, 2.0f * timeStep);

    float baseHeight = Max(isometricCameraHeight_, currentCameraDistance_);
    float speedMultiplier = 1.0f;

    if (smoothedSpeed > 10.0f) {
        speedMultiplier = 1.0f + ((smoothedSpeed - 10.0f) * 0.02f);
    }

    float dynamicHeight = baseHeight * speedMultiplier;
    dynamicHeight = Clamp(dynamicHeight, 200.0f, maxCameraDistance_);

    // Calculate isometric camera position
    float angleRad = isometricCameraAngle_ * M_DEGTORAD;
    Vector3 cameraOffset(
            dynamicHeight * Sin(angleRad),
            dynamicHeight * Cos(angleRad),
            dynamicHeight * Sin(angleRad)
    );

    targetPosition = lookAtPosition + cameraOffset;

    // Check for obstructions in isometric view
    if (actor) {
        float obstacleDistance;
        if (CheckCameraObstruction(targetPosition, lookAtPosition, actor, obstacleDistance)) {
            // For isometric, just increase height to clear obstruction
            float clearanceHeight = dynamicHeight * 1.5f;
            clearanceHeight = Clamp(clearanceHeight, dynamicHeight, maxCameraDistance_);

            cameraOffset = Vector3(
                    clearanceHeight * Sin(angleRad),
                    clearanceHeight * Cos(angleRad),
                    clearanceHeight * Sin(angleRad)
            );
            targetPosition = lookAtPosition + cameraOffset;
        }
    }

    // Handle mouse controls (preserve existing functionality)
    if (GetSubsystem<Input>()->GetMouseButtonDown(MOUSEB_MIDDLE)) {
        IntVector2 mouseMove = GetSubsystem<Input>()->GetMouseMove();
        const float PAN_SPEED = 0.5f;

        Vector3 right = cameraNode->GetWorldRotation() * Vector3::RIGHT;
        Vector3 forward = Vector3(cameraNode->GetWorldDirection().x_, 0,
                                  cameraNode->GetWorldDirection().z_).Normalized();

        targetPosition += right * mouseMove.x_ * PAN_SPEED;
        targetPosition -= forward * mouseMove.y_ * PAN_SPEED;
        lookAtPosition += right * mouseMove.x_ * PAN_SPEED;
        lookAtPosition -= forward * mouseMove.y_ * PAN_SPEED;
    }

    // Mouse wheel zoom (manual override)
    int wheelDelta = GetSubsystem<Input>()->GetMouseMoveWheel();
    if (wheelDelta != 0) {
        isometricCameraHeight_ = Clamp(isometricCameraHeight_ - wheelDelta * 20.0f,
                                       100.0f, 600.0f);
        targetCameraDistance_ = isometricCameraHeight_;
    }

    // Smooth camera movement
    smoothCameraPosition_ = smoothCameraPosition_.Lerp(targetPosition, cameraSmoothSpeed_ * timeStep);

    cameraNode->SetWorldPosition(smoothCameraPosition_);
    cameraNode->LookAt(lookAtPosition, Vector3::UP);
}

void AlgebraKart::UpdateSteeringWheelDisplay() {
    if (!steerWheelSprite_ || !steerActorSprite_) return;

    // Check if we're in a valid state to show steering wheel
    bool shouldShowSteering = true;
    bool isInVehicle = false;

    if (hasValidSteeringData_) {
        String actorName = String("actor-") + clientName_;
        Node *actorNode = scene_->GetChild(actorName);

        if (actorNode) {
            ClientObj *actor = actorNode->GetDerivedComponent<ClientObj>();
            if (actor) {
                NetworkActor *na = actorNode->GetDerivedComponent<NetworkActor>();
                if (na && na->entered_) {
                    isInVehicle = true;
                    shouldShowSteering = true;
                }
            }
        }
    }

    if (shouldShowSteering) {
        // Show vehicle steering wheel
        steerWheelSprite_->SetVisible(true);

        // Apply steering rotation - convert steering value to degrees
        // Assuming steering ranges from -1.0 to 1.0, map to -180 to 180 degrees
        float steeringDegrees = currentSteeringValue_ * 180.0f; // Max 180 degrees rotation
        steerWheelSprite_->SetRotation(steeringDegrees);

        URHO3D_LOGDEBUGF("Steering wheel updated: value=%f, degrees=%f",
                         currentSteeringValue_, steeringDegrees);
    }
    else {
        // Hide both when not needed
        steerWheelSprite_->SetVisible(false);
    }
}

void AlgebraKart::InitializeMissileCameraSystem() {
    URHO3D_LOGINFO("Initializing Missile Camera System...");

    // Initialize missile camera parameters
    missileCameraActive_ = false;
    missileCameraDistance_ = 50.0f;
    missileCameraHeight_ = 15.0f;
    missileCameraOffset_ = Vector3(0, 10, -30);
    maxMissileCameraTime_ = 15.0f;
    missileCameraSmoothSpeed_ = 8.0f;
    missileCameraInitialized_ = false;

    // Create missile camera UI
    CreateMissileCameraUI();

    URHO3D_LOGINFO("Missile Camera System initialized");
}

void AlgebraKart::CreateMissileCamera() {
    if (missileCameraNode_) {
        URHO3D_LOGWARNING("Missile camera already exists");
        return;
    }

    // Create missile camera node
    missileCameraNode_ = scene_->CreateChild("MissileCamera", LOCAL);
    missileCamera_ = missileCameraNode_->CreateComponent<Camera>();

    // Configure missile camera
    missileCamera_->SetFarClip(5000.0f);
    missileCamera_->SetNearClip(0.1f);
    missileCamera_->SetFov(75.0f);
    missileCamera_->SetFillMode(FILL_SOLID);

    // Set initial position (will be updated in tracking)
    missileCameraNode_->SetPosition(Vector3(0, 100, 0));

    // Setup viewport for missile camera
    SetupMissileCameraViewport();

    // Initialize smooth camera values
    smoothMissileCameraPosition_ = Vector3::ZERO;
    smoothMissileCameraRotation_ = Quaternion::IDENTITY;
    missileCameraInitialized_ = false;

    URHO3D_LOGINFO("Missile camera created");
}

void AlgebraKart::DestroyMissileCamera() {
    if (!missileCameraNode_) return;

    // Remove viewport
    auto* renderer = GetSubsystem<Renderer>();
    renderer->SetViewport(4, nullptr); // Assuming viewport 4 for missile camera

    // Clean up camera node
    missileCameraNode_->Remove();
    missileCameraNode_.Reset();
    missileCamera_.Reset();
    missileViewport_.Reset();

    // Reset tracking
    trackedMissileNode_.Reset();
    missileCameraActive_ = false;
    missileCameraInitialized_ = false;

    // Hide UI elements
    if (missileCameraStatusText_) {
        missileCameraStatusText_->SetVisible(false);
    }
    if (missileCameraFrame_) {
        missileCameraFrame_->SetVisible(false);
    }

    URHO3D_LOGINFO("Missile camera destroyed");
}

void AlgebraKart::StartMissileTracking(Node* missileNode) {
    if (!missileNode) {
        URHO3D_LOGERROR("Cannot track null missile node");
        return;
    }

    // Create camera if it doesn't exist
    if (!missileCameraNode_) {
        CreateMissileCamera();
    }

    // Set tracked missile
    trackedMissileNode_ = missileNode;
    missileCameraActive_ = true;
    missileCameraTimer_.Reset();

    // Initialize camera position relative to missile
    Vector3 missilePos = missileNode->GetWorldPosition();
    smoothMissileCameraPosition_ = missilePos + missileCameraOffset_;
    smoothMissileCameraRotation_ = Quaternion::IDENTITY;
    missileCameraInitialized_ = false;

    // Show UI elements
    if (missileCameraStatusText_) {
        missileCameraStatusText_->SetVisible(true);
        missileCameraStatusText_->SetText("MISSILE CAM - TRACKING");
    }
    if (missileCameraFrame_) {
        missileCameraFrame_->SetVisible(true);
    }

    URHO3D_LOGINFOF("Started tracking missile: %s", missileNode->GetName().CString());
}

void AlgebraKart::StopMissileTracking() {
    if (!missileCameraActive_) return;

    missileCameraActive_ = false;
    trackedMissileNode_.Reset();

    // Keep camera active for a brief moment to show impact
    auto* time = GetSubsystem<Time>();
    float elapsedTime = time->GetElapsedTime();

    // Schedule camera destruction after a short delay
    //GetSubsystem<Engine>()->DelayedExecute(2.0f, [this]() {
    //    DestroyMissileCamera();
    //});
    // Fix destroy missile camera

    // Update UI
    if (missileCameraStatusText_) {
        missileCameraStatusText_->SetText("MISSILE CAM - IMPACT");
    }

    URHO3D_LOGINFO("Stopped missile tracking");
}

void AlgebraKart::UpdateMissileCamera(float timeStep) {
    if (!missileCameraActive_ || !missileCameraNode_ || trackedMissileNode_.Expired()) {
        // Check if missile was destroyed
        if (missileCameraActive_ && trackedMissileNode_.Expired()) {
            StopMissileTracking();
        }
        return;
    }

    Node* missileNode = trackedMissileNode_.Get();
    if (!missileNode) {
        StopMissileTracking();
        return;
    }

    // Check maximum tracking time
    if (missileCameraTimer_.GetMSec(false) > maxMissileCameraTime_ * 1000) {
        URHO3D_LOGINFO("Missile camera timeout reached");
        StopMissileTracking();
        return;
    }

    // Get missile properties
    Vector3 missilePos = missileNode->GetWorldPosition();
    Vector3 missileVelocity = Vector3::ZERO;
    Quaternion missileRotation = missileNode->GetWorldRotation();

    // Try to get missile velocity from RigidBody
    RigidBody* missileBody = missileNode->GetComponent<RigidBody>();
    if (missileBody) {
        missileVelocity = missileBody->GetLinearVelocity();
    }

    // Calculate dynamic camera position based on missile movement
    Vector3 targetCameraPosition;
    Vector3 lookAtPosition = missilePos;

    if (missileVelocity.LengthSquared() > 1.0f) {
        // Missile is moving - position camera behind it
        Vector3 missileDirection = missileVelocity.Normalized();
        float speed = missileVelocity.Length();

        // Dynamic offset based on speed
        float dynamicDistance = missileCameraDistance_ + (speed * 0.5f);
        dynamicDistance = Clamp(dynamicDistance, 20.0f, 200.0f);

        float dynamicHeight = missileCameraHeight_ + (speed * 0.1f);
        dynamicHeight = Clamp(dynamicHeight, 5.0f, 50.0f);

        // Position camera behind and above the missile
        Vector3 cameraOffset = -missileDirection * dynamicDistance + Vector3::UP * dynamicHeight;

        // Add some side offset for better view
        Vector3 rightVector = missileDirection.CrossProduct(Vector3::UP).Normalized();
        cameraOffset += rightVector * (dynamicDistance * 0.3f);

        targetCameraPosition = missilePos + cameraOffset;

        // Look ahead of the missile for better anticipation
        lookAtPosition = missilePos + missileDirection * (dynamicDistance * 0.5f);
    } else {
        // Missile is stationary or moving slowly - use fixed offset
        targetCameraPosition = missilePos + missileCameraOffset_;
    }

    // Check for camera obstructions
    Vector3 rayDirection = (targetCameraPosition - missilePos).Normalized();
    float rayDistance = (targetCameraPosition - missilePos).Length();

    PhysicsRaycastResult result;
    Ray cameraRay(missilePos, rayDirection);
    scene_->GetComponent<PhysicsWorld>()->RaycastSingle(result, cameraRay, rayDistance, 2);
    if (result.body_) {
        // Obstruction detected - move camera closer
        if (result.distance_ > 5.0f) {
            targetCameraPosition = missilePos + rayDirection * (result.distance_ - 2.0f);
        }
    }

    // Initialize smooth camera if needed
    if (!missileCameraInitialized_) {
        smoothMissileCameraPosition_ = targetCameraPosition;
        smoothMissileCameraRotation_ = Quaternion::IDENTITY;
        missileCameraInitialized_ = true;
    }

    // Smooth camera movement
    smoothMissileCameraPosition_ = smoothMissileCameraPosition_.Lerp(
            targetCameraPosition, missileCameraSmoothSpeed_ * timeStep);

    // Update camera position and rotation
    missileCameraNode_->SetWorldPosition(smoothMissileCameraPosition_);
    missileCameraNode_->LookAt(lookAtPosition, Vector3::UP);

    // Update UI with missile information
    if (missileCameraStatusText_) {
        float timeRemaining = maxMissileCameraTime_ - (missileCameraTimer_.GetMSec(false) / 1000.0f);
        String statusText = String("MISSILE CAM - ") + String(timeRemaining, 1) + String("s");

        // Add missile speed if available
        if (missileVelocity.LengthSquared() > 0.1f) {
            float speedKmh = missileVelocity.Length() * 3.6f;
            statusText += String(" | ") + String(speedKmh, 0) + String(" km/h");
        }

        missileCameraStatusText_->SetText(statusText);
    }
}

void AlgebraKart::SetupMissileCameraViewport() {
    if (!missileCamera_) return;

    auto* graphics = GetSubsystem<Graphics>();
    auto* renderer = GetSubsystem<Renderer>();

    // Create missile camera viewport (top-right corner)
    int viewportWidth = 300;
    int viewportHeight = 200;
    int viewportX = graphics->GetWidth() - viewportWidth - 20;
    int viewportY = 20;

    IntRect missileViewportRect(viewportX, viewportY,
                                viewportX + viewportWidth,
                                viewportY + viewportHeight);

    missileViewport_ = new Viewport(context_, scene_, missileCamera_, missileViewportRect);

    // Set render path (can use a simpler one for performance)
    SharedPtr<RenderPath> renderPath = SharedPtr<RenderPath>(new RenderPath());
    renderPath->Load(GetSubsystem<ResourceCache>()->GetResource<XMLFile>("RenderPaths/Forward.xml"));

    // Remove some effects for better performance
    renderPath->RemoveCommands("BloomHDR");
    renderPath->RemoveCommands("FXAA2");

    missileViewport_->SetRenderPath(renderPath);

    // Set the viewport (using viewport index 4)
    renderer->SetViewport(4, missileViewport_);

    URHO3D_LOGINFO("Missile camera viewport created");
}

void AlgebraKart::CreateMissileCameraUI() {
    if (headless_) return;

    auto* ui = GetSubsystem<UI>();
    auto* cache = GetSubsystem<ResourceCache>();
    auto* graphics = GetSubsystem<Graphics>();

    // Create status text for missile camera
    missileCameraStatusText_ = ui->GetRoot()->CreateChild<Text>();
    missileCameraStatusText_->SetFont(cache->GetResource<Font>(INGAME_FONT), 12);
    missileCameraStatusText_->SetColor(Color::YELLOW);
    missileCameraStatusText_->SetAlignment(HA_RIGHT, VA_TOP);
    missileCameraStatusText_->SetPosition(-20, 25);
    missileCameraStatusText_->SetText("MISSILE CAM");
    missileCameraStatusText_->SetVisible(false);
    missileCameraStatusText_->SetPriority(100); // High priority to appear on top

    // Create frame border for missile viewport
    missileCameraFrame_ = ui->GetRoot()->CreateChild<BorderImage>();
    missileCameraFrame_->SetTexture(cache->GetResource<Texture2D>("Textures/UI/UIElement.png"));
    missileCameraFrame_->SetImageBorder(IntRect(2, 2, 2, 2));
    missileCameraFrame_->SetColor(Color::RED);
    missileCameraFrame_->SetSize(304, 204); // Slightly larger than viewport
    missileCameraFrame_->SetAlignment(HA_RIGHT, VA_TOP);
    missileCameraFrame_->SetPosition(-22, 18);
    missileCameraFrame_->SetVisible(false);
    missileCameraFrame_->SetPriority(99);

    // Add crosshair overlay
    auto* crosshair = missileCameraFrame_->CreateChild<Text>();
    crosshair->SetFont(cache->GetResource<Font>(INGAME_FONT), 16);
    crosshair->SetColor(Color::RED);
    crosshair->SetText("+");
    crosshair->SetAlignment(HA_CENTER, VA_CENTER);
    crosshair->SetPosition(0, 0);
}

void AlgebraKart::FireMissileWithCamera(NetworkActor* actor, const Vector3& target) {
    if (!actor || !globalMissileManager_) {
        URHO3D_LOGERROR("Cannot fire missile - invalid actor or missile manager");
        return;
    }

    // Fire the missile using existing missile system
    Vector3 startPos = actor->GetBody()->GetPosition() + Vector3(0, 5, 0);
    Missile *m = globalMissileManager_->CreateMissile(scene_, actor, startPos, target);
    Node* missileNode = m->GetNode();
    if (missileNode) {
        // Start tracking the missile with camera
        StartMissileTracking(missileNode);

        URHO3D_LOGINFOF("Missile fired and camera tracking started");

        // Play missile launch sound
        PlaySoundEffectLocal("Sounds/missile_launch.ogg");
    } else {
        URHO3D_LOGERROR("Failed to create missile");
    }
}