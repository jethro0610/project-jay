#include "SpreadActivatorSystem.h"
using namespace glm;

void SpreadActivatorSystem::Execute(
    World* world,
    Entity* entities, 
    SpreadManager* spreadManager,
    SpreadActivatorComponent& spreadActivatorComponent, 
    SpreadDetectComponent& spreadDetectComponent,
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
        const bool activated = spreadManager->AddSpread(spreadKey, transform.position_.y);
        if (!activated || !entity.HasComponent<SpreadDetectComponent>())
            continue;

        ivec2* lastDetect = spreadDetectComponent.lastDetect[i];
        for (int s = 0; s < MAX_DETECT - 1; s++) 
            lastDetect[s + 1] = lastDetect[s];
        lastDetect[0] = spreadKey;
    }
}
