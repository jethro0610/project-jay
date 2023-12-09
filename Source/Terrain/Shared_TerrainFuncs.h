#ifndef SHARED_SHADER 

#pragma once
#include "Types/Noise.h"
#include "Shared_TerrainProperties.h"
#include <glm/vec2.hpp> 
#include <glm/gtx/compatibility.hpp> 
#define NOISE_TYPE Noise&
#define ACCURACY_TYPE NoiseAccuracy
#define ACCURACY_DEFAULT NA_Normal 
#define SAMPLENOISE(noisePos, accuracy) noise.Sample(noisePos, accuracy)
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
uniform vec4 u_noiseProps;
uniform vec4 u_terrainMeshOffset;
SAMPLER2D(s_terrainNoise, 15);
#define NOISE_TYPE float
#define ACCURACY_TYPE int 
#define ACCURACY_DEFAULT 0
#define SAMPLENOISE(noisePos, accuracy) texture2DLod(s_terrainNoise, noisePos / 1024.0f + vec2(0.5f, 0.5f), 0)
#define INLINE 

#endif

INLINE float sampleNoise(vec2 position, NOISE_TYPE noise, ACCURACY_TYPE accuracy = ACCURACY_DEFAULT) {
    return SAMPLENOISE(position, accuracy);
}

INLINE float sampleNoise(vec2 position, float scale, NOISE_TYPE noise, ACCURACY_TYPE accuracy = ACCURACY_DEFAULT) {
    vec2 samplePos = position * scale;
    return sampleNoise(samplePos, noise, accuracy);
}

INLINE float sampleNoiseBlob(vec2 position, float jaggedness, NOISE_TYPE noise, ACCURACY_TYPE accuracy = ACCURACY_DEFAULT) {
    if (position.x != 0.0f || position.y != 0.0f)
        position = normalize(position) * jaggedness;
    return sampleNoise(position, noise, accuracy);
}

INLINE vec2 getTerrainDistance(vec2 position, TerrainProperties props, ACCURACY_TYPE accuracy = ACCURACY_DEFAULT) {
    float blobVal = sampleNoiseBlob(position, props.edgeJaggedness, props.noise, accuracy);
    blobVal = (blobVal + 1.0f) * 0.5f;

    float blobRadius = props.minRadius + blobVal * (props.maxRadius - props.minRadius);
    float curRadius = length(position);
    float edgeDistance = curRadius - blobRadius;
    float edgeCloseness = max(1.0f + edgeDistance * props.edgeFalloff, 0.0f);
    float edgeHeight = -pow(edgeCloseness, props.edgePower);

    float terrainVal = sampleNoise(position, 0.75f, props.noise, accuracy);
    terrainVal = (terrainVal + 1.0f) * 0.5f;
    float terrainHeight = terrainVal * 12.0f;

    return vec2(edgeDistance, terrainHeight + edgeHeight);
}

INLINE vec3 getTerrainNormal(vec2 position, TerrainProperties props, ACCURACY_TYPE accuracy = ACCURACY_DEFAULT) {
    vec2 dX = position - vec2(1.0f, 0.0f);
    vec2 dZ = position - vec2(0.0f, 1.0f);

    float height = getTerrainDistance(position, props, accuracy).y;
    float gradX = getTerrainDistance(dX, props, accuracy).y - height;
    float gradZ = getTerrainDistance(dZ, props, accuracy).y - height;

    return normalize(vec3(gradX, 1.0f, gradZ));
}
