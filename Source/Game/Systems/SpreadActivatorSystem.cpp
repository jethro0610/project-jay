#include "SpreadActivatorSystem.h"
using namespace glm;

void SpreadActivatorSystem::Execute(
    World* world,
    Entity* entities, 
    SpreadManager* spreadManager,
    SpreadActivatorComponent& spreadActivatorComponent, 
    TransformComponent& transformComponent,
    GroundTraceComponent& groundTraceComponent
) {
    for (int i = 0; i < MAX_ENTITIES; i++) {
        const Entity& entity = entities[i];
        if (!entity.alive_)
            continue;

        if (!entity.HasComponents<SpreadActivatorComponent, TransformComponent>())
            continue;

        bool onGround = false;
        if (entity.HasComponent<GroundTraceComponent>()) {
            onGround = groundTraceComponent.onGround[i];
        }

        if (!onGround && spreadActivatorComponent.groundOnly[i])
            continue;
        
        const Transform transform = transformComponent.transform[i];
        ivec2 spreadKey = spreadManager->WorldPositionToSpreadKey(transform.position_);
        spreadManager->AddSpread(spreadKey, transform.position_.y);
    }
}
