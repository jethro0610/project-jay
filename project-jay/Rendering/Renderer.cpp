#pragma once
#include "Renderer.h"

Renderer::Renderer(ResourceManager* resourceManager) {
    camera_ = nullptr;
    resourceManager_ = resourceManager;
    Init_P();
}

void Renderer::UpdateViewMatrix() {
    assert(camera_ != nullptr);
    viewMatrix_ = camera_->GetViewMatrix();
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
        std::string model = frameInfo.staticMeshComponents.model[i];
        if (frameInfo.staticMeshComponents.model[i] == "") // TODO: change this to use entity
            continue;

        assert(resourceManager_->loadedStaticModels_.count(model) != 0);

        if (outStaticModelRenderList.count(model) == 0)
            outStaticModelRenderList[model] = std::vector<int>();

        outStaticModelRenderList[model].push_back(i);
    }
}