#pragma once
#include <vector_const.h>
#include <glm/mat4x4.hpp>
#include "RenderDefs.h"
#include "Texture.h"

enum TriangleType {
    ONE_SIDED,
    TWO_SIDED,
    TWO_SIDED_NEGATIVE_BACK
};

namespace MaterialConstants {
    const int MAX_TEXTURES_PER_MATERIAL = 8;
    const int SHADOW_TEXINDEX = 14;
    const int TERRAIN_NOISE_TEXINDEX = 15;
};

struct Material {
    // If necessary, can typedef this so there's separate
    // VS and FS instead of one shader program
    MaterialShaderHandle shaderHandle;
    MaterialShaderHandle shadowShaderHandle;
    vector_const<Texture*, MaterialConstants::MAX_TEXTURES_PER_MATERIAL> textures;
    bool castShadows;
    TriangleType triangleType;
    glm::mat4 properties;
};
