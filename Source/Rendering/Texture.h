#pragma once
#include "RenderDefs.h"

struct Texture {
    TextureHandle handle;
    Texture() {
        handle = DEFAULT_HANDLE;
    }
    Texture(TextureHandle h) {
        handle = h;
    }
};
