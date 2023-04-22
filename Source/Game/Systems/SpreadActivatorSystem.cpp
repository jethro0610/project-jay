#include "SpreadActivatorSystem.h"
#include "../Entity/Entity.h"
#include "../../Helpers/EntityHelpers.h"
#include "../World/SpreadManager.h"
#include "../World/World.h"
#include "../Components/GroundTraceComponent.h"
#include "../Components/SpreadActivatorComponent.h"
#include "../Components/SpreadDetectComponent.h"
#include "../Components/TransformComponent.h"
using namespace glm;

constexpr EntityKey key = GetEntityKey<SpreadActivatorComponent, TransformComponent>();
constexpr EntityKey detectKey = GetEntityKey<SpreadDetectComponent>();
constexpr EntityKey groundKey = GetEntityKey<GroundTraceComponent>();

void SpreadActivatorSystem::Execute(
    Entity* entities, 
    SpreadManager& spreadManager,
    World& world,
    GroundTraceComponent& groundTraceComponent,
    SpreadActivatorComponent& spreadActivatorComponent,
    SpreadDetectComponent& spreadDetectComponent,
    TransformComponent& transformComponent
) {
    for (int i = 0; i < MAX_ENTITIES; i++) {
        const Entity& entity = entities[i];
        if (!entity.alive_)
            continue;
        if (!entity.MatchesKey(key))
            continue;

        bool onGround = false;
        if (entity.MatchesKey(groundKey))
            onGround = groundTraceComponent.onGround[i];
        if (!onGround && spreadActivatorComponent.groundOnly[i])
            continue;

        const int16_t radius = spreadActivatorComponent.radius[i]; 
        if (radius == 0)
            continue;

        // TODO : Is the has detect here necessary? Can maybe just assign it
        // by default
        const vec3 position = transformComponent.transform[i].position_;
        const bool hasDetect = entity.MatchesKey(detectKey);
        if (radius > 0) {
            const AddSpreadInfo addSpreadInfo = spreadManager.AddSpread(position, radius - 1);
            if (!addSpreadInfo.added || !hasDetect)
                continue;
            spreadDetectComponent.lastAdd[i] = addSpreadInfo.key;
        }
        else if (radius < 0)
            spreadManager.RemoveSpread(position, -radius);
    }
}
