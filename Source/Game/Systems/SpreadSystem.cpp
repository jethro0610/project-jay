#include "SpreadSystem.h"
using namespace glm;

void SpreadSystem::Execute(
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
        spreadManager->AddSpread(ivec2(transform.position_.x, transform.position_.z), transform.position_.y);
    }
}
