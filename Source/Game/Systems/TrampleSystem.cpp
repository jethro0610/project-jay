#include <glm/vec3.hpp>
#include "TrampleSystem.h"
#include "../Entity/Entity.h"
#include "../World/SpreadManager.h"
#include "../Components/GroundTraceComponent.h"
#include "../Components/TrampleComponent.h"
#include "../Components/TransformComponent.h"
using namespace glm;

constexpr EntityKey key = GetEntityKey<TrampleComponent, TransformComponent>();
constexpr EntityKey groundKey = GetEntityKey<GroundTraceComponent>();

void TrampleSystem::Execute(
    std::array<Entity, MAX_ENTITIES>& entities,
    SpreadManager& spreadManager,
    GroundTraceComponent& groundTraceComponent,
    TrampleComponent& trampleComponent,
    TransformComponent& transformComponent
) {
    spreadManager.ClearTramples();
    for (int i = 0; i < MAX_ENTITIES; i++) {
        const Entity& entity = entities[i];
        if (!entity.ShouldUpdate())
            continue;
        if (!entity.MatchesKey(key))
            continue;

        const vec3& position = transformComponent.transform[i].position;

        if (entity.MatchesKey(groundKey)) {
            if (groundTraceComponent.enteredGround[i])
                spreadManager.Trample(position, trampleComponent.groundEntryRadius[i]);
            else if (groundTraceComponent.onGround[i])
                spreadManager.Trample(position, trampleComponent.radius[i]);
            else
                continue;
        }
        spreadManager.Trample(transformComponent.transform[i].position, trampleComponent.radius[i]);
    }
}
