#pragma once
#import "RenderDefs.h"

struct VertexShader {
    ShaderHandle handle;
    VertexShader() {
        handle = BGFX_INVALID_HANDLE;
    }
    VertexShader(ShaderHandle h) {
        handle = h;
    }
};
