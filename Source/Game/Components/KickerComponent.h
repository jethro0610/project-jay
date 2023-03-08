#pragma once
#include "../Entity/Entity.h"
#include <algorithm>

#include "ComponentDesc.h"

struct KickerCProperties {
    bool& kicking;
};

struct KickerComponent {
    bool kicking[MAX_ENTITIES];

    KickerComponent() {
        std::fill_n(kicking, MAX_ENTITIES, true);
    };
    KickerComponent(const KickerComponent&) = delete;
    KickerComponent& operator=(const KickerComponent&) = delete;

    KickerCProperties operator[](int index) {
        return KickerCProperties {
            kicking[index]
        };
    }

    inline static int ID = 5;
    inline static ComponentDesc description {
        "Kicker Component",
        0,
        {}
    };
};
