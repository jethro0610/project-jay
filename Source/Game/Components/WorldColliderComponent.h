#pragma once
#include "Component.h"

class WorldColliderComponent : public Component {
public:
    bool colliding[MAX_ENTITIES];

    WorldColliderComponent(){
        std::fill_n(colliding, MAX_ENTITIES, false);
    };
    WorldColliderComponent(const WorldColliderComponent&) = delete;
    WorldColliderComponent& operator=(const WorldColliderComponent&) = delete;

    static constexpr std::string GetName() { return "world_collider"; }
    static constexpr uint8_t GetID() { return 12; }

    void Load(nlohmann::json& data, EntityID entity) {
    }
};
