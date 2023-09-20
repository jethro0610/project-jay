#include <nlohmann/json.hpp>
#include "ParticleManager.h"
#include "../Helpers/MapCheck.h"
#include "../Helpers/Assert.h"
#include "../Helpers/LoadHelpers.h"

ParticleManager::ParticleManager() {
    usableEmitters_.push_front(0);
}

EmitterID ParticleManager::RequestEmitter(std::string name) {
    EmitterID emitter = usableEmitters_.front();
    usableEmitters_.pop_front();
    if (usableEmitters_.size() <= 0)
        usableEmitters_.push_front(emitter + 1);

    alive_[emitter] = true;
    SetProperties(emitter, name);
    // TODO: Set particles array to 0 on emitter
    return emitter; 
}

void ParticleManager::ReleaseEmitter(EmitterID emitter) {
    alive_[emitter] = false;
    usableEmitters_.push_front(emitter);
}

void ParticleManager::Update(float deltaTime) {
    for (int i = 0; i < MAX_EMITTERS; i++) {
        if (alive_[i])
            emitters_[i].Update(deltaTime);
    }
}

void ParticleManager::Emmit(EmitterID emitter) {
    emitters_[emitter].Emmit();
}

void ParticleManager::SetActive(EmitterID emitter, bool active) {
    emitters_[emitter].active_ = active;
}

void ParticleManager::SetTransform(EmitterID emitter, const Transform& transform) {
    emitters_[emitter].transform_ = transform;
}

void ParticleManager::SetProperties(EmitterID emitter, std::string name) {
    emitters_[emitter].properties_ = &GetFromMap<EmitterProperties>(emitterProps_, name, "Tried using unloaded particle emitter " + name);
}

void ParticleManager::LoadEmitterProperty(std::string name) {
    ForceMapUnique(emitterProps_, name, "Emitter " + name + " is already loaded");

    std::ifstream inFile("emitters/" + name + ".json");
    ASSERT(inFile.is_open(), "Failed to load emitter" + name);

    nlohmann::json data = nlohmann::json::parse(inFile);
    EmitterProperties properties;
    properties.spawnInterval = GetFloat(data, "spawn_interval", 1.0f);
    properties.spawnCount = GetInt(data, "spawn_count", 1);
    properties.lifetime = GetFloat(data, "lifetime", 1.0f);

    auto scaleRange = data["scale_range"];
    properties.minScale = scaleRange[0];
    properties.maxScale = scaleRange[1];
    properties.endScale = GetFloat(data, "end_scale");

    properties.spawnRadius = GetFloat(data, "spawn_radius", 0.0f);
    auto velocityRange = data["velocity_range"];
    properties.minVelocity = GetVec3(velocityRange[0]);
    properties.maxVelocity = GetVec3(velocityRange[1]);
    properties.acceleration = GetVec3(data, "acceleration");

    emitterProps_[name] = properties; 
    DEBUGLOG("Loaded entity " << name);
}
