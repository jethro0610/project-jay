#include "Game.h"

void Game::Init() {
    world_ = new World();

    std::vector<WorldVertex> vertices;
    std::vector<uint16_t> indices;
    world_->Temp_Generate(vertices, indices);
    dxResources_->Temp_UpdateWorld(vertices, indices);
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