#include "Terrain.h"
#include "Shared_TerrainFuncs.h"
#include "Logging/Logger.h"
#include "Level/LevelProperties.h"
#include "Resource/ResourceManager.h"
#include "Components/TransformComponent.h"
#include <FastNoiseLite.h>
#include <glm/gtx/compatibility.hpp>
#include <thread>
using namespace glm;

Terrain::Terrain(
    LevelProperties& levelProperties,
    ResourceManager& resourceManager
):
resourceManager_(resourceManager)
{
    for (int x = 0; x < RESOLUTION; x++) {
    for (int y = 0; y < RESOLUTION; y++) {
        terrainMap_[x][y] = vec2(0.0f);
    } }
}

void TestStatic() {

}

void Terrain::GenerateTerrainMap(
    const std::array<NoiseLayer, NoiseLayer::MAX>& noiseLayers,
    const BlobProperties& blob
) {
    FastNoiseLite blobNoise(blob.seed);
    std::array<FastNoiseLite, 4> noises;
    for (int i = 0; i < 4; i++)
        noises[i].SetSeed(noiseLayers[i].seed);

    // TODO: Make this into a generic threaded loop calculator
    const auto cores = std::thread::hardware_concurrency();
    int sectionSize = RESOLUTION / cores;
    std::vector<std::thread> threads;
    for (int i = 0; i < cores; i++) {
        int add = (i == cores - 1 && RESOLUTION % cores != 0) ? 1 : 0;
        threads.push_back(std::thread([this, noiseLayers, blob, blobNoise, noises, sectionSize, i, add] {
            this->GenerateTerrainMapSection(
                noiseLayers,
                blob,
                blobNoise,
                noises,
                ivec2(i * sectionSize, 0),
                ivec2((i + 1) * sectionSize + add, RESOLUTION)
            ); 
        }));
    }
    for (std::thread& t : threads)
        t.join();
     
    resourceManager_.UpdateTerrainMapTexture((glm::vec2*)terrainMap_);
}

void Terrain::GenerateTerrainMap(
    const std::array<NoiseLayer, NoiseLayer::MAX>& noiseLayers,
    const BlobProperties& blob,
    EntityList& entities,
    ComponentList& components
) {
    TransformComponent& transformComponent = components.Get<TransformComponent>();
    vector_const<EntityID , MAX_ENTITIES> entitiesToRespoition;
    for (int i = 0; i < MAX_ENTITIES; i++) {
        const Entity& entity = entities[i];
        if (!entity.alive_) continue;
            
        Transform& transform = transformComponent.transform[i];
        if (abs(transform.position.y - GetHeight(transform.position)) < 0.5f)
            entitiesToRespoition.push_back(i);
    }

    GenerateTerrainMap(noiseLayers, blob);

    for (EntityID e : entitiesToRespoition) {
        Transform& transform = transformComponent.transform[e];
        transform.position.y = GetHeight(transform.position);
    }
}

void Terrain::GenerateTerrainMapSection(
    const std::array<NoiseLayer, NoiseLayer::MAX>& noiseLayers,
    const BlobProperties& blob,
    const FastNoiseLite& blobNoise,
    const std::array<FastNoiseLite, 4>& noises,
    const glm::ivec2& start,
    const glm::ivec2& end
) {
    for (int x = start.x; x < end.x; x++) {
    for (int y = start.y; y < end.y; y++) {
        vec2 pos = vec2(x - HALF_RESOLUTION, y - HALF_RESOLUTION);
        vec2 samplePos = normalize(pos);
        samplePos *= blob.frequency;
        float noiseVal = blobNoise.GetNoise(samplePos.x, samplePos.y);
        noiseVal = (noiseVal + 1.0f) * 0.5f;

        float blobRadius = lerp(blob.minRadius, blob.maxRadius, noiseVal);
        float curRadius = length(pos) / WORLD_TO_TERRAIN_SCALAR;

        terrainMap_[y][x].x = curRadius - blobRadius;
    } }

    for (int x = start.x; x < end.x; x++) {
    for (int y = start.y; y < end.y; y++) {
        terrainMap_[y][x].y = 0.0f;

        for (int i = 0; i < 4; i++) {
            const NoiseLayer& layer = noiseLayers[i];
            if (!layer.active) continue;

            float layerVal = noises[i].GetNoise(
                (float)x * layer.frequency.x, 
                (float)y * layer.frequency.y
            );
            layerVal = (layerVal + 1.0f) * 0.5f;
            layerVal = std::pow(layerVal, layer.exponent);
            layerVal *= layer.multiplier;

            terrainMap_[y][x].y += layerVal;
        }

        float edgeDistance = max((terrainMap_[y][x].x + EDGE_OFFSET) * 0.1f, 0.0f);
        terrainMap_[y][x].y += min(0.0f, -pow(edgeDistance, 2.0f));
    }}
}

