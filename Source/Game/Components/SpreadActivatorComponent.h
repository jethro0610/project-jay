#pragma once
#include "../Entity/Entity.h"
#include "Component.h"
#include <algorithm>

struct SpreadActivatorCProperties {
    bool& groundOnly;
};

struct SpreadActivatorComponent : public Component {
    bool groundOnly[MAX_ENTITIES];
    bool active[MAX_ENTITIES];

    SpreadActivatorComponent() {
        std::fill_n(groundOnly, MAX_ENTITIES, true);
        std::fill_n(active, MAX_ENTITIES, false);
    };
    SpreadActivatorComponent(const SpreadActivatorComponent&) = delete;
    SpreadActivatorComponent& operator=(const SpreadActivatorComponent&) = delete;

    SpreadActivatorCProperties operator[](int index) {
        return {
            groundOnly[index]
        };
    };

    inline static int ID = 9;
    void Load(nlohmann::json& data, uint16_t entity) {
        groundOnly[entity] = data["groundOnly"].get<bool>();
    }
};
