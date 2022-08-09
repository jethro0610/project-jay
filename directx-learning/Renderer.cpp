#pragma once
#include "Renderer.h"

void Renderer::Init() {
    UpdateProjMatrix(45.0f, 0.1f, 1000.0f);
}

void Renderer::UpdateViewMatrix() {
    viewMatrix_ = glm::lookAtLH(
        glm::vec3(5.0f, 2.0f, -10.0f),
        glm::vec3(5.0f, 2.0f, -9.0f),
        glm::vec3(0.0f, 1.0f, 0.0f)
    );
}

void Renderer::UpdateProjMatrix(float fov, float nearClip, float farClip) {
    projMatrix_ = glm::perspectiveFovLH_ZO(glm::radians(fov), (float)width_, (float)height_, nearClip, farClip);
}

glm::mat4 Renderer::GetModelViewProjection() {
    glm::mat4 worldMatrix = glm::mat4(1.0f);
    glm::mat4 modelViewProj = projMatrix_ * viewMatrix_ * worldMatrix;
    modelViewProj = glm::transpose(modelViewProj);
    return modelViewProj;
}

void Renderer::Render() {
    UpdateViewMatrix();
    PlatformRender();
}