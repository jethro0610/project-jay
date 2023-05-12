#pragma once
#include <glm/vec3.hpp>
#include "../../Constants/GameConstants.h"
#include "../../Types/FixedVector.h"
#include "../../Types/EntityTypes.h"
class TransformComponent;

struct Seed {
    glm::vec3 initialPosition;
    EntityIDNullable targetEntity;
    float startTime;
};

class SeedManager {
public:
    FixedVector<Seed, MAX_SEED> seeds_;
    glm::vec3 seedPositions_[MAX_SEED];

    void CreateSeed(glm::ivec3 position);
    void Execute(TransformComponent& transformComponent);
};
