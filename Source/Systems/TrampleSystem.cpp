#include "TrampleSystem.h"
#include "Components/GroundTraceComponent.h"
#include "Components/TrampleComponent.h"
#include "Components/TransformComponent.h"
#include "Spread/SpreadManager.h"
using namespace glm;

constexpr EntityKey key = GetEntityKey<TrampleComponent, TransformComponent>();
constexpr EntityKey groundKey = GetEntityKey<GroundTraceComponent>();

void TrampleSystem::Execute(
    EntityList& entities,
    ComponentList& components,
    SpreadManager& spreadManager
) {
    auto& groundTraceComponent = components.Get<GroundTraceComponent>();
    auto& trampleComponent = components.Get<TrampleComponent>();
    auto& transformComponent = components.Get<TransformComponent>();

    spreadManager.ClearTramples();
    for (int i = 0; i < MAX_ENTITIES; i++) {
        if (!entities[i].ShouldUpdate(key)) continue;

        const vec3& position = transformComponent.transform[i].position;

        if (entities[i].MatchesKey(groundKey)) {
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
