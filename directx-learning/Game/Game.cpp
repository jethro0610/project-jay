#include "Game.h"

void Game::Init() {

}

void Game::Update() {
    UpdateCameraTransform();
    renderer_->Render();
}

void Game::UpdateCameraTransform() {
    lookX_ -= deltaLookX_ / 240.0f;
    lookY_ -= deltaLookY_ / 240.0f;
    lookY_ = clamp(lookY_, radians(-80.0f), radians(80.0f));

    renderer_->cameraTransform_.rotation_ = quat(vec3(lookY_, lookX_, 0.0f));
    vec3 forwardMovement = renderer_->cameraTransform_.rotation_ * Transform::forward * forwardInput_ * 0.1f;
    vec3 rightMovement = renderer_->cameraTransform_.rotation_ * Transform::right * sideInput_ * 0.1f;
    renderer_->cameraTransform_.position_ += forwardMovement + rightMovement;
}