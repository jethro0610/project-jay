#pragma once
#include "Renderer.h"
#include "Transform.h"
void Renderer::Init() {
    UpdateProjMatrix(45.0f, 0.1f, 1000.0f);
    testRot = 0.0f;
}

void Renderer::UpdateViewMatrix() {
    viewMatrix_ = glm::lookAtLH(
        glm::vec3(0.0f, 0.0f, -10.0f),
        glm::vec3(0.0f, 0.0f, -9.0f),
        glm::vec3(0.0f, 1.0f, 0.0f)
    );
}

void Renderer::UpdateProjMatrix(float fov, float nearClip, float farClip) {
    projMatrix_ = glm::perspectiveFovLH_ZO(glm::radians(fov), (float)width_, (float)height_, nearClip, farClip);
}

glm::mat4 Renderer::GetWorldViewProjection() {
    testRot += 0.05f;
    Transform testTransform;
    float sinRot = glm::sin(testRot);
    float cosRot = glm::cos(testRot);
    testTransform.position_ = glm::vec3(sinRot * 0.0f, 0.0f, 0.0f);
    testTransform.rotation_ = glm::quat(glm::vec3((cosRot / 60.0f) * 90.0f, 0.0f, 0.0f));
    glm::mat4 worldMatrix = testTransform.GetWorldMatrix();
    glm::mat4 worldViewProj = projMatrix_ * viewMatrix_ * worldMatrix;
    worldViewProj = glm::transpose(worldViewProj);
    return worldViewProj;
}

void Renderer::Render() {
    UpdateViewMatrix();
    PlatformRender();
}