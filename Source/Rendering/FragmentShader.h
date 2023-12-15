#pragma once
#include "RenderDefs.h"
#include "Debug/DebugName.h"

struct FragmentShader {
    USE_DEBUG_NAME;
    ShaderHandle handle;
    FragmentShader() {
        handle = DEFAULT_HANDLE;
    }
    FragmentShader(ShaderHandle h) {
        handle = h;
    }
};
