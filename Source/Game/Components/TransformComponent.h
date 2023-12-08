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

    std::array<bool, MAX_ENTITIES> useTilt;
    std::array<glm::vec3, MAX_ENTITIES> lastUp;
    std::array<glm::vec3, MAX_ENTITIES> up;
    std::array<float, MAX_ENTITIES> lastTilt;
    std::array<float, MAX_ENTITIES> tilt;

    std::array<bool, MAX_ENTITIES> destroyOnBound;

    TransformComponent() {
        interpolate.fill(false);
        targetable.fill(false);
        useTilt.fill(false);
        lastUp.fill(Transform::worldUp);
        up.fill(Transform::worldUp);
        lastTilt.fill(0.0f);
        tilt.fill(0.0f);
        destroyOnBound.fill(false);
    };
    TransformComponent(const TransformComponent&) = delete;
    TransformComponent& operator=(const TransformComponent&) = delete;

    static constexpr std::string GetName() { return "transform"; }
    static constexpr int GetID() { return 10; }

    void Load(nlohmann::json* data, EntityID entity) {
        interpolate[entity] = GetBoolean(data, "interpolate", false);
        targetable[entity] = GetBoolean(data, "targetable", false);
        useTilt[entity] = GetBoolean(data, "use_tilt", false);
        destroyOnBound[entity] = GetBoolean(data, "destroy_on_bound", false);
    }
};
