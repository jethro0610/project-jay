#pragma once
#include "Renderer.h"

Renderer::Renderer(ResourceManager* resourceManager) {
    resourceManager_ = resourceManager;
    cameraTransform_.position_ = vec3(0.0f, 0.0f, 10.0f);
    Init_P();
}

void Renderer::UpdateViewMatrix() {
    vec3 forward = rotate(cameraTransform_.rotation_, Transform::worldForward);
    vec3 up = rotate(cameraTransform_.rotation_, Transform::worldUp);

    viewMatrix_ = lookAtRH(
        cameraTransform_.position_,
        cameraTransform_.position_ + forward,
        up
    );
}

void Renderer::UpdateProjMatrix(float fov, float nearClip, float farClip) {
    projMatrix_ = perspectiveFovRH_ZO(radians(fov), (float)width_, (float)height_, nearClip, farClip);
}

mat4 Renderer::GetWorldViewProjection(mat4 worldMatrix) {
    return projMatrix_ * viewMatrix_ * worldMatrix;
}

void Renderer::Render(float deltaTime, float elapsedTime, RenderComponents renderComponents) {
    UpdateViewMatrix();
    Clear_P();
    RenderWorld_P();
    StaticModelRenderList staticModelRenderList;
    BuildStaticModelRenderList(renderComponents, staticModelRenderList);
    RenderStaticMeshes_P(renderComponents, staticModelRenderList);
    Present_P();
}

void Renderer::BuildStaticModelRenderList(
    RenderComponents frameInfo,
    StaticModelRenderList& outStaticModelRenderList
) {
    for (int i = 0; i < MAX_ENTITIES; i++) {
        if (frameInfo.activeEntityComponents->active[i] == false)
            continue;

        std::string model = frameInfo.staticMeshComponents->model[i];
        if (frameInfo.staticMeshComponents->model[i] == NO_MODEL)
            continue;

        assert(resourceManager_->loadedStaticModels_.count(model) != 0);

        if (outStaticModelRenderList.count(model) == 0)
            outStaticModelRenderList[model] = std::vector<int>();

        outStaticModelRenderList[model].push_back(i);
    }
}
