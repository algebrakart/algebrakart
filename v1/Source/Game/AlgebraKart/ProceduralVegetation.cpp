#include "ProceduralVegetation.h"
#include <Urho3D/Core/Context.h>
#include <Urho3D/Scene/Scene.h>
#include <Urho3D/Scene/Node.h>
#include <Urho3D/Graphics/StaticModel.h>
#include <Urho3D/Graphics/Model.h>
#include <Urho3D/Graphics/Material.h>
#include <Urho3D/Resource/Image.h>
#include <Urho3D/Physics/RigidBody.h>
#include <Urho3D/Physics/CollisionShape.h>
#include <Urho3D/Resource/ResourceCache.h>
#include <Urho3D/Math/Ray.h>
#include <Urho3D/Physics/PhysicsWorld.h>
#include <Urho3D/Graphics/Camera.h>
#include <Urho3D/IO/Log.h>
#include <Urho3D/Math/MathDefs.h>
#include <Urho3D/Math/Random.h>

using namespace Urho3D;

ProceduralVegetation::ProceduralVegetation(Context* context) :
        Component(context),
        dynamicGrowth_(true),
        updateFrequency_(1.0f),
        updateTimer_(0.0f),
        lodHighDistance_(100.0f),
        lodMediumDistance_(300.0f),
        lodLowDistance_(500.0f),
        maxVegetationPerFrame_(50),
        maxUpdatesPerFrame_(20),
        generationRadius_(200.0f),
        noiseScale_(0.01f),
        noisePersistence_(0.5f),
        noiseOctaves_(3),
        densitySeed_(12345),
        typeSeed_(54321),
        positionSeed_(98765),
        seasonalMultiplier_(1.0f)
{
    // Initialize default vegetation parameters
    InitializeDefaultParams();

    // Initialize default biome configurations
    InitializeDefaultBiomes();
}

void ProceduralVegetation::RegisterObject(Context* context)
{
    context->RegisterFactory<ProceduralVegetation>();
}

void ProceduralVegetation::Initialize(Terrain* terrain)
{
    terrain_ = terrain;

    if (!terrain_)
    {
        URHO3D_LOGERROR("ProceduralVegetation: No terrain provided");
        return;
    }

    // Set generation bounds based on terrain
    Vector3 terrainSize = terrain_->GetNumVertices().Length() * terrain_->GetSpacing();
    Vector3 terrainPos = terrain_->GetNode()->GetWorldPosition();

    generationBounds_.min_ = terrainPos - terrainSize * 0.5f;
    generationBounds_.max_ = terrainPos + terrainSize * 0.5f;

    URHO3D_LOGINFO("ProceduralVegetation: Initialized with terrain bounds: " +
                   generationBounds_.min_.ToString() + " to " + generationBounds_.max_.ToString());
}

void ProceduralVegetation::InitializeDefaultParams()
{
    auto* cache = GetSubsystem<ResourceCache>();

    // Grass parameters
    VegetationParams grassParams;
    grassParams.type_ = VEG_GRASS;
    grassParams.modelPaths_.Push("Models/Vegetation/Grass01.mdl");
    grassParams.modelPaths_.Push("Models/Vegetation/Grass02.mdl");
    grassParams.density_ = 2.0f;
    grassParams.minScale_ = 0.8f;
    grassParams.maxScale_ = 1.2f;
    grassParams.minSlope_ = 0.0f;
    grassParams.maxSlope_ = 30.0f;
    grassParams.growthTime_ = 5.0f;
    grassParams.maxAge_ = 50.0f;
    grassParams.hasCollision_ = false;
    grassParams.castShadows_ = false;
    vegetationParams_[VEG_GRASS] = grassParams;

    // Bush parameters
    VegetationParams bushParams;
    bushParams.type_ = VEG_BUSH;
    bushParams.modelPaths_.Push("Models/AssetPack/tree-elipse.mdl");
    bushParams.modelPaths_.Push("Models/AssetPack/tree-lime.mdl");
    bushParams.density_ = 0.5f;
    bushParams.minScale_ = 0.5f;
    bushParams.maxScale_ = 1.5f;
    bushParams.minSlope_ = 0.0f;
    bushParams.maxSlope_ = 25.0f;
    bushParams.growthTime_ = 15.0f;
    bushParams.maxAge_ = 100.0f;
    bushParams.hasCollision_ = true;
    bushParams.clusterRadius_ = 5.0f;
    bushParams.clusterStrength_ = 0.7f;
    vegetationParams_[VEG_BUSH] = bushParams;

    // Small tree parameters
    VegetationParams smallTreeParams;
    smallTreeParams.type_ = VEG_SMALL_TREE;
    smallTreeParams.modelPaths_.Push("Models/AssetPack/tree-birch02.mdl");
    smallTreeParams.modelPaths_.Push("Models/AssetPack/tree-oak.mdl");
    smallTreeParams.density_ = 0.2f;
    smallTreeParams.minScale_ = 0.8f;
    smallTreeParams.maxScale_ = 1.5f;
    smallTreeParams.minSlope_ = 0.0f;
    smallTreeParams.maxSlope_ = 20.0f;
    smallTreeParams.growthTime_ = 30.0f;
    smallTreeParams.maxAge_ = 200.0f;
    smallTreeParams.hasCollision_ = true;
    smallTreeParams.clusterRadius_ = 8.0f;
    smallTreeParams.clusterStrength_ = 0.6f;
    vegetationParams_[VEG_SMALL_TREE] = smallTreeParams;

    // Medium tree parameters
    VegetationParams mediumTreeParams;
    mediumTreeParams.type_ = VEG_MEDIUM_TREE;
    mediumTreeParams.modelPaths_.Push("Models/AssetPack/tree-baobab_orange.mdl");
    mediumTreeParams.modelPaths_.Push("Models/AssetPack/tree-oak.mdl");
    mediumTreeParams.density_ = 0.1f;
    mediumTreeParams.minScale_ = 1.2f;
    mediumTreeParams.maxScale_ = 2.0f;
    mediumTreeParams.minSlope_ = 0.0f;
    mediumTreeParams.maxSlope_ = 15.0f;
    mediumTreeParams.growthTime_ = 60.0f;
    mediumTreeParams.maxAge_ = 400.0f;
    mediumTreeParams.hasCollision_ = true;
    mediumTreeParams.clusterRadius_ = 12.0f;
    mediumTreeParams.clusterStrength_ = 0.5f;
    vegetationParams_[VEG_MEDIUM_TREE] = mediumTreeParams;

    // Large tree parameters
    VegetationParams largeTreeParams;
    largeTreeParams.type_ = VEG_LARGE_TREE;
    largeTreeParams.modelPaths_.Push("Models/AssetPack/tree-fir.mdl");
    largeTreeParams.density_ = 0.05f;
    largeTreeParams.minScale_ = 2.0f;
    largeTreeParams.maxScale_ = 3.5f;
    largeTreeParams.minSlope_ = 0.0f;
    largeTreeParams.maxSlope_ = 10.0f;
    largeTreeParams.growthTime_ = 120.0f;
    largeTreeParams.maxAge_ = 800.0f;
    largeTreeParams.hasCollision_ = true;
    largeTreeParams.clusterRadius_ = 20.0f;
    largeTreeParams.clusterStrength_ = 0.4f;
    vegetationParams_[VEG_LARGE_TREE] = largeTreeParams;

    // Pine tree parameters
    VegetationParams pineParams;
    pineParams.type_ = VEG_PINE_TREE;
    pineParams.modelPaths_.Push("Models/AssetPack/tree-fir.mdl");
    pineParams.density_ = 0.3f;
    pineParams.minScale_ = 1.0f;
    pineParams.maxScale_ = 2.5f;
    pineParams.minSlope_ = 0.0f;
    pineParams.maxSlope_ = 35.0f;
    pineParams.minAltitude_ = 100.0f;
    pineParams.growthTime_ = 45.0f;
    pineParams.maxAge_ = 300.0f;
    pineParams.hasCollision_ = true;
    pineParams.clusterRadius_ = 15.0f;
    pineParams.clusterStrength_ = 0.8f;
    vegetationParams_[VEG_PINE_TREE] = pineParams;
}

void ProceduralVegetation::InitializeDefaultBiomes()
{
    // Grassland biome
    BiomeConfig grassland;
    grassland.type_ = BIOME_GRASSLAND;
    grassland.temperature_ = 22.0f;
    grassland.humidity_ = 60.0f;
    grassland.fertility_ = 80.0f;
    biomeConfigs_[BIOME_GRASSLAND] = grassland;

    // Forest biome
    BiomeConfig forest;
    forest.type_ = BIOME_FOREST;
    forest.temperature_ = 18.0f;
    forest.humidity_ = 80.0f;
    forest.fertility_ = 90.0f;
    biomeConfigs_[BIOME_FOREST] = forest;

    // Mountain biome
    BiomeConfig mountain;
    mountain.type_ = BIOME_MOUNTAIN;
    mountain.temperature_ = 10.0f;
    mountain.humidity_ = 50.0f;
    mountain.fertility_ = 60.0f;
    biomeConfigs_[BIOME_MOUNTAIN] = mountain;

    // Desert biome
    BiomeConfig desert;
    desert.type_ = BIOME_DESERT;
    desert.temperature_ = 35.0f;
    desert.humidity_ = 20.0f;
    desert.fertility_ = 30.0f;
    biomeConfigs_[BIOME_DESERT] = desert;
}

