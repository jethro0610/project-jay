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

    resourceManager_.LoadEntity("player");
    resourceManager_.LoadEntity("test_projectile");
    entityManager_.CreateEntity("player");
    entityManager_.transformComponent_.transform[PLAYER_ENTITY] = spawnTransform;

    camera_.trackEntity_ = PLAYER_ENTITY;

    // Create the testing terrain modifier entity
    /* uint16_t terrainModEntity = entityManager_.CreateEntity(); */
    /* auto terrainModProps = entityManager_.RegisterComponent<TerrainModComponent>(terrainModEntity); */
    /* terrainModProps.position = vec3(0.0f); */
    /* terrainModProps.radius = 16.0f; */

    uint16_t holdEntity = entityManager_.CreateEntity();
    entityManager_.RegisterComponent(entityManager_.transformComponent_, holdEntity);
    entityManager_.transformComponent_.transform[holdEntity].position_ = vec3(0.0f, 40.0f, 0.0f);
    entityManager_.transformComponent_.transform[holdEntity].scale_ = vec3(2.0f);
    entityManager_.transformComponent_.interpolate[holdEntity] = true;
    entityManager_.RegisterComponent(entityManager_.velocityComponent_, holdEntity);
    entityManager_.RegisterComponent(entityManager_.projectileComponent_, holdEntity);
    entityManager_.RegisterComponent(entityManager_.holdableComponent_, holdEntity);
    entityManager_.RegisterComponent(entityManager_.staticModelComponent_, holdEntity);
    entityManager_.staticModelComponent_.model[holdEntity] = "st_sphere";
    entityManager_.staticModelComponent_.materials[holdEntity][0] = "playerMaterial";
    entityManager_.RegisterComponent(entityManager_.bubbleComponent_, holdEntity);
    entityManager_.bubbleComponent_.radius[holdEntity] = 2.0f;
    entityManager_.RegisterComponent(entityManager_.intervalSpawnComponent_, holdEntity);
    entityManager_.intervalSpawnComponent_.spawnInterval[holdEntity] = 120;
    entityManager_.intervalSpawnComponent_.entityToSpawn[holdEntity] = "test_projectile";

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
        IntervalSpawnSystem::Execute(
            entityManager_,
            entityManager_.transformComponent_,
            entityManager_.intervalSpawnComponent_,
            entityManager_.projectileComponent_,
            entityManager_.velocityComponent_
        );
        TerrainModSystem::Execute(
            entityManager_,
            entityManager_.terrainModComponent_
        );
        TransformSystem::UpdateLastTransforms(
            entityManager_,
            entityManager_.transformComponent_
        );
        IntersectSystem::Execute(
            entityManager_,
            spreadManager_,
            entityManager_.transformComponent_,
            entityManager_.bubbleComponent_,
            entityManager_.pickupComponent_,
            entityManager_.holdableComponent_,
            entityManager_.kickerComponent_,
            entityManager_.kickableComponent_
        );
        PickupSystem::ExecuteHold(
            entityManager_,
            entityManager_.pickupComponent_,
            entityManager_.transformComponent_,
            entityManager_.velocityComponent_,
            entityManager_.projectileComponent_
        );
        SpreadActivatorSystem::Execute(
            entityManager_,
            world_,
            spreadManager_,
            entityManager_.spreadActivatorComponent_,
            entityManager_.spreadDetectComponent_,
            entityManager_.transformComponent_,
            entityManager_.groundTraceComponent_
        );
        SpreadDetectSystem::Execute(
            entityManager_, 
            spreadManager_, 
            entityManager_.transformComponent_,
            entityManager_.spreadDetectComponent_
        );
        PlayerInputSystem::Execute(
            entityManager_,
            camera_, 
            inputs_, 
            entityManager_.movementComponent_,
            entityManager_.pickupComponent_,
            entityManager_.spreadActivatorComponent_
        );
        GroundStickSystem::Step(
            entityManager_,
            world_,
            entityManager_.transformComponent_,
            entityManager_.groundTraceComponent_
        );
        MovementSystem::Execute(
            entityManager_,
            entityManager_.movementComponent_,
            entityManager_.groundTraceComponent_,
            entityManager_.transformComponent_,
            entityManager_.velocityComponent_,
            entityManager_.spreadDetectComponent_
        );
        ProjectileSystem::CalculateVelocities(
            entityManager_,
            world_,
            entityManager_.projectileComponent_,
            entityManager_.velocityComponent_,
            entityManager_.transformComponent_
        );
        VelocitySystem::Apply(
            entityManager_,
            entityManager_.transformComponent_,
            entityManager_.velocityComponent_
        );
        GroundTraceSystem::Execute(
            entityManager_,
            world_, 
            entityManager_.transformComponent_,
            entityManager_.groundTraceComponent_
        );
        GroundStickSystem::Stick(
            entityManager_,
            world_,
            entityManager_.transformComponent_,
            entityManager_.groundTraceComponent_
        );
        CollisionSystem::Execute(
            entityManager_,
            world_, 
            entityManager_.transformComponent_,
            entityManager_.worldColliderComponent_
        );
        timeAccumlulator_ -= TIMESTEP;
    }
    TransformSystem::UpdateRenderTransforms(
        timeAccumlulator_, 
        entityManager_,
        entityManager_.transformComponent_
    );
    camera_.Update(deltaTime, inputs_);
    spreadManager_.UpdateRenderData_P();
    RenderComponents renderComponents {
        entityManager_.staticModelComponent_,
        entityManager_.transformComponent_
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
