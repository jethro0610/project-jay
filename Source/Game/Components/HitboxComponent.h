#pragma once
#include "../Entity/Entity.h"
#include <bitset>
#include <algorithm>

// TODO: Have two seperate flags? Property flag and state flag?
// Maybe have hitbox component handle pushouts?

enum HitboxFlag {
    SendKick,
    RecieveKick,
    HitboxFlagCount 
};

struct HitboxCProperties {
    float& radius;
    std::bitset<HitboxFlagCount>& flags;
};

struct HitboxComponent {
    float radius[MAX_ENTITIES];
    std::bitset<HitboxFlagCount> flags[MAX_ENTITIES];

    HitboxComponent () {
        std::fill_n(radius, MAX_ENTITIES, -1.0f);
        std::fill_n(flags, MAX_ENTITIES, 0); // Zero out all hitbox flags
    };
    HitboxComponent (const HitboxComponent &) = delete;
    HitboxComponent & operator=(const HitboxComponent &) = delete;

    HitboxCProperties operator[](int index) {
        return HitboxCProperties {
            radius[index],
            flags[index]
        };
    }

    static int ID;
};
