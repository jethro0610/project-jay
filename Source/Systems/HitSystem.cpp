#include "HitSystem.h"
#include "Seed/SeedManager.h"
#include "Components/GroundTraceComponent.h"
#include "Components/HitboxComponent.h"
#include "Components/HurtboxComponent.h"
#include "Components/TransformComponent.h"
#include "Components/VelocityComponent.h"
#include "Logging/Logger.h"
#include <glm/gtx/compatibility.hpp>
using namespace glm;

const int HURTCOOLDOWN = 16;

constexpr EntityKey hitKey = GetEntityKey<HitboxComponent, TransformComponent>();
constexpr EntityKey hurtKey = GetEntityKey<HurtboxComponent, TransformComponent>();

void HitSystem::Execute(
    EntityList& entities,
    ComponentList& components,
    SeedManager& seedManager
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
        else if (hurtboxComponent.endStunOnGround[i] && groundTraceComponent.onGround[i])
            hurtboxComponent.stun[i] = false;

        if (hurtboxComponent.stunTime[i] <= 0)
            continue;

        if (hurtboxComponent.stunTimer[i] < hurtboxComponent.stunTime[i])
            hurtboxComponent.stunTimer[i]++;
        else
            hurtboxComponent.stun[i] = false;
    }

    for (int h = 0; h < MAX_ENTITIES; h++) {
        if (!entities[h].ShouldUpdate(hitKey)) continue;
        if (hurtboxComponent.stun[h]) 
            continue;

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
                if (!collision.isColliding)
                    continue;

                vec3 planarForward = hitterTransform.GetForwardVector();
                planarForward.y = 0.0f;
                planarForward = normalize(planarForward);

                vec3 vectorToTarget = targetTransform.position - hitterTransform.position;
                vectorToTarget.y = 0.0f;
                vectorToTarget = normalize(vectorToTarget);

                if (dot(planarForward, vectorToTarget) > hitbox.forwardRange)
                    hitList.push_back({h, t, b, collision});
            }
        }
    }

    for (const Hit& hit : hitList) {
        Hitbox& hitbox = hitboxComponent.hitboxes[hit.hitter][hit.hitboxId];
        Hurtbox& hurtbox = hurtboxComponent.hurtbox[hit.target];

        groundTraceComponent.disableStick[hit.target] = true;
        hurtboxComponent.stun[hit.target] = true;
        hurtboxComponent.stunTimer[hit.target] = 0;
        entities[hit.hitter].StartHitlag(hitbox.hitlag, false);
        entities[hit.target].StartHitlag(hitbox.hitlag, true);
        hurtboxComponent.cooldown[hit.target] = HURTCOOLDOWN;
        hurtboxComponent.hurt[hit.target] = true;
        hitboxComponent.hit[hit.hitter] = true;

        if (hurtboxComponent.seedAmount[hit.target] > 0) {
            seedManager.CreateMultipleSeed(
                transformComponent.transform[hit.target].position, 
                hurtboxComponent.seedAmount[hit.target],
                hurtboxComponent.seedRadius[hit.target]
            );
        }

        if (!hurtbox.recieveKnockback)
            continue;

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

        vec3 planarVelocity = velocityComponent.velocity[hit.target];
        planarVelocity.y = 0.0f;
        switch (hurtboxComponent.faceDirection[hit.target]) {
            case HurtboxComponent::FD_Forward:
                transformComponent.transform[hit.target].rotation = quatLookAtRH(normalize(planarVelocity), Transform::worldUp); 
                break;

            case HurtboxComponent::FD_Backward:
                transformComponent.transform[hit.target].rotation = quatLookAtRH(normalize(-planarVelocity), Transform::worldUp); 
                break;

            case HurtboxComponent::FD_None:
                break;
        }
        // transformComponent.transformLastUpdate[hit.target].rotation = transformComponent.transform[hit.target].rotation;
        // transformComponent.renderTransform[hit.target].rotation = transformComponent.transform[hit.target].rotation;


        if (hurtbox.rotate)
            velocityComponent.angularVelocity[hit.target] = quat(
                cross(Transform::worldUp, normalizeRes * length(velocityComponent.velocity[hit.target]) * 0.001f)
            );
    }
}

// Add curving hits, sp release, damage, angular momentum, hitlag, etc...
