#ifndef SHARED_SHADER
#pragma once
class Noise;
class Terrain;
#endif

enum TerrainAccuracy {
    TA_Normal,
    TA_Low
};

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
    int noise;
    #endif
};