void ProceduralVegetation::GenerateVegetation()
{
    if (!terrain_)
        return;

    URHO3D_LOGINFO("ProceduralVegetation: Starting vegetation generation...");

    ClearVegetation();

    // Generate vegetation in chunks for better performance
    const float chunkSize = 100.0f;
    int chunksX = (int)Ceil((generationBounds_.max_.x_ - generationBounds_.min_.x_) / chunkSize);
    int chunksZ = (int)Ceil((generationBounds_.max_.z_ - generationBounds_.min_.z_) / chunkSize);

    for (int x = 0; x < chunksX; ++x)
    {
        for (int z = 0; z < chunksZ; ++z)
        {
            Vector3 chunkMin = generationBounds_.min_ + Vector3(x * chunkSize, 0, z * chunkSize);
            Vector3 chunkMax = chunkMin + Vector3(chunkSize, 0, chunkSize);

            GenerateVegetationInChunk(chunkMin, chunkMax);
        }
    }

    URHO3D_LOGINFOF("ProceduralVegetation: Generated %d vegetation instances", vegetation_.Size());
}

void ProceduralVegetation::GenerateVegetationInChunk(const Vector3& chunkMin, const Vector3& chunkMax)
{
    const float sampleDistance = 2.0f; // Distance between sample points

    for (float x = chunkMin.x_; x < chunkMax.x_; x += sampleDistance)
    {
        for (float z = chunkMin.z_; z < chunkMax.z_; z += sampleDistance)
        {
            Vector3 position(x, 0, z);

            // Get terrain height at this position
            float height, slope;
            Vector3 normal;
            GetTerrainProperties(position, height, slope, normal);

            if (height < -500.0f) // Skip water areas
                continue;

            position.y_ = height;

            // Determine biome at this position
            BiomeType biome = GetBiomeAtPosition(position);

            // Generate each vegetation type
            for (int vegType = 0; vegType < VEG_COUNT; ++vegType)
            {
                VegetationType type = (VegetationType)vegType;

                if (!IsSuitablePosition(position, type))
                    continue;

                float density = CalculateDensity(position, type);
                float probability = density * sampleDistance * sampleDistance;

                // Use noise for natural distribution
                float noise = PerlinNoise(position.x_ * noiseScale_, position.z_ * noiseScale_);
                probability *= (0.5f + noise * 0.5f);

                if (Random(1.0f) < probability)
                {
                    // Create vegetation instance
                    VegetationInstance instance;
                    instance.position_ = position;
                    instance.type_ = type;
                    instance.stage_ = dynamicGrowth_ ? GROWTH_SEED : GROWTH_MATURE;
                    instance.scale_ = Lerp(vegetationParams_[(unsigned)type].minScale_, // CAST TO UNSIGNED
                                           vegetationParams_[(unsigned)type].maxScale_, Random(1.0f)); // CAST TO UNSIGNED
                    instance.rotation_ = Random(360.0f);
                    instance.health_ = Random(80.0f, 100.0f);

                    // Adjust initial scale for growth stage
                    if (instance.stage_ == GROWTH_SEED)
                        instance.scale_ *= 0.1f;

                    // Add some variation to avoid conflicts
                    instance.position_ += Vector3(Random(-1.0f, 1.0f), 0, Random(-1.0f, 1.0f));

                    // Check for conflicts with existing vegetation
                    if (!CheckPositionConflicts(instance.position_, instance.scale_))
                    {
                        vegetation_.Push(instance);
                    }
                }
            }
        }
    }
}

void ProceduralVegetation::Update(float timeStep)
{
    if (vegetation_.Empty() || !dynamicGrowth_)
        return;

    updateTimer_ += timeStep;

    if (updateTimer_ < 1.0f / updateFrequency_)
        return;

    updateTimer_ = 0.0f;

    // Update growth for a subset of vegetation each frame
    unsigned startIndex = (unsigned)(Random((float)vegetation_.Size()));
    unsigned count = Min(maxUpdatesPerFrame_, vegetation_.Size());

    for (unsigned i = 0; i < count; ++i)
    {
        unsigned index = (startIndex + i) % vegetation_.Size();
        VegetationInstance& instance = vegetation_[index];

        // Update growth
        if (instance.stage_ < GROWTH_MATURE)
        {
            float growthMultiplier = CalculateGrowthMultiplier(instance.position_);
            instance.growthTimer_ += timeStep * growthMultiplier * seasonalMultiplier_;

            const VegetationParams& params = vegetationParams_[(unsigned)instance.type_]; // CAST TO UNSIGNED
            float stageTime = params.growthTime_ / GROWTH_STAGES_COUNT;

            if (instance.growthTimer_ >= stageTime)
            {
                instance.stage_ = (GrowthStage)(instance.stage_ + 1);
                instance.growthTimer_ = 0.0f;
                instance.needsUpdate_ = true;

                // Update scale based on new stage
                float stageProgress = (float)instance.stage_ / (float)GROWTH_STAGES_COUNT;
                float targetScale = Lerp(params.minScale_, params.maxScale_, Random(1.0f));
                instance.scale_ = targetScale * stageProgress;
            }
        }

        // Update aging
        instance.ageTimer_ += timeStep;
        const VegetationParams& params = vegetationParams_[(unsigned)instance.type_]; // CAST TO UNSIGNED

        if (instance.ageTimer_ > params.maxAge_)
        {
            // Remove old vegetation
            RemoveVegetation(index);
            --i; // Adjust loop counter
            continue;
        }

        // Update visual representation if needed
        if (instance.needsUpdate_)
        {
            UpdateVegetationNode(instance);
            instance.needsUpdate_ = false;
        }
    }

    // Update seasonal multiplier (simulate seasons)
    float seasonCycle = Sin(growthTimer_.GetMSec(false) * 0.0001f) * 0.5f + 0.5f;
    seasonalMultiplier_ = 0.5f + seasonCycle;
}

