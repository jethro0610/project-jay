#pragma once

#ifdef WINDOWS
#include "DirectXLayer.h"
#endif

#include <string>
#include <vector>
#include <glm.hpp>
#include "RenderTypes.h"

class Renderer {
public:
    #ifdef WINDOWS
    Renderer(DirectXLayer* dxLayer);
    DirectXLayer* dxLayer_;
    #endif
    ~Renderer();

    void Render();
    void PlatformRender();
};

