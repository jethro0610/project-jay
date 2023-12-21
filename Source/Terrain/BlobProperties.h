#pragma once
struct BlobProperties {
    int seed;
    float frequency;
    float minRadius;
    float maxRadius;

    BlobProperties() {
        seed = 1337;
        frequency = 100.0f;
        minRadius = 150.0f;
        maxRadius = 150.0f;
    }
};
