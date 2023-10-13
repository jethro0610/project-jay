#include <glm/gtx/string_cast.hpp>
#include "HitSystem.h"
#include "../Entity/Entity.h"
#include "../Entity/EntityKey.h"
#include "../Components/HitboxComponent.h"
#include "../Components/HurtboxComponent.h"
#include "../Components/TransformComponent.h"
#include "../Components/VelocityComponent.h"
#include "../../Logging/Logger.h"
using namespace glm;

constexpr EntityKey hitKey = GetEntityKey<HitboxComponent, TransformComponent>();
constexpr EntityKey hurtKey = GetEntityKey<HurtboxComponent, TransformComponent>();

void HitSystem::Execute(
    EntityList& entities,
    ComponentList& components
) {
    auto& hitboxComponent = std::get<HitboxComponent&>(components);
    auto& transformComponent = std::get<TransformComponent&>(components);
    auto& hurtboxComponent = std::get<HurtboxComponent&>(components);
    auto& velocityComponent = std::get<VelocityComponent&>(components);
    HitList hitList;

    for (int h = 0; h < MAX_ENTITIES; h++) {
        if (!entities[h].ShouldUpdate(hitKey)) continue;
        if (!hitboxComponent.hitbox[h].active) continue;
        const Transform& hitterTransform = transformComponent.transform[h];
        const Hitbox& hitbox = hitboxComponent.hitbox[h];

        for (int t = 0; t < MAX_ENTITIES; t++) {
            if (!entities[t].ShouldUpdate(hurtKey)) continue;
            if (h == t) continue;
            const Transform& targetTransform = transformComponent.transform[t];
            const Hurtbox& hurtbox = hurtboxComponent.hurtbox[t];
            
            Collision collision = Collision::GetCollision(
                hitterTransform, 
                hitbox, 
                targetTransform, 
                hurtbox 
            );
            if (collision.isColliding)
                hitList.push_back({h, t, collision});
        }
    }

    for (const Hit& hit : hitList) {
        vec3 direction = normalize(hit.collision.resolution);
        velocityComponent.velocity[hit.target] = 
            direction * hitboxComponent.hitbox[hit.hitter].horizontalKb + 
            Transform::worldUp * hitboxComponent.hitbox[hit.hitter].verticalKb;
    }
}

// Add curving hits, sp release, damage, angular momentum, hitlag, etc...
