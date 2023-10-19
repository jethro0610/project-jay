#pragma once
#include <array>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <vector_contig.h>
#include "World.h"
#include "../../Constants/GameConstants.h"
#include "../Entity/EntityID.h"
#include "../Entity/EntityList.h"
#include "../Components/ComponentList.h"
#include "../../Types/Transform.h"

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
    vector_contig<Seed, MAX_SEED> seeds_;
    std::array<glm::vec4, MAX_SEED> positions_;

    void RemoveOldest();
    void CreateSeed(glm::vec3 position, EntityID capturer = NULL_ENTITY, glm::vec3 offset = glm::vec3(0.0f));
    void CreateMultipleSeed(glm::ivec3 position, int amount, int radius = 2, EntityID capturer = NULL_ENTITY);

    void CalculatePositions(
        ComponentList& components,
        World& world,
        float interpTime
    );

    void GetCaptures(
        EntityList& entities,
        ComponentList& components
    );
};
