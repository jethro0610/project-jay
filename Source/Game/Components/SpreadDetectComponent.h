#pragma once
#include "../Entity/Entity.h"
#include "Component.h"
#include <glm.hpp>
#include <queue>
#include <algorithm>

const uint8_t MAX_DETECT = 4;

struct SpreadDetectComponent : public Component {
    bool detecedSpread[MAX_ENTITIES];
    glm::ivec2 lastDetect[MAX_ENTITIES][MAX_DETECT];

    SpreadDetectComponent() {
        std::fill_n(detecedSpread, MAX_ENTITIES, false);
        for (int i = 0; i < MAX_ENTITIES; i++)
            std::fill_n(lastDetect[i], MAX_DETECT, glm::ivec2(0, 0));
    };
    SpreadDetectComponent (const SpreadDetectComponent &) = delete;
    SpreadDetectComponent & operator=(const SpreadDetectComponent &) = delete;

    void operator[](int index) {
        return; 
    };

    inline static int ID = 10;
    void Load(nlohmann::json& data, uint16_t entity) {
    }
};
