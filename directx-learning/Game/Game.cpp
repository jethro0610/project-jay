#include "Game.h"
#include "../Logging/Logger.h"

using namespace std::chrono;

Game::Game() {
    resolutionWidth_ = 1280;
    resolutionHeight_ = 720;
}

void Game::Init() {
    // Initialize the time
    currentTimeUSec_ = duration_cast<microseconds>(system_clock::now().time_since_epoch()).count();
    lastTimeUSec_ = currentTimeUSec_;
    elapsedTime_ = 0.0f;

    // Create the camera and assign it to the renderer
    camera_ = new Camera(&entityManager_.GetComponent<TransformComponents>(), 10.0f);
    renderer_->camera_ = camera_;

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
    entityManager_.CreateEntity();

    auto transformProperties = entityManager_.RegisterComponent<TransformComponents>(PLAYER_ENTITY);
    transformProperties.transform = spawnTransform;
    transformProperties.interpolate = true;

    auto colliderProperties = entityManager_.RegisterComponent<ColliderComponents>(PLAYER_ENTITY);
    colliderProperties.radius = 1.0f;

    auto modelProperties = entityManager_.RegisterComponent<StaticModelComponents>(PLAYER_ENTITY);
    modelProperties.model = "st_sphere";

    auto groundTraceProperties = entityManager_.RegisterComponent<GroundTraceComponents>(PLAYER_ENTITY);
    groundTraceProperties.distance = 1.25f;

    auto desiredMovementProperties = entityManager_.RegisterComponent<DesiredMovementComponents>(PLAYER_ENTITY);
    desiredMovementProperties.recievesFrom = RECIEVE_MOVEMENT_PLAYER;
    
    entityManager_.RegisterComponent<VelocityComponents>(PLAYER_ENTITY);

    camera_->trackEntity_ = PLAYER_ENTITY;
}

void Game::Update(float deltaTime, float elapsedTime) {
    camera_->Update(deltaTime, inputs_);

    timeAccumlulator_ += deltaTime;
    while (timeAccumlulator_ >= TIMESTEP) {
        TransformSystem::UpdateLastTransforms(
            entityManager_.entities_,
            entityManager_.GetComponent<TransformComponents>()
        );
        PlayerInputSystem::Execute(
            inputs_, 
            camera_, 
            entityManager_.entities_,
            entityManager_.GetComponent<DesiredMovementComponents>()
        );
        GroundTraceSystem::Execute(
            world_, 
            entityManager_.entities_,
            entityManager_.GetComponent<TransformComponents>(),
            entityManager_.GetComponent<GroundTraceComponents>()
        );
        MovementSystem::Execute(
            entityManager_.entities_,
            entityManager_.GetComponent<DesiredMovementComponents>(),
            entityManager_.GetComponent<GroundTraceComponents>(),
            entityManager_.GetComponent<TransformComponents>(), 
            entityManager_.GetComponent<VelocityComponents>(),
            entityManager_.GetComponent<ColliderComponents>()
        );
        CollisionSystem::Execute(
            world_, 
            entityManager_.entities_,
            entityManager_.GetComponent<TransformComponents>(),
            entityManager_.GetComponent<ColliderComponents>(),
            entityManager_.GetComponent<GroundTraceComponents>()
        );
        timeAccumlulator_ -= TIMESTEP;
    }
    TransformSystem::UpdateRenderTransforms(
        timeAccumlulator_, 
        entityManager_.entities_,
        entityManager_.GetComponent<TransformComponents>()
    );

    RenderComponents renderComponents {
        entityManager_.GetComponent<StaticModelComponents>(),
        entityManager_.GetComponent<TransformComponents>()
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