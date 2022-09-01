#pragma once
#include "../Entity/Entity.h"
#include "../../Types/Transform.h"

struct ColliderCProperties {
    float& radius;
};

struct ColliderComponent {
    float radius[MAX_ENTITIES];
    glm::vec3 hitNormal[MAX_ENTITIES];
    bool hit[MAX_ENTITIES];
    ColliderComponent() {
        std::fill_n(radius, MAX_ENTITIES, -1.0f);
        std::fill_n(hitNormal, MAX_ENTITIES, glm::vec3(0.0f, 0.0f, 0.0f));
        std::fill_n(hit, MAX_ENTITIES, false);
    };
    ColliderComponent(const ColliderComponent&) = delete;
    ColliderComponent& operator=(const ColliderComponent&) = delete;

    ColliderCProperties operator[](int index) {
        return ColliderCProperties{
            radius[index]
        };
    }

    static int ID;
};
