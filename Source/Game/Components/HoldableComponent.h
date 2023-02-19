#pragma once
#include "../Entity/Entity.h"
#include "../../Types/Transform.h"
#include <algorithm>

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

    static int ID;
};

