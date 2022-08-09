#pragma once
#include "Renderer.h"
#include "Transform.h"
void Renderer::Init() {
    UpdateProjMatrix(45.0f, 0.1f, 1000.0f);
    testRot = 0.0f;
}

void Renderer::UpdateViewMatrix() {
    viewMatrix_ = lookAtLH(
        vec3(0.0f, 0.0f, -10.0f),
        vec3(0.0f, 0.0f, -9.0f),
        vec3(0.0f, 1.0f, 0.0f)
    );
}

void Renderer::UpdateProjMatrix(float fov, float nearClip, float farClip) {
    projMatrix_ = perspectiveFovLH_ZO(radians(fov), (float)width_, (float)height_, nearClip, farClip);
}

mat4 Renderer::GetWorldViewProjection() {
    testRot += 0.05f;
    Transform testTransform;
    float sinRot = sin(testRot);
    float cosRot = cos(testRot);
    testTransform.position_ = vec3(sinRot * 0.0f, 0.0f, 0.0f);
    testTransform.rotation_ = quat(vec3((cosRot / 60.0f) * 90.0f, 0.0f, 0.0f));
    mat4 worldMatrix = testTransform.GetWorldMatrix();
    mat4 worldViewProj = projMatrix_ * viewMatrix_ * worldMatrix;
    worldViewProj = transpose(worldViewProj);
    return worldViewProj;
}

void Renderer::Render() {
    UpdateViewMatrix();
    PlatformRender();
}