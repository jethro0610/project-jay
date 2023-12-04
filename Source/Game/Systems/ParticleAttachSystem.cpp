#include "ParticleAttachSystem.h"
#include "../ParticleEmitter.h"
#include "../Entity/Entity.h"
#include "../Entity/EntityKey.h"
#include "../Components/TransformComponent.h"

constexpr EntityKey key = GetEntityKey<TransformComponent>();

void ParticleAttachSystem::Execute(
    EntityList& entities,
    ComponentList& components
) {
    TransformComponent& transformComponent = components.Get<TransformComponent>();
    for (int i = 0; i < MAX_ENTITIES; i++) {
        const Entity& entity = entities[i];
        if (!entity.ShouldUpdate(key)) continue;

        for (ParticleEmitter* emitter : entity.emitters_)
            emitter->transform_ = transformComponent.renderTransform[i];
    }
}
