#pragma once
#include <array>
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>
#include <vector_contig.h>
#include "../Types/Transform.h"

const int MAX_PARTICLES = 512;

struct Particle {
    glm::vec3 position;
    glm::vec3 velocity;
    float rotation;
    float initialScale;
    float scale;
    float time;
    glm::vec2 padding;
};

struct EmitterProperties {
    float spawnRate;
    float lifetime;

    float minScale;
    float maxScale;
    float endScale;

    float spawnRadius;

    glm::vec3 minVelocity;
    glm::vec3 maxVelocity;

    glm::vec3 acceleration;
};

class ParticleEmitter {
    vector_contig<Particle, MAX_PARTICLES> particles_;
    Transform transform_;
    EmitterProperties properties_;
    float timer_;

public:
    ParticleEmitter() {
        transform_ = Transform();
        timer_ = 0.0f;
    }

    void Update(float deltaTime);
    void SetTransform(const Transform& transform) { transform_ = transform; }
    void SetPosition(const glm::vec3& position) { transform_.position = position; }
    void SetProperties(const EmitterProperties& properties) { properties_ = properties; }
    void* GetRenderData() { return particles_.data(); }
    int GetParticleCount() const { return particles_.size(); }
};
