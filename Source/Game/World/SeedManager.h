#pragma once
#include <glm/vec3.hpp>
#include "World.h"
#include "../../Constants/GameConstants.h"
#include "../../Types/FixedVector.h"
#include "../Entity/EntityID.h"
#include "../../Types/Transform.h"
class Entity;
class BubbleComponent;
class MeterComponent;
class TransformComponent;

const float SEED_GRAVITY_SCALE = 24.0f;
const float SEED_EASE_SPEED = 16.0f;
const float SEED_FALL_SPEED = 0.5f;
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
    glm::vec4 positions_[MAX_SEED];

    void CreateSeed(glm::vec3 position, EntityID capturer = NULL_ENTITY, glm::vec3 offset = glm::vec3(0.0f));
    void CreateMultipleSeed(glm::ivec3 position, int amount, int radius = 2);

    void CalculatePositions(
        World& world,
        MeterComponent& meterComponent,
        TransformComponent& transformComponent,
        float interpTime
    );

    void GetCaptures(
        Entity* entities,
        BubbleComponent& bubbleComponent, 
        TransformComponent& transformComponent
    );
};
