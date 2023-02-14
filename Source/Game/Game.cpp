#include "Game.h"
#include "../Logging/Logger.h"

using namespace std::chrono;

Game::Game() {
    resolutionWidth_ = 1280;
    resolutionHeight_ = 720;
    timeAccumlulator_ = 0.0f;
    gamepad_ = Gamepad();
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

    entityManager_.RegisterComponent<WorldColliderComponent>(PLAYER_ENTITY);

    auto modelProperties = entityManager_.RegisterComponent<StaticModelComponent>(PLAYER_ENTITY);
    modelProperties.model = "st_sphere";
    modelProperties.materials[0] = "playerMaterial";

    auto groundTraceProperties = entityManager_.RegisterComponent<GroundTraceComponent>(PLAYER_ENTITY);
    groundTraceProperties.distance = 2.0f;
    groundTraceProperties.stickType = StickType::StepUp;

    auto movementProperties = entityManager_.RegisterComponent<MovementComponent>(PLAYER_ENTITY);
    
    auto spreadProperites = entityManager_.RegisterComponent<SpreadActivatorComponent>(PLAYER_ENTITY);
    spreadProperites.radius = NO_SPREAD;
    entityManager_.RegisterComponent<VelocityComponent>(PLAYER_ENTITY);

    auto pickupProps = entityManager_.RegisterComponent<PickupComponent>(PLAYER_ENTITY);
    pickupProps.range = 1.0f;

    entityManager_.RegisterComponent<SpreadDetectComponent>(PLAYER_ENTITY);
    
    auto hitboxProps = entityManager_.RegisterComponent<HitboxComponent>(PLAYER_ENTITY);
    hitboxProps.radius = 2.0f;
    hitboxProps.properties.set(HitboxProperty::SendKick);

    camera_->trackEntity_ = PLAYER_ENTITY;

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
    holdableProps.range = 2.0f;
    auto meshProps = entityManager_.RegisterComponent<StaticModelComponent>(holdEntity);
    meshProps.model = "st_sphere";
    meshProps.materials[0] = "playerMaterial";
    auto pickupHitboxProps = entityManager_.RegisterComponent<HitboxComponent>(holdEntity);
    pickupHitboxProps.radius = 2.0f;
    pickupHitboxProps.properties.set(HitboxProperty::RecieveKick);

    for (int x = -MAX_X_CHUNKS / 2; x < MAX_X_CHUNKS / 2; x++)
    for (int y = -MAX_Y_CHUNKS / 2; y < MAX_Y_CHUNKS / 2; y++)
    for (int z = -MAX_Z_CHUNKS / 2; z < MAX_Z_CHUNKS / 2; z++) {
        ivec3 chunk(x, y, z);
        std::vector<WorldVertex> vertices;
        std::vector<uint16_t> indices;
        world_->GetMeshGPUCompute(dxResources_, chunk, vertices, indices);
        SendWorldMeshToGPU_P(chunk, vertices, indices);
    }
}

void Game::Update(float deltaTime, float elapsedTime) {
    timeAccumlulator_ += deltaTime;
    while (timeAccumlulator_ >= TIMESTEP) {
        FlushInputs_P();
        TerrainModSystem::Execute(
            entityManager_.entities_,
            entityManager_.GetComponent<TerrainModComponent>()
        );
        TransformSystem::UpdateLastTransforms(
            entityManager_.entities_,
            entityManager_.GetComponent<TransformComponent>()
        );
        PickupSystem::ExecutePickup(
            entityManager_.entities_,
            entityManager_.GetComponent<PickupComponent>(),
            entityManager_.GetComponent<HoldableComponent>(),
            entityManager_.GetComponent<TransformComponent>()
        );
        PickupSystem::ExecuteHold(
            entityManager_.entities_,
            entityManager_.GetComponent<PickupComponent>(),
            entityManager_.GetComponent<TransformComponent>(),
            entityManager_.GetComponent<VelocityComponent>(),
            entityManager_.GetComponent<ProjectileComponent>()
        );
        KickSystem::Execute(
            entityManager_.entities_,
            entityManager_.GetComponent<TransformComponent>(),
            entityManager_.GetComponent<HitboxComponent>()
        );
        SpreadActivatorSystem::Execute(
            world_,
            entityManager_.entities_,
            spreadManager_,
            entityManager_.GetComponent<SpreadActivatorComponent>(),
            entityManager_.GetComponent<SpreadDetectComponent>(),
            entityManager_.GetComponent<TransformComponent>(), 
            entityManager_.GetComponent<GroundTraceComponent>()
        );
        SpreadDetectSystem::Execute(
            entityManager_.entities_, 
            spreadManager_, 
            entityManager_.GetComponent<TransformComponent>(),
            entityManager_.GetComponent<SpreadDetectComponent>()
        );
        PlayerInputSystem::Execute(
            inputs_, 
            camera_, 
            entityManager_.entities_,
            entityManager_.GetComponent<MovementComponent>(),
            entityManager_.GetComponent<PickupComponent>(),
            entityManager_.GetComponent<MeterComponent>(),
            entityManager_.GetComponent<SpreadActivatorComponent>()
        );
        GroundStickSystem::Step(
            world_,
            entityManager_.entities_,
            entityManager_.GetComponent<TransformComponent>(),
            entityManager_.GetComponent<GroundTraceComponent>()
        );
        MovementSystem::Execute(
            entityManager_.entities_,
            entityManager_.GetComponent<MovementComponent>(),
            entityManager_.GetComponent<GroundTraceComponent>(),
            entityManager_.GetComponent<TransformComponent>(), 
            entityManager_.GetComponent<VelocityComponent>(),
            entityManager_.GetComponent<SpreadDetectComponent>()
        );
        ProjectileSystem::CalculateVelocities(
            entityManager_.entities_,
            entityManager_.GetComponent<ProjectileComponent>(),
            entityManager_.GetComponent<VelocityComponent>(),
            entityManager_.GetComponent<TransformComponent>(),
            world_
        );
        VelocitySystem::Apply(
            entityManager_.entities_,
            entityManager_.GetComponent<TransformComponent>(),
            entityManager_.GetComponent<VelocityComponent>()
        );
        GroundTraceSystem::Execute(
            world_, 
            entityManager_.entities_,
            entityManager_.GetComponent<TransformComponent>(),
            entityManager_.GetComponent<GroundTraceComponent>()
        );
        GroundStickSystem::Stick(
            world_,
            entityManager_.entities_,
            entityManager_.GetComponent<TransformComponent>(),
            entityManager_.GetComponent<GroundTraceComponent>()
        );
        CollisionSystem::Execute(
            world_, 
            entityManager_.entities_,
            entityManager_.GetComponent<TransformComponent>(),
            entityManager_.GetComponent<WorldColliderComponent>()
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
