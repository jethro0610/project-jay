#ifndef SHARED_SHADER 

#pragma once
#include "Terrain/Terrain.h"
#include "Shared_TerrainProperties.h"
#include <glm/vec2.hpp> 
#include <glm/gtx/compatibility.hpp> 
#define TERRAIN_TYPE Terrain&
#define ACCURACY_TYPE TerrainAccuracy
#define ACCURACY_DEFAULT TA_Normal 
#define SAMPLETERRAINMAP(pos, accuracy) terrain.SampleTerrainMap(pos, accuracy)
#define INLINE inline
using namespace glm;

#else

#include <Shared_TerrainProperties.h>
uniform vec4 u_terrainProps[2];
#define u_minHeight u_terrainProps[0].x
#define u_minRadius u_terrainProps[0].y
#define u_maxRadius u_terrainProps[0].z
#define u_edgeJaggedness u_terrainProps[0].w
#define u_edgeFalloff u_terrainProps[1].x
#define u_edgePower u_terrainProps[1].y
uniform vec4 u_terrainMeshOffset;
SAMPLER2D(s_terrainMap, 15);
#define TERRAIN_TYPE float
#define ACCURACY_TYPE int 
#define ACCURACY_DEFAULT 0
#define SAMPLEHEIGHTMAP(pos, accuracy) texture2DLod(s_terrainHeightmap, pos / TERRAIN_RANGE + vec2(0.5f, 0.5f), 0)
#define INLINE 

#endif

INLINE vec2 sampleTerrainMap(vec2 position, TERRAIN_TYPE terrain, ACCURACY_TYPE accuracy = ACCURACY_DEFAULT) {
    return SAMPLETERRAINMAP(position, accuracy);
}

INLINE vec2 getTerrainDistance(vec2 position, TerrainProperties props, ACCURACY_TYPE accuracy = ACCURACY_DEFAULT) {
    vec2 terrainVal = sampleTerrainMap(position, props.terrain, accuracy);
    float terrainHeight = terrainVal.y * 12.0f;

    return vec2(terrainVal.x, terrainHeight);
}

INLINE vec3 getTerrainNormal(vec2 position, TerrainProperties props, ACCURACY_TYPE accuracy = ACCURACY_DEFAULT) {
    vec2 dX = position - vec2(1.0f, 0.0f);
    vec2 dZ = position - vec2(0.0f, 1.0f);

    float height = getTerrainDistance(position, props, accuracy).y;
    float gradX = getTerrainDistance(dX, props, accuracy).y - height;
    float gradZ = getTerrainDistance(dZ, props, accuracy).y - height;

    return normalize(vec3(gradX, 1.0f, gradZ));
}
