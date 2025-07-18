#pragma once
#include "Types/Transform.h"
#include "Rendering/Material.h"
#include  "Debug/DebugName.h"
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>
#include <vector_contig.h>

class Terrain;
const int MAX_PARTICLES = 4096;

struct Particle {
    glm::vec4 initialPosition;
    glm::vec4 position;
    glm::vec4 velocity;
    float rotation;
    float initialScale;
    float scale;
    float time;
};

struct EmitterProperties {
    enum EmissionType {
        ET_Default,
        ET_PlanarDisc,
        ET_Sphere,
    };

    USE_DEBUG_NAME;
    EmitterProperties() {
        emissionType = ET_Default;
        material = {};
        localSpace = false;
        spawnInterval = 0.0f;
        spawnCount = 0.0f;
        lifetime = 0.0f;
        minScale = 0.0f;
        maxScale = 0.0f;
        endScale = 0.0f;
        spawnRadius = 0.0f;
        minVelocity = {};
        maxVelocity = {};
        damping = {};
        acceleration = {};
        startColor = {};
        endColor = {};
        terrainCollision = false;
    }
    EmissionType emissionType;
    Material material;
    bool localSpace;
    float spawnInterval;
    int spawnCount;
    float lifetime;

    float minScale;
    float maxScale;
    float endScale;

    float spawnRadius;

    glm::vec4 minVelocity;
    glm::vec4 maxVelocity;
    glm::vec4 damping;

    glm::vec4 acceleration;

    glm::vec4 startColor;
    glm::vec4 endColor;

    bool terrainCollision;
};

struct ParticleEmitter {
    EmitterProperties properties_;
    bool alive_;
    bool active_;
    bool release_;
    vector_contig<Particle, MAX_PARTICLES> particles_;
    Transform transform_;
    Transform lastTransform_;
    float timer_;
    glm::vec3 velocityOffset_;

    ParticleEmitter() {
        alive_ = false;
        active_ = false;
        release_ = false;
        transform_ = Transform();
        properties_ = {};
        timer_ = 0.0f;
        velocityOffset_ = glm::vec3(0.0f);
    }

    void Update(float deltaTime);
    void Emmit();
    void Reset();

    Terrain* terrain_;
};
