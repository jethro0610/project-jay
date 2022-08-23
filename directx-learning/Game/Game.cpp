#include "Game.h"
#include "../Logging/Logger.h"

using namespace std::chrono;

Game::Game() {
    resolutionWidth_ = 1280;
    resolutionHeight_ = 720;
    camera_ = new Camera(&transformC_, 10.0f);
    timeAccumlulator_ = 0.0f;
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

    Transform spawnTransform;
    spawnTransform.position_ = vec3(10.0f, 50.0f, 10.0f);
    activeEntityC_.active[PLAYER_ENTITY] = true;
    transformC_.transform[PLAYER_ENTITY] = spawnTransform;
    transformC_.interpolate[PLAYER_ENTITY] = true;
    colliderC_.radius[PLAYER_ENTITY] = 1.0f;
    staticModelC_.model[PLAYER_ENTITY] = "st_sphere";
    groundTraceC_.distance[PLAYER_ENTITY] = 1.25f;
    desiredMovementC_.recievesFrom[PLAYER_ENTITY] = RECIEVE_MOVEMENT_PLAYER;

    camera_->trackEntity_ = PLAYER_ENTITY;
}

void Game::Update(float deltaTime, float elapsedTime) {
    camera_->Update(deltaTime, inputs_);

    timeAccumlulator_ += deltaTime;
    while (timeAccumlulator_ >= TIMESTEP) {
        TransformSystem::UpdateLastTransforms(activeEntityC_, transformC_);
        PlayerInputSystem::Execute(inputs_, camera_, desiredMovementC_);
        GroundTraceSystem::Execute(world_, transformC_, groundTraceC_);
        MovementSystem::Execute(desiredMovementC_, groundTraceC_, transformC_, velocityC_, colliderC_);
        CollisionSystem::Execute(world_, activeEntityC_, transformC_, colliderC_, groundTraceC_);
        timeAccumlulator_ -= TIMESTEP;
    }
    TransformSystem::UpdateRenderTransforms(timeAccumlulator_, activeEntityC_, transformC_);

    RenderComponents renderComponents {
        activeEntityC_,
        staticModelC_,
        transformC_
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