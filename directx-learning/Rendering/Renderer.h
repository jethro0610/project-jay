#pragma once

#ifdef WINDOWS
#include "DirectXLayer.h"
#endif

#include "../Transform.h"
#include "RenderTypes.h"

class Renderer {
public:
    #ifdef WINDOWS
    Renderer(DirectXLayer* dxLayer);
    DirectXLayer* dxLayer_;
    #endif
    void Init();
    ~Renderer();

    int width_;
    int height_;

    mat4 viewMatrix_;
    mat4 projMatrix_;
    void UpdateViewMatrix();
    void UpdateProjMatrix(float fov, float nearClip, float farClip);
    mat4 GetWorldViewProjection();

    float testRot_;

    Transform cameraTransform_;

    void Render();
    void PlatformRender();
};
