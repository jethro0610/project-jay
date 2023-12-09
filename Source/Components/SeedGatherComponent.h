#pragma once
#include "Component.h"

class SeedGatherComponent: public Component {
public:
    std::array<float, MAX_ENTITIES> radius;
    std::array<bool, MAX_ENTITIES> active;

    SeedGatherComponent() {
        active.fill(true);
    };
    SeedGatherComponent(const SeedGatherComponent&) = delete;
    SeedGatherComponent& operator=(const SeedGatherComponent&) = delete;

    static constexpr std::string GetName() { return "seed_gather"; }
    static constexpr int GetID() { return 18; }

    void Load(nlohmann::json& data, EntityID entity) {
        radius[entity] = GetFloat(data, "radius");
    }
};