void ProceduralVegetation::UpdateVegetationNode(VegetationInstance& instance)
{
    if (instance.node_.Expired())
    {
        // Create new node
        Node* node = CreateVegetationNode(instance);
        instance.node_ = node;
    }
    else
    {
        // Update existing node
        Node* node = instance.node_.Get();
        node->SetScale(instance.scale_);

        // Update model based on growth stage
        StaticModel* model = node->GetComponent<StaticModel>();
        if (model)
        {
            auto* cache = GetSubsystem<ResourceCache>();
            String modelPath = GetRandomModel(instance.type_, instance.stage_);
            Model* newModel = cache->GetResource<Model>(modelPath);
            if (newModel && newModel != model->GetModel())
            {
                model->SetModel(newModel);
            }
        }

        // Add/remove collision based on growth stage
        const VegetationParams& params = vegetationParams_[(unsigned)instance.type_]; // CAST TO UNSIGNED
        RigidBody* body = node->GetComponent<RigidBody>();

        if (params.hasCollision_ && instance.stage_ >= GROWTH_YOUNG && !body)
        {
            body = node->CreateComponent<RigidBody>();
            body->SetCollisionLayer(2);
            body->SetMass(0.0f);

            CollisionShape* shape = node->CreateComponent<CollisionShape>();
            StaticModel* model = node->GetComponent<StaticModel>();
            if (model && model->GetModel())
                shape->SetTriangleMesh(model->GetModel());
        }
        else if ((!params.hasCollision_ || instance.stage_ < GROWTH_YOUNG) && body)
        {
            node->RemoveComponent<RigidBody>();
            node->RemoveComponent<CollisionShape>();
        }
    }
}

Node* ProceduralVegetation::CreateVegetationNode(const VegetationInstance& instance)
{
    auto* cache = GetSubsystem<ResourceCache>();
    Scene* scene = GetScene();

    if (!scene)
        return nullptr;

    // Create node
    Node* node = scene->CreateChild("Vegetation", LOCAL);
    node->SetPosition(instance.position_);
    node->SetRotation(Quaternion(instance.rotation_, Vector3::UP));
    node->SetScale(instance.scale_);

    // Create visual model
    StaticModel* model = node->CreateComponent<StaticModel>();
    String modelPath = GetRandomModel(instance.type_, instance.stage_);

    Model* modelRes = cache->GetResource<Model>(modelPath);
    if (modelRes)
    {
        model->SetModel(modelRes);

        // Set material
        const VegetationParams& params = vegetationParams_[(unsigned)instance.type_]; // CAST TO UNSIGNED

        // Create collision if needed
        if (params.hasCollision_ && instance.stage_ >= GROWTH_YOUNG)
        {
            RigidBody* body = node->CreateComponent<RigidBody>();
            body->SetCollisionLayer(2); // Static geometry layer
            body->SetMass(0.0f); // Static body

            CollisionShape* shape = node->CreateComponent<CollisionShape>();
            if (modelRes)
                shape->SetTriangleMesh(modelRes);
            else
                shape->SetBox(Vector3::ONE);
        }

        if (!params.materialPaths_.Empty())
        {
            String materialPath = params.materialPaths_[Random((int)params.materialPaths_.Size())];
            Material* material = cache->GetResource<Material>(materialPath);
            if (material)
                model->SetMaterial(material);
        }
        else
        {
            // Use default material
            Material* material = cache->GetResource<Material>("Materials/LOWPOLY-COLORS.xml");
            if (material)
                model->SetMaterial(material);
        }

        model->SetCastShadows(params.castShadows_);
    }

    return node;
}

