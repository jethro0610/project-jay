#include <glm/gtx/compatibility.hpp>
#include "SeedManager.h"
#include "../Components/MeterComponent.h"
#include "../Components/TransformComponent.h"
#include "../Time.h"
#include "../../Constants/TimeConstants.h"
using namespace glm;

// TODO: Track any entity that bubbles onto it
void SeedManager::CreateSeed(glm::vec3 position, glm::vec3 offset) {
    Seed seed {
        position,
        offset,
        sqrtf(offset.y / SEED_GRAVITY_SCALE),
        PLAYER_ENTITY,
        Time::GetTime(),
        Time::GetTime()
    };
    seeds_.Append(seed);
}

void SeedManager::UpdateSeeds(
    World& world,
    MeterComponent& meterComponent, 
    TransformComponent& transformComponent
) {
    for (int i = 0; i < seeds_.GetCount(); i++) {
        // Seed& seed = seeds_[i];
        // seed.lastPosition = seed.position;
        //
        // // TODO: Move GRAVITY_ACCELERATION to game constants
        // seed.velocity.y -= 1.0f;
        // seed.velocity.y = max(seed.velocity.y, -25.0f);
        // seed.position += seed.velocity * TIMESTEP;
        //
        // vec3 nearestDown = world.GetNearestInDirection(seed.position, -Transform::worldUp);
        // float distFromGround = distance(seed.position, nearestDown);
        // if (distFromGround < 0.25f) {
        //     seed.position = nearestDown + Transform::worldUp * 0.25f;
        //     seed.velocity.x *= 0.75f;
        //     seed.velocity.y *= -0.75f;
        //     seed.velocity.z *= 0.75f;
        // }
    }
}

void SeedManager::UpdateSeedPositions(
    World& world,
    MeterComponent& meterComponent,
    TransformComponent& transformComponent,
    float interpTime
) {
    for (int i = 0; i < seeds_.GetCount(); i++) {
        Seed& seed = seeds_[i];

        float timeSinceStart = Time::GetTime() - seed.startTime;
        vec3 physicsOffset;

        // Gravity based y position
        // physicsOffset.y = -SEED_GRAVITY_SCALE * powf(timeSinceStart - seed.gravityOffset, 2.0f) + seed.offset.y;
        // physicsOffset.y = max(physicsOffset.y, 0.0f);

        float logisitic = 1 + expf(-SEED_EASE_SPEED * timeSinceStart);
        physicsOffset.x = seed.offset.x * 2 / logisitic - seed.offset.x;
        physicsOffset.y = seed.offset.y * 2 / logisitic - seed.offset.y;
        physicsOffset.z = seed.offset.z * 2 / logisitic - seed.offset.z;
        seedPositions_[i] = seed.position + physicsOffset;

        // TODO: When using pure heightmaps, check distance and set height to that

        if (seed.targetEntity == NO_ENTITY)
            continue;

        float timeSinceCapture = Time::GetTime() - seed.captureTime;
        timeSinceCapture *= 3.0f;
        if (timeSinceCapture >= 1.0f) {
            meterComponent.meter[seed.targetEntity] += 1;
            seeds_.Remove(i--);
            continue;
        }
        vec3 initialPosition = seed.position;
        vec3 targetPosition = transformComponent.renderTransform[seed.targetEntity].position_;
        timeSinceCapture = std::powf(timeSinceCapture, 3.0f);
        seedPositions_[i] = lerp(seedPositions_[i], targetPosition, timeSinceCapture); 
    }
}

void SeedManager::CreateMultipleSeed(glm::ivec3 position, uint32_t amount, float radius) {
    for (int i = 0; i < amount; i++) {
        CreateSeed(position);
    }
}
