#include "ParticleEmitter.h"
#include "../Helpers/Random.h"
#include "../Helpers/Assert.h"
using namespace glm;

void ParticleEmitter::Update(float deltaTime) {
    ASSERT(alive_ == true, "Using dead particle emitter");
    for (int i = 0; i < particles_.size(); i++) {
        Particle& particle = particles_[i];
        if (particle.time > 1.0f)
            particles_.remove(i--);

        particle.time += deltaTime / properties_->lifetime;
        particle.velocity += properties_->acceleration * deltaTime;
        particle.position += particle.velocity * deltaTime;
        particle.scale = std::lerp(particle.initialScale, properties_->endScale, particle.time);
    }

    if (!active_)
        return;

    timer_ += deltaTime;
    if (timer_ >= properties_->spawnInterval) {
        for (int i = 0; i < properties_->spawnCount; i++)
            Emmit();
        timer_ = 0.0f;
    }
}

void ParticleEmitter::Emmit() {
    ASSERT(alive_ == true, "Using dead particle emitter");
    Particle particle;
    particle.initialPosition = vec4(transform_.position + RandomVector(properties_->spawnRadius), 0.0f);
    particle.position = particle.initialPosition;
    particle.initialScale = RandomFloatRange(properties_->minScale, properties_->maxScale);
    particle.scale = particle.initialScale;
    particle.velocity = RandomVec4(properties_->minVelocity, properties_->maxVelocity);
    particle.rotation = 0.0f;
    particle.time = 0.0f;
    particles_.push_back(particle); 
}
