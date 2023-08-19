#pragma once
#include <glm/mat4x4.hpp>
#include <vector>
#include "RenderDefs.h"

const int MAX_TEXTURES_PER_MATERIAL = 8;
const int SHADOW_TEXINDEX = 8;
const int WORLD_NOISE_TEXINDEX = 9;
const int NUM_SAMPLERS = 10;

enum TriangleType {
    ONE_SIDED,
    TWO_SIDED,
    TWO_SIDED_NEGATIVE_BACK
};

struct Material {
    MaterialShader shader;
    MaterialShader shadowShader;
    std::vector<Texture> textures;
    bool castShadows;
    TriangleType triangleType;
    glm::mat4 properties;
};
