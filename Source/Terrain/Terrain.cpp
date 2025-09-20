#include "Terrain.h"
#include "Shared_TerrainFuncs.h"
#include "Level/LevelProperties.h"
#include "Resource/ResourceManager.h"
#include "Entity/EntityList.h"
#include "Entity/Entity.h"
#include "Helpers/Shared_Ease.h"
#include "Helpers/Assert.h"
#include "Helpers/LoadHelpers.h"
#include "Level/Level.h"
#include "Helpers/Random.h"
#include "Helpers/Assert.h"
#include <fstream>
#include <FastNoiseLite.h>
#include <glm/gtx/compatibility.hpp>
#include <thread>
#include <vector_contig.h>
#include <omp.h>
#include <glm/gtx/string_cast.hpp>

#ifdef _DEBUG
#include "GenerateStaticTerrainModifiers.h"
#endif

using namespace glm;
using namespace TerrainConsts;


Terrain::Terrain(
    LevelProperties& levelProperties,
    ResourceManager& resourceManager
):
resourceManager_(resourceManager)
{
    for (int y = 0; y < RESOLUTION; y++) {
    for (int x = 0; x < RESOLUTION; x++) {
        terrainMap_[y][x] = vec2(0.0f);

        #ifdef _DEBUG
        DBG_terrainMapLow_[y][x] = vec2(0.0f);
        #endif
    }}

    #ifdef _DEBUG
    DBG_landMapName_ = "lm_default";
    DBG_lowRes_ = true;

    #define TERRAINMOD(TYPE, ARR) for (int i = 0; i < 128; i++) { ARR[i].terrain_ = this; DBG_modifiers_.push_back(&ARR[i]); }
    EXPANDTERRAINMODS 
    #undef TERRAINMOD
    #endif
}

