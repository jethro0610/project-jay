#include "SpreadActivatorSystem.h"
#include "../Entity/EntityManager.h"
#include "../World/SpreadManager.h"
#include "../World/World.h"
using namespace glm;

void SpreadActivatorSystem::Execute(
    EntityManager& entityManager, 
    World& world,
    SpreadManager& spreadManager
) {
    SpreadActivatorComponent& spreadActivatorComponent= entityManager.spreadActivatorComponent_;
    TransformComponent& transformComponent = entityManager.transformComponent_;
    GroundTraceComponent& groundTraceComponent = entityManager.groundTraceComponent_;
    SpreadDetectComponent& spreadDetectComponent=  entityManager.spreadDetectComponent_;
    for (int i = 0; i < MAX_ENTITIES; i++) {
        const Entity& entity = entityManager.entities_[i];

        if (!entity.alive_)
            continue;

        if (!entity.HasComponents({spreadActivatorComponent, transformComponent}))
            continue;

        const bool hasDetect = entity.HasComponent(spreadDetectComponent);
        const int16_t radius = spreadActivatorComponent.radius[i]; 
        if (radius == 0)
            continue;
        else if (radius > 0)
            AddSpread(entityManager, world, spreadManager, i, hasDetect, radius - 1);
        else
            RemoveSpread(entityManager, world, spreadManager, i, -(radius - 1));
    }
}

void SpreadActivatorSystem::AddSpread(
    EntityManager& entityManager, 
    World& world,
    SpreadManager& spreadManager,
    EntityID entity,
    bool hasDetect,
    int16_t radius
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

    const AddSpreadInfo addSpreadInfo = spreadManager.AddSpread(position, radius);
    if (!addSpreadInfo.added || !hasDetect)
        return;
    spreadDetectComponent.lastAdd[entity] = addSpreadInfo.key;
}
void SpreadActivatorSystem::RemoveSpread(
    EntityManager& entityManager,
    World& world,
    SpreadManager& spreadManager,
    EntityID entity,
    int16_t radius
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
    spreadManager.RemoveSpread(position, radius);
}
