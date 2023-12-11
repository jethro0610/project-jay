#pragma once
#include "RenderDefs.h"

struct FragmentShader {
    ShaderHandle handle;
    FragmentShader() {
        handle = DEFAULT_HANDLE;
    }
    FragmentShader(ShaderHandle h) {
        handle = h;
    }
};