glm::vec2 Terrain::SampleTerrainMap(float x, float y, TerrainAccuracy accuracy) const {
    #ifdef _DEBUG
    if (DBG_lowRes_) {
        switch (accuracy) {
            case TA_Normal: {
                x *= WORLD_TO_TERRAIN_SCALAR_LOW;
                x += HALF_RESOLUTION_LOW - 0.5f;
                y *= WORLD_TO_TERRAIN_SCALAR_LOW;
                y += HALF_RESOLUTION_LOW - 0.5f;

                int sX = std::clamp((int)x, 0, RESOLUTION_LOW);
                int sY = std::clamp((int)y, 0, RESOLUTION_LOW);
                int sX1 = std::min(sX + 1, RESOLUTION_LOW);
                int sY1 = std::min(sY + 1, RESOLUTION_LOW);

                float a = x - sX;
                float b = y - sY;

                return 
                    (1 - b) * (1 - a) * DBG_terrainMapLow_[sY][sX] +
                    b * (1 - a) * DBG_terrainMapLow_[sY1][sX] +
                    (1 - b) * a * DBG_terrainMapLow_[sY][sX1] +
                    b * a * DBG_terrainMapLow_[sY1][sX1];
            }

            case TA_Low: {
                x *= WORLD_TO_TERRAIN_SCALAR_LOW;
                x += HALF_RESOLUTION_LOW - 0.5f;
                y *= WORLD_TO_TERRAIN_SCALAR_LOW;
                y += HALF_RESOLUTION_LOW - 0.5f;

                int sX = (int)x % RESOLUTION_LOW;
                int sY = (int)y % RESOLUTION_LOW;
                return DBG_terrainMapLow_[sY][sX];
            }
        }
    }
    #endif

    switch (accuracy) {
        case TA_Normal: {
            x *= WORLD_TO_TERRAIN_SCALAR;
            x += HALF_RESOLUTION - 0.5f;
            y *= WORLD_TO_TERRAIN_SCALAR;
            y += HALF_RESOLUTION - 0.5f;

            int sX = std::clamp((int)x, 0, RESOLUTION);
            int sY = std::clamp((int)y, 0, RESOLUTION);
            int sX1 = std::min(sX + 1, RESOLUTION);
            int sY1 = std::min(sY + 1, RESOLUTION);

            float a = x - sX;
            float b = y - sY;

            vec2 val =
                (1 - b) * (1 - a) * terrainMap_[sY][sX] +
                b * (1 - a) * terrainMap_[sY1][sX] +
                (1 - b) * a * terrainMap_[sY][sX1] +
                b * a * terrainMap_[sY1][sX1];

            if (val.x < -RANGE)
                val.x = RANGE;

            return val;
        }

        case TA_Low: {
            x *= WORLD_TO_TERRAIN_SCALAR;
            x += HALF_RESOLUTION - 0.5f;
            y *= WORLD_TO_TERRAIN_SCALAR;
            y += HALF_RESOLUTION - 0.5f;

            int sX = (int)x % RESOLUTION;
            int sY = (int)y % RESOLUTION;
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

vec2 Terrain::GetRawDistance(const vec2& position, TerrainAccuracy accuracy) const {
    return getTerrainDistance(position, *this, accuracy, false);
}

vec2 Terrain::GetRawDistance(const vec3& position, TerrainAccuracy accuracy) const {
    return GetRawDistance(vec2(position.x, position.z));
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
    return height;
}

float Terrain::GetRawHeight(const glm::vec2& position, TerrainAccuracy accuracy) const {
    return SampleTerrainMap(position).y;
}

float Terrain::GetRawHeight(const glm::vec3& position, TerrainAccuracy accuracy) const {
    return SampleTerrainMap(vec2(position.x, position.z)).y;
}

vec3 Terrain::GetNormal(const vec2& position, TerrainAccuracy accuracy) const {
    return getTerrainNormal(position, *this, accuracy);
}

vec3 Terrain::GetNormal(const vec3& position, TerrainAccuracy accuracy) const {
    return GetNormal(vec2(position.x, position.z), accuracy);
}

vec2 Terrain::GetDirectionToEdge(const vec2& position, TerrainAccuracy accuracy) const {
    return getDirectionToEdge(position, *this, accuracy);
}

vec3 Terrain::GetDirectionToEdge(const vec3& position, TerrainAccuracy accuracy) const {
    vec2 dir = getDirectionToEdge(vec2(position.x, position.z), *this, accuracy);
    return vec3(dir.x, 0.0f, dir.y);
}

#ifdef _DEBUG
#define INDEX(xVal, yVal) ((yVal) * RESOLUTION + (xVal))
void Blur(
    float* inBuffer,
    float* outBuffer
) {
    const int KERNEL_SIZE = 15;
    const float KERNEL[KERNEL_SIZE] = { 0.0005f, 0.0024f, 0.0092f, 0.0278f, 0.0656f, 0.1210f, 0.1747f, 0.1974f, 0.1747f, 0.1210f, 0.0650f, 0.0278f, 0.0092f, 0.0024f, 0.0005f};
    float* sepBuffer = new float[RESOLUTION * RESOLUTION];

    #pragma omp parallel for
    for (int y = 0; y < RESOLUTION; y++) {
    for (int x = 0; x < RESOLUTION; x++) {
        float value = 0.0f;
        for (int i = 0; i < KERNEL_SIZE; i++) {
            int sampX = clamp(x + (i - 8), 0, RESOLUTION - 1); 
            value += inBuffer[INDEX(sampX, y)] * KERNEL[i];
        }
        sepBuffer[INDEX(x, y)] = value;
    }}

    #pragma omp parallel for
    for (int x = 0; x < RESOLUTION; x++) {
    for (int y = 0; y < RESOLUTION; y++) {
        float value = 0.0f;
        for (int i = 0; i < KERNEL_SIZE; i++) {
            int sampY = clamp(y + (i - 8), 0, RESOLUTION - 1); 
            value += sepBuffer[INDEX(x, sampY)] * KERNEL[i];
        }
        outBuffer[INDEX(x, y)] = value;
    }}

    delete[] sepBuffer;
}

template <const int RES>
float SampleAdditiveMap(float x, float y, uint8_t additiveMap[RES][RES]) {
    x *= WORLD_TO_TERRAIN_SCALAR;
    x += HALF_RESOLUTION;
    y *= WORLD_TO_TERRAIN_SCALAR;
    y += HALF_RESOLUTION;

    int sX = std::clamp((int)x, 0, RES);
    int sY = std::clamp((int)y, 0, RES);
    int sX1 = std::min(sX + 1, RES);
    int sY1 = std::min(sY + 1, RES);

    float a = x - sX;
    float b = y - sY;

    return 
        (1 - b) * (1 - a) * additiveMap[sY][sX] +
        b * (1 - a) * additiveMap[sY1][sX] +
        (1 - b) * a * additiveMap[sY][sX1] +
        b * a * additiveMap[sY1][sX1];
}

template <const int RES>
void TemplateGenerateTerrainHeights(
    glm::vec2 terrainMap[RES][RES],
    uint8_t additiveMap[RES][RES],
    vector_const<StaticTerrainModifier*, StaticTerrainModifier::MAX>& modifiers
) {
    const int HALF_RES = RES * 0.5f;
    const float WORLD_TO_TERRAIN = RES / RANGE;
    // float* preblurred = new float[RESOLUTION * RESOLUTION];
    // float* blurred = new float[RESOLUTION * RESOLUTION];

    #pragma omp parallel for
    for (int y = 0; y < RES; y++) {
    for (int x = 0; x < RES; x++) {
         float height = additiveMap[y][x] * 5;

        float wX = x - HALF_RES;
        wX /= WORLD_TO_TERRAIN;
        float wY = y - HALF_RES;
        wY /= WORLD_TO_TERRAIN;
        vec2 pos = vec2(wX, wY);
        pos += vec2((1.0f / WORLD_TO_TERRAIN) * 0.5f);

        for (StaticTerrainModifier* modifier : modifiers) {
            if (modifier->active_ && modifier->InRange(pos))
                height += modifier->GetHeight(pos);
        }
        terrainMap[y][x].y = height;
    }}
    // Blur(preblurred, blurred);

    // for (int y = 0; y < RES; y++) {
    // for (int x = 0; x < RES; x++) {
    //     terrainMap[y][x].y = blurred[INDEX(x, y)];
    // }}

    // delete[] preblurred;
    // delete[] blurred;
}

void Terrain::GenerateTerrainHeights(bool lowRes, EntityList* entities) {
    if (!lowRes)
        DEBUGLOG("Generating terrain height...");
    vector_const<int, EntityList::MAX> groundedEntities;
    if (entities != nullptr) {
        for (int i = 0; i < EntityList::MAX; i++) {
            Entity& entity = (*entities)[i];
            if (!entity.alive_) continue;

            if (abs(GetHeight(entity.transform_.position) - entity.transform_.position.y) < 5.0f)
                groundedEntities.push_back(i);
        }
    }

    if (lowRes) {
        TemplateGenerateTerrainHeights<RESOLUTION_LOW>(
            DBG_terrainMapLow_, 
            DBG_additiveMapLow_,
            DBG_modifiers_
        );
        DBG_lowRes_ = true;
        resourceManager_.UpdateTerrainMapTextureLow((glm::vec2*)DBG_terrainMapLow_);
    }
    else {
        TemplateGenerateTerrainHeights<RESOLUTION>(
            terrainMap_, 
            DBG_additiveMap_,
            DBG_modifiers_
        );
        resourceManager_.UpdateTerrainMapTexture((glm::vec2*)terrainMap_);
        DBG_lowRes_ = false;
    }

    for (int entityIndex : groundedEntities) {
        Entity& entity = (*entities)[entityIndex];
        entity.transform_.position.y = GetHeight(entity.transform_.position);
    }
}

void GenerateUnsignedDistanceField(uint8_t* source, int* yArray, float* outArray, bool inverted) {
    #pragma omp parallel for
    for (int y = 0; y < RESOLUTION; y++) {
    for (int x = 0; x < RESOLUTION; x++) {
        int i = y * RESOLUTION + x;

        if (!source[i])
            yArray[i] = inverted ? RESOLUTION : 0;
        else
            yArray[i] = inverted ? 0 : RESOLUTION;
    }}

    #pragma omp parallel for
    for (int x = 0; x < RESOLUTION; x++) {
        for (int y = 1; y < RESOLUTION; y++) {
            int val = yArray[INDEX(x, y - 1)] + 1;
            yArray[INDEX(x, y)] = std::min(val, yArray[INDEX(x, y)]);
        }

        for (int y = RESOLUTION - 2; y >= 0; y--) {
            int val = yArray[INDEX(x, y + 1)] + 1;
            yArray[INDEX(x, y)] = std::min(val, yArray[INDEX(x, y)]);
        }
    }

    #pragma omp parallel for
    for (int y = 0; y < RESOLUTION; y++) {
        int vertices[RESOLUTION];
        int intersections[RESOLUTION];

        int lastVertIdx = 0;
        vertices[0] = 0;
        intersections[0] = -(RESOLUTION * RESOLUTION);
        intersections[1] = RESOLUTION * RESOLUTION;

        for (int vertX = 1; vertX < RESOLUTION - 1; vertX++) {
            int vertY = yArray[INDEX(vertX, y)];

            while(true) {
                int lastVertX = vertices[lastVertIdx];
                int lastVertY = yArray[INDEX(lastVertX, y)];
                if (vertX == lastVertX)
                    DEBUGLOG(vertX);
                int intersection = 
                    ((vertY * vertY + vertX * vertX) - (lastVertY * lastVertY + lastVertX * lastVertX)) /
                    (2 * vertX - 2 * lastVertX);

                if (intersection > intersections[lastVertIdx]) {
                    lastVertIdx++;
                    vertices[lastVertIdx] = vertX;
                    intersections[lastVertIdx] = intersection;    
                    intersections[lastVertIdx + 1] = RESOLUTION * RESOLUTION;    
                    break;
                }
                else
                    lastVertIdx--;
            }
        }

        int vertIdx = 0;
        for (int x = 0; x < RESOLUTION; x++) {
            while(intersections[vertIdx + 1] < x)
                vertIdx++;

            int vertX = vertices[vertIdx];
            int vertY = yArray[INDEX(vertX, y)];

            int diff = x - vertX;
            outArray[INDEX(x, y)] = sqrt(diff * diff + vertY * vertY) / TerrainConsts::WORLD_TO_TERRAIN_SCALAR;
        }
    }
}

void Terrain::GenerateTerrainDistances(EntityList* entities) {
    DEBUGLOG("Generating terrain distances...");
    uint8_t* landMap = new uint8_t[RESOLUTION * RESOLUTION];
    int* yArray = new int[RESOLUTION * RESOLUTION];
    float* positive = new float[RESOLUTION * RESOLUTION];
    float* negative = new float[RESOLUTION * RESOLUTION];
    float* preblurred = new float[RESOLUTION * RESOLUTION];
    float* blurred = new float[RESOLUTION * RESOLUTION];

    // Determine which entities are on the ground before regenerating
    vector_const<int, EntityList::MAX> groundedEntities;
    if (entities != nullptr) {
        for (int i = 0; i < EntityList::MAX; i++) {
            Entity& entity = (*entities)[i];
            if (!entity.alive_) continue;

            if (abs(GetHeight(entity.transform_.position) - entity.transform_.position.y) < 5.0f)
                groundedEntities.push_back(i);
        }
    }

    // Load the landmap
    std::ifstream landMapFile("./landmaps/" + DBG_landMapName_ + ".lmp", std::ios::binary);
    ASSERT(landMapFile.is_open(), "Tried generating from invalid landmap " + DBG_landMapName_);
    landMapFile.read((char*)landMap, RESOLUTION * RESOLUTION * sizeof(uint8_t));
    landMapFile.close();

    GenerateUnsignedDistanceField(landMap, yArray, positive, false);
    GenerateUnsignedDistanceField(landMap, yArray, negative, true);

    for (int y = 0; y < RESOLUTION; y++) {
    for (int x = 0; x < RESOLUTION; x++) {
        preblurred[INDEX(x, y)] = negative[INDEX(x, y)] - positive[INDEX(x, y)];
    }}
    Blur(preblurred, blurred);
    for (int y = 0; y < RESOLUTION; y++) {
    for (int x = 0; x < RESOLUTION; x++) {
        terrainMap_[y][x].x = blurred[INDEX(x, y)];
    }}
    resourceManager_.UpdateTerrainMapTexture((glm::vec2*)terrainMap_);

    // Generate the low resolution distance field
    const int scaleFactor = RESOLUTION / RESOLUTION_LOW;
    #pragma omp parallel for
    for (int y = 0; y < RESOLUTION_LOW; y++) {
    for (int x = 0; x < RESOLUTION_LOW; x++) {
        float averageDist = 0.0f;
        for (int dy = 0; dy < scaleFactor; dy++) {
        for (int dx = 0; dx < scaleFactor; dx++) {
            averageDist += terrainMap_[y * scaleFactor + dy][x * scaleFactor + dx].x; 
        }}
        DBG_terrainMapLow_[y][x].x = averageDist / (scaleFactor * scaleFactor);
    }} 
    resourceManager_.UpdateTerrainMapTextureLow((glm::vec2*)DBG_terrainMapLow_);

    // Load the additive map
    std::ifstream additiveMapFile("./landmaps/" + DBG_landMapName_ + ".amp", std::ios::binary);
    ASSERT(additiveMapFile.is_open(), "Tried generating from invalid additive map " + DBG_landMapName_);
    additiveMapFile.read((char*)DBG_additiveMap_, RESOLUTION * RESOLUTION * sizeof(uint8_t));
    additiveMapFile.close();

    // Generate the low resolution additive map
    #pragma omp parallel for
    for (int y = 0; y < RESOLUTION_LOW; y++) {
    for (int x = 0; x < RESOLUTION_LOW; x++) {
        uint32_t average = 0;
        for (int dy = 0; dy < scaleFactor; dy++) {
        for (int dx = 0; dx < scaleFactor; dx++) {
            average += DBG_additiveMap_[y * scaleFactor + dy][x * scaleFactor + dx]; 
        }}
        DBG_additiveMapLow_[y][x] = average / (scaleFactor * scaleFactor);
    }} 

    // Reground all grounded entities
    for (int entityIndex : groundedEntities) {
        Entity& entity = (*entities)[entityIndex];
        float height = GetHeight(vec2(entity.transform_.position.x, entity.transform_.position.z));
        if (height > -INFINITY)
            entity.transform_.position.y = height;
    }

    delete[] landMap;
    delete[] yArray;
    delete[] positive;
    delete[] negative;
    delete[] blurred;
}

void Terrain::ReloadTerrainDistances(EntityList* entities) {
    std::string landmapXCF = "../Assets/landmaps/" + DBG_landMapName_ + ".xcf";
    std::string output = "./landmaps/" + DBG_landMapName_;
    std::string landmapCommand = "magick " + landmapXCF + " -flatten -resize 4096x4096 -channel B -separate " + " GRAY:" + output + ".lmp";
    std::string additiveCommand = "magick " + landmapXCF + " -flatten -resize 4096x4096 -channel R -separate " + " GRAY:" + output + ".amp";
    system(landmapCommand.c_str());
    system(additiveCommand.c_str());
    GenerateTerrainDistances(entities);
    GenerateTerrainHeights(false, entities);
}
#endif

vec3 Terrain::RaycastTerrain(const vec3& origin, const vec3& direction) {
    vec3 curOrigin = origin; 
    float curHeight = GetHeight(curOrigin);

    for (int i = 0; i < 512; i++) {
        vec2 distance = GetDistance(curOrigin, TA_Normal);
        if (distance.x < 0.0f)
            curHeight = distance.y;

        float diff = curOrigin.y - curHeight;
        if (abs(diff) <= 0.5f)
            return vec3(curOrigin.x, curHeight, curOrigin.z);

        curOrigin += direction * diff;
    }
    return vec3(0.0f);
}

bool Terrain::PointIsInSameIsland(const vec3& origin, const vec3& point, float edgeDistance) {
    enum Direction {
        UP,
        DOWN,
        LEFT,
        RIGHT,
        NONE = -1
    };

    vec3 planarOrigin = vec3(origin.x, 0.0f, origin.z);
    vec3 planarPoint = vec3(point.x, 0.0f, point.z);

    if (GetDistance(planarOrigin).x > 0.0f)
        return false;

    if (GetDistance(planarPoint).x > edgeDistance)
        return false;
    
    float STEP_X = 10.0f;
    float STEP_Z = 10.0f;

    int lastDirection = NONE;
    vec3 directionVectors[4];
    float momentums[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
    vec3 searcher = planarOrigin;
    for (int i = 0; i < 128; i++) {
        if (distance(searcher, planarPoint) < 20.0f)
            return true;

        directionVectors[UP] = searcher + vec3(0.0f, 0.0f, 1.0f) * STEP_Z * momentums[UP];
        directionVectors[DOWN] = searcher + vec3(0.0f, 0.0f, -1.0f) * STEP_Z * momentums[DOWN];
        directionVectors[LEFT] = searcher + vec3(-1.0f, 0.0f, 0.0f) * STEP_X * momentums[LEFT];
        directionVectors[RIGHT] = searcher + vec3(1.0f, 0.0f, 0.0f) * STEP_X * momentums[RIGHT];

        int shortestDirection = NONE;
        float shortestDistance = INFINITY;
        for (int d = 0; d < 4; d++) {
            if (d != lastDirection && d % 2 == lastDirection % 2 && lastDirection != NONE)
                continue;

            if (GetDistance(directionVectors[d], TA_Low).x > 0.0f)
                continue;

            float dist = distance(directionVectors[d], point);
            if (dist < shortestDistance) {
                shortestDirection = d;
                shortestDistance = dist;
            }
        }

        for (int d = 0; d < 4; d++) {
            if (d == shortestDirection)
                momentums[d] += 0.1f;
            else
                momentums[d] -= 0.05f;

            momentums[d] = clamp(momentums[d], 1.0f, 1.5f);
        }

        if (shortestDirection == NONE)
            break;

        lastDirection = shortestDirection;
        searcher = directionVectors[shortestDirection];
    }

    return false;
}

vec3 Terrain::GetRandomPointInSameIslandFast(const vec3& origin) {
    vec3 planarOrigin = vec3(origin.x, 0.0f, origin.z);

    vec3 searcher = planarOrigin;
    vec3 direction = RandomVectorPlanar();
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 16; j++) {
            float distanceToEdge = GetDistance(searcher, TA_Low).x;
            if (distanceToEdge > -5.0f)
                break;

            // Multiply by 0.95 to prevent over shooting
            searcher += direction * (distanceToEdge) * 0.95f;
        }
        float pi = glm::pi<float>();
        quat randomRotation = quat(vec3(0.0f, lerp(-pi / 2, pi / 2, RandomFloatRange(0.0f, 1.0f)), 0.0f));
        vec3 negativeEdge = -GetDirectionToEdge(searcher);
        direction = rotate(randomRotation, negativeEdge);
    }

    vec3 point;
    vec2 dist;
    float t = RandomFloatRange(0.0f, 0.25f);
    point = lerp(searcher, planarOrigin, t);
    dist = GetDistance(point);
    point.y += dist.y;

    return point;
}

vec3 Terrain::GetRandomPointInSameIsland(const vec3& origin, float minDist, float maxDist, float edgeDist) {
    vec3 point;
    do {
        point = origin + RandomVectorPlanar(minDist, maxDist);
    }
    while (!PointIsInSameIsland(origin, point, edgeDist));
    point.y = GetHeight(point);
    return point;
}

void Terrain::Reset() {
    #ifdef _DEBUG
    for (StaticTerrainModifier* modifier : DBG_modifiers_)
        modifier->active_ = false;
    #endif
}

#ifdef _DEBUG
StaticTerrainModifier& Terrain::CreateStaticModifier(int typeId, const glm::vec3& pos) {
    switch (typeId) {
        #define TERRAINMOD(TYPE, ARR)           \
        case TYPE::ID: {                        \
            for (int i = 0; i < 128; i++) {     \
                if (ARR[i].active_)             \
                    continue;                   \
                                                \
                ARR[i].Init(pos);               \
                return ARR[i];                  \
            }                                   \
            break;                              \
        }
        EXPANDTERRAINMODS 
        #undef TERRAINMOD
    }
    ASSERT(false, "Out of available modifiers");
}

StaticTerrainModifier& Terrain::CreateStaticModifier(int typeId, const glm::vec2& pos) {
    return CreateStaticModifier(typeId, vec3(pos.x, 0.0f, pos.y));
}

bool Terrain::DestroyPendingModifiers() {
    bool destroy = false;
    for (StaticTerrainModifier* modifier : DBG_modifiers_) {
        if (!modifier->active_) continue;
        if (!modifier->destroy_) continue;

        modifier->active_ = false;
        modifier->destroy_ = false;
        destroy = true;
    }
    return destroy;
}
#endif

void Terrain::RecordDynamicModifiersLastTick() {
    dynamicTerrainBubbles_.RecordLastTick();
    dynamicTerrainNegatives_.RecordLastTick();
    dynamicTerrainAdditives_.RecordLastTick();
}

void Terrain::InterpolateDynamicModifiers(float interpTime) {
    dynamicTerrainBubbles_.Interpolate(interpTime);
    dynamicTerrainNegatives_.Interpolate(interpTime);
    dynamicTerrainAdditives_.Interpolate(interpTime);
}
