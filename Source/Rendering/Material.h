#pragma once
#include <vector_const.h>
#include <glm/mat4x4.hpp>
#include "RenderDefs.h"
#include "Texture.h"

const int MAX_TEXTURES_PER_MATERIAL = 8;
const int SHADOW_TEXINDEX = 14;
const int WORLD_NOISE_TEXINDEX = 15;

enum TriangleType {
    ONE_SIDED,
    TWO_SIDED,
    TWO_SIDED_NEGATIVE_BACK
};

struct Material {
    MaterialShaderHandle shaderHandle;
    MaterialShaderHandle shadowShaderHandle;
    vector_const<Texture*, MAX_TEXTURES_PER_MATERIAL> textures;
    bool castShadows;
    TriangleType triangleType;
    glm::mat4 properties;
};
