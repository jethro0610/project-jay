#include "ParticleManager.h"
#include "Helpers/Assert.h"
#include <nlohmann/json.hpp>

ParticleManager::ParticleManager() {
    usableEmitters_.push_front(0);
}

ParticleEmitter* ParticleManager::RequestEmitter() {
    int emitterIndex = usableEmitters_.front();
    usableEmitters_.pop_front();
    if (usableEmitters_.size() <= 0)
        usableEmitters_.push_front(emitterIndex + 1);
    ASSERT((emitterIndex < MAX_EMITTERS), "Too many emitters");

    ParticleEmitter& emitter = emitters_[emitterIndex];
    emitter.alive_ = true;
    emitters_[emitterIndex].properties_ = {};

    DEBUGLOG("Assign emitter: " << emitterIndex);

    // TODO: Set particles array to 0 on emitter
    return &emitters_[emitterIndex]; 
}


void ParticleManager::Update(float deltaTime) {
    for (int i = 0; i < MAX_EMITTERS; i++) {
        if (!emitters_[i].alive_)
            continue;

        if (emitters_[i].release_)
            emitters_[i].active_ = false;

        emitters_[i].Update(deltaTime);

        // Only release once the emitter is finished
        if (emitters_[i].release_ && emitters_[i].particles_.size() == 0) {
            emitters_[i] = ParticleEmitter();
            usableEmitters_.push_front(i);
        }
    }
}

void ParticleManager::Reset() {
    for (int i = 0; i < MAX_EMITTERS; i++)
        emitters_[i].Reset();

    usableEmitters_.clear();
    usableEmitters_.push_front(0);
}
