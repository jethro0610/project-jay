#include <glm/gtx/string_cast.hpp>
#include <glm/gtx/compatibility.hpp>
#include "HitSystem.h"
#include "../Entity/Entity.h"
#include "../Entity/EntityKey.h"
#include "../Components/HitboxComponent.h"
#include "../Components/HurtboxComponent.h"
#include "../Components/TransformComponent.h"
#include "../Components/VelocityComponent.h"
#include "../../Logging/Logger.h"
using namespace glm;

const int HITSTUN = 4;
const int HITCOOLDOWN = 16;

constexpr EntityKey hitKey = GetEntityKey<HitboxComponent, TransformComponent>();
constexpr EntityKey hurtKey = GetEntityKey<HurtboxComponent, TransformComponent>();

void HitSystem::Execute(
    EntityList& entities,
    ComponentList& components
) {
    auto& hitboxComponent = components.Get<HitboxComponent>();
    auto& transformComponent = components.Get<TransformComponent>();
    auto& hurtboxComponent = components.Get<HurtboxComponent>();
    auto& velocityComponent = components.Get<VelocityComponent>();
    HitList hitList;

    for (int i = 0; i < MAX_ENTITIES; i++) {
        if (!entities[i].ShouldUpdate(hitKey)) continue;
        int& cooldown = hitboxComponent.cooldown[i];
        if (cooldown > 0)
            cooldown--;
    }

    for (int h = 0; h < MAX_ENTITIES; h++) {
        if (!entities[h].ShouldUpdate(hitKey)) continue;
        if (!hitboxComponent.hitbox[h].active) continue;
        if (hitboxComponent.cooldown[h] > 0) continue;
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
        Hitbox& hitbox = hitboxComponent.hitbox[hit.hitter];
        Hurtbox& hurtbox = hurtboxComponent.hurtbox[hit.target];
        const vec3 normalizeRes = normalize(hit.collision.resolution);

        if (hitbox.useVelocity) {
            // Get the planar velocity of the hitter
            vec3 velocity = velocityComponent.velocity[hit.hitter];
            velocity.y = 0.0f;

            // Normalize the planar velocity
            float magnitude = length(velocity);
            velocity /= magnitude;

            // Apply the direction influence. This is based on the resolution vector
            // as it pushes the object in the direction of its collision
            velocity = lerp(velocity, normalizeRes, hitbox.directionInfluence);

            // Apply the planar velocity and vertical knockback
            velocity = normalize(velocity) * magnitude * hitboxComponent.hitbox[hit.hitter].horizontalKb;
            velocity.y = hitboxComponent.hitbox[hit.hitter].verticalKb;
            velocityComponent.velocity[hit.target] = velocity;
        }
        else {
            velocityComponent.velocity[hit.target] = 
                normalizeRes * hitboxComponent.hitbox[hit.hitter].horizontalKb + 
                Transform::worldUp * hitboxComponent.hitbox[hit.hitter].verticalKb;
        }
        velocityComponent.angularVelocity[hit.target] = quat(
            cross(Transform::worldUp, normalizeRes * length(velocityComponent.velocity[hit.target]) * 0.001f)
        );

        entities[hit.hitter].stunTimer_ = HITSTUN;
        entities[hit.target].stunTimer_ = HITSTUN;
        hitboxComponent.cooldown[hit.hitter] = HITCOOLDOWN;
    }
}

// Add curving hits, sp release, damage, angular momentum, hitlag, etc...
