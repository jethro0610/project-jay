#ifndef SHARED_SHADER 

#pragma once
#include <glm/vec2.hpp> 
#include <glm/gtx/compatibility.hpp> 
#include "../../Types/Noise.h"
#include "../../Logging/Logger.h"
#include "../../Logging/ScreenText.h"
#include "Shared_WorldProperties.h"
#define NOISE_TYPE Noise&
#define SAMPLENOISE(noisePos) noise.Sample(noisePos)
#define SAMPLENOISEFAST(noisePos) noise.SampleFast(noisePos)
#define INLINE inline
using namespace glm;

#else

#include <Shared_WorldProperties.h>
uniform vec4 u_worldProps[2];
#define u_minHeight u_worldProps[0].x
#define u_minRadius u_worldProps[0].y
#define u_maxRadius u_worldProps[0].z
#define u_edgeJaggedness u_worldProps[0].w
#define u_edgeFalloff u_worldProps[1].x
#define u_edgePower u_worldProps[1].y
uniform vec4 u_noiseProps;
uniform vec4 u_worldMeshOffset;
SAMPLER2D(s_worldNoise, 15);
#define NOISE_TYPE float
#define SAMPLENOISE(noisePos) texture2DLod(s_worldNoise, noisePos / 1024.0f + vec2(0.5f, 0.5f), 0)
#define SAMPLENOISEFAST(noisePos) (1 + 1)
#define INLINE 

#endif

INLINE float sampleNoise(vec2 position, NOISE_TYPE noise) {
    return SAMPLENOISE(position);
}

INLINE float sampleNoiseFast(vec2 position, NOISE_TYPE noise) {
    return SAMPLENOISEFAST(position);
}

INLINE float sampleNoise(vec2 position, float scale, NOISE_TYPE noise) {
    vec2 samplePos = position * scale;
    return sampleNoise(samplePos, noise);
}

INLINE float sampleNoiseFast(vec2 position, float scale, NOISE_TYPE noise) {
    vec2 samplePos = position * scale;
    return sampleNoiseFast(samplePos, noise);
}

INLINE float sampleNoiseBlob(vec2 position, float jaggedness, NOISE_TYPE noise) {
    if (position.x != 0.0f || position.y != 0.0f)
        position = normalize(position) * jaggedness;
    return sampleNoise(position, noise);
}

INLINE float sampleNoiseBlobFast(vec2 position, float jaggedness, NOISE_TYPE noise) {
    if (position.x != 0.0f || position.y != 0.0f)
        position = normalize(position) * jaggedness;
    return sampleNoiseFast(position, noise);
}

INLINE vec2 getWorldDistance(vec2 position, WorldProperties props) {
    float blobVal = sampleNoiseBlob(position, props.edgeJaggedness, props.noise);
    blobVal = (blobVal + 1.0f) * 0.5f;

    float blobRadius = props.minRadius + blobVal * (props.maxRadius - props.minRadius);
    float curRadius = length(position);
    float edgeDistance = curRadius - blobRadius;
    float edgeCloseness = max(1.0f + edgeDistance * props.edgeFalloff, 0.0f);
    float edgeHeight = -pow(edgeCloseness, props.edgePower);

    float terrainVal = sampleNoise(position, 0.75f, props.noise);
    terrainVal = (terrainVal + 1.0f) * 0.5f;
    float terrainHeight = terrainVal * 12.0f;

    return vec2(edgeDistance, terrainHeight + edgeHeight);
}

INLINE vec2 getWorldDistanceFast(vec2 position, WorldProperties props) {
    float blobVal = sampleNoiseBlobFast(position, props.edgeJaggedness, props.noise);
    blobVal = (blobVal + 1.0f) * 0.5f;

    float blobRadius = props.minRadius + blobVal * (props.maxRadius - props.minRadius);
    float curRadius = length(position);
    float edgeDistance = curRadius - blobRadius;
    float edgeCloseness = max(1.0f + edgeDistance * props.edgeFalloff, 0.0f);
    float edgeHeight = -pow(edgeCloseness, props.edgePower);

    float terrainVal = sampleNoiseFast(position, 0.75f, props.noise);
    terrainVal = (terrainVal + 1.0f) * 0.5f;
    float terrainHeight = terrainVal * 12.0f;

    return vec2(edgeDistance, terrainHeight + edgeHeight);
}

INLINE vec3 getWorldNormal(vec2 position, WorldProperties props) {
    vec2 dX = position - vec2(1.0f, 0.0f);
    vec2 dZ = position - vec2(0.0f, 1.0f);

    float height = getWorldDistance(position, props).y;
    float gradX = getWorldDistance(dX, props).y - height;
    float gradZ = getWorldDistance(dZ, props).y - height;

    return normalize(vec3(gradX, 1.0f, gradZ));
}
