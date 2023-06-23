#include "RenderDefs.h"

struct Material {
    MaterialShader shader;
    Texture textures[8];
    uint8_t numTextures;
};
