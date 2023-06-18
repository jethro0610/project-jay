#include <glm/gtx/compatibility.hpp>
#include "SeedManager.h"
#include "../Components/MeterComponent.h"
#include "../Components/TransformComponent.h"
#include "../Time.h"
#include "../../Constants/TimeConstants.h"
using namespace glm;

// TODO: Track any entity that bubbles onto it
void SeedManager::CreateSeed(glm::vec3 position, glm::vec3 velocity) {
    Seed seed {
        position,
        position,
        velocity,
        NO_ENTITY
    };
    seeds_.Append(seed);
}

void SeedManager::UpdateSeeds(
    World& world,
    MeterComponent& meterComponent, 
    TransformComponent& transformComponent
) {
    for (int i = 0; i < seeds_.GetCount(); i++) {
        Seed& seed = seeds_[i];
        seed.lastPosition = seed.position;

        // TODO: Move GRAVITY_ACCELERATION to game constants
        seed.velocity.y -= 1.0f;
        seed.velocity.y = max(seed.velocity.y, -25.0f);
        seed.position += seed.velocity * TIMESTEP;

        vec3 nearestDown = world.GetNearestInDirection(seed.position, -Transform::worldUp);
        float distFromGround = distance(seed.position, nearestDown);
        if (distFromGround < 0.25f) {
            seed.position = nearestDown + Transform::worldUp * 0.25f;
            seed.velocity.x *= 0.75f;
            seed.velocity.y *= -0.75f;
            seed.velocity.z *= 0.75f;
        }

        // if (timeSinceStart >= 1.0f) {
        //     meterComponent.meter[seed.targetEntity] += 1;
        //     seeds_.Remove(i--);
        //     continue;
        // }
        // vec3 initialPosition = seed.initialPosition;
        // vec3 targetPosition = transformComponent.renderTransform[seed.targetEntity].position_;
        // timeSinceStart = std::powf(timeSinceStart, 3.0f);
        // seedPositions_[i] = lerp(initialPosition, targetPosition, timeSinceStart); 
    }
}

void SeedManager::UpdateSeedPositions(float interpTime) {
    for (int i = 0; i < seeds_.GetCount(); i++) {
        Seed& seed = seeds_[i];
        seedPositions_[i] = lerp(seed.lastPosition, seed.position, interpTime / TIMESTEP);
    }
}

void SeedManager::CreateMultipleSeed(glm::ivec3 position, uint32_t amount, float radius) {
    for (int i = 0; i < amount; i++) {
        CreateSeed(position);
    }
}
