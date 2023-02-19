#pragma once
#include "../Entity/Entity.h"
#include "../../Types/Transform.h"
#include <algorithm>

struct BubbleCProperties {
    float& radius;
};

struct BubbleComponent {
    float radius[MAX_ENTITIES];

    BubbleComponent() {
        std::fill_n(radius, MAX_ENTITIES, 0.0f);
    };
    BubbleComponent(const BubbleComponent&) = delete;
    BubbleComponent& operator=(const BubbleComponent&) = delete;

    BubbleCProperties operator[](int index) {
        return BubbleCProperties {
            radius[index]
        };
    }

    static int ID;
};
