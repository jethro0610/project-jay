#pragma once
#include "RenderDefs.h"

const int MAX_TEXTURES_PER_MATERIAL = 8;
const int SHADOW_TEXINDEX = 8;
const int WORLD_NOISE_TEXINDEX = 9;
const int NUM_SAMPLERS = 10;

struct Material {
    MaterialShader shader;
    MaterialShader shadowShader;
    Texture textures[MAX_TEXTURES_PER_MATERIAL];
    int numTextures;
    bool twoSided;
};
