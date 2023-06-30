#pragma once
#include <glm/vec2.hpp>
#include <queue>
#include "Component.h"
#include "../../Constants/SpreadConstants.h"

const uint8_t MAX_DETECT = 16;
const uint8_t MAX_ADD_DISTANCE = 2;
const glm::ivec2 NO_ADD = glm::ivec2(-1024, -1024);

class SpreadDetectComponent : public Component {
public:
    bool deteced[MAX_ENTITIES];
    SpreadKey lastAdd[MAX_ENTITIES];
    SpreadKey lastKey[MAX_ENTITIES];

    SpreadDetectComponent() {
        std::fill_n(deteced, MAX_ENTITIES, false);
        std::fill_n(lastAdd, MAX_ENTITIES, NO_ADD);
    };
    SpreadDetectComponent (const SpreadDetectComponent &) = delete;
    SpreadDetectComponent & operator=(const SpreadDetectComponent &) = delete;

    static constexpr std::string GetName() { return "spread_detect"; }
    static constexpr uint8_t GetID() { return 8; }

    void Load(nlohmann::json& data, EntityID entity) {
    }
};
