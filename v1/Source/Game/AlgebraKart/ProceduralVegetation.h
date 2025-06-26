#pragma once

#include <Urho3D/Scene/Component.h>
#include <Urho3D/Core/Context.h>
#include <Urho3D/Graphics/StaticModel.h>
#include <Urho3D/Physics/RigidBody.h>
#include <Urho3D/Physics/CollisionShape.h>
#include <Urho3D/Graphics/Terrain.h>
#include <Urho3D/Math/Random.h>
#include <Urho3D/Container/Vector.h>
#include <Urho3D/Container/HashMap.h>
#include <Urho3D/Core/Timer.h>

using namespace Urho3D;

/// Vegetation types
enum VegetationType
{
    VEG_GRASS = 0,
    VEG_BUSH,
    VEG_SMALL_TREE,
    VEG_MEDIUM_TREE,
    VEG_LARGE_TREE,
    VEG_PINE_TREE,
    VEG_COUNT
};

/// Biome types for different vegetation patterns
enum BiomeType
{
    BIOME_GRASSLAND = 0,
    BIOME_FOREST,
    BIOME_DESERT,
    BIOME_MOUNTAIN,
    BIOME_SWAMP,
    BIOME_COUNT
};

/// Growth stage for dynamic vegetation
enum GrowthStage
{
    GROWTH_SEED = 0,
    GROWTH_SPROUT,
    GROWTH_YOUNG,
    GROWTH_MATURE,
    GROWTH_OLD,
    GROWTH_STAGES_COUNT
};

/// Individual vegetation instance
struct VegetationInstance
{
    Vector3 position_;
    VegetationType type_;
    GrowthStage stage_;
    float scale_;
    float rotation_;
    float health_;
    float growthTimer_;
    float ageTimer_;
    WeakPtr<Node> node_;
    bool needsUpdate_;

    VegetationInstance() :
            type_(VEG_GRASS), stage_(GROWTH_SEED), scale_(0.1f),
            rotation_(0.0f), health_(100.0f), growthTimer_(0.0f),
            ageTimer_(0.0f), needsUpdate_(true) {}
};

/// Vegetation distribution parameters
struct VegetationParams
{
    VegetationType type_;
    Vector<String> modelPaths_;
    Vector<String> materialPaths_;
    float density_;              // Instances per square meter
    float minScale_;
    float maxScale_;
    float minSlope_;             // Minimum terrain slope
    float maxSlope_;             // Maximum terrain slope
    float minAltitude_;          // Minimum terrain height
    float maxAltitude_;          // Maximum terrain height
    float growthTime_;           // Time to reach maturity
    float maxAge_;               // Maximum age before decay
    float clusterRadius_;        // Radius for clustering
    float clusterStrength_;      // How much clustering affects placement
    bool hasCollision_;          // Whether to create collision shapes
    bool castShadows_;

    VegetationParams() :
            type_(VEG_GRASS), density_(0.1f), minScale_(0.5f), maxScale_(2.0f),
            minSlope_(0.0f), maxSlope_(45.0f), minAltitude_(-1000.0f), maxAltitude_(1000.0f),
            growthTime_(30.0f), maxAge_(300.0f), clusterRadius_(10.0f), clusterStrength_(0.5f),
            hasCollision_(false), castShadows_(true) {}
};

/// Biome configuration
struct BiomeConfig
{
    BiomeType type_;
    Vector<VegetationParams> vegetationTypes_;
    float temperature_;          // Affects growth rate
    float humidity_;            // Affects vegetation density
    float fertility_;           // Affects vegetation health

    BiomeConfig() : type_(BIOME_GRASSLAND), temperature_(20.0f), humidity_(50.0f), fertility_(75.0f) {}
};

/// Procedural Vegetation System Component
class ProceduralVegetation : public Component
{
URHO3D_OBJECT(ProceduralVegetation, Component);

public:
    /// Construct.
    explicit ProceduralVegetation(Context* context);
    /// Register object factory and attributes.
    static void RegisterObject(Context* context);

    /// Initialize the vegetation system
    void Initialize(Terrain* terrain);

    float PerlinNoise(float x, float y);
    void RemoveVegetationInRadius(const Vector3& center, float radius);
    void SetExclusionZone(const Vector3& center, float radius, bool permanent);
    bool IsInExclusionZone(const Vector3& position) const;
    void CreateTrackExclusionZones(const Vector<Vector3>& trackPoints, float trackWidth);
    void AddVegetationCluster(const Vector3& center, VegetationType type,
                                                    unsigned count, float radius);
    void SetVegetationDensityMap(Image* densityMap);
    float GetDensityFromMap(const Vector3& position) const;
    void SaveVegetationToFile(const String& filename);
    bool LoadVegetationFromFile(const String& filename);
    void GenerateVegetationAroundPoint(const Vector3& center, float radius,
                                                             float density, VegetationType preferredType);
    Vector<VegetationInstance*> GetVegetationInRadius(const Vector3& center, float radius);
    void DamageVegetationInRadius(const Vector3& center, float radius, float damage);
    void SetEnvironmentalParameters(float temperature, float humidity, float windStrength);
    void CreateVegetationAlongPath(const Vector<Vector3>& pathPoints,
                                                         float pathWidth, VegetationType type, float density);
    void GenerateVegetationInChunk(const Vector3& chunkMin, const Vector3& chunkMax);

