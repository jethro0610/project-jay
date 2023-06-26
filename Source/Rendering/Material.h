#pragma once
#include "RenderDefs.h"

const uint8_t MAX_TEXTURES_PER_MATERIAL = 8;

struct Material {
    MaterialShader shader;
    Texture textures[MAX_TEXTURES_PER_MATERIAL];
    uint8_t numTextures;
};
