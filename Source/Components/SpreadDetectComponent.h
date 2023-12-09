#pragma once
#include "Component.h"
#include "Spread/SpreadKey.h"
#include <glm/vec2.hpp>


class SpreadDetectComponent : public Component {
public:
    static constexpr int MAX_DETECT = 16;
    static constexpr int MAX_ADD_DISTANCE = 2;
    static constexpr glm::ivec2 NO_ADD = glm::ivec2(-1024, -1024);

    std::array<bool, MAX_ENTITIES> deteced;
    std::array<SpreadKey, MAX_ENTITIES> lastAdd;
    std::array<SpreadKey, MAX_ENTITIES> lastKey;

    SpreadDetectComponent() {
        deteced.fill(false);
        lastAdd.fill(NO_ADD);
    };
    SpreadDetectComponent (const SpreadDetectComponent &) = delete;
    SpreadDetectComponent & operator=(const SpreadDetectComponent &) = delete;

    static constexpr std::string GetName() { return "spread_detect"; }
    static constexpr int GetID() { return 8; }

    void Load(nlohmann::json& data, EntityID entity) {
    }
};
