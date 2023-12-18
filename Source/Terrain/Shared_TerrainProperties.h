#ifndef SHARED_SHADER
#pragma once
class Noise;
class Terrain;
enum TerrainAccuracy {
    TA_Normal,
    TA_Low
};
#endif
#define TERRAIN_RANGE 1024.0f

struct TerrainProperties {
    float minHeight;
    float minRadius;
    float maxRadius;
    float edgeJaggedness;
    float edgeFalloff;
    float edgePower;

    #ifndef SHARED_SHADER
    Terrain& terrain;
    #else
    int terrain;
    #endif
};
