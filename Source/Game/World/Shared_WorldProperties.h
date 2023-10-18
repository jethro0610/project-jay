#ifndef SHARED_SHADER
#pragma once
class Noise;
#endif

struct WorldProperties {
    float minHeight;
    float minRadius;
    float maxRadius;
    float edgeJaggedness;
    float edgeFalloff;
    float edgePower;

    #ifndef SHARED_SHADER
    Noise& noise;
    #else
    int noise;
    #endif
};
