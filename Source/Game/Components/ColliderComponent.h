#pragma once
#include <bitset>
#include "Component.h"

const int MAX_COLLIDER_PROPERTIES = 32;
enum ColliderProperties {
    Meteor,
    CaptureSeed
};

const int MAX_METEORED_BEHAVIORS = 32;
enum MeteoredBehaviors {
    Launch    
};

const int MA= 30;

class ColliderComponent : public Component {
public:
    float radius0[MAX_ENTITIES];
    float radius1[MAX_ENTITIES];
    int recieveMeteorCooldown[MAX_ENTITIES];
    int sendMeteorCooldown[MAX_ENTITIES];
    int consecutiveMeteorSends[MAX_ENTITIES];
    std::bitset<MAX_COLLIDER_PROPERTIES> properties[MAX_ENTITIES];
    std::bitset<MAX_METEORED_BEHAVIORS> meteoredBehaviors[MAX_ENTITIES];

    ColliderComponent() {
        std::fill_n(radius0, MAX_ENTITIES, 0.0f);
        std::fill_n(radius1, MAX_ENTITIES, 0.0f);
        std::fill_n(recieveMeteorCooldown, MAX_ENTITIES, 0);
        std::fill_n(sendMeteorCooldown, MAX_ENTITIES, 0);
        std::fill_n(consecutiveMeteorSends, MAX_ENTITIES, 0);
        std::fill_n(properties, MAX_ENTITIES, 0);
        std::fill_n(meteoredBehaviors, MAX_ENTITIES, 0);
    };
    ColliderComponent(const ColliderComponent&) = delete;
    ColliderComponent& operator=(const ColliderComponent&) = delete;

    static constexpr std::string GetName() { return "collider"; }
    static constexpr int GetID() { return 0; }

    void Load(nlohmann::json& data, EntityID entity) {
        radius0[entity] = data["radius0"].get<float>();
        radius1[entity] = GetFloat(data, "radius1");

        properties[entity].set(Meteor, GetBoolean(data, "meteor"));
        properties[entity].set(CaptureSeed, GetBoolean(data, "capture_seed"));

        meteoredBehaviors[entity].set(Launch, GetBoolean(data, "launch_on_meteor"));
    }
};
