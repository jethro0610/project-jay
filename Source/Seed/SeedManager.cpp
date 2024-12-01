#include "SeedManager.h"
#include "Time/Time.h"
#include "Helpers/Random.h"
#include "Logging/Logger.h"
#include "Entity/Entity.h"
#include "Entity/EntityList.h"
#include <glm/gtx/compatibility.hpp>
using namespace glm;

constexpr float SEED_GRAVITY_SCALE = 24.0f;
constexpr float SEED_EASE_SPEED = 16.0f;
constexpr float SEED_FALL_SPEED = 1.0f;
constexpr float MIN_CAPTURE_TIME = 0.15f;
constexpr float CAPTURE_LENGTH = 0.75f;

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
void SeedManager::CreateSeed(glm::vec3 position, Entity* capturer, glm::vec3 offset) {
    if (seeds_.size() >= MAX_SEED)
        RemoveOldest();

    Seed seed {
        vec4(position, 0.0f),
        offset,
        sqrtf(offset.y / SEED_GRAVITY_SCALE),
        capturer,
        GlobalTime::GetTime(),
        GlobalTime::GetTime(),
        RandomFloatRange(-0.5f, 0.5f),
        RandomVector(
            vec3(0.0f, 0.0f, 0.0f),
            vec3(60.0f, 60.0f, 60.0f)
        ),
        RandomFloatRange(0.25f, 1.0f),
        RandomFloatRange(0.0f, 2.0f)
    };
    seeds_.push_back(seed);
}

void SeedManager::CreateMultipleSeed(glm::vec3 position, int amount, float radius, Entity* capturer, glm::vec3 offset) {
    for (int i = 0; i < amount; i++) {
        CreateSeed(position, capturer, RandomVector(radius) + offset);
    }
}

void SeedManager::CalculatePositions(
    Terrain& terrain
) {
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

        float height = terrain.GetHeight(vec2(positions_[i].x, positions_[i].z), TA_Low) + 2.0f * easeStartTime;
        if (positions_[i].y < height)
            positions_[i].y = height;
        
        positions_[i].x += sin(GlobalTime::GetTime() + seed.jitterOffset.x) * 1.0f * easeStartTime;
        positions_[i].y += (sin(GlobalTime::GetTime() + seed.jitterOffset.y) * 4.0f * seed.jitterRange + seed.heightOffset) * easeStartTime;
        positions_[i].z += sin(GlobalTime::GetTime() + seed.jitterOffset.z) * 1.0f * easeStartTime;

        float timeSinceCapture = GlobalTime::GetTime() - seed.captureTime;
        if (seed.targetEntity == nullptr || timeSinceCapture < 0.0f)
            continue;

        float t = timeSinceCapture / (CAPTURE_LENGTH + seed.captureTimeOffset);
        if (t >= 1.0f) {
            seed.targetEntity->DoCaptureSeed();
            seeds_.remove(i--);
            continue;
        }
        vec3 initialPosition = seed.position;
        vec4 targetPosition = vec4(seed.targetEntity->transform_.position, 0.0f);
        t = std::pow(t, 2.0f);
        positions_[i] = lerp(positions_[i], targetPosition, t); 

        height = terrain.GetHeight(vec2(positions_[i].x, positions_[i].z), TA_Low) + 2.0f;
        if (positions_[i].y < height)
            positions_[i].y = height;
    }
}

void SeedManager::GetCaptures(
    EntityList& entities
) {
    float time = GlobalTime::GetTime();
    for (int i = 0; i < 128; i++) {
        if (!entities[i].alive_) continue;
        if (!entities[i].GetFlag(Entity::EF_CaptureSeed)) continue;

        for (int j = 0; j < seeds_.size(); j++) {
            if (seeds_[j].targetEntity != nullptr)
                continue;
            if (time - seeds_[j].startTime < MIN_CAPTURE_TIME)
                continue;

            if (distance(vec4(entities[i].transform_.position, 0.0f), positions_[j]) < 32.0f) {
                seeds_[j].targetEntity = &entities[i];
                seeds_[j].captureTime = time;
            }
        }
    }
}

void SeedManager::Reset() {
    seeds_.clear();
}
