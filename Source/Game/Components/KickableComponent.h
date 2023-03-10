#pragma once
#include "../Entity/Entity.h"
#include "Component.h"
#include <bitset>
#include <algorithm>


enum KickProperties {
    Destroy,
    Spread,
    KickPropertyCount 
};

struct KickableCProperties {
    std::bitset<KickPropertyCount>& properties;
    bool& canKick;
};

struct KickableComponent : public Component {
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

    std::string GetName() const { return "kickable"; }
    void Load(nlohmann::json& data, uint16_t entity) {
        if (data["destroy"].get<bool>() == true)
            properties[entity].set(Destroy);
        if (data["spread"].get<bool>() == true)
            properties[entity].set(Spread);
    }
};