void ProceduralVegetation::AddVegetationCluster(const Vector3& center, VegetationType type,
                                                unsigned count, float radius)
{
    const VegetationParams& params = vegetationParams_[(unsigned)type]; // CAST TO UNSIGNED

    for (unsigned i = 0; i < count; ++i)
    {
        // Generate random position within cluster radius
        float angle = Random(360.0f) * M_DEGTORAD;
        float distance = Random(radius);

        Vector3 position = center + Vector3(
                Cos(angle) * distance,
                0.0f,
                Sin(angle) * distance
        );

        // Get terrain height
        float height, slope;
        Vector3 normal;
        GetTerrainProperties(position, height, slope, normal);
        position.y_ = height;

        // Check if position is suitable
        if (!IsSuitablePosition(position, type) || IsInExclusionZone(position))
            continue;

        // Check for conflicts
        if (CheckPositionConflicts(position, params.minScale_))
            continue;

        // Create vegetation instance
        VegetationInstance instance;
        instance.position_ = position;
        instance.type_ = type;
        instance.stage_ = dynamicGrowth_ ?
                          (GrowthStage)Random(GROWTH_STAGES_COUNT) : GROWTH_MATURE;
        instance.scale_ = Lerp(params.minScale_, params.maxScale_, Random(1.0f));
        instance.rotation_ = Random(360.0f);
        instance.health_ = Random(80.0f, 100.0f);

        // Adjust scale for growth stage
        if (instance.stage_ != GROWTH_MATURE)
        {
            float stageMultiplier = (float)instance.stage_ / (float)GROWTH_STAGES_COUNT;
            instance.scale_ *= stageMultiplier;
        }

        vegetation_.Push(instance);
    }
}

void ProceduralVegetation::GenerateVegetationAroundPoint(const Vector3& center, float radius,
                                                         float density, VegetationType preferredType)
{
    const float sampleDistance = 1.0f;
    int samples = (int)(radius * 2.0f / sampleDistance);

    for (int x = -samples; x <= samples; ++x)
    {
        for (int z = -samples; z <= samples; ++z)
        {
            Vector3 offset(x * sampleDistance, 0, z * sampleDistance);
            Vector3 position = center + offset;

            float distance = offset.Length();
            if (distance > radius)
                continue;

            // Get terrain height
            float height, slope;
            Vector3 normal;
            GetTerrainProperties(position, height, slope, normal);
            position.y_ = height;

            // Skip if in exclusion zone
            if (IsInExclusionZone(position))
                continue;

            // Determine vegetation type (prefer specified type but allow others)
            VegetationType type = preferredType;
            if (Random(1.0f) > 0.7f) // 30% chance for other types
            {
                type = (VegetationType)Random(VEG_COUNT);
            }

            if (!IsSuitablePosition(position, type))
                continue;

            // Calculate probability based on distance from center
            float distanceMultiplier = 1.0f - (distance / radius);
            float probability = density * distanceMultiplier * sampleDistance * sampleDistance;

            if (Random(1.0f) < probability)
            {
                const VegetationParams& params = vegetationParams_[(unsigned)type]; // CAST TO UNSIGNED

                // Check for conflicts
                if (CheckPositionConflicts(position, params.minScale_))
                    continue;

                // Create vegetation instance
                VegetationInstance instance;
                instance.position_ = position;
                instance.type_ = type;
                instance.stage_ = dynamicGrowth_ ? GROWTH_SEED : GROWTH_MATURE;
                instance.scale_ = Lerp(params.minScale_, params.maxScale_, Random(1.0f));
                instance.rotation_ = Random(360.0f);
                instance.health_ = Random(80.0f, 100.0f);

                vegetation_.Push(instance);
            }
        }
    }
}


void ProceduralVegetation::GenerateVegetationInArea(const Vector3& center, float radius)
{
    if (!terrain_)
        return;

    Vector3 areaMin = center - Vector3(radius, 0, radius);
    Vector3 areaMax = center + Vector3(radius, 0, radius);

    // Clamp to generation bounds
    areaMin.x_ = Max(areaMin.x_, generationBounds_.min_.x_);
    areaMin.z_ = Max(areaMin.z_, generationBounds_.min_.z_);
    areaMax.x_ = Min(areaMax.x_, generationBounds_.max_.x_);
    areaMax.z_ = Min(areaMax.z_, generationBounds_.max_.z_);

    GenerateVegetationInChunk(areaMin, areaMax);
}

bool ProceduralVegetation::IsSuitablePosition(const Vector3& position, VegetationType type)
{
    const VegetationParams& params = vegetationParams_[type];

    float height, slope;
    Vector3 normal;
    GetTerrainProperties(position, height, slope, normal);

    // Check altitude constraints
    if (height < params.minAltitude_ || height > params.maxAltitude_)
        return false;

    // Check slope constraints
    if (slope < params.minSlope_ || slope > params.maxSlope_)
        return false;

    return true;
}

void ProceduralVegetation::GetTerrainProperties(const Vector3& position, float& height, float& slope, Vector3& normal)
{
    if (!terrain_)
    {
        height = 0.0f;
        slope = 0.0f;
        normal = Vector3::UP;
        return;
    }

    height = terrain_->GetHeight(position);
    normal = terrain_->GetNormal(position);

    // Calculate slope from normal
    slope = Acos(normal.DotProduct(Vector3::UP)) * M_RADTODEG;
}

