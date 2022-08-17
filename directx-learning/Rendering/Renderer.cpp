#pragma once
#include "Renderer.h"

void Renderer::Init() {
    UpdateProjMatrix(70.0f, 0.5f, 1000.0f);
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

void Renderer::Temp_GetWorldAndNormalMatrix(mat4& outWorld, mat4& outNormal) {
    testRot_ += 0.005f;
    Transform testTransform;

    float sinRot = sin(testRot_);
    float cosRot = cos(testRot_);
    //testTransform.rotation_ = quat(vec3(testRot_ / 3.0f, testRot_, 0.0f));
    testTransform.GetWorldAndNormalMatrix(outWorld, outNormal);
}

mat4 Renderer::GetWorldViewProjection(mat4 worldMatrix) {
    return projMatrix_ * viewMatrix_ * worldMatrix;
}

void Renderer::Render() {
    UpdateViewMatrix();
    PlatformRender();
}