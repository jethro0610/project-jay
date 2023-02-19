#pragma once
#include "../Entity/Entity.h"
#include <algorithm>

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

    static int ID;
};
