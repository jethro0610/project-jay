#pragma once
#include "RenderDefs.h"

struct FragmentShader {
    ShaderHandle handle;
    FragmentShader() {
        handle = BGFX_INVALID_HANDLE;
    }
    FragmentShader(ShaderHandle h) {
        handle = h;
    }
};
