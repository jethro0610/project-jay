#pragma once
#include "Component.h"

class TrampleComponent : public Component {
public:
    std::array<int, MAX_ENTITIES> radius;
    std::array<int, MAX_ENTITIES> groundEntryRadius;

    TrampleComponent() {
        radius.fill(0);
        groundEntryRadius.fill(0);
    };
    TrampleComponent(const TrampleComponent&) = delete;
    TrampleComponent& operator=(const TrampleComponent&) = delete;

    static constexpr std::string GetName() { return "trample"; }
    static constexpr int GetID() { return 15; }

    void Load(nlohmann::json& data, EntityID entity) {
        radius[entity] = GetInt(data, "radius");
        groundEntryRadius[entity] = GetInt(data, "ground_entry_radius");
    }
};
