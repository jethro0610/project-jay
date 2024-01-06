#pragma once
#include "Terrain/Terrain.h"
#include "Entity/EntityIDS.h"
#include <array>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <vector_contig.h>

struct Seed {
    glm::vec3 position;
    glm::vec3 offset;
    float gravityOffset;
    EntityIDS targetEntity;
    float startTime;
    float captureTime;
    glm::vec3 jitterOffset;
    float jitterRange;
    float heightOffset;
};

class SeedManager {
public:
    static constexpr int MAX_SEED = 4096;
    static constexpr float SEED_GRAVITY_SCALE = 24.0f;
    static constexpr float SEED_EASE_SPEED = 16.0f;
    static constexpr float SEED_FALL_SPEED = 1.0f;
    static constexpr float MIN_CAPTURE_TIME = 0.75f;
    static constexpr float MIN_REMOVE_TIME = 0.15f;

    vector_contig<Seed, MAX_SEED> seeds_;
    std::array<glm::vec4, MAX_SEED> positions_;

    void RemoveOldest();
    void CreateSeed(glm::vec3 position, EntityIDS capturer = NULL_ENTITY, glm::vec3 offset = glm::vec3(0.0f));
    void CreateMultipleSeed(glm::vec3 position, int amount, float radius = 2.0f, EntityIDS capturer = NULL_ENTITY);

    void CalculatePositions(
        Terrain& terrain,
        float interpTime
    );

    void GetCaptures(
    );

    void Reset();
};
