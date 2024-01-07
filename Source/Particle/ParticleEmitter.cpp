#include "ParticleEmitter.h"
#include "Helpers/Random.h"
#include "Helpers/Assert.h"
using namespace glm;

void ParticleEmitter::Update(float deltaTime) {
    ASSERT(alive_ == true, "Using dead particle emitter");

    for (int i = 0; i < particles_.size(); i++) {
        Particle& particle = particles_[i];
        if (particle.time > 1.0f)
            particles_.remove(i--);

        particle.time += deltaTime / properties_.lifetime;
        particle.velocity += properties_.acceleration * deltaTime;
        particle.position += particle.velocity * deltaTime;
        particle.scale = mix(particle.initialScale, properties_.endScale, particle.time);
    }

    if (active_)
        timer_ += deltaTime;
    else {
        timer_ = 0.0f;
        lastTransform_ = transform_;
    }

    if (timer_ >= properties_.spawnInterval) {
        Emmit();
        timer_ = 0.0f;
        lastTransform_ = transform_;
    }
}

void ParticleEmitter::Emmit() {
    ASSERT(alive_ == true, "Using dead particle emitter");
    // mat4 worldMatrix = transpose(transform_.ToMatrix());

    for (int i = 0; i < properties_.spawnCount; i++) {
        Particle particle;

        float lerpAmount = (std::rand() % 1000) * 0.001f;
        vec4 initialPos = properties_.localSpace ? 
            vec4(0.0f) : 
            vec4(mix(lastTransform_.position, transform_.position, lerpAmount), 0.0f);

        vec4 offset = vec4(RandomVector(properties_.spawnRadius), 1.0f);
        particle.initialPosition = initialPos + offset;
        particle.position = particle.initialPosition;

        particle.initialScale = RandomFloatRange(properties_.minScale, properties_.maxScale);
        particle.scale = particle.initialScale;
        particle.velocity = rotate(transform_.rotation, RandomVec4(properties_.minVelocity, properties_.maxVelocity));
        particle.velocity += vec4(velocityOffset_, 0.0f);
        particle.rotation = 0.0f;
        particle.time = 0.0f;
        particles_.push_back(particle); 
    }
}

void ParticleEmitter::Reset() {
    alive_ = false;
    active_ = false;
    release_ = false;
    transform_ = Transform();
    properties_ = {};
    timer_ = 0.0f;
    velocityOffset_ = glm::vec3(0.0f);
    particles_.clear();
}