glm::vec2 Terrain::SampleTerrainMap(float x, float y, TerrainAccuracy accuracy) const {
    switch (accuracy) {
        case TA_Normal: {
            x *= WORLD_TO_TERRAIN_SCALAR;
            x += HALF_RESOLUTION;
            y *= WORLD_TO_TERRAIN_SCALAR;
            y += HALF_RESOLUTION;

            int sX = std::clamp((int)x, 0, RESOLUTION);
            int sY = std::clamp((int)y, 0, RESOLUTION);
            int sX1 = std::min(sX + 1, RESOLUTION);
            int sY1 = std::min(sY + 1, RESOLUTION);

            float a = x - sX;
            float b = y - sY;

            return 
                (1 - b) * (1 - a) * terrainMap_[sY][sX] +
                b * (1 - a) * terrainMap_[sY1][sX] +
                (1 - b) * a * terrainMap_[sY][sX1] +
                b * a * terrainMap_[sY1][sX1];
        }

        case TA_Low: {
            x *= WORLD_TO_TERRAIN_SCALAR;
            x += HALF_RESOLUTION;
            y *= WORLD_TO_TERRAIN_SCALAR;
            y += HALF_RESOLUTION;

            int sX = (int)x % RESOLUTION;//std::clamp((int)x, 0, RESOLUTION);
            int sY = (int)y % RESOLUTION;//std::clamp((int)y, 0, RESOLUTION);
            return terrainMap_[sY][sX];
        }
        
        default:
            return vec2(0.0f, 0.0f);
    }
}

glm::vec2 Terrain::SampleTerrainMap(const glm::vec2& position, TerrainAccuracy accuracy) const {
    return SampleTerrainMap(position.x, position.y, accuracy);
}

vec2 Terrain::GetDistance(const vec2& position, TerrainAccuracy accuracy) const {
    return getTerrainDistance(position, *this, accuracy);
}

vec2 Terrain::GetDistance(const vec3& position, TerrainAccuracy accuracy) const {
    return GetDistance(vec2(position.x, position.z), accuracy);
}

float Terrain::GetHeight(const vec2& position, TerrainAccuracy accuracy) const {
    vec2 worldDistance = getTerrainDistance(
        position,
        *this,
        accuracy
    );

    if (worldDistance.x > 0.0f)
        return -INFINITY;

    return worldDistance.y;
}

float Terrain::GetHeight(const vec3& position, TerrainAccuracy accuracy) const {
    float height = GetHeight(vec2(position.x, position.z), accuracy);
    // If the position is below 10 units of the ground,
    // count it as un grounded
    // if (position.y < height - 10.0f)
    //     return -INFINITY;

    return height;
}

vec3 Terrain::GetNormal(const vec2& position, TerrainAccuracy accuracy) const {
    return getTerrainNormal(position, *this, accuracy);
}

vec3 Terrain::GetNormal(const vec3& position, TerrainAccuracy accuracy) const {
    return GetNormal(vec2(position.x, position.z), accuracy);
}
