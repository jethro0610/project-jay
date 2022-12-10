#pragma once
#include "../Entity/Entity.h"
#include "../../Types/Transform.h"
#include <algorithm>

struct FlagsCProperties {
    bool& canPickup;
};

struct FlagsComponent {
    bool canPickup[MAX_ENTITIES];

    FlagsComponent () {
        std::fill_n(canPickup, MAX_ENTITIES, false);
    };
    FlagsComponent (const FlagsComponent &) = delete;
    FlagsComponent & operator=(const FlagsComponent &) = delete;

    FlagsCProperties operator[](int index) {
        return FlagsCProperties {
            canPickup[index]
        };
    }

    static int ID;
};
