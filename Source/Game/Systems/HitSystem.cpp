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
    std::array<Entity, MAX_ENTITIES>& entities,
    HitboxComponent& hitboxComponent,
    HurtboxComponent& hurtboxComponent, 
    TransformComponent& transformComponent,
    VelocityComponent& velocityComponent
) {
    HitList hitList;

    for (int h = 0; h < MAX_ENTITIES; h++) {
        const Entity& hitter = entities[h]; 
        if (!hitter.ShouldUpdate())
            continue;
        if (!hitter.MatchesKey(hitKey))
            continue;
        if (!hitboxComponent.hitbox[h].active)
            continue;

        const Transform& hitterTransform = transformComponent.transform[h];
        const Hitbox& hitbox = hitboxComponent.hitbox[h];

        for (int t = 0; t < MAX_ENTITIES; t++) {
            if (h == t)
                continue;
            const Entity& target = entities[t]; 
            if (!target.ShouldUpdate())
                continue;
            if (!target.MatchesKey(hurtKey))
                continue;

            // Handle clanks?

            const Transform& targetTransform = transformComponent.transform[t];
            const Hurtbox& hurtbox = hurtboxComponent.hurtbox[t];
            
            Collision collision = Collision::GetCollision(
                hitterTransform, 
                hitbox, 
                targetTransform, 
                hurtbox 
            );

            if (collision.isColliding) {
                hitList.push_back({h, t, collision});
                DEBUGLOG("hit");
            }
        }
    }

    for (const Hit& hit : hitList) {
        // Add curving hits, spread release, damage, angular momentum, hitlag, etc...
        vec3 direction = normalize(hit.collision.resolution);
        velocityComponent.velocity[hit.target] = 
            direction * hitboxComponent.hitbox[hit.hitter].horizontalKb + 
            Transform::worldUp * hitboxComponent.hitbox[hit.target].verticalKb;
    }
}
