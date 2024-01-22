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
uniform vec4 u_terrainMeshOffset;
SAMPLER2D(s_terrainMap, 15);

#define TERRAIN_TYPE int
#define TERRAIN_DEFAULT = 0
#define ACCURACY_TYPE int 
#define ACCURACY_DEFAULT = 0
#define SAMPLETERRAINMAP(pos, accuracy) texture2DLod(s_terrainMap, pos / TERRAIN_RANGE + vec2(0.5f, 0.5f), 0)
#define INLINE 

#endif

INLINE vec2 getTerrainDistance(vec2 position, TERRAIN_TYPE terrain TERRAIN_DEFAULT, ACCURACY_TYPE accuracy ACCURACY_DEFAULT) {
    vec2 pos = SAMPLETERRAINMAP(position, accuracy);
    float edgeDistance = max((pos.x + GET_TC_VAR(EDGE_OFFSET)) * 0.1f, 0.0f);
    pos.y += min(0.0f, -(edgeDistance * edgeDistance));
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
