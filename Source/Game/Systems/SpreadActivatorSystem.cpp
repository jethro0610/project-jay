#include "SpreadActivatorSystem.h"
using namespace glm;

void SpreadActivatorSystem::Execute(
    EntityManager& entityManager, 
    World& world,
    SpreadManager& spreadManager,
    SpreadActivatorComponent& spreadActivatorComponent, 
    SpreadDetectComponent& spreadDetectComponent,
    TransformComponent& transformComponent,
    GroundTraceComponent& groundTraceComponent
) {
    for (int i = 0; i < MAX_ENTITIES; i++) {
        const Entity& entity = entityManager.entities_[i];
        if (!entity.alive_)
            continue;

        if (!entity.HasComponents({spreadActivatorComponent, transformComponent}))
            continue;
        
        if (!spreadActivatorComponent.active[i])
            continue;

        bool onGround = false;
        if (entity.HasComponent(groundTraceComponent)) {
            onGround = groundTraceComponent.onGround[i];
        }

        if (!onGround && spreadActivatorComponent.groundOnly[i])
            continue;
        
        const vec3 position = transformComponent.transform[i].position_;
        const bool hasDetect = entity.HasComponent(spreadDetectComponent);

        const AddSpreadInfo addSpreadInfo = spreadManager.AddSpread(position);
        if (!addSpreadInfo.added || !hasDetect)
            continue;

        ivec2* lastDetect = spreadDetectComponent.lastDetect[i];
        for (int s = 0; s < MAX_DETECT - 1; s++) 
            lastDetect[s + 1] = lastDetect[s];
        lastDetect[0] = addSpreadInfo.key;
    }
}