void ProceduralVegetation::RemoveVegetation(unsigned index)
{
    if (index >= vegetation_.Size())
        return;

    VegetationInstance& instance = vegetation_[index];
    if (!instance.node_.Expired())
    {
        instance.node_.Get()->Remove();
    }

    vegetation_.Erase(index);
}

float ProceduralVegetation::CalculateDensity(const Vector3& position, VegetationType type)
{
    const VegetationParams& params = vegetationParams_[type];
    BiomeType biome = GetBiomeAtPosition(position);
    const BiomeConfig& biomeConfig = biomeConfigs_[biome];

    float baseDensity = params.density_;

    // Modify density based on biome
    switch (biome)
    {
        case BIOME_FOREST:
            if (type >= VEG_SMALL_TREE)
                baseDensity *= 2.0f;
            else if (type == VEG_GRASS)
                baseDensity *= 0.5f;
            break;

        case BIOME_GRASSLAND:
            if (type == VEG_GRASS)
                baseDensity *= 3.0f;
            else if (type >= VEG_MEDIUM_TREE)
                baseDensity *= 0.2f;
            break;

        case BIOME_DESERT:
            baseDensity *= 0.1f;
            break;

        case BIOME_MOUNTAIN:
            if (type == VEG_PINE_TREE)
                baseDensity *= 1.5f;
            else
                baseDensity *= 0.7f;
            break;
    }

    // Apply fertility modifier
    baseDensity *= biomeConfig.fertility_ / 100.0f;

    return baseDensity;
}

BiomeType ProceduralVegetation::GetBiomeAtPosition(const Vector3& position)
{
    // Simple biome determination based on altitude and position
    // You can make this more sophisticated with noise functions

    float height, slope;
    Vector3 normal;
    GetTerrainProperties(position, height, slope, normal);

    if (height > 200.0f)
        return BIOME_MOUNTAIN;
    else if (height < 0.0f)
        return BIOME_SWAMP;
    else
    {
        // Use noise to determine between forest and grassland
        float biomeNoise = PerlinNoise(position.x_ * 0.001f, position.z_ * 0.001f);
        if (biomeNoise > 0.2f)
            return BIOME_FOREST;
        else
            return BIOME_GRASSLAND;
    }
}

String ProceduralVegetation::GetRandomModel(VegetationType type, GrowthStage stage)
{
    const VegetationParams& params = vegetationParams_[type];

    if (params.modelPaths_.Empty())
        return "Models/Box.mdl"; // Fallback

    // For now, use random model from available paths
    // You could implement stage-specific models here
    unsigned index = Random((int)params.modelPaths_.Size());
    return params.modelPaths_[index];
}

float ProceduralVegetation::CalculateGrowthMultiplier(const Vector3& position)
{
    BiomeType biome = GetBiomeAtPosition(position);
    const BiomeConfig& config = biomeConfigs_[biome];

    // Growth is affected by temperature, humidity, and fertility
    float tempFactor = 1.0f - Abs(config.temperature_ - 20.0f) / 40.0f; // Optimal at 20°C
    float humidityFactor = config.humidity_ / 100.0f;
    float fertilityFactor = config.fertility_ / 100.0f;

    return Max(0.1f, tempFactor * humidityFactor * fertilityFactor);
}

bool ProceduralVegetation::CheckPositionConflicts(const Vector3& position, float radius)
{
    // Check conflicts with existing vegetation
    for (const VegetationInstance& existing : vegetation_)
    {
        float distance = (position - existing.position_).Length();
        float minDistance = radius + existing.scale_;

        if (distance < minDistance)
            return true; // Conflict found
    }

    return false; // No conflicts
}

void ProceduralVegetation::ClearVegetation()
{
    for (VegetationInstance& instance : vegetation_)
    {
        if (!instance.node_.Expired())
            instance.node_.Get()->Remove();
    }

    vegetation_.Clear();
}

void ProceduralVegetation::SetBiomeConfig(BiomeType biome, const BiomeConfig& config)
{
    biomeConfigs_[biome] = config;
}

void ProceduralVegetation::SetVegetationParams(VegetationType type, const VegetationParams& params)
{
    vegetationParams_[type] = params;
}

void ProceduralVegetation::UpdateLOD(const Vector3& cameraPosition)
{
    lastCameraPosition_ = cameraPosition;

    for (VegetationInstance& instance : vegetation_)
    {
        if (instance.node_.Expired())
            continue;

        float distance = (cameraPosition - instance.position_).Length();
        Node* node = instance.node_.Get();
        StaticModel* model = node->GetComponent<StaticModel>();

        if (!model)
            continue;

        // Set LOD based on distance
        if (distance > lodLowDistance_)
        {
            node->SetEnabled(false);
        }
        else if (distance > lodMediumDistance_)
        {
            node->SetEnabled(true);
            model->SetLodBias(2.0f);
        }
        else if (distance > lodHighDistance_)
        {
            node->SetEnabled(true);
            model->SetLodBias(1.0f);
        }
        else
        {
            node->SetEnabled(true);
            model->SetLodBias(1.0f);
        }
    }
}

