#pragma once
#include <glm.hpp>
#include <queue>
#include "Component.h"

const uint8_t MAX_DETECT = 16;
const uint8_t MAX_ADD_DISTANCE = 2;
const glm::ivec2 NO_ADD = glm::ivec2(-1024, -1024);

struct SpreadDetectComponent : public Component {
    bool deteced[MAX_ENTITIES];
    glm::ivec2 lastAdd[MAX_ENTITIES];
    glm::ivec2 lastKey[MAX_ENTITIES];

    SpreadDetectComponent() {
        std::fill_n(deteced, MAX_ENTITIES, false);
        std::fill_n(lastAdd, MAX_ENTITIES, NO_ADD);
    };
    SpreadDetectComponent (const SpreadDetectComponent &) = delete;
    SpreadDetectComponent & operator=(const SpreadDetectComponent &) = delete;

    std::string GetName() const { return "spread_detect"; }
    void Load(nlohmann::json& data, EntityID entity) {
    }
};
