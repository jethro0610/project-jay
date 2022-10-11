#include "Game.h"
#include "../Logging/Logger.h"

using namespace std::chrono;

Game::Game() {
    resolutionWidth_ = 1280;
    resolutionHeight_ = 720;
    timeAccumlulator_ = 0.0f;
}

void Game::Init() {
    world_ = new World(entityManager_.entities_, &entityManager_.GetComponent<TerrainModComponent>());
    camera_ = new Camera(&entityManager_.GetComponent<TransformComponent>(), 15.0f);
    spreadManager_ = new SpreadManager(resourceManager_, world_);

    // Initialize the time
    currentTimeUSec_ = duration_cast<microseconds>(system_clock::now().time_since_epoch()).count();
    lastTimeUSec_ = currentTimeUSec_;
    elapsedTime_ = 0.0f;

    // Create the camera and assign it to the renderer
    renderer_->camera_ = camera_;

    resourceManager_->LoadStaticModel("st_sphere");
    // Create the player entity
    Transform spawnTransform;
    spawnTransform.position_ = vec3(10.0f, 50.0f, 10.0f);
    entityManager_.CreateEntity();

    auto transformProperties = entityManager_.RegisterComponent<TransformComponent>(PLAYER_ENTITY);
    transformProperties.transform = spawnTransform;
    transformProperties.interpolate = true;

    auto colliderProperties = entityManager_.RegisterComponent<ColliderComponent>(PLAYER_ENTITY);
    colliderProperties.radius = 1.0f;

    auto modelProperties = entityManager_.RegisterComponent<StaticModelComponent>(PLAYER_ENTITY);
    modelProperties.model = "st_sphere";
    modelProperties.materials[0] = "playerMaterial";

    auto groundTraceProperties = entityManager_.RegisterComponent<GroundTraceComponent>(PLAYER_ENTITY);
    groundTraceProperties.distance = 2.0f;

    auto movementProperties = entityManager_.RegisterComponent<MovementComponent>(PLAYER_ENTITY);
    movementProperties.recievesFrom = RecieveMovementFrom::Player;
    movementProperties.supportedMoveModes.set(MoveMode::Default);
    movementProperties.supportedMoveModes.set(MoveMode::Ski);
    
    entityManager_.RegisterComponent<VelocityComponent>(PLAYER_ENTITY);

    camera_->trackEntity_ = PLAYER_ENTITY;

    // Create the testing terrain modifier entity
    int terrainModEntity = entityManager_.CreateEntity();
    auto terrainModProps = entityManager_.RegisterComponent<TerrainModComponent>(terrainModEntity);
    terrainModProps.position = vec3(0.0f);
    terrainModProps.radius = 16.0f;

    for (int x = -MAX_X_CHUNKS / 2; x < MAX_X_CHUNKS / 2; x++)
    for (int y = -MAX_Y_CHUNKS / 2; y < MAX_Y_CHUNKS / 2; y++)
    for (int z = -MAX_Z_CHUNKS / 2; z < MAX_Z_CHUNKS / 2; z++) {
        ivec3 chunk(x, y, z);
        std::vector<WorldVertex> vertices;
        std::vector<uint16_t> indices;
        world_->GetMeshGPUCompute(dxResources_, chunk, vertices, indices);
        SendWorldMeshToGPU_P(chunk, vertices, indices);
    }

    for (int x = 0; x < 64; x++) 
    for (int z = 0; z < 64; z++) {
        spreadManager_->AddSpread(ivec2(x * 2.0f, z * 2.0f), 48.0f);
    }
}

void Game::Update(float deltaTime, float elapsedTime) {
    timeAccumlulator_ += deltaTime;

    while (timeAccumlulator_ >= TIMESTEP) {
        TerrainModSystem::Execute(
            entityManager_.entities_,
            entityManager_.GetComponent<TerrainModComponent>()
        );
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
    spreadManager_->UpdateRenderData_P();
    RenderComponents renderComponents {
        entityManager_.GetComponent<StaticModelComponent>(),
        entityManager_.GetComponent<TransformComponent>()
    };
    renderer_->Render(deltaTime, elapsedTime, entityManager_.entities_, renderComponents, spreadManager_);
}

void Game::UpdateTime() {
    lastTimeUSec_ = currentTimeUSec_;
    currentTimeUSec_ = duration_cast<microseconds>(system_clock::now().time_since_epoch()).count();
    deltaTime_ = (currentTimeUSec_ - lastTimeUSec_) * 0.000001f;
    elapsedTime_ += deltaTime_;
    deltaTime_ = min<float>(MAX_DELTA_TIME, deltaTime_);
}
