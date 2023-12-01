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
#include "../../Logging/ScreenText.h"
using namespace glm;

void SeedManager::RemoveOldest() {
    int oldest = -1;
    float age = INFINITY;

    for (int i = 0; i < seeds_.size(); i++) {
        if (seeds_[i].startTime < age) {
            oldest = i;
            age = seeds_[i].startTime;
        }
    }

    seeds_.remove(oldest);
}

// TODO: Track any entity that bubbles onto it
void SeedManager::CreateSeed(glm::vec3 position, EntityID capturer, glm::vec3 offset) {
    if (seeds_.size() >= MAX_SEED)
        RemoveOldest();

    Seed seed {
        vec4(position, 0.0f),
        offset,
        sqrtf(offset.y / SEED_GRAVITY_SCALE),
        capturer,
        GlobalTime::GetTime(),
        GlobalTime::GetTime() + MIN_REMOVE_TIME,
        RandomVector(
            vec3(0.0f, 0.0f, 0.0f),
            vec3(60.0f, 60.0f, 60.0f)
        ),
        RandomFloatRange(0.25f, 1.0f),
        RandomFloatRange(0.0f, 2.0f)
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
        float clampStartTime = min(timeSinceStart, 1.0f);
        float easeStartTime = 1.0f - (1.0f - clampStartTime) * (1.0f - clampStartTime);
        vec3 physicsOffset;

        float logisitic = 1 + expf(-SEED_EASE_SPEED * timeSinceStart);
        physicsOffset.x = seed.offset.x * 2 / logisitic - seed.offset.x;
        physicsOffset.y = seed.offset.y * 2 / logisitic - seed.offset.y - timeSinceStart * SEED_FALL_SPEED;
        physicsOffset.z = seed.offset.z * 2 / logisitic - seed.offset.z;
        positions_[i] = vec4(seed.position + physicsOffset, 0.0f);

        float height = world.GetHeight(vec2(positions_[i].x, positions_[i].z), NA_Low) + 4.0f * easeStartTime;
        if (positions_[i].y < height)
            positions_[i].y = height;
        
        positions_[i].x += sin(GlobalTime::GetTime() + seed.jitterOffset.x) * 1.0f * easeStartTime;
        positions_[i].y += (sin(GlobalTime::GetTime() + seed.jitterOffset.y) * 4.0f * seed.jitterRange + seed.heightOffset) * easeStartTime;
        positions_[i].z += sin(GlobalTime::GetTime() + seed.jitterOffset.z) * 1.0f * easeStartTime;

        float timeSinceCapture = GlobalTime::GetTime() - seed.captureTime;
        if (seed.targetEntity == NULL_ENTITY || timeSinceCapture < 0.0f)
            continue;

        timeSinceCapture *= 3.0f;
        if (timeSinceCapture >= 1.0f) {
            meterComponent.meter[seed.targetEntity] += 1;
            meterComponent.meter[seed.targetEntity] = min(meterComponent.meter[seed.targetEntity], meterComponent.maxMeter[seed.targetEntity]);
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
