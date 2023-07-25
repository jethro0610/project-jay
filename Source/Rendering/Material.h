#pragma once
#include "RenderDefs.h"

const int MAX_TEXTURES_PER_MATERIAL = 8;

struct Material {
    MaterialShader shader;
    Texture textures[MAX_TEXTURES_PER_MATERIAL];
    int numTextures;
    bool twoSided;

    Material() {
        numTextures = 0;
        twoSided = false;
    }
};
