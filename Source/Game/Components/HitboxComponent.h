#pragma once
#include "../Entity/Entity.h"
#include <bitset>
#include <algorithm>

// TODO: Have two seperate flags? Property flag and state flag?
// Maybe have hitbox component handle pushouts?

enum HitboxProperty {
    SendKick,
    RecieveKick,
    SpreadOnKick,
    HitboxPropertyCount 
};

enum HitboxState {
    SentKick,
    RecievedKick,
    HitboxStateCount
};

struct HitboxCProperties {
    std::bitset<HitboxPropertyCount>& properties;
};

struct HitboxComponent {
    std::bitset<HitboxPropertyCount> properties[MAX_ENTITIES];
    std::bitset<HitboxStateCount> state[MAX_ENTITIES];

    HitboxComponent () {
        std::fill_n(properties, MAX_ENTITIES, 0); 
        std::fill_n(state, MAX_ENTITIES, 0); 
    };
    HitboxComponent (const HitboxComponent &) = delete;
    HitboxComponent & operator=(const HitboxComponent &) = delete;

    HitboxCProperties operator[](int index) {
        return HitboxCProperties {
            properties[index]
        };
    }

    static int ID;
};
