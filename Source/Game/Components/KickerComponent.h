#pragma once
#include "Component.h"
#include <algorithm>

struct KickerComponent : public Component {
    bool kicking[MAX_ENTITIES];

    KickerComponent() {
        std::fill_n(kicking, MAX_ENTITIES, true);
    };
    KickerComponent(const KickerComponent&) = delete;
    KickerComponent& operator=(const KickerComponent&) = delete;

    std::string GetName() const { return "kicker"; }
    void Load(nlohmann::json& data, EntityID entity) {
    }
};
