#pragma once
#include "../Entity/Entity.h"
#include <algorithm>

#include "ComponentDesc.h"

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

    inline static int ID = 0;
    inline static ComponentDesc description {
        "Bubble Component",
        2,
        {
            {"Radius", "float"},
        }
    };
};
