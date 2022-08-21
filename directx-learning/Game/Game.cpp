#include "Game.h"

void Game::Init() {
    world_ = new World();
    for (int x = 0; x < MAX_X_COORDINATES; x++)
    for (int y = 0; y < MAX_Y_COORDINATES; y++)
    for (int z = 0; z < MAX_Z_COORDINATES; z++) {
        ivec3 coordinates(x, y, z);
        std::vector<WorldVertex> vertices;
        std::vector<uint16_t> indices;
        world_->GetMeshGPUCompute(dxResources_, coordinates, vertices, indices);
        SendWorldMeshToGPU_P(coordinates, vertices, indices);
    }
}

void Game::Update() {
    UpdateCameraTransform();
    renderer_->Render();
}

void Game::UpdateCameraTransform() {
    lookX_ += deltaLookX_;
    lookY_ += deltaLookY_;
    lookY_ = clamp(lookY_, radians(-80.0f), radians(80.0f));

    renderer_->cameraTransform_.rotation_ = quat(vec3(lookY_, lookX_, 0.0f));
    vec3 forwardMovement = renderer_->cameraTransform_.GetForwardVector() * forwardInput_ * 0.1f;
    vec3 rightMovement = renderer_->cameraTransform_.GetRightVector() * sideInput_ * 0.1f;
    renderer_->cameraTransform_.position_ += forwardMovement + rightMovement;
}