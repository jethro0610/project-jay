#ifndef SHARED_SHADER
#pragma once
class FastNoiseLite;
#endif

struct WorldProperties {
    float minHeight;
    float minRadius;
    float maxRadius;
    float edgeJaggedness;
    float edgeFalloff;
    float edgePower;

    #ifndef SHARED_SHADER
    FastNoiseLite& noise;
    #else
    int noise;
    #endif
};
