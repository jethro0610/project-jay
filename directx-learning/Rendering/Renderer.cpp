#pragma once
#include "Renderer.h"

void Renderer::Init() {
    UpdateProjMatrix(70.0f, 0.1f, 1000.0f);
    cameraTransform_.position_ = vec3(0.0f, 0.0f, -10.0f);
    testRot_ = 0.0f;
}

void Renderer::UpdateViewMatrix() {
    vec3 forward = vec3(0.0f, 0.0f, 1.0f);
    forward = rotate(cameraTransform_.rotation_, forward);

    vec3 up = vec3(0.0f, 1.0f, 0.0f);
    up = rotate(cameraTransform_.rotation_, up);

    viewMatrix_ = lookAtLH(
        cameraTransform_.position_,
        cameraTransform_.position_ + forward,
        up
    );
}

void Renderer::UpdateProjMatrix(float fov, float nearClip, float farClip) {
    projMatrix_ = perspectiveFovLH_ZO(radians(fov), (float)width_, (float)height_, nearClip, farClip);
}

mat4 Renderer::GetWorldViewProjection() {
    testRot_ += 0.05f;
    Transform testTransform;
    
    float sinRot = sin(testRot_);
    float cosRot = cos(testRot_);
    testTransform.position_ = vec3(sinRot * 0.0f, 0.0f, 0.0f);
    testTransform.rotation_ = quat(vec3( radians(sinRot * 45.0f), 0.0f, 0.0f));
    
    mat4 worldMatrix = testTransform.GetWorldMatrix();
    mat4 worldViewProj = projMatrix_ * viewMatrix_ * worldMatrix;
    return worldViewProj;
}

void Renderer::Render() {
    UpdateViewMatrix();
    PlatformRender();
}