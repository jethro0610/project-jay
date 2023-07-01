#pragma once
#include "../../Types/Transform.h"
#include "Component.h"

class TransformComponent : public Component {
public:
    Transform transform[MAX_ENTITIES];
    Transform transformLastUpdate[MAX_ENTITIES];
    Transform renderTransform[MAX_ENTITIES];
    bool interpolate[MAX_ENTITIES];

    TransformComponent() {
        std::fill_n(interpolate, MAX_ENTITIES, false);
    };
    TransformComponent(const TransformComponent&) = delete;
    TransformComponent& operator=(const TransformComponent&) = delete;

    static constexpr std::string GetName() { return "transform"; }
    static constexpr int GetID() { return 10; }

    void Load(nlohmann::json& data, EntityID entity) {
        interpolate[entity] = data["interpolate"].get<bool>();
    }
};