float ProceduralVegetation::PerlinNoise(float x, float y)
{
    // Simple Perlin noise implementation
    // You might want to use a more sophisticated noise library

    float total = 0.0f;
    float frequency = 1.0f;
    float amplitude = 1.0f;
    float maxValue = 0.0f;

    for (int i = 0; i < noiseOctaves_; ++i)
    {
        total += Sin(x * frequency) * Sin(y * frequency) * amplitude;
        maxValue += amplitude;
        amplitude *= noisePersistence_;
        frequency *= 2.0f;
    }

    return total / maxValue;
}

void ProceduralVegetation::RemoveVegetationInRadius(const Vector3& center, float radius)
{
    Vector<unsigned> indicesToRemove;

    for (unsigned i = 0; i < vegetation_.Size(); ++i)
    {
        float distance = (vegetation_[i].position_ - center).Length();
        if (distance <= radius)
        {
            indicesToRemove.Push(i);
        }
    }

    // Remove from back to front to maintain indices
    for (int i = indicesToRemove.Size() - 1; i >= 0; --i)
    {
        RemoveVegetation(indicesToRemove[i]);
    }

    URHO3D_LOGDEBUGF("Removed %d vegetation instances in radius %.1f around %s",
                     indicesToRemove.Size(), radius, center.ToString().CString());
}

void ProceduralVegetation::SetExclusionZone(const Vector3& center, float radius, bool permanent)
{
    // Store exclusion zones to prevent future generation
    ExclusionZone zone;
    zone.center_ = center;
    zone.radius_ = radius;
    zone.permanent_ = permanent;
    exclusionZones_.Push(zone);

    // Remove existing vegetation in the zone
    RemoveVegetationInRadius(center, radius);
}

bool ProceduralVegetation::IsInExclusionZone(const Vector3& position) const
{
    for (const ExclusionZone& zone : exclusionZones_)
    {
        float distance = (position - zone.center_).Length();
        if (distance <= zone.radius_)
            return true;
    }
    return false;
}

void ProceduralVegetation::CreateTrackExclusionZones(const Vector<Vector3>& trackPoints, float trackWidth)
{
    // Create exclusion zones along a track defined by points
    for (unsigned i = 0; i < trackPoints.Size(); ++i)
    {
        SetExclusionZone(trackPoints[i], trackWidth, true);

        // Also create zones between consecutive points
        if (i < trackPoints.Size() - 1)
        {
            Vector3 start = trackPoints[i];
            Vector3 end = trackPoints[i + 1];
            Vector3 direction = (end - start).Normalized();
            float distance = (end - start).Length();

            // Create exclusion zones along the track segment
            int numSegments = (int)(distance / (trackWidth * 0.5f));
            for (int j = 0; j <= numSegments; ++j)
            {
                float t = (float)j / (float)numSegments;
                Vector3 segmentPos = start.Lerp(end, t);
                SetExclusionZone(segmentPos, trackWidth, true);
            }
        }
    }
}

void ProceduralVegetation::SetVegetationDensityMap(Image* densityMap)
{
    densityMap_ = densityMap;
}

float ProceduralVegetation::GetDensityFromMap(const Vector3& position) const
{
    if (!densityMap_ || !terrain_)
        return 1.0f;

    // Convert world position to texture coordinates
    Vector3 terrainSize = terrain_->GetNumVertices().Length() * terrain_->GetSpacing();
    Vector3 terrainPos = terrain_->GetNode()->GetWorldPosition();

    Vector3 localPos = position - terrainPos + terrainSize * 0.5f;
    float u = localPos.x_ / terrainSize.x_;
    float v = localPos.z_ / terrainSize.z_;

    // Clamp to valid range
    u = Clamp(u, 0.0f, 1.0f);
    v = Clamp(v, 0.0f, 1.0f);

    // Sample density map
    int x = (int)(u * (densityMap_->GetWidth() - 1));
    int y = (int)(v * (densityMap_->GetHeight() - 1));

    Color densityColor = densityMap_->GetPixel(x, y);
    return densityColor.r_; // Use red channel for density
}

void ProceduralVegetation::SaveVegetationToFile(const String& filename)
{
    File file(context_, filename, FILE_WRITE);
    if (!file.IsOpen())
    {
        URHO3D_LOGERROR("Failed to open vegetation save file: " + filename);
        return;
    }

    // Write header
    file.WriteString("VEGETATION_DATA_V1");
    file.WriteUInt(vegetation_.Size());

    // Write vegetation instances
    for (const VegetationInstance& instance : vegetation_)
    {
        file.WriteVector3(instance.position_);
        file.WriteUInt((unsigned)instance.type_);
        file.WriteUInt((unsigned)instance.stage_);
        file.WriteFloat(instance.scale_);
        file.WriteFloat(instance.rotation_);
        file.WriteFloat(instance.health_);
        file.WriteFloat(instance.ageTimer_);
    }

    URHO3D_LOGINFOF("Saved %d vegetation instances to %s", vegetation_.Size(), filename.CString());
}

