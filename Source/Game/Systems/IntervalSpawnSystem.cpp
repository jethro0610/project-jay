#include "IntervalSpawnSystem.h"
#include "../Components/StaticModelComponent.h"
#include "../Systems/ProjectileSystem.h"
using namespace glm;
    
void IntervalSpawnSystem::Execute (
    EntityManager& entityManager,
    TransformComponent& transformComponent,
    IntervalSpawnComponent& intervalSpawnComponent,
    ProjectileComponent& projectileComponent,
    VelocityComponent& velocityComponent
) {
    for (int i = 0; i < MAX_ENTITIES; i++) {
        const Entity& entity = entityManager.entities_[i];
        if (!entity.alive_)
            continue;
        if (!entity.HasComponent<IntervalSpawnComponent>())
            continue;

        uint16_t& spawnTimer = intervalSpawnComponent.spawnTimer[i];
        spawnTimer += 1;
        if (spawnTimer >= intervalSpawnComponent.spawnInterval[i]) {
            uint16_t newEntity = entityManager.CreateEntity();
            auto transformProps = entityManager.RegisterComponent<TransformComponent>(newEntity);
            transformProps.transform.position_ = transformComponent.transform[i].position_;
            transformProps.transform.position_ += vec3(0.0f, 2.5f, 0.0f);
            transformProps.interpolate = true;

            auto modelProperties = entityManager.RegisterComponent<StaticModelComponent>(newEntity);
            modelProperties.model = "st_sphere";
            modelProperties.materials[0] = "playerMaterial";

            entityManager.RegisterComponent<VelocityComponent>(newEntity);

            auto projectileProps = entityManager.RegisterComponent<ProjectileComponent>(newEntity);
            projectileProps.type = ProjectileType::Random;
            projectileProps.param1 = 30.0f;
            projectileProps.param2 = 50.0f;
            ProjectileSystem::Launch(projectileComponent, velocityComponent, transformComponent, newEntity);

            // Create the spawn entity
            spawnTimer = 0;
        }
    }
}
