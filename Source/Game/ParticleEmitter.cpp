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
        Emmit();
        timer_ = 0.0f;
    }
}

void ParticleEmitter::Emmit() {
    ASSERT(alive_ == true, "Using dead particle emitter");
    mat4 worldMatrix = transpose(transform_.ToMatrix());
    for (int i = 0; i < properties_->spawnCount; i++) {
        Particle particle;

        vec4 offset = vec4(RandomVector(properties_->spawnRadius), 1.0f);
        particle.initialPosition = offset * worldMatrix;
        particle.position = particle.initialPosition;

        particle.initialScale = RandomFloatRange(properties_->minScale, properties_->maxScale);
        particle.scale = particle.initialScale;
        particle.velocity = RandomVec4(properties_->minVelocity, properties_->maxVelocity);
        particle.rotation = 0.0f;
        particle.time = 0.0f;
        particles_.push_back(particle); 
    }
}
