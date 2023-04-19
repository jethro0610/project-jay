#include "GroundStickSystem.h"
#include "../World/World.h"
#include "../Entity/EntityManager.h"

void GroundStickSystem::Step(EntityManager& entityManager, World& world) {
    TransformComponent& transformComponent = entityManager.transformComponent_;
    GroundTraceComponent& groundTraceComponent = entityManager.groundTraceComponent_;
    for (int i = 0 ; i < MAX_ENTITIES; i++) {
        const Entity& entity = entityManager.entities_[i];
        if (!entity.alive_)
            continue;
        if (!entity.HasComponents({transformComponent, groundTraceComponent}))
            continue;

        if (groundTraceComponent.onGround[i] && groundTraceComponent.stickType[i] >= StickType::StepUp)
            transformComponent.transform[i].position_.y += 1.0f;
    }
}
void GroundStickSystem::Stick(EntityManager& entityManager, World& world) {
    TransformComponent& transformComponent = entityManager.transformComponent_;
    GroundTraceComponent& groundTraceComponent = entityManager.groundTraceComponent_;
    for (int i = 0 ; i < MAX_ENTITIES; i++) {
        const Entity& entity = entityManager.entities_[i];
        if (!entity.alive_)
            continue;
        if (!entity.HasComponents({transformComponent, groundTraceComponent}))
            continue;

        if (groundTraceComponent.onGround[i] && groundTraceComponent.stickType[i] >= StickType::StickOnly)
            transformComponent.transform[i].position_.y = groundTraceComponent.groundPosition[i];
    }
}
