#include <nlohmann/json.hpp>
#include "ParticleManager.h"
#include "../Helpers/MapCheck.h"
#include "../Helpers/Assert.h"
#include "../Helpers/LoadHelpers.h"
#include "../Rendering/Renderer.h"

ParticleManager::ParticleManager() {
    usableEmitters_.push_front(0);
}

ParticleEmitter* ParticleManager::RequestEmitter(std::string name) {
    int emitterIndex = usableEmitters_.front();
    usableEmitters_.pop_front();
    if (usableEmitters_.size() <= 0)
        usableEmitters_.push_front(emitterIndex + 1);
    ASSERT((emitterIndex < MAX_EMITTERS), "Too many emitters");

    ParticleEmitter& emitter = emitters_[emitterIndex];
    emitter.alive_ = true;
    emitters_[emitterIndex].properties_ = &GetFromMap<EmitterProperties>(
        emitterProps_, 
        name, 
        "Tried using unloaded particle emitter " + name
    );

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

void ParticleManager::LoadEmitterProperty(std::string name, Renderer& renderer) {
    ForceMapUnique(emitterProps_, name, "Emitter " + name + " is already loaded");

    std::ifstream inFile("emitters/" + name + ".json");
    ASSERT(inFile.is_open(), "Failed to load emitter" + name);

    nlohmann::json data = nlohmann::json::parse(inFile);
    EmitterProperties properties;

    properties.material = &renderer.GetMaterial(GetString(data, "material", "null_material"));
    properties.localSpace = GetBoolean(data, "local_space");

    properties.spawnInterval = GetFloat(data, "spawn_interval", 1.0f);
    properties.spawnCount = GetInt(data, "spawn_count", 1);
    properties.lifetime = GetFloat(data, "lifetime", 1.0f);

    auto scaleRange = data["scale_range"];
    properties.minScale = scaleRange[0];
    properties.maxScale = scaleRange[1];
    properties.endScale = GetFloat(data, "end_scale");

    properties.spawnRadius = GetFloat(data, "spawn_radius", 0.0f);
    auto velocityRange = data["velocity_range"];
    properties.minVelocity = GetVec4(velocityRange[0]);
    properties.maxVelocity = GetVec4(velocityRange[1]);
    properties.acceleration = GetVec4(data, "acceleration");

    properties.startColor = GetVec4(data, "start_color");
    properties.endColor = GetVec4(data, "end_color");

    emitterProps_[name] = properties; 
    DEBUGLOG("Loaded entity " << name);
}
