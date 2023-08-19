#pragma once
#include <glm/vec3.hpp>
#include "World.h"
#include "../../Constants/GameConstants.h"
#include "../../Types/FixedVector.h"
#include "../Entity/EntityID.h"
#include "../Entity/EntityLimits.h"
#include "../../Types/Transform.h"
class Entity;
class ColliderComponent;
class MeterComponent;
class TransformComponent;

const float SEED_GRAVITY_SCALE = 24.0f;
const float SEED_EASE_SPEED = 16.0f;
const float SEED_FALL_SPEED = 1.0f;
const float MIN_CAPTURE_TIME = 0.75f;
const float MIN_REMOVE_TIME = 0.15f;

struct Seed {
    glm::vec3 position;
    glm::vec3 offset;
    float gravityOffset;
    EntityID targetEntity;
    float startTime;
    float captureTime;
};

class SeedManager {
public:
    FixedVector<Seed, MAX_SEED> seeds_;
    std::array<glm::vec4, MAX_SEED> positions_;

    void CreateSeed(glm::vec3 position, EntityID capturer = NULL_ENTITY, glm::vec3 offset = glm::vec3(0.0f));
    void CreateMultipleSeed(glm::ivec3 position, int amount, int radius = 2, EntityID capturer = NULL_ENTITY);

    void CalculatePositions(
        World& world,
        MeterComponent& meterComponent,
        TransformComponent& transformComponent,
        float interpTime
    );

    void GetCaptures(
        std::array<Entity, MAX_ENTITIES>& entities,
        ColliderComponent& colliderComponent, 
        TransformComponent& transformComponent
    );
};
