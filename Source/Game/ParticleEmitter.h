#pragma once
#include <array>
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>
#include <vector_contig.h>
#include "../Types/Transform.h"

const int MAX_PARTICLES = 64;

struct Particle {
    glm::vec3 position;
    glm::vec3 velocity;
    float rotation;
    float scale;
    float time;
    glm::vec3 padding;
};

struct ParticleEmitter {
    vector_contig<Particle, MAX_PARTICLES> particles;
    float timer;
    
    void Update(Transform& origin, float deltaTime);
};
