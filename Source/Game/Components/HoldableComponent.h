#pragma once
#include "../Entity/Entity.h"
#include "../../Types/Transform.h"
#include <algorithm>

struct HoldableCProperties {
};

enum UseType {
    Throw
};

struct HoldableComponent {
    UseType useType[MAX_ENTITIES];

    HoldableComponent() {
    };
    HoldableComponent (const HoldableComponent&) = delete;
    HoldableComponent& operator=(const HoldableComponent&) = delete;

    HoldableCProperties operator[](int index) {
        return HoldableCProperties {
        };
    }

    static int ID;
};

