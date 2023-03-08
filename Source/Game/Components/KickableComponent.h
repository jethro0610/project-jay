#pragma once
#include "../Entity/Entity.h"
#include <bitset>
#include <algorithm>

#include "ComponentDesc.h"

enum KickProperties {
    Destroy,
    Spread,
    KickPropertyCount 
};

struct KickableCProperties {
    std::bitset<KickPropertyCount>& properties;
    bool& canKick;
};

struct KickableComponent {
    std::bitset<KickPropertyCount> properties[MAX_ENTITIES];
    bool canKick[MAX_ENTITIES];

    KickableComponent() {
        std::fill_n(properties, MAX_ENTITIES, 0); 
        std::fill_n(canKick, MAX_ENTITIES, true); 
    };
    KickableComponent(const KickableComponent&) = delete;
    KickableComponent& operator=(const KickableComponent&) = delete;

    KickableCProperties operator[](int index) {
        return {
            properties[index],
            canKick[index]
        };
    }

    inline static int ID = 4;
    inline static ComponentDesc description {
        "Kickable Component",
        2,
        {
            {"Destroy On Kicked", "bool"},
            {"Spread On Kickied", "bool"}
        }
    };
};
