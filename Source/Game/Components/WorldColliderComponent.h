#pragma once
#include "Component.h"
#include <algorithm>

struct WorldColliderComponent : public Component {
    bool colliding[MAX_ENTITIES];

    WorldColliderComponent(){
        std::fill_n(colliding, MAX_ENTITIES, false);
    };
    WorldColliderComponent(const WorldColliderComponent&) = delete;
    WorldColliderComponent& operator=(const WorldColliderComponent&) = delete;

    std::string GetName() const { return "world_collider"; }
    void Load(nlohmann::json& data, EntityID entity) {
    }
};
