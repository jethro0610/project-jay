#ifndef SHARED_SHADER 

#pragma once
#include "Terrain/Terrain.h"
#include "Helpers/Shared_Ease.h"
#include "Logging/Logger.h"
#include "Shared_DynamicTerrainModifier.h"
#include "Shared_DynamicBubble.h"
#include "Shared_DynamicNegative.h"

#define TERRAIN_TYPE const Terrain&
#define TERRAIN_DEFAULT
#define ACCURACY_TYPE TerrainAccuracy
#define ACCURACY_DEFAULT = TA_Normal 
#define SAMPLETERRAINMAP(pos, accuracy) terrain.SampleTerrainMap(pos, accuracy)
#define INLINE inline
#define DYN_BUBBLES terrain.dynamicTerrainBubbles_
#define DYN_NEGATIVES terrain.dynamicTerrainNegatives_
using namespace glm;

#else
#include <Shared_TerrainConstants.h>
#include <Shared_Ease.h>
#include <Shared_DynamicTerrainModifier.h>
#include <Shared_DynamicBubble.h>
#include <Shared_DynamicNegative.h>
uniform mat4 u_dynamicTerrainBubbles[DYN_MOD_MAX];
uniform mat4 u_dynamicTerrainNegatives[DYN_MOD_MAX];
SAMPLER2D(s_terrainMap, 15);

#define TERRAIN_TYPE int
#define TERRAIN_DEFAULT = 0
#define ACCURACY_TYPE int 
#define ACCURACY_DEFAULT = 0
#define SAMPLETERRAINMAP(pos, accuracy) texture2DLod(s_terrainMap, pos / TERRAIN_RANGE + vec2(0.5f, 0.5f), 0)
#define INLINE 
#define DYN_BUBBLES u_dynamicTerrainBubbles
#define DYN_NEGATIVES u_dynamicTerrainNegatives
#endif

INLINE float smax(float a, float b, float k) {
    return log(exp(k * a) + exp(k * b)) / k;
}

INLINE vec2 getTerrainDistance(vec2 position, TERRAIN_TYPE terrain TERRAIN_DEFAULT, ACCURACY_TYPE accuracy ACCURACY_DEFAULT) {
    vec2 pos = SAMPLETERRAINMAP(position, accuracy);

    #pragma unroll
    for (int i = 0; i < DYN_MOD_MAX; i++) {
        if (!DYN_MOD_IS_ACTIVE(DYN_NEGATIVES[i])) continue;

        vec2 dynModPos = vec2(DYN_MOD_POS_X(DYN_NEGATIVES[i]), DYN_MOD_POS_Z(DYN_NEGATIVES[i]));
        float dist = distance(dynModPos, position);
        float outer = dist - DYN_NEG_OUTER_RADIUS(DYN_NEGATIVES[i]);
        float inner = dist - DYN_NEG_INNER_RADIUS(DYN_NEGATIVES[i]);
        float negativeVal = max(outer, -inner);
        pos.x = smax(pos.x, -negativeVal, DYN_NEG_SHARPNESS(DYN_NEGATIVES[i]));
    }

    #pragma unroll
    for (int i = 0; i < DYN_MOD_MAX; i++) {
        if (!DYN_MOD_IS_ACTIVE(DYN_BUBBLES[i])) continue;

        vec2 dynModPos = vec2(DYN_MOD_POS_X(DYN_BUBBLES[i]), DYN_MOD_POS_Z(DYN_BUBBLES[i]));
        float dist = distance(dynModPos, position);
        float dScalar = 1.0f - min(dist / DYN_BUB_RADIUS(DYN_BUBBLES[i]), 1.0f);
        dScalar = EaseInOut(dScalar, DYN_BUB_INPOWER(DYN_BUBBLES[i]), DYN_BUB_OUTPOWER(DYN_BUBBLES[i]));
        if (dist < DYN_BUB_RADIUS(DYN_BUBBLES[i]))
            pos.y += dScalar * DYN_BUB_HEIGHT(DYN_BUBBLES[i]);
    }

    float t = pos.x / 32.0f;
    t = clamp(t, 0.0f, 1.0f);

    pos.y -= (t * t * t) * 256.0f;
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

INLINE vec2 getDirectionToEdge(vec2 position, TERRAIN_TYPE terrain TERRAIN_DEFAULT, ACCURACY_TYPE accuracy ACCURACY_DEFAULT) {
    vec2 dX = position - vec2(1.0f, 0.0f);
    vec2 dZ = position - vec2(0.0f, 1.0f);

    float origin = getTerrainDistance(position, terrain, accuracy).x;
    float gradX = getTerrainDistance(dX, terrain, accuracy).x - origin;
    float gradZ = getTerrainDistance(dZ, terrain, accuracy).x - origin;

    return normalize(vec2(gradX, gradZ));
}
