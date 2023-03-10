#pragma once
#include "../Entity/Entity.h"
#include "Component.h"
#include <algorithm>

struct WorldColliderComponent : public Component {
    // TODO: Add radius
    bool colliding[MAX_ENTITIES];

    WorldColliderComponent(){
        std::fill_n(colliding, MAX_ENTITIES, false);
    };
    WorldColliderComponent(const WorldColliderComponent&) = delete;
    WorldColliderComponent& operator=(const WorldColliderComponent&) = delete;

    void operator[](int index) {
        return; 
    }

    uint8_t GetID() const { return 15; }
    void Load(nlohmann::json& data, uint16_t entity) {
    }
};
