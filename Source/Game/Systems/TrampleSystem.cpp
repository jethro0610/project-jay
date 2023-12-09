#include "TrampleSystem.h"
#include "Game/Components/GroundTraceComponent.h"
#include "Game/Components/TrampleComponent.h"
#include "Game/Components/TransformComponent.h"
#include "Game/Spread/SpreadManager.h"
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
