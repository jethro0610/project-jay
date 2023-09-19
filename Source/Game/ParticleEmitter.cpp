#include "ParticleEmitter.h"
#include "../Helpers/Random.h"
using namespace glm;

void ParticleEmitter::Update(float deltaTime) {
    timer += deltaTime;
    if (timer >= spawnRate) {
        Particle particle;
        particle.position = transform.position + RandomVector(spawnRadius);
        particle.initialScale = RandomFloatRange(minScale, maxScale);
        particle.scale = particle.initialScale;
        particle.velocity = RandomVector(velocityMin, velocityMax);
        particle.rotation = 0.0f;
        particle.time = 0.0f;

        particles.push_back(particle); 
        timer = 0.0f;
    }

    for (int i = 0; i < particles.size(); i++) {
        Particle& particle = particles[i];
        if (particle.time > lifetime)
            particles.remove(i--);

        particle.time += deltaTime;
        particle.velocity += acceleration * deltaTime;
        particle.position += particle.velocity * deltaTime;
        particle.scale = std::lerp(particle.initialScale, endScale, particle.time / lifetime);
    }
}
