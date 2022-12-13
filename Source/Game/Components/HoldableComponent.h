#pragma once
#include "../Entity/Entity.h"
#include "../../Types/Transform.h"
#include <algorithm>

struct HoldableCProperties {
    float& range;
};

enum UseType {
    Throw
};

struct HoldableComponent {
    float range[MAX_ENTITIES];
    UseType useType[MAX_ENTITIES];

    HoldableComponent() {
        std::fill_n(range, MAX_ENTITIES, 0.0f);
    };
    HoldableComponent (const HoldableComponent&) = delete;
    HoldableComponent& operator=(const HoldableComponent&) = delete;

    HoldableCProperties operator[](int index) {
        return HoldableCProperties {
            range[index]
        };
    }

    static int ID;
};

