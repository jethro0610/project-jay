#include "IntervalSpawnSystem.h"
#include "../Components/StaticModelComponent.h"
using namespace glm;
    
void IntervalSpawnSystem::Execute (
    EntityManager& entityManager,
    TransformComponent& transformComponent,
    IntervalSpawnComponent& intervalSpawnComponent
) {
    for (int i = 0; i < MAX_ENTITIES; i++) {
        const Entity& entity = entityManager.entities_[i];
        if (!entity.alive_)
            continue;
        if (!entity.HasComponent<IntervalSpawnComponent>())
            continue;

        uint16_t& spawnTimer = intervalSpawnComponent.spawnTimer[i];
        if (spawnTimer >= intervalSpawnComponent.spawnInterval[i]) {
            uint16_t newEntity = entityManager.CreateEntity();
            auto transformProps = entityManager.RegisterComponent<TransformComponent>(newEntity);
            transformProps.transform.position_ = transformComponent.transform[i].position_;
            transformProps.transform.position_ += vec3(0.0f, 2.5f, 0.0f);
            transformProps.interpolate = true;

            auto modelProperties = entityManager.RegisterComponent<StaticModelComponent>(PLAYER_ENTITY);
            modelProperties.model = "st_sphere";
            modelProperties.materials[0] = "playerMaterial";

            // Create the spawn entity
            spawnTimer = 0;
        }
    }
}
