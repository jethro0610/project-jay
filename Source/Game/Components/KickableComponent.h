#pragma once
#include "Component.h"
#include <bitset>
#include <algorithm>


enum KickProperties {
    Destroy,
    Spread,
    KickPropertyCount 
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

    std::string GetName() const { return "kickable"; }
    void Load(nlohmann::json& data, EntityID entity) {
        if (data["destroy"].get<bool>() == true)
            properties[entity].set(Destroy);
        else if (data["spread"].get<bool>() == true)
            properties[entity].set(Spread);
    }
};
