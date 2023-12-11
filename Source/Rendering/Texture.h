#pragma once
#include "RenderDefs.h"

struct Texture {
    TextureHandle handle;
    Texture() {
        handle = BGFX_INVALID_HANDLE;
    }
    Texture(TextureHandle h) {
        handle = h;
    }
};
