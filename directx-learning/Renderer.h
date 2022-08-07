#pragma once

#ifdef WINDOWS
#include "DirectXLayer.h"
#endif

#include <string>

class Renderer {
public:
    #ifdef WINDOWS
    Renderer(DirectXLayer* dxResources);
    DirectXLayer* dxLayer;
    #endif
    ~Renderer();

    void Draw();
};

