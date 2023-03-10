#pragma once
#include "../Entity/Entity.h"
#include "Component.h"
#include <algorithm>

struct KickerCProperties {
    bool& kicking;
};

struct KickerComponent : public Component {
    bool kicking[MAX_ENTITIES];

    KickerComponent() {
        std::fill_n(kicking, MAX_ENTITIES, true);
    };
    KickerComponent(const KickerComponent&) = delete;
    KickerComponent& operator=(const KickerComponent&) = delete;

    KickerCProperties operator[](int index) {
        return KickerCProperties {
            kicking[index]
        };
    }

    inline static int ID = 5;
    void Load(nlohmann::json& json, uint16_t entity) {
    }
};
