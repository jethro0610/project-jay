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
    EntityList& entities,
    ComponentList& components,
    SpreadManager& spreadManager
) {
    auto& groundTraceComponent = std::get<GroundTraceComponent&>(components);
    auto& trampleComponent = std::get<TrampleComponent&>(components);
    auto& transformComponent = std::get<TransformComponent&>(components);

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
