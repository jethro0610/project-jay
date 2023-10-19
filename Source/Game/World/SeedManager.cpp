#include <glm/gtx/compatibility.hpp>
#include "SeedManager.h"
#include "../Entity/Entity.h"
#include "../Entity/EntityKey.h"
#include "../Components/MeterComponent.h"
#include "../Components/SeedGatherComponent.h"
#include "../Components/TransformComponent.h"
#include "../Time.h"
#include "../../Constants/TimeConstants.h"
#include "../../Helpers/Random.h"
using namespace glm;

// TODO: Track any entity that bubbles onto it
void SeedManager::CreateSeed(glm::vec3 position, EntityID capturer, glm::vec3 offset) {
    if (seeds_.size() >= MAX_SEED)
        seeds_.remove(0);

    Seed seed {
        vec4(position, 0.0f),
        offset,
        sqrtf(offset.y / SEED_GRAVITY_SCALE),
        capturer,
        GlobalTime::GetTime(),
        GlobalTime::GetTime() + MIN_REMOVE_TIME
    };
    seeds_.push_back(seed);
}

void SeedManager::CreateMultipleSeed(glm::ivec3 position, int amount, int radius, EntityID capturer) {
    for (int i = 0; i < amount; i++) {
        CreateSeed(position, capturer, RandomVector(radius));
    }
}

void SeedManager::CalculatePositions(
    ComponentList& components,
    World& world,
    float interpTime
) {
    auto& meterComponent = components.Get<MeterComponent>();
    auto& transformComponent = components.Get<TransformComponent>();

    for (int i = 0; i < seeds_.size(); i++) {
        Seed& seed = seeds_[i];

        float timeSinceStart = GlobalTime::GetTime() - seed.startTime;
        vec3 physicsOffset;

        float logisitic = 1 + expf(-SEED_EASE_SPEED * timeSinceStart);
        physicsOffset.x = seed.offset.x * 2 / logisitic - seed.offset.x;
        physicsOffset.y = seed.offset.y * 2 / logisitic - seed.offset.y - timeSinceStart * SEED_FALL_SPEED;
        physicsOffset.z = seed.offset.z * 2 / logisitic - seed.offset.z;
        positions_[i] = vec4(seed.position + physicsOffset, 0.0f);

        float height = world.GetHeightFast(vec2(positions_[i].x, positions_[i].z))  + 0.25f;
        if (positions_[i].y < height)
            positions_[i].y = height;

        float timeSinceCapture = GlobalTime::GetTime() - seed.captureTime;
        if (seed.targetEntity == NULL_ENTITY || timeSinceCapture < 0.0f)
            continue;

        timeSinceCapture *= 3.0f;
        if (timeSinceCapture >= 1.0f) {
            meterComponent.meter[seed.targetEntity] += 1;
            seeds_.remove(i--);
            continue;
        }
        vec3 initialPosition = seed.position;
        vec4 targetPosition = vec4(transformComponent.renderTransform[seed.targetEntity].position, 0.0f);
        timeSinceCapture = std::pow(timeSinceCapture, 3.0f);
        positions_[i] = lerp(positions_[i], targetPosition, timeSinceCapture); 
    }
}

constexpr EntityKey key = GetEntityKey<SeedGatherComponent, TransformComponent>();
void SeedManager::GetCaptures(
    EntityList& entities,
    ComponentList& components
) {
    auto& seedGatherComponent = components.Get<SeedGatherComponent>();
    auto& transformComponent = components.Get<TransformComponent>();

    float time = GlobalTime::GetTime();
    for (int i = 0; i < MAX_ENTITIES; i++) {
        if (!entities[i].ShouldUpdate(key)) continue;
        if (!seedGatherComponent.active[i]) continue;

        for (int j = 0; j < seeds_.size(); j++) {
            if (seeds_[j].targetEntity != NULL_ENTITY)
                continue;
            if (time - seeds_[j].startTime < MIN_CAPTURE_TIME)
                continue;

            if (distance(vec4(transformComponent.transform[i].position, 0.0f), positions_[j]) < seedGatherComponent.radius[i]) {
                seeds_[j].targetEntity = i;
                seeds_[j].captureTime = time;
            }
        }
    }
}
