#include "Game.h"
#include "../Logging/Logger.h"

using namespace std::chrono;

void Game::Init() {
    // Initialize the time
    currentTimeUSec_ = duration_cast<microseconds>(system_clock::now().time_since_epoch()).count();
    lastTimeUSec_ = currentTimeUSec_;
    elapsedTime_ = 0.0f;

    // Create the camera and assign it to the renderer
    renderer_.camera_ = &camera_;

    resourceManager_.LoadStaticModel("st_sphere");
    // Create the player entity
    Transform spawnTransform;
    spawnTransform.position_ = vec3(10.0f, 50.0f, 10.0f);

    entityManager_.CreateEntity();
    auto transformProperties = entityManager_.RegisterComponent<TransformComponent>(PLAYER_ENTITY);
    transformProperties.transform = spawnTransform;
    transformProperties.interpolate = true;
    entityManager_.RegisterComponent<WorldColliderComponent>(PLAYER_ENTITY);
    auto modelProperties = entityManager_.RegisterComponent<StaticModelComponent>(PLAYER_ENTITY);
    modelProperties.model = "st_sphere";
    modelProperties.materials[0] = "playerMaterial";
    auto groundTraceProperties = entityManager_.RegisterComponent<GroundTraceComponent>(PLAYER_ENTITY);
    groundTraceProperties.distance = 2.0f;
    groundTraceProperties.stickType = StickType::StepUp;
    auto movementProperties = entityManager_.RegisterComponent<MovementComponent>(PLAYER_ENTITY);
    entityManager_.RegisterComponent<SpreadActivatorComponent>(PLAYER_ENTITY);
    entityManager_.RegisterComponent<VelocityComponent>(PLAYER_ENTITY);
    entityManager_.RegisterComponent<PickupComponent>(PLAYER_ENTITY);
    entityManager_.RegisterComponent<SpreadDetectComponent>(PLAYER_ENTITY);
    auto playerBubbleProps = entityManager_.RegisterComponent<BubbleComponent>(PLAYER_ENTITY);
    playerBubbleProps.radius = 2.0f;
    entityManager_.RegisterComponent<KickerComponent>(PLAYER_ENTITY);

    camera_.trackEntity_ = PLAYER_ENTITY;

    // Create the testing terrain modifier entity
    /* uint16_t terrainModEntity = entityManager_.CreateEntity(); */
    /* auto terrainModProps = entityManager_.RegisterComponent<TerrainModComponent>(terrainModEntity); */
    /* terrainModProps.position = vec3(0.0f); */
    /* terrainModProps.radius = 16.0f; */

    uint16_t holdEntity = entityManager_.CreateEntity();
    auto transformProps = entityManager_.RegisterComponent<TransformComponent>(holdEntity);
    transformProps.transform.position_ = vec3(0.0f, 40.0f, 0.0f);
    transformProps.transform.scale_ = vec3(2.0f);
    transformProps.interpolate = true;
    entityManager_.RegisterComponent<VelocityComponent>(holdEntity);
    entityManager_.RegisterComponent<ProjectileComponent>(holdEntity);
    auto holdableProps = entityManager_.RegisterComponent<HoldableComponent>(holdEntity);
    auto meshProps = entityManager_.RegisterComponent<StaticModelComponent>(holdEntity);
    meshProps.model = "st_sphere";
    meshProps.materials[0] = "playerMaterial";
    auto pickupBubbleProps = entityManager_.RegisterComponent<BubbleComponent>(holdEntity);
    pickupBubbleProps.radius = 2.0f;
    entityManager_.RegisterComponent<KickableComponent>(holdEntity);

    for (int x = -MAX_X_CHUNKS / 2; x < MAX_X_CHUNKS / 2; x++)
    for (int y = -MAX_Y_CHUNKS / 2; y < MAX_Y_CHUNKS / 2; y++)
    for (int z = -MAX_Z_CHUNKS / 2; z < MAX_Z_CHUNKS / 2; z++) {
        ivec3 chunk(x, y, z);
        std::vector<WorldVertex> vertices;
        std::vector<uint16_t> indices;
        world_.GetMeshGPUCompute(&dxResources_, chunk, vertices, indices);
        SendWorldMeshToGPU_P(chunk, vertices, indices);
    }
}

