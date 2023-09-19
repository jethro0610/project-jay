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

struct ParticleEmitter {
    vector_contig<Particle, MAX_PARTICLES> particles;
    Transform transform;

    float spawnRate;
    float lifetime;
    float timer;

    float minScale;
    float maxScale;
    float endScale;

    float spawnRadius;

    glm::vec3 velocityMin;
    glm::vec3 velocityMax;

    glm::vec3 acceleration;
    
    void Update(float deltaTime);
};
