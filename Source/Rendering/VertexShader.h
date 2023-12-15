#pragma once
#include "RenderDefs.h"
#include "Debug/DebugName.h"

struct VertexShader {
    USE_DEBUG_NAME;
    ShaderHandle handle;
    VertexShader() {
        handle = DEFAULT_HANDLE;
    }
    VertexShader(ShaderHandle h) {
        handle = h;
    }
};
