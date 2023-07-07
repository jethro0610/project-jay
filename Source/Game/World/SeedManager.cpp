#include <glm/gtx/compatibility.hpp>
#include "SeedManager.h"
#include "../Entity/Entity.h"
#include "../Entity/EntityKey.h"
#include "../Components/ColliderComponent.h"
#include "../Components/MeterComponent.h"
#include "../Components/TransformComponent.h"
#include "../Time.h"
#include "../../Constants/TimeConstants.h"
using namespace glm;

// TODO: Track any entity that bubbles onto it
void SeedManager::CreateSeed(glm::vec3 position, EntityID capturer, glm::vec3 offset) {
    Seed seed {
        vec4(position, 0.0f),
        offset,
        sqrtf(offset.y / SEED_GRAVITY_SCALE),
        capturer,
        GlobalTime::GetTime(),
        GlobalTime::GetTime() + MIN_REMOVE_TIME
    };
    seeds_.Append(seed);
}

void SeedManager::CreateMultipleSeed(glm::ivec3 position, int amount, int radius) {
    for (int i = 0; i < amount; i++) {
        vec3 offset = vec3(
            (rand() % (radius * 100)) * 0.01f - radius / 2.0f,
            (rand() % (radius * 100)) * 0.01f - radius / 2.0f,
            (rand() % (radius * 100)) * 0.01f - radius / 2.0f
        );
        CreateSeed(position, NULL_ENTITY, offset);
    }
}

void SeedManager::CalculatePositions(
    World& world,
    MeterComponent& meterComponent,
    TransformComponent& transformComponent,
    float interpTime
) {
    for (int i = 0; i < seeds_.GetCount(); i++) {
        Seed& seed = seeds_[i];

        float timeSinceStart = GlobalTime::GetTime() - seed.startTime;
        vec3 physicsOffset;

        float logisitic = 1 + expf(-SEED_EASE_SPEED * timeSinceStart);
        physicsOffset.x = seed.offset.x * 2 / logisitic - seed.offset.x;
        physicsOffset.y = seed.offset.y * 2 / logisitic - seed.offset.y - timeSinceStart * SEED_FALL_SPEED;
        physicsOffset.z = seed.offset.z * 2 / logisitic - seed.offset.z;
        positions_[i] = vec4(seed.position + physicsOffset, 0.0f);

        // TODO: When using pure heightmaps, check distance and set height to that

        float timeSinceCapture = GlobalTime::GetTime() - seed.captureTime;
        if (seed.targetEntity == NULL_ENTITY || timeSinceCapture < 0.0f)
            continue;

        timeSinceCapture *= 3.0f;
        if (timeSinceCapture >= 1.0f) {
            meterComponent.meter[seed.targetEntity] += 1;
            seeds_.Remove(i--);
            continue;
        }
        vec3 initialPosition = seed.position;
        vec4 targetPosition = vec4(transformComponent.renderTransform[seed.targetEntity].position, 0.0f);
        timeSinceCapture = std::pow(timeSinceCapture, 3.0f);
        positions_[i] = lerp(positions_[i], targetPosition, timeSinceCapture); 
    }
}

constexpr EntityKey key = GetEntityKey<ColliderComponent, TransformComponent>();

void SeedManager::GetCaptures(
    Entity* entities,
    ColliderComponent& colliderComponent, 
    TransformComponent& transformComponent
) {
    float time = GlobalTime::GetTime();
    for (int i = 0; i < MAX_ENTITIES; i++) {
        const Entity& entity = entities[i];
        if (!entity.alive_)
            continue;
        if (!entity.MatchesKey(key))
            continue;
        if (!colliderComponent.properties[i].test(ColliderProperties::CaptureSeed))
            continue;

        for (int j = 0; j < seeds_.GetCount(); j++) {
            if (seeds_[j].targetEntity != NULL_ENTITY)
                continue;
            if (time - seeds_[j].startTime < MIN_CAPTURE_TIME)
                continue;

            if (distance(vec4(transformComponent.transform[i].position, 0.0f), positions_[j]) < colliderComponent.radius1[i]) {
                seeds_[j].targetEntity = i;
                seeds_[j].captureTime = time;
            }
        }
    }
}
