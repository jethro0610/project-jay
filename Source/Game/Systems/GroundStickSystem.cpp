#include "GroundStickSystem.h"

void GroundStickSystem::Step (
    World* world, 
    Entity* entities,
    TransformComponent& transformComponent, 
    GroundTraceComponent& groundTraceComponent
) {
    for (int i = 0 ; i < MAX_ENTITIES; i++) {
        const Entity& entity = entities[i];
        if (!entities->HasComponents<TransformComponent, GroundTraceComponent>())
            continue;

        if (groundTraceComponent.onGround[i] && groundTraceComponent.stickType[i] >= StickType::StepUp)
            transformComponent.transform[i].position_.y += 0.25f;
    }
}
void GroundStickSystem::Stick (
    World* world, 
    Entity* entities,
    TransformComponent& transformComponent, 
    GroundTraceComponent& groundTraceComponent
) {
    for (int i = 0 ; i < MAX_ENTITIES; i++) {
        const Entity& entity = entities[i];
        if (!entities->HasComponents<TransformComponent, GroundTraceComponent>())
            continue;

        if (groundTraceComponent.onGround[i] && groundTraceComponent.stickType[i] >= StickType::StickOnly)
            transformComponent.transform[i].position_.y = groundTraceComponent.groundPosition[i];
    }
}
