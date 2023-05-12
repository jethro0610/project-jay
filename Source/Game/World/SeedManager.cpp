#include <glm/gtx/compatibility.hpp>
#include "SeedManager.h"
#include "../Components/TransformComponent.h"
#include "../Time.h"
using namespace glm;

void SeedManager::CreateSeed(glm::ivec3 position) {
    Seed seed {
        position,
        -1,
        Time::GetTime()
    };
    seeds_.Append(seed);
}


void SeedManager::Execute(TransformComponent& transformComponent) {
    for (int i = 0; i < seeds_.GetCount(); i++) {
        Seed& seed = seeds_[i];
        float timeSinceStart = Time::GetTime() - seed.startTime; 
        timeSinceStart /= 0.3f;
        if (timeSinceStart >= 1.0f) {
            seeds_.Remove(i--);
            continue;
        }
        vec3 initialPosition = seed.initialPosition;
        vec3 targetPosition = transformComponent.renderTransform[PLAYER_ENTITY].position_;
        timeSinceStart = std::powf(timeSinceStart, 3.0f);
        seedPositions_[i] = lerp(initialPosition, targetPosition, timeSinceStart); 
    }
}
