#include "GroundStickSystem.h"
#include "../Components/GroundTraceComponent.h"
#include "../Components/TransformComponent.h"
#include "../Entity/Entity.h"
#include "../Entity/EntityKey.h"
#include "../World/World.h"

constexpr EntityKey key = GetEntityKey<GroundTraceComponent, TransformComponent>();

void GroundStickSystem::Step(
    Entity* entities, 
    World& world, 
    GroundTraceComponent& groundTraceComponent,
    TransformComponent& transformComponent
) {
    for (int i = 0 ; i < MAX_ENTITIES; i++) {
        const Entity& entity = entities[i];
        if (!entity.alive_)
            continue;
        if (!entity.MatchesKey(key))
            continue;

        if (groundTraceComponent.onGround[i] && groundTraceComponent.stickType[i] >= StickType::StepUp)
            transformComponent.transform[i].position_.y += 1.0f;
    }
}

void GroundStickSystem::Stick(
    Entity* entities, 
    World& world, 
    GroundTraceComponent& groundTraceComponent,
    TransformComponent& transformComponent
) {
    for (int i = 0 ; i < MAX_ENTITIES; i++) {
        const Entity& entity = entities[i];
        if (!entity.alive_)
            continue;
        if (!entity.MatchesKey(key))
            continue;

        if (groundTraceComponent.onGround[i] && groundTraceComponent.stickType[i] >= StickType::StickOnly)
            transformComponent.transform[i].position_.y = groundTraceComponent.groundPosition[i];
    }
}
