#pragma once
#include <bitset>
#include <glm/vec3.hpp>
#include "Component.h"
#include "../ParticleEmitter.h"
#include "../ParticleManager.h"

class ProjectileComponent : public Component {
public:
    // Assign this particle manager first
    ParticleManager* particleManager;

    std::array<bool, MAX_ENTITIES> launchOnSpawn;
    std::array<glm::vec3, MAX_ENTITIES> minLaunchVelocity;
    std::array<glm::vec3, MAX_ENTITIES> maxLaunchVelocity;
    std::array<ParticleEmitter*, MAX_ENTITIES> emitter;

    // TODO: Target

    ProjectileComponent() {
        launchOnSpawn.fill(false);
        minLaunchVelocity.fill(glm::vec3(0.0f));
        maxLaunchVelocity.fill(glm::vec3(0.0f));
        emitter.fill(nullptr);
    }

    static constexpr std::string GetName() { return "projectile"; }
    static constexpr int GetID() { return 6; }

    void Load(nlohmann::json& data, EntityID entity) {
        launchOnSpawn[entity] = GetBoolean(data, "launch_on_spawn");
        minLaunchVelocity[entity] = GetVec3(data, "min_launch_velocity");
        maxLaunchVelocity[entity] = GetVec3(data, "max_launch_velocity");
        if (data.contains("emitter"))
            emitter[entity] = particleManager->RequestEmitter(data["emitter"]);
    }
};
