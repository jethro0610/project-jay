#include <glm/vec3.hpp>
#include <ctime>
#include "Game.h"
#include "../Constants/GameConstants.h"
#include "../Constants/TimeConstants.h"
#include "Time.h"
#include "../Logging/Logger.h"
#include "../Helpers/Random.h"
using namespace glm;

using namespace std::chrono;
#define GETCOMP(COMP) entityManager_.GetComponent<COMP>()

void Game::Init() {
    srand(time(0));
    GETCOMP(StaticModelComponent).renderer = &renderer_;

    // Create the camera and assign it to the renderer
    renderer_.camera_ = &camera_;

    // Create the player entity
    entityManager_.LoadEntity("e_player");
    Transform playerTransform;
    playerTransform.position = vec3(0.0f, 50.0f, 0.0f);
    entityManager_.CreateEntity("e_player", playerTransform);

    entityManager_.LoadEntity("e_rock");
    Transform rockTransform;

    for (int j = 0; j < 6; j++) {
        float x0 = (rand() % 200) - 100;
        float z0 = (rand() % 200) - 100;
        for (int i = 0; i < 4; i++) {
            vec2 planarPos = RandomVector2D(15.0f); 
            rockTransform.position =  vec3(x0 + planarPos.x, 20.0f, z0 + planarPos.y);

            float rx = (rand() % 360);
            float ry = (rand() % 360);
            float rz = (rand() % 360);
            rockTransform.rotation = quat(vec3(rx, ry, rz));

            float scale = RandomFloatRange(1.5f, 3.0);
            rockTransform.scale = vec3(scale);
            entityManager_.CreateEntity("e_rock", rockTransform);
        }
    }

    // entityManager_.LoadEntity("target_test");
    // Transform targetTransform;
    // targetTransform.position = vec3(50.0f, 15.0f, 50.0f);
    // targetTransform.scale = vec3(3.0f);
    // entityManager_.CreateEntity("target_test", targetTransform);

    entityManager_.LoadEntity("e_tree");
    Transform treeTransform;
    treeTransform.position = vec3(50.0f, 10.0f, 50.0f);
    entityManager_.CreateEntity("e_tree", treeTransform);

    treeTransform.position = vec3(-27.0f, 0.0f, -43.0f);
    treeTransform.rotation = quat(vec3(0.0f, 33.0f, 0.0f));
    entityManager_.CreateEntity("e_tree", treeTransform);

    treeTransform.position = vec3(-38.0f, 5.0f, 105.0f);
    treeTransform.rotation = quat(vec3(0.0f, 15.0f, 0.0f));
    entityManager_.CreateEntity("e_tree", treeTransform);

    entityManager_.LoadEntity("e_test_skel");
    Transform skelTransform;
    skelTransform.position = vec3(0.0f, 0.0f, 0.0f);
    skelTransform.scale = vec3(2.0f);
    // entityManager_.CreateEntity("e_test_skel", skelTransform);

    camera_.target_ = PLAYER_ENTITY;
}

void Game::Update() {
    timeAccumlulator_ += GlobalTime::GetDeltaTime();
    while (timeAccumlulator_ >= TIMESTEP) {
        FlushInputs_P();
        FreezeSytem::Execute(entityManager_.entities_);
        IntervalSpawnSystem::Execute(
            entityManager_.entities_,
            entityManager_,
            seedManager_,
            GETCOMP(IntervalSpawnComponent),
            GETCOMP(ProjectileComponent),
            GETCOMP(TransformComponent),
            GETCOMP(VelocityComponent)
        );
        TransformSystem::UpdateLastTransforms(
            entityManager_.entities_,
            GETCOMP(TransformComponent)
        );
        CollisionSystem::Execute(
            entityManager_.entities_,
            seedManager_,
            spreadManager_,
            GETCOMP(ColliderComponent),
            GETCOMP(MeterComponent),
            GETCOMP(ProjectileComponent),
            GETCOMP(TransformComponent),
            GETCOMP(VelocityComponent)
        );
        SpreadActivatorSystem::Execute(
            entityManager_.entities_,
            spreadManager_,
            world_,
            GETCOMP(GroundTraceComponent),
            GETCOMP(MeterComponent),
            GETCOMP(SpreadActivatorComponent),
            GETCOMP(SpreadDetectComponent),
            GETCOMP(TransformComponent)
        );
        SpreadDetectSystem::Execute(
            entityManager_.entities_,
            spreadManager_,
            GETCOMP(SpreadDetectComponent),
            GETCOMP(TransformComponent)
        );
        playerController_.Execute(
            world_,
            spreadManager_,
            camera_,
            GETCOMP(GroundTraceComponent),
            GETCOMP(MeterComponent),
            GETCOMP(MovementComponent),
            GETCOMP(SpreadActivatorComponent),
            GETCOMP(TransformComponent),
            GETCOMP(VelocityComponent),
            inputs_
        );
        VelocitySystem::CalculateGravity(
            entityManager_.entities_,
            GETCOMP(VelocityComponent)
        );
        MovementSystem::Execute(
            entityManager_.entities_,
            GETCOMP(GroundTraceComponent),
            GETCOMP(MovementComponent),
            GETCOMP(SpreadDetectComponent),
            GETCOMP(TransformComponent),
            GETCOMP(VelocityComponent)
        );
        ProjectileSystem::Execute(
            entityManager_.entities_,
            GETCOMP(ProjectileComponent),
            GETCOMP(TransformComponent),
            GETCOMP(VelocityComponent) 
        );
        GroundTraceSystem::Execute(
            entityManager_.entities_,
            world_,
            GETCOMP(GroundTraceComponent),
            GETCOMP(TransformComponent)
        );
        GroundStickSystem::Stick(
            entityManager_.entities_,
            world_,
            GETCOMP(GroundTraceComponent),
            GETCOMP(TransformComponent),
            GETCOMP(VelocityComponent)
        );
        VelocitySystem::Apply(
            entityManager_.entities_,
            GETCOMP(TransformComponent),
            GETCOMP(VelocityComponent)
        );
        DestroyMeterSystem::Execute(
            entityManager_.entities_,
            entityManager_,
            seedManager_,
            GETCOMP(MeterComponent),
            GETCOMP(TransformComponent)
        );
        timeAccumlulator_ -= TIMESTEP;
    }
    TransformSystem::UpdateRenderTransforms(
        entityManager_.entities_,
        GETCOMP(TransformComponent),
        timeAccumlulator_
    );
    seedManager_.CalculatePositions(
        world_,
        GETCOMP(MeterComponent),
        GETCOMP(TransformComponent),
        timeAccumlulator_
    );
    seedManager_.GetCaptures(
        entityManager_.entities_,
        GETCOMP(ColliderComponent),
        GETCOMP(TransformComponent)
    );
    camera_.Update(inputs_);
    SkeletonSystem::CalculatePoses(
        entityManager_.entities_,
        GETCOMP(StaticModelComponent),
        GETCOMP(TransformComponent),
        GlobalTime::GetDeltaTime()
    );
    renderer_.Render(
        entityManager_.entities_, 
        seedManager_,
        spreadManager_, 
        world_, 
        GETCOMP(MeterComponent),
        GETCOMP(StaticModelComponent),
        GETCOMP(TransformComponent)
    );
}
