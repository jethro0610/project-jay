#include "HitSystem.h"
#include "Components/GroundTraceComponent.h"
#include "Components/HitboxComponent.h"
#include "Components/HurtboxComponent.h"
#include "Components/TransformComponent.h"
#include "Components/VelocityComponent.h"
#include <glm/gtx/compatibility.hpp>
using namespace glm;

const int HURTCOOLDOWN = 16;

constexpr EntityKey hitKey = GetEntityKey<HitboxComponent, TransformComponent>();
constexpr EntityKey hurtKey = GetEntityKey<HurtboxComponent, TransformComponent>();

void HitSystem::Execute(
    EntityList& entities,
    ComponentList& components
) {
    auto& groundTraceComponent = components.Get<GroundTraceComponent>();
    auto& hitboxComponent = components.Get<HitboxComponent>();
    auto& hurtboxComponent = components.Get<HurtboxComponent>();
    auto& transformComponent = components.Get<TransformComponent>();
    auto& velocityComponent = components.Get<VelocityComponent>();
    HitList hitList;

    for (int i = 0; i < MAX_ENTITIES; i++) {
        if (!entities[i].ShouldUpdate(hurtKey)) continue;
        hurtboxComponent.hurt[i] = false;
        int& cooldown = hurtboxComponent.cooldown[i];
        if (cooldown > 0)
            cooldown--;
    }

    for (int h = 0; h < MAX_ENTITIES; h++) {
        if (!entities[h].ShouldUpdate(hitKey)) continue;
        for (int b = 0; b < hitboxComponent.hitboxes[h].size(); b++) {
            if (!hitboxComponent.hitboxes[h][b].active) continue;
            const Transform& hitterTransform = transformComponent.transform[h];
            const Hitbox& hitbox = hitboxComponent.hitboxes[h][b];
            hitboxComponent.hit[h] = false;

            for (int t = 0; t < MAX_ENTITIES; t++) {
                if (!entities[t].ShouldUpdate(hurtKey)) continue;
                if (h == t) continue;
                if (hurtboxComponent.cooldown[t] > 0) continue;

                const Transform& targetTransform = transformComponent.transform[t];
                const Hurtbox& hurtbox = hurtboxComponent.hurtbox[t];
                
                Collision collision = Collision::GetCollision(
                    hitterTransform, 
                    hitbox, 
                    targetTransform, 
                    hurtbox 
                );
                if (collision.isColliding)
                    hitList.push_back({h, t, b, collision});
            }
        }
    }

    for (const Hit& hit : hitList) {
        Hitbox& hitbox = hitboxComponent.hitboxes[hit.hitter][hit.hitboxId];
        Hurtbox& hurtbox = hurtboxComponent.hurtbox[hit.target];
        
        if (!hurtbox.recieveKnockback)
            continue;

        groundTraceComponent.disableStick[hit.target] = true;

        const vec3 normalizeRes = normalize(hit.collision.resolution);
        if (hitbox.useVelocity) {
            // Get the planar velocity of the hitter
            vec3 velocity = velocityComponent.velocity[hit.hitter];
            velocity.y = 0.0f;

            // Normalize the planar velocity
            float magnitude = length(velocity);
            velocity /= magnitude;

            // Esnure the object goes the minimum knockback 
            magnitude = max(magnitude, hitbox.minHorizontalKb);

            // Apply the direction influence. This is based on the resolution vector
            // as it pushes the object in the direction of its collision
            velocity = lerp(velocity, normalizeRes, hitbox.directionInfluence);

            // Apply the planar velocity and vertical knockback
            velocity = normalize(velocity) * magnitude * hitbox.horizontalKb;
            velocity.y = hitbox.verticalKb;
            velocityComponent.velocity[hit.target] = velocity;
        }
        else {
            velocityComponent.velocity[hit.target] = 
                normalizeRes * hitbox.horizontalKb + 
                Transform::worldUp * hitbox.verticalKb;
        }

        if (hurtbox.rotate)
            velocityComponent.angularVelocity[hit.target] = quat(
                cross(Transform::worldUp, normalizeRes * length(velocityComponent.velocity[hit.target]) * 0.001f)
            );

        entities[hit.hitter].stunTimer_ = hitbox.hitlag;
        entities[hit.target].stunTimer_ = hitbox.hitlag;
        hurtboxComponent.cooldown[hit.target] = HURTCOOLDOWN;
        hurtboxComponent.hurt[hit.target] = true;
        hitboxComponent.hit[hit.hitter] = true;
    }
}

// Add curving hits, sp release, damage, angular momentum, hitlag, etc...
