#pragma once

#ifdef WINDOWS
#include "DirectXLayer.h"
#endif

#include <string>
#include <vector>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
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

    glm::mat4 viewMatrix_;
    glm::mat4 projMatrix_;
    void UpdateViewMatrix();
    void UpdateProjMatrix(float fov, float nearClip, float farClip);
    glm::mat4 GetWorldViewProjection();

    float testRot;

    void Render();
    void PlatformRender();
};

