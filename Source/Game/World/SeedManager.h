#pragma once
#include <glm/vec3.hpp>
#include "World.h"
#include "../../Constants/GameConstants.h"
#include "../../Types/FixedVector.h"
#include "../../Types/EntityTypes.h"
#include "../../Types/Transform.h"
class MeterComponent;
class TransformComponent;

const float SEED_GRAVITY_SCALE = 24.0f;
const float SEED_EASE_SPEED = 16.0f;

struct Seed {
    glm::vec3 position;
    glm::vec3 offset;
    float gravityOffset;
    EntityIDNullable targetEntity;
    float startTime;
    float captureTime;
};

class SeedManager {
public:
    FixedVector<Seed, MAX_SEED> seeds_;
    glm::vec3 seedPositions_[MAX_SEED];

    void CreateSeed(glm::vec3 position, glm::vec3 velocity = glm::vec3(0.0f));
    void CreateMultipleSeed(glm::ivec3 position, uint32_t amount, float radius = 2.0f);

    void Execute(
        World& world,
        MeterComponent& meterComponent,
        TransformComponent& transformComponent,
        float interpTime
    );
};
