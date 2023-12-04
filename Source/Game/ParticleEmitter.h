#pragma once
#include <array>
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>
#include <vector_contig.h>
#include "../Types/Transform.h"
#include "../Rendering/Material.h"

const int MAX_PARTICLES = 2048;

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
    Material* material;
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

    glm::vec4 acceleration;

    glm::vec4 startColor;
    glm::vec4 endColor;
};

struct ParticleEmitter {
    bool alive_;
    bool active_;
    bool release_;
    vector_contig<Particle, MAX_PARTICLES> particles_;
    Transform transform_;
    Transform lastTransform_;
    EmitterProperties* properties_;
    float timer_;
    glm::vec3 velocityOffset_;

    ParticleEmitter() {
        alive_ = false;
        active_ = false;
        release_ = false;
        transform_ = Transform();
        properties_ = nullptr;
        timer_ = 0.0f;
        velocityOffset_ = glm::vec3(0.0f);
    }

    void Update(float deltaTime);
    void Emmit();
};
