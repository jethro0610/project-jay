#include <glm/gtx/compatibility.hpp>
#include "SeedManager.h"
#include "../Components/MeterComponent.h"
#include "../Components/TransformComponent.h"
#include "../Time.h"
using namespace glm;

// TODO: Track any entity that bubbles onto it

void SeedManager::CreateSeed(glm::ivec3 position) {
    Seed seed {
        position,
        PLAYER_ENTITY,
        Time::GetTime()
    };
    seeds_.Append(seed);
}

void SeedManager::Execute(
    MeterComponent& meterComponent, 
    TransformComponent& transformComponent
) {
    for (int i = 0; i < seeds_.GetCount(); i++) {
        Seed& seed = seeds_[i];
        float timeSinceStart = Time::GetTime() - seed.startTime; 
        timeSinceStart /= 0.3f;
        if (timeSinceStart >= 1.0f) {
            meterComponent.meter[seed.targetEntity] += 1;
            seeds_.Remove(i--);
            continue;
        }
        vec3 initialPosition = seed.initialPosition;
        vec3 targetPosition = transformComponent.renderTransform[PLAYER_ENTITY].position_;
        timeSinceStart = std::powf(timeSinceStart, 3.0f);
        seedPositions_[i] = lerp(initialPosition, targetPosition, timeSinceStart); 
    }
}
