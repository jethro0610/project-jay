#pragma once
#include "../Entity/Entity.h"
#include <algorithm>
#include "../../Types/Transform.h"

#include "ComponentDesc.h"

struct TransformCProperties {
    Transform& transform;
    bool& interpolate;
};

struct TransformComponent {
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

    inline static int ID = 13;
    inline static ComponentDesc description {
        "Transform Component",
        1,
        {
            {"Interpolate", "bool"}
        }
    };
};
