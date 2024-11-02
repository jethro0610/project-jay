#include "Terrain.h"
#include "Shared_TerrainFuncs.h"
#include "Level/LevelProperties.h"
#include "Resource/ResourceManager.h"
#include "Entity/EntityList.h"
#include "Entity/Entity.h"
#include "Helpers/Ease.h"
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
    DBG_bubbles_.clear();
    DBG_curves_.clear();
    DBG_nodeModel_ = resourceManager.GetModel("st_default");
    DBG_bubbleMaterial_.shader = resourceManager.GetShader("vs_static", "fs_color");
    DBG_bubbleMaterial_.properties.color = vec4(0.0f, 1.0f, 0.0f, 0.5f);
    DBG_curveMaterial_.shader = resourceManager.GetShader("vs_static", "fs_color");
    DBG_curveMaterial_.properties.color = vec4(0.0f, 0.0f, 1.0f, 0.5f);
    DBG_curveControlMaterial_.shader = resourceManager.GetShader("vs_static", "fs_color_front");
    DBG_curveControlMaterial_.properties.color = vec4(0.5f, 0.0f, 1.0f, 0.5f);
    #endif
}

glm::vec2 Terrain::SampleTerrainMap(float x, float y, TerrainAccuracy accuracy) const {
    #ifdef _DEBUG
    if (DBG_lowRes_) {
        x *= WORLD_TO_TERRAIN_SCALAR_LOW;
        x += HALF_RESOLUTION_LOW;
        y *= WORLD_TO_TERRAIN_SCALAR_LOW;
        y += HALF_RESOLUTION_LOW;

        int sX = (int)x % RESOLUTION_LOW;
        int sY = (int)y % RESOLUTION_LOW;
        return DBG_terrainMapLow_[sY][sX];
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

vec3 Terrain::GetNormal(const vec2& position, TerrainAccuracy accuracy) const {
    return getTerrainNormal(position, *this, accuracy);
}

vec3 Terrain::GetNormal(const vec3& position, TerrainAccuracy accuracy) const {
    return GetNormal(vec2(position.x, position.z), accuracy);
}

vec3 Terrain::GetDirectionToEdge(const vec2& position, TerrainAccuracy accuracy) const {
    return getDirectionToEdge(position, *this, accuracy);
}

vec3 Terrain::GetDirectionToEdge(const vec3& position, TerrainAccuracy accuracy) const {
    return getDirectionToEdge(vec2(position.x, position.z), *this, accuracy);
}

#ifdef _DEBUG
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

TerrainBubble* Terrain::AddBubble(vec3 position) {
    int bubbleIdx = DBG_bubbles_.push_back({vec4(position, 50.0f), false, false});
    return &DBG_bubbles_[bubbleIdx];
}

TerrainCurve* Terrain::AddCurve(vec3 position) {
    vec4 pos = vec4(position, 50.0f);
    TerrainCurve curve;
    curve.destroy_ = false;
    curve.DBG_selectedPoint_ = -1;
    for (int i = 0; i < 4; i++) {
        curve.points[i] = pos + vec4(i * 50.0f, 0.0f, 0.0f, 0.0f);
    }
    int curveIdx = DBG_curves_.push_back(curve);
    return &DBG_curves_[curveIdx];
}

bool Terrain::DestroyControls() {
    bool destroyed = false;
    int i = 0;
    while(i < DBG_bubbles_.size()) {
        while (i < DBG_bubbles_.size() && DBG_bubbles_[i].destroy_) {
            destroyed = true;
            DBG_bubbles_[i].destroy_ = false;
            DBG_bubbles_.remove(i);
        }
        i++;
    }

    i = 0;
    while(i < DBG_curves_.size()) {
        while (i < DBG_curves_.size() && DBG_curves_[i].destroy_) {
            destroyed = true;
            DBG_curves_[i].destroy_ = false;
            DBG_curves_.remove(i);
        }
        i++;
    }

    return destroyed;
}

struct InverseInfluence {
    float inverseWeight;
    float height;
};

template <const int RES>
void TemplateGenerateTerrainHeights(
    glm::vec2 terrainMap[RES][RES],
    uint32_t affectMap[RES][RES],
    uint8_t additiveMap[RES][RES],
    vector_contig<TerrainBubble, TerrainBubble::MAX>& bubbles,
    vector_contig<TerrainCurve, TerrainCurve::MAX>& curves
) {
    const int HALF_RES = RES * 0.5f;
    const float WORLD_TO_TERRAIN = RES / RANGE;

    for (int y = 0; y < RES; y++) {
    for (int x = 0; x < RES; x++) {
        affectMap[y][x] = 0;
    }}

    for (int i = 0; i < bubbles.size(); i++) 
        bubbles[i].WriteAffect<RES>(affectMap, i);
    for (int i = 0; i < curves.size(); i++) 
        curves[i].WriteAffect<RES>(affectMap, i + TerrainBubble::MAX);

    #pragma omp parallel for
    for (int y = 0; y < RES; y++) {
    for (int x = 0; x < RES; x++) {
        float additiveHeight = additiveMap[y][x];
        terrainMap[y][x].y = additiveHeight;

        float wX = x - HALF_RES;
        wX /= WORLD_TO_TERRAIN;
        float wY = y - HALF_RES;
        wY /= WORLD_TO_TERRAIN;
        vec2 pos = vec2(wX, wY);
        pos += vec2((1.0f / WORLD_TO_TERRAIN) * 0.5f);

        vector_const<InverseInfluence, TerrainBubble::MAX + TerrainCurve::MAX> inverseInfluences;
        bool onPoint = false;
        for (int i = 0; i < bubbles.size(); i++) {
            if (!(affectMap[y][x] & 1UL << i)) 
                continue;

            TerrainInfluence influence = bubbles[i].GetInfluence(pos, -additiveHeight);
            if (influence.distance == 0.0f) {
                terrainMap[y][x].y = influence.height;
                onPoint = true;
                break;
            }
            else if (influence.distance <= 1.0f) {
                float inverseDistance = 1.0f / (influence.distance * influence.distance);
                inverseDistance -= 1.0f;
                inverseInfluences.push_back({inverseDistance, influence.height});
            }
        }
        if (onPoint)
            continue;

        for (int i = 0; i < curves.size(); i++) {
            if (!(affectMap[y][x] & 1UL << (i + TerrainBubble::MAX))) 
                continue;

            TerrainInfluence influence = curves[i].GetInfluence(pos, -additiveHeight);
            if (influence.distance == 0.0f) {
                terrainMap[y][x].y = influence.height;
                onPoint = true;
                break;
            }
            else if (influence.distance <= 1.0f) {
                float inverseDistance = 1.0f / (influence.distance * influence.distance);
                inverseDistance -= 1.0f;
                inverseInfluences.push_back({inverseDistance, influence.height});
            }
        }
        if (onPoint)
            continue;

        float totalInverseDistances = 0.0f;
        for (int i = 0; i < inverseInfluences.size(); i++)
            totalInverseDistances += inverseInfluences[i].inverseWeight;
        if (totalInverseDistances == 0.0f)
            continue;

        for (int i = 0; i < inverseInfluences.size(); i++)
            terrainMap[y][x].y += (inverseInfluences[i].inverseWeight / totalInverseDistances) * inverseInfluences[i].height;
    }}
}

void Terrain::GenerateTerrainHeights(bool lowRes, EntityList* entities) {
    DEBUGLOG("Generating terrain height...");
    vector_const<int, 128> groundedEntities;
    if (entities != nullptr) {
        for (int i = 0; i < 128; i++) {
            Entity& entity = (*entities)[i];
            if (!entity.alive_) continue;

            if (abs(GetHeight(entity.transform_.position) - entity.transform_.position.y) < 5.0f)
                groundedEntities.push_back(i);
        }
    }

    if (lowRes) {
        TemplateGenerateTerrainHeights<RESOLUTION_LOW>(
            DBG_terrainMapLow_, 
            DBG_affectMapLow_, 
            DBG_additiveMapLow_,
            DBG_bubbles_, 
            DBG_curves_
        );
        DBG_lowRes_ = true;
        resourceManager_.UpdateTerrainMapTextureLow((glm::vec2*)DBG_terrainMapLow_);
    }
    else {
        TemplateGenerateTerrainHeights<RESOLUTION>(
            terrainMap_, 
            DBG_affectMap_, 
            DBG_additiveMap_,
            DBG_bubbles_, 
            DBG_curves_
        );
        resourceManager_.UpdateTerrainMapTexture((glm::vec2*)terrainMap_);
        DBG_lowRes_ = false;
    }

    for (int entityIndex : groundedEntities) {
        Entity& entity = (*entities)[entityIndex];
        entity.transform_.position.y = GetHeight(entity.transform_.position);
    }
}

#define INDEX(xVal, yVal) ((yVal) * RESOLUTION + (xVal))

void GenerateUnsignedDistanceField(uint8_t* source, int* yArray, float* outArray, bool inverted) {
    #pragma omp parallel for
    for (int y = 0; y < RESOLUTION; y++) {
    for (int x = 0; x < RESOLUTION; x++) {
        int i = y * RESOLUTION + x;

        if (!source[i])
            yArray[i] = inverted ? RESOLUTION * RESOLUTION : 0;
        else
            yArray[i] = inverted ? 0 : RESOLUTION * RESOLUTION;
    }}

    #pragma omp parallel for
    for (int x = 0; x < RESOLUTION; x++) {
        for (int y = 1; y < RESOLUTION; y++) {
            int val = yArray[INDEX(x, y - 1)] + 1;
            yArray[INDEX(x, y)] = std::min(val, yArray[INDEX(x, y)]);
        }

        for (int y = RESOLUTION - 1; y <= 0; y++) {
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
            outArray[INDEX(x, y)] = sqrt(diff * diff + vertY * vertY);
        }
    }
}

void Terrain::GenerateTerrainDistances(EntityList* entities) {
    DEBUGLOG("Generating terrain distances...");
    uint8_t* landMap = new uint8_t[RESOLUTION * RESOLUTION];
    int* yArray = new int[RESOLUTION * RESOLUTION];
    float* positive = new float[RESOLUTION * RESOLUTION];
    float* negative = new float[RESOLUTION * RESOLUTION];

    // Determine which entities are on the ground before regenerating
    vector_const<int, 128> groundedEntities;
    if (entities != nullptr) {
        for (int i = 0; i < 128; i++) {
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
        terrainMap_[y][x].x = negative[INDEX(x, y)] - positive[INDEX(x, y)];
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
        vec2 distance = GetDistance(curOrigin, TA_Low);
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

vec3 Terrain::GetRandomPointInSameIsland(const vec3& origin, float minDist, float maxDist) {
    vec3 point;
    do {
        point = origin + RandomVectorPlanar(minDist, maxDist);
    }
    while (!PointIsInSameIsland(origin, point));
    point.y = GetHeight(point);
    return point;
}
