#include "Game.h"

using namespace std::chrono;

Game::Game() {
    resolutionWidth_ = 1280;
    resolutionHeight_ = 720;
    camera_ = new Camera(&transformC_, 10.0f);
}

void Game::Init() {
    // Initialize the time
    currentTimeUSec_ = duration_cast<microseconds>(system_clock::now().time_since_epoch()).count();
    lastTimeUSec_ = currentTimeUSec_;
    elapsedTime_ = 0.0f;

    resourceManager_->LoadStaticModel("st_sphere");
    resourceManager_->LoadStaticModel("st_toruscone");

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

    for (int x = 0; x < 6; x++)
    for (int y = 0; y < 6; y++) {
        Transform spawnTransform;
        spawnTransform.position_ = vec3(10.0f * (x + 1), 50.0f, 10.0f * (y + 1));
        spawnTransform.scale_ = vec3(1.0f, 1.0f, 1.0f);
        activeEntityC_.active[x + y * 6] = true;
        transformC_.transform[x + y * 6] = spawnTransform;
        colliderC_.radius[x + y * 6] = 1.25f;
        staticModelC_.model[x + y * 6] = "st_sphere";
    }

    camera_->trackEntity_ = 0;
}

void Game::Update(float deltaTime, float elapsedTime) {
    camera_->Update(deltaTime, forwardInput_, sideInput_, deltaLookX_, deltaLookY_);
    CollisionSystem::Execute(world_, activeEntityC_, transformC_, colliderC_, deltaTime_);

    RenderComponents renderComponents {
        &activeEntityC_,
        &staticModelC_,
        &transformC_
    };
    renderer_->Render(deltaTime, elapsedTime, renderComponents);
}

void Game::UpdateTime() {
    lastTimeUSec_ = currentTimeUSec_;
    currentTimeUSec_ = duration_cast<microseconds>(system_clock::now().time_since_epoch()).count();
    deltaTime_ = (currentTimeUSec_ - lastTimeUSec_) * 0.000001f;
    elapsedTime_ += deltaTime_;
    deltaTime_ = min<float>(MAX_DELTA_TIME, deltaTime_);
}