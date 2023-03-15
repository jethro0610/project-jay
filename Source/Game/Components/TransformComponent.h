#pragma once
#include "Component.h"
#include "../../Types/Transform.h"
#include <algorithm>

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

    std::string GetName() const { return "transform"; }
    void Load(nlohmann::json& data, EntityID entity) {
        interpolate[entity] = data["interpolate"].get<bool>();
    }
};
