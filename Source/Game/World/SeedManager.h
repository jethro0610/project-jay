#pragma once
#include <glm/vec3.hpp>
#include "World.h"
#include "../../Constants/GameConstants.h"
#include "../../Types/FixedVector.h"
#include "../../Types/EntityTypes.h"
#include "../../Types/Transform.h"
class MeterComponent;
class TransformComponent;

struct Seed {
    glm::vec3 position;
    glm::vec3 lastPosition;
    glm::vec3 velocity;
    EntityIDNullable targetEntity;
};

class SeedManager {
public:
    FixedVector<Seed, MAX_SEED> seeds_;
    glm::vec3 seedPositions_[MAX_SEED];

    void CreateSeed(glm::vec3 position, glm::vec3 velocity = glm::vec3(0.0f));
    void CreateMultipleSeed(glm::ivec3 position, uint32_t amount, float radius = 2.0f);

    void UpdateSeeds(
        World& world,
        MeterComponent& meterComponent, 
        TransformComponent& transformComponent
    );

    void UpdateSeedPositions(float interpTime);
};