bool ProceduralVegetation::LoadVegetationFromFile(const String& filename)
{
    File file(context_, filename, FILE_READ);
    if (!file.IsOpen())
    {
        URHO3D_LOGERROR("Failed to open vegetation load file: " + filename);
        return false;
    }

    // Read and verify header
    String header = file.ReadString();
    if (header != "VEGETATION_DATA_V1")
    {
        URHO3D_LOGERROR("Invalid vegetation file format: " + filename);
        return false;
    }

    // Clear existing vegetation
    ClearVegetation();

    // Read vegetation count
    unsigned count = file.ReadUInt();
    vegetation_.Reserve(count);

    // Read vegetation instances
    for (unsigned i = 0; i < count; ++i)
    {
        VegetationInstance instance;
        instance.position_ = file.ReadVector3();
        instance.type_ = (VegetationType)file.ReadUInt();
        instance.stage_ = (GrowthStage)file.ReadUInt();
        instance.scale_ = file.ReadFloat();
        instance.rotation_ = file.ReadFloat();
        instance.health_ = file.ReadFloat();
        instance.ageTimer_ = file.ReadFloat();
        instance.needsUpdate_ = true;

        vegetation_.Push(instance);
    }

    URHO3D_LOGINFOF("Loaded %d vegetation instances from %s", vegetation_.Size(), filename.CString());
    return true;
}

Vector<VegetationInstance*> ProceduralVegetation::GetVegetationInRadius(const Vector3& center, float radius)
{
    Vector<VegetationInstance*> result;

    for (VegetationInstance& instance : vegetation_)
    {
        float distance = (instance.position_ - center).Length();
        if (distance <= radius)
        {
            result.Push(&instance);
        }
    }

    return result;
}

void ProceduralVegetation::DamageVegetationInRadius(const Vector3& center, float radius, float damage)
{
    Vector<VegetationInstance*> affected = GetVegetationInRadius(center, radius);

    for (VegetationInstance* instance : affected)
    {
        instance->health_ -= damage;
        if (instance->health_ <= 0.0f)
        {
            // Mark for removal
            instance->health_ = 0.0f;
            instance->needsUpdate_ = true;
        }
        else
        {
            instance->needsUpdate_ = true;
        }
    }
}

void ProceduralVegetation::SetEnvironmentalParameters(float temperature, float humidity, float windStrength)
{
    environmentalTemp_ = temperature;
    environmentalHumidity_ = humidity;
    environmentalWind_ = windStrength;

    // Environmental factors affect growth
    for (VegetationInstance& instance : vegetation_)
    {
        instance.needsUpdate_ = true;
    }
}

void ProceduralVegetation::CreateVegetationAlongPath(const Vector<Vector3>& pathPoints,
                                                     float pathWidth, VegetationType type, float density)
{
    for (unsigned i = 0; i < pathPoints.Size() - 1; ++i)
    {
        Vector3 start = pathPoints[i];
        Vector3 end = pathPoints[i + 1];
        Vector3 direction = (end - start).Normalized();
        Vector3 perpendicular = Vector3(direction.z_, 0, -direction.x_);

        float segmentLength = (end - start).Length();
        int numSamples = (int)(segmentLength / 2.0f);

        for (int j = 0; j <= numSamples; ++j)
        {
            float t = (float)j / (float)numSamples;
            Vector3 centerPoint = start.Lerp(end, t);

            // Create vegetation on both sides of the path
            for (int side = -1; side <= 1; side += 2)
            {
                float sideOffset = Random(pathWidth * 0.5f, pathWidth * 2.0f);
                Vector3 position = centerPoint + perpendicular * (side * sideOffset);

                if (Random(1.0f) < density)
                {
                    // Get terrain height
                    float height, slope;
                    Vector3 normal;
                    GetTerrainProperties(position, height, slope, normal);
                    position.y_ = height;

                    if (IsSuitablePosition(position, type) && !IsInExclusionZone(position))
                    {
                        const VegetationParams& params = vegetationParams_[type];

                        VegetationInstance instance;
                        instance.position_ = position;
                        instance.type_ = type;
                        instance.stage_ = GROWTH_MATURE;
                        instance.scale_ = Lerp(params.minScale_, params.maxScale_, Random(1.0f));
                        instance.rotation_ = Random(360.0f);
                        instance.health_ = Random(80.0f, 100.0f);

                        vegetation_.Push(instance);
                    }
                }
            }
        }
    }
}
