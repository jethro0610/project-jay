#pragma once
#include "RenderDefs.h"
#include "Debug/DebugName.h"

struct Shader {
    USE_DEBUG_NAME;
    ShaderHandle handle;
    Shader() {
        handle = DEFAULT_HANDLE;
    }
    Shader(ShaderHandle h) {
        handle = h;
    }
};
