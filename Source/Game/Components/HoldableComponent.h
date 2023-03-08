#pragma once
#include "../Entity/Entity.h"
#include <algorithm>

#include "ComponentDesc.h"

struct HoldableCProperties {
};


struct HoldableComponent {

    HoldableComponent() {
    };
    HoldableComponent (const HoldableComponent&) = delete;
    HoldableComponent& operator=(const HoldableComponent&) = delete;

    HoldableCProperties operator[](int index) {
        return HoldableCProperties {
        };
    }

    inline static int ID = 2;
    inline static ComponentDesc description {
        "Holdable Component",
        0,
        {}
    };
};

