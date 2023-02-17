#include "KickSystem.h"

void KickSystem::Execute(
    Entity* entities,
    SpreadManager* spreadManager,
    TransformComponent& transformComponent,
    HitboxComponent& hitboxComponent
) {
    for (int i = 0; i < MAX_ENTITIES; i++) {
        const Entity& entity = entities[i]; 
        if (!entity.alive_)
            continue;
        if (!entity.HasComponent<HitboxComponent>())
            continue;
        if (!hitboxComponent.properties[i][HitboxProperty::RecieveKick])
            continue;
        if (!hitboxComponent.state[i][HitboxState::RecievedKick])
            continue;

        const Transform& transform = transformComponent.transform[i];
        ivec2 spreadKey = spreadManager->WorldPositionToSpreadKey(transform.position_);
        spreadManager->AddSpread(spreadKey, transform.position_.y);
    }
}
