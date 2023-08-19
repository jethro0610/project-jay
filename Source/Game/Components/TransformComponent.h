#pragma once
#include "../../Types/Transform.h"
#include "Component.h"

class TransformComponent : public Component {
public:
    std::array<Transform, MAX_ENTITIES> transform;
    std::array<Transform, MAX_ENTITIES> transformLastUpdate;
    std::array<Transform, MAX_ENTITIES> renderTransform;
    std::array<bool, MAX_ENTITIES> interpolate;
    std::array<bool, MAX_ENTITIES> targetable;

    TransformComponent() {
        interpolate.fill(false);
        targetable.fill(false);
    };
    TransformComponent(const TransformComponent&) = delete;
    TransformComponent& operator=(const TransformComponent&) = delete;

    static constexpr std::string GetName() { return "transform"; }
    static constexpr int GetID() { return 10; }

    void Load(nlohmann::json& data, EntityID entity) {
        interpolate[entity] = data["interpolate"].get<bool>();
        targetable[entity] = GetBoolean(data, "targetable", false);
    }
};