void Game::Update(float deltaTime, float elapsedTime) {
    timeAccumlulator_ += deltaTime;
    while (timeAccumlulator_ >= TIMESTEP) {
        FlushInputs_P();
        TerrainModSystem::Execute(
            entityManager_,
            entityManager_.GetComponent<TerrainModComponent>()
        );
        TransformSystem::UpdateLastTransforms(
            entityManager_,
            entityManager_.GetComponent<TransformComponent>()
        );
        IntersectSystem::Execute(
            entityManager_,
            spreadManager_,
            entityManager_.GetComponent<TransformComponent>(),
            entityManager_.GetComponent<BubbleComponent>(),
            entityManager_.GetComponent<PickupComponent>(),
            entityManager_.GetComponent<HoldableComponent>(),
            entityManager_.GetComponent<KickerComponent>(),
            entityManager_.GetComponent<KickableComponent>()
        );
        PickupSystem::ExecuteHold(
            entityManager_,
            entityManager_.GetComponent<PickupComponent>(),
            entityManager_.GetComponent<TransformComponent>(),
            entityManager_.GetComponent<VelocityComponent>(),
            entityManager_.GetComponent<ProjectileComponent>()
        );
        SpreadActivatorSystem::Execute(
            entityManager_,
            world_,
            spreadManager_,
            entityManager_.GetComponent<SpreadActivatorComponent>(),
            entityManager_.GetComponent<SpreadDetectComponent>(),
            entityManager_.GetComponent<TransformComponent>(), 
            entityManager_.GetComponent<GroundTraceComponent>()
        );
        SpreadDetectSystem::Execute(
            entityManager_, 
            spreadManager_, 
            entityManager_.GetComponent<TransformComponent>(),
            entityManager_.GetComponent<SpreadDetectComponent>()
        );
        PlayerInputSystem::Execute(
            entityManager_,
            camera_, 
            inputs_, 
            entityManager_.GetComponent<MovementComponent>(),
            entityManager_.GetComponent<PickupComponent>(),
            entityManager_.GetComponent<SpreadActivatorComponent>()
        );
        GroundStickSystem::Step(
            entityManager_,
            world_,
            entityManager_.GetComponent<TransformComponent>(),
            entityManager_.GetComponent<GroundTraceComponent>()
        );
        MovementSystem::Execute(
            entityManager_,
            entityManager_.GetComponent<MovementComponent>(),
            entityManager_.GetComponent<GroundTraceComponent>(),
            entityManager_.GetComponent<TransformComponent>(), 
            entityManager_.GetComponent<VelocityComponent>(),
            entityManager_.GetComponent<SpreadDetectComponent>()
        );
        ProjectileSystem::CalculateVelocities(
            entityManager_,
            world_,
            entityManager_.GetComponent<ProjectileComponent>(),
            entityManager_.GetComponent<VelocityComponent>(),
            entityManager_.GetComponent<TransformComponent>()
        );
        VelocitySystem::Apply(
            entityManager_,
            entityManager_.GetComponent<TransformComponent>(),
            entityManager_.GetComponent<VelocityComponent>()
        );
        GroundTraceSystem::Execute(
            entityManager_,
            world_, 
            entityManager_.GetComponent<TransformComponent>(),
            entityManager_.GetComponent<GroundTraceComponent>()
        );
        GroundStickSystem::Stick(
            entityManager_,
            world_,
            entityManager_.GetComponent<TransformComponent>(),
            entityManager_.GetComponent<GroundTraceComponent>()
        );
        CollisionSystem::Execute(
            entityManager_,
            world_, 
            entityManager_.GetComponent<TransformComponent>(),
            entityManager_.GetComponent<WorldColliderComponent>()
        );
        timeAccumlulator_ -= TIMESTEP;
    }
    TransformSystem::UpdateRenderTransforms(
        timeAccumlulator_, 
        entityManager_,
        entityManager_.GetComponent<TransformComponent>()
    );
    camera_.Update(deltaTime, inputs_);
    spreadManager_.UpdateRenderData_P();
    RenderComponents renderComponents {
        entityManager_.GetComponent<StaticModelComponent>(),
        entityManager_.GetComponent<TransformComponent>()
    };
    renderer_.Render(deltaTime, elapsedTime, entityManager_.entities_, renderComponents, spreadManager_);
}

void Game::UpdateTime() {
    lastTimeUSec_ = currentTimeUSec_;
    currentTimeUSec_ = duration_cast<microseconds>(system_clock::now().time_since_epoch()).count();
    deltaTime_ = (currentTimeUSec_ - lastTimeUSec_) * 0.000001f;
    elapsedTime_ += deltaTime_;
    deltaTime_ = min<float>(MAX_DELTA_TIME, deltaTime_);
}
