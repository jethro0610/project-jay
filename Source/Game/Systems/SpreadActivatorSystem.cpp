#include "SpreadActivatorSystem.h"
using namespace glm;

void SpreadActivatorSystem::Execute(
    EntityManager& entityManager, 
    World& world,
    SpreadManager& spreadManager
) {
    for (int i = 0; i < MAX_ENTITIES; i++) {
        const Entity& entity = entityManager.entities_[i];
        SpreadActivatorComponent& spreadActivatorComponent= entityManager.spreadActivatorComponent_;
        TransformComponent& transformComponent = entityManager.transformComponent_;
        GroundTraceComponent& groundTraceComponent = entityManager.groundTraceComponent_;
        SpreadDetectComponent& spreadDetectComponent=  entityManager.spreadDetectComponent_;

        if (!entity.alive_)
            continue;

        if (!entity.HasComponents({spreadActivatorComponent, transformComponent}))
            continue;
        
        switch (spreadActivatorComponent.mode[i]) {
            case SpreadActivatorMode::Spread:
                AddSpread(entityManager, world, spreadManager, i);
                break;

            case SpreadActivatorMode::Cut:
                RemoveSpread(entityManager, world, spreadManager, i);
                break;

            case SpreadActivatorMode::NoSpread:
                break;

            default:
                break;
        }
    }
}

void SpreadActivatorSystem::AddSpread(
    EntityManager& entityManager, 
    World& world,
    SpreadManager& spreadManager,
    EntityID entity
) {
    SpreadActivatorComponent& spreadActivatorComponent= entityManager.spreadActivatorComponent_;
    TransformComponent& transformComponent = entityManager.transformComponent_;
    GroundTraceComponent& groundTraceComponent = entityManager.groundTraceComponent_;
    SpreadDetectComponent& spreadDetectComponent=  entityManager.spreadDetectComponent_;

    bool onGround = false;
    if (entityManager.entities_[entity].HasComponent(groundTraceComponent)) {
        onGround = groundTraceComponent.onGround[entity];
    }

    if (!onGround && spreadActivatorComponent.groundOnly[entity])
        return;
    
    const vec3 position = transformComponent.transform[entity].position_;
    const bool hasDetect = entityManager.entities_[entity].HasComponent(spreadDetectComponent);

    const AddSpreadInfo addSpreadInfo = spreadManager.AddSpread(position);
    if (!addSpreadInfo.added || !hasDetect)
        return;

    ivec2* lastDetect = spreadDetectComponent.lastDetect[entity];
    for (int s = 0; s < MAX_DETECT - 1; s++) 
        lastDetect[s + 1] = lastDetect[s];
    lastDetect[0] = addSpreadInfo.key;
}
void SpreadActivatorSystem::RemoveSpread(
    EntityManager& entityManager,
    World& world,
    SpreadManager& spreadManager,
    EntityID entity 
) {

    SpreadActivatorComponent& spreadActivatorComponent= entityManager.spreadActivatorComponent_;
    TransformComponent& transformComponent = entityManager.transformComponent_;
    GroundTraceComponent& groundTraceComponent = entityManager.groundTraceComponent_;
    SpreadDetectComponent& spreadDetectComponent=  entityManager.spreadDetectComponent_;

    bool onGround = false;
    if (entityManager.entities_[entity].HasComponent(groundTraceComponent)) {
        onGround = groundTraceComponent.onGround[entity];
    }

    if (!onGround && spreadActivatorComponent.groundOnly[entity])
        return;

    const vec3 position = transformComponent.transform[entity].position_;
    spreadManager.RemoveSpread(position, 3);
}
