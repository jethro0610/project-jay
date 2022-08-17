#include "Game.h"
#include "Skeleton.h"

void Game::Init() {
    Skeleton testSkeleton = Skeleton("sk_test.jsk");
}

void Game::Update() {
    renderer_->cameraTransform_.position_ += vec3(sideInput_, 0.0f, forwardInput_) * 0.1f;
    renderer_->Render();
}