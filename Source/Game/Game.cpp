#include "Game.h"
#include "../Logging/Logger.h"

using namespace std::chrono;

Game::Game() {
    resolutionWidth_ = 1600;
    resolutionHeight_ = 900;
    timeAccumlulator_ = 0.0f;
}

void Game::Init() {
    // Initialize the time
    currentTimeUSec_ = duration_cast<microseconds>(system_clock::now().time_since_epoch()).count();
    lastTimeUSec_ = currentTimeUSec_;
    elapsedTime_ = 0.0f;

    // Create the camera and assign it to the renderer
    camera_ = new Camera(&entityManager_.GetComponent<TransformComponent>(), 15.0f);
    renderer_->camera_ = camera_;

    resourceManager_->LoadStaticModel("st_capsule");

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

    auto transformProperties = entityManager_.RegisterComponent<TransformComponent>(PLAYER_ENTITY);
    transformProperties.transform = spawnTransform;
    transformProperties.interpolate = true;

    auto colliderProperties = entityManager_.RegisterComponent<ColliderComponent>(PLAYER_ENTITY);
    colliderProperties.radius = 1.0f;

    auto modelProperties = entityManager_.RegisterComponent<StaticModelComponent>(PLAYER_ENTITY);
    modelProperties.model = "st_capsule";
    modelProperties.materials[0] = "playerMaterial";

    auto groundTraceProperties = entityManager_.RegisterComponent<GroundTraceComponent>(PLAYER_ENTITY);
    groundTraceProperties.distance = 2.0f;

    auto movementProperties = entityManager_.RegisterComponent<MovementComponent>(PLAYER_ENTITY);
    movementProperties.recievesFrom = RecieveMovementFrom::Player;
    movementProperties.supportedMoveModes.set(MoveMode::Default);
    movementProperties.supportedMoveModes.set(MoveMode::Ski);
    
    entityManager_.RegisterComponent<VelocityComponent>(PLAYER_ENTITY);

    camera_->trackEntity_ = PLAYER_ENTITY;
}

void Game::Update(float deltaTime, float elapsedTime) {
    timeAccumlulator_ += deltaTime;
    while (timeAccumlulator_ >= TIMESTEP) {
        TransformSystem::UpdateLastTransforms(
            entityManager_.entities_,
            entityManager_.GetComponent<TransformComponent>()
        );
        PlayerInputSystem::Execute(
            inputs_, 
            camera_, 
            entityManager_.entities_,
            entityManager_.GetComponent<MovementComponent>()
        );
        MovementSystem::Execute(
            entityManager_.entities_,
            entityManager_.GetComponent<MovementComponent>(),
            entityManager_.GetComponent<GroundTraceComponent>(),
            entityManager_.GetComponent<TransformComponent>(), 
            entityManager_.GetComponent<VelocityComponent>(),
            entityManager_.GetComponent<ColliderComponent>()
        );
        GroundTraceSystem::Execute(
            world_, 
            entityManager_.entities_,
            entityManager_.GetComponent<TransformComponent>(),
            entityManager_.GetComponent<GroundTraceComponent>()
        );
        CollisionSystem::Execute(
            world_, 
            entityManager_.entities_,
            entityManager_.GetComponent<TransformComponent>(),
            entityManager_.GetComponent<ColliderComponent>(),
            entityManager_.GetComponent<GroundTraceComponent>()
        );
        timeAccumlulator_ -= TIMESTEP;
    }
    TransformSystem::UpdateRenderTransforms(
        timeAccumlulator_, 
        entityManager_.entities_,
        entityManager_.GetComponent<TransformComponent>()
    );
    camera_->Update(deltaTime, inputs_);
    RenderComponents renderComponents {
        entityManager_.GetComponent<StaticModelComponent>(),
        entityManager_.GetComponent<TransformComponent>()
    };
    renderer_->Render(deltaTime, elapsedTime, entityManager_.entities_, renderComponents);
}

void Game::UpdateTime() {
    lastTimeUSec_ = currentTimeUSec_;
    currentTimeUSec_ = duration_cast<microseconds>(system_clock::now().time_since_epoch()).count();
    deltaTime_ = (currentTimeUSec_ - lastTimeUSec_) * 0.000001f;
    elapsedTime_ += deltaTime_;
    deltaTime_ = min<float>(MAX_DELTA_TIME, deltaTime_);
}