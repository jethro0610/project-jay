#ifndef SHARED_SHADER 

#pragma once
#include "Terrain/Terrain.h"

#define TERRAIN_TYPE const Terrain&
#define TERRAIN_DEFAULT
#define ACCURACY_TYPE TerrainAccuracy
#define ACCURACY_DEFAULT = TA_Normal 
#define SAMPLETERRAINMAP(pos, accuracy) terrain.SampleTerrainMap(pos, accuracy)
#define INLINE inline
using namespace glm;

#else
#include <Shared_TerrainConstants.h>
uniform vec4 u_terrainHoles[8];
SAMPLER2D(s_terrainMap, 15);

#define TERRAIN_TYPE int
#define TERRAIN_DEFAULT = 0
#define ACCURACY_TYPE int 
#define ACCURACY_DEFAULT = 0
#define SAMPLETERRAINMAP(pos, accuracy) texture2DLod(s_terrainMap, pos / TERRAIN_RANGE + vec2(0.5f, 0.5f), 0)
#define INLINE 

#endif
INLINE float Ease(float x) {
    return x < 0.5 ? 2 * x * x : 1 - pow(-2 * x + 2, 2) / 2;
}

INLINE vec2 getTerrainDistance(vec2 position, TERRAIN_TYPE terrain TERRAIN_DEFAULT, ACCURACY_TYPE accuracy ACCURACY_DEFAULT) {
    vec2 pos = SAMPLETERRAINMAP(position, accuracy);

    float t = pos.x / 32.0f;
    t = clamp(t, 0.0f, 1.0f);

    pos.y -= (t * t) * 128.0f;
    return pos;
}

INLINE vec3 getTerrainNormal(vec2 position, TERRAIN_TYPE terrain TERRAIN_DEFAULT, ACCURACY_TYPE accuracy ACCURACY_DEFAULT) {
    vec2 dX = position - vec2(1.0f, 0.0f);
    vec2 dZ = position - vec2(0.0f, 1.0f);

    float height = getTerrainDistance(position, terrain, accuracy).y;
    float gradX = getTerrainDistance(dX, terrain, accuracy).y - height;
    float gradZ = getTerrainDistance(dZ, terrain, accuracy).y - height;

    return normalize(vec3(gradX, 1.0f, gradZ));
}

INLINE vec3 getDirectionToEdge(vec2 position, TERRAIN_TYPE terrain TERRAIN_DEFAULT, ACCURACY_TYPE accuracy ACCURACY_DEFAULT) {
    vec2 dX = position - vec2(1.0f, 0.0f);
    vec2 dZ = position - vec2(0.0f, 1.0f);

    float origin = getTerrainDistance(position, terrain, accuracy).x;
    float gradX = getTerrainDistance(dX, terrain, accuracy).x - origin;
    float gradZ = getTerrainDistance(dZ, terrain, accuracy).x - origin;

    return normalize(vec3(gradX, 0.0f, gradZ));
}
