#include "ParticleEmitter.h"
#include "Helpers/Random.h"
#include "Helpers/Assert.h"
#include <glm/gtx/compatibility.hpp>
using namespace glm;

void ParticleEmitter::Update(float deltaTime) {
    ASSERT(alive_ == true, "Using dead particle emitter");

    for (int i = 0; i < particles_.size(); i++) {
        Particle& particle = particles_[i];
        if (particle.time > 1.0f)
            particles_.remove(i--);

        particle.time += deltaTime / properties_.lifetime;
        particle.velocity += properties_.acceleration * deltaTime;
        particle.velocity.x = glm::lerp(particle.velocity.x, 0.0f, 1.0f - std::exp(-properties_.damping.x * deltaTime));
        particle.velocity.y = glm::lerp(particle.velocity.y, 0.0f, 1.0f - std::exp(-properties_.damping.y * deltaTime));
        particle.velocity.z = glm::lerp(particle.velocity.z, 0.0f, 1.0f - std::exp(-properties_.damping.z * deltaTime));
        particle.velocity.w = glm::lerp(particle.velocity.w, 0.0f, 1.0f - std::exp(-properties_.damping.w * deltaTime));
        particle.position += particle.velocity * deltaTime;
        particle.scale = mix(particle.initialScale, properties_.endScale, particle.time);
    }

    if (active_)
        timer_ += deltaTime;
    else {
        timer_ = 0.0f;
        lastTransform_ = transform_;
        return;
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
        Particle particle = {};

        float lerpAmount = (std::rand() % 1000) * 0.001f;
        vec4 initialPos = properties_.localSpace ? 
            vec4(0.0f) : 
            vec4(mix(lastTransform_.position, transform_.position, lerpAmount), 0.0f);

        vec4 offset = vec4(RandomVector(properties_.spawnRadius), 1.0f);
        particle.initialPosition = initialPos + offset;
        particle.position = particle.initialPosition;

        particle.initialScale = RandomFloatRange(properties_.minScale, properties_.maxScale);
        particle.scale = particle.initialScale;

        switch (properties_.emissionType) {
            case EmitterProperties::ET_Default: {
                particle.velocity = rotate(transform_.rotation, RandomVec4(properties_.minVelocity, properties_.maxVelocity));
                break;
            }

            case EmitterProperties::ET_PlanarDisc: {
                vec3 planarRandom = RandomVectorPlanar(properties_.minVelocity.x, properties_.maxVelocity.x);
                float y = RandomFloatRange(properties_.minVelocity.y, properties_.maxVelocity.y);
                particle.velocity = rotate(transform_.rotation, vec4(planarRandom.x, y, planarRandom.z, 0.0f));
                break;
            }

            case EmitterProperties::ET_Sphere: {
                vec3 direction = RandomVector(properties_.minVelocity.x, properties_.maxVelocity.x);
                particle.velocity = rotate(transform_.rotation, vec4(direction, 0.0f));
                break;
            }
        }
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
