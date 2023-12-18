#pragma once
#include "RenderDefs.h"
#include "Texture.h"
#include "Debug/DebugName.h"
#include <vector_const.h>
#include <glm/mat4x4.hpp>

enum TriangleType {
    ONE_SIDED,
    TWO_SIDED,
    TWO_SIDED_NEGATIVE_BACK
};

struct Material {
    USE_DEBUG_NAME;
    static constexpr int MAX_TEXTURES_PER_MATERIAL = 8;
    static constexpr int SHADOW_TEXINDEX = 14;
    static constexpr int TERRAIN_HEIGHTMAP_TEXINDEX = 15;

    Material() {
        shaderHandle = BGFX_INVALID_HANDLE;
        shadowShaderHandle = BGFX_INVALID_HANDLE;
        textures = {};
        castShadows = false;
        triangleType = TriangleType::ONE_SIDED;
        properties = {};
    }

    // If necessary, can typedef this so there's separate
    // VS and FS instead of one shader program
    MaterialShaderHandle shaderHandle;
    MaterialShaderHandle shadowShaderHandle;
    vector_const<Texture*, MAX_TEXTURES_PER_MATERIAL> textures;
    bool castShadows;
    TriangleType triangleType;
    glm::mat4 properties;
};
