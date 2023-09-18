#include "ParticleEmitter.h"
#include "../Helpers/Random.h"
using namespace glm;

void ParticleEmitter::Update(Transform& origin, float deltaTime) {
    timer += deltaTime;
    if (timer >= 0.5f) {
        Particle particle;
        particle.position = origin.position;
        particle.scale = 1.0f;
        vec2 randomVec = RandomVector2D(2.0f);
        particle.velocity.x = 0.0f;
        particle.velocity.z = 0.0f;
        particle.velocity.y = 10.0f;
        particle.rotation = 0.0f;
        particle.time = 0.0f;

        particles.push_back(particle); 
        timer = 0.0f;
    }

    for (Particle& particle : particles) {
        particle.time += deltaTime;
        particle.velocity.y -= 1.0f * deltaTime;
        particle.position += particle.velocity * deltaTime;
    }
}
