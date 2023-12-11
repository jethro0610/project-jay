#pragma once
#include "RenderDefs.h"

struct VertexShader {
    ShaderHandle handle;
    VertexShader() {
        handle = DEFAULT_HANDLE;
    }
    VertexShader(ShaderHandle h) {
        handle = h;
    }
};
