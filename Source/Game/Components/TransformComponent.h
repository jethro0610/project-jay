#pragma once
#include "../Entity/Entity.h"
#include "Component.h"
#include "../../Types/Transform.h"
#include <algorithm>

struct TransformCProperties {
    Transform& transform;
    bool& interpolate;
};

struct TransformComponent : public Component {
    Transform transform[MAX_ENTITIES];
    Transform transformLastUpdate[MAX_ENTITIES];
    Transform renderTransform[MAX_ENTITIES];
    bool interpolate[MAX_ENTITIES];

    TransformComponent() {
        std::fill_n(interpolate, MAX_ENTITIES, false);
    };
    TransformComponent(const TransformComponent&) = delete;
    TransformComponent& operator=(const TransformComponent&) = delete;

    TransformCProperties operator[](int index) {
        return TransformCProperties {
            transform[index],
            interpolate[index]
        };
    }

    std::string GetName() const { return "transform"; }
    void Load(nlohmann::json& data, uint16_t entity) {
        interpolate[entity] = data["interpolate"].get<bool>();
    }
};
