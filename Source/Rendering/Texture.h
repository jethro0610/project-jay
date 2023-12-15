#pragma once
#include "RenderDefs.h"
#include "Debug/DebugName.h"

struct Texture {
    TextureHandle handle;
    Texture() {
        handle = DEFAULT_HANDLE;
    }
    Texture(TextureHandle h) {
        handle = h;
    }

    USE_DEBUG_NAME;
};