    /// Generate vegetation for the entire terrain
    void GenerateVegetation();

    /// Generate vegetation in a specific area
    void GenerateVegetationInArea(const Vector3& center, float radius);

    /// Update vegetation growth and LOD
    void Update(float timeStep);

    /// Clear all vegetation
    void ClearVegetation();

    /// Set biome configuration
    void SetBiomeConfig(BiomeType biome, const BiomeConfig& config);

    /// Get biome at world position
    BiomeType GetBiomeAtPosition(const Vector3& position);

    /// Set vegetation parameters for a type
    void SetVegetationParams(VegetationType type, const VegetationParams& params);

    /// Enable/disable dynamic growth
    void SetDynamicGrowth(bool enable) { dynamicGrowth_ = enable; }

    /// Set LOD distances
    void SetLODDistances(float high, float medium, float low)
    {
        lodHighDistance_ = high;
        lodMediumDistance_ = medium;
        lodLowDistance_ = low;
    }

    /// Set update frequency for growth simulation
    void SetUpdateFrequency(float frequency) { updateFrequency_ = frequency; }

    /// Get vegetation count
    unsigned GetVegetationCount() const { return vegetation_.Size(); }

    /// Set generation area (for performance)
    void SetGenerationBounds(const BoundingBox& bounds) { generationBounds_ = bounds; }

private:
    /// Check if position is suitable for vegetation type
    bool IsSuitablePosition(const Vector3& position, VegetationType type);

    /// Get terrain properties at position
    void GetTerrainProperties(const Vector3& position, float& height, float& slope, Vector3& normal);

    /// Create vegetation node
    Node* CreateVegetationNode(const VegetationInstance& instance);

    /// Update vegetation node based on growth stage
    void UpdateVegetationNode(VegetationInstance& instance);

    /// Remove vegetation instance
    void RemoveVegetation(unsigned index);

    /// Calculate density based on biome and terrain
    float CalculateDensity(const Vector3& position, VegetationType type);

    /// Generate cluster of vegetation
    void GenerateCluster(const Vector3& center, VegetationType type, unsigned count);

    /// Update LOD for vegetation instances
    void UpdateLOD(const Vector3& cameraPosition);

    /// Get random vegetation model for type and stage
    String GetRandomModel(VegetationType type, GrowthStage stage);

    /// Calculate growth multiplier based on environmental factors
    float CalculateGrowthMultiplier(const Vector3& position);

    /// Check if position conflicts with existing objects
    bool CheckPositionConflicts(const Vector3& position, float radius);

    void InitializeDefaultParams();
    void InitializeDefaultBiomes();

private:
    /// Terrain reference
    WeakPtr<Terrain> terrain_;

    /// All vegetation instances
    Vector<VegetationInstance> vegetation_;

    /// Vegetation parameters by type - CHANGED: Use unsigned instead of VegetationType
    HashMap<unsigned, VegetationParams> vegetationParams_;

    /// Biome configurations - CHANGED: Use unsigned instead of BiomeType
    HashMap<unsigned, BiomeConfig> biomeConfigs_;

    /// Generation bounds
    BoundingBox generationBounds_;

    /// Growth simulation settings
    bool dynamicGrowth_;
    float updateFrequency_;
    float updateTimer_;

    /// LOD distances
    float lodHighDistance_;
    float lodMediumDistance_;
    float lodLowDistance_;

    /// Performance settings
    unsigned maxVegetationPerFrame_;
    unsigned maxUpdatesPerFrame_;
    float generationRadius_;

    /// Noise parameters for procedural placement
    float noiseScale_;
    float noisePersistence_;
    int noiseOctaves_;

    /// Cached camera position for LOD
    Vector3 lastCameraPosition_;

    /// Seeds for consistent generation
    unsigned densitySeed_;
    unsigned typeSeed_;
    unsigned positionSeed_;

    /// Growth timers
    Timer growthTimer_;
    float seasonalMultiplier_;

    struct ExclusionZone
    {
        Vector3 center_;
        float radius_;
        bool permanent_;

        ExclusionZone() : radius_(0.0f), permanent_(false) {}
    };

    Vector<ExclusionZone> exclusionZones_;
    SharedPtr<Image> densityMap_;

// Environmental parameters
    float environmentalTemp_;
    float environmentalHumidity_;
    float environmentalWind_;
};