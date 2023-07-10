#pragma once
#include <bitset>
#include "Component.h"

const int MAX_COLLIDER_PROPERTIES = 32;
enum ColliderProperties {
    SendMeteor,
    RecieveMeteor,
    RecieveProjectile,
    CaptureSeed,
};

const int MAX_RECIEVE_METEOR_BEHAVIORS = 32;
enum RecieveMeteorBehavior {
    Launch,
    Damage,
    ReleaseSeed 
};

class ColliderComponent : public Component {
public:
    std::bitset<MAX_COLLIDER_PROPERTIES> properties[MAX_ENTITIES];

    float radius0[MAX_ENTITIES];
    float radius1[MAX_ENTITIES];

    int sendMeteorCooldown[MAX_ENTITIES];
    int consecutiveMeteorSends[MAX_ENTITIES];

    std::bitset<MAX_RECIEVE_METEOR_BEHAVIORS> recieveMeteorBehaviors[MAX_ENTITIES];
    int recieveMeteorCooldown[MAX_ENTITIES];
    int recieveMeteorDamage[MAX_ENTITIES];
    int recieveMeteorSeedAmount[MAX_ENTITIES];

    ColliderComponent() {
        std::fill_n(properties, MAX_ENTITIES, 0);
        std::fill_n(radius0, MAX_ENTITIES, 0.0f);
        std::fill_n(radius1, MAX_ENTITIES, 0.0f);

        std::fill_n(sendMeteorCooldown, MAX_ENTITIES, 0);
        std::fill_n(consecutiveMeteorSends, MAX_ENTITIES, 0);

        std::fill_n(recieveMeteorBehaviors, MAX_ENTITIES, 0);
        std::fill_n(recieveMeteorCooldown, MAX_ENTITIES, 0);
        std::fill_n(recieveMeteorDamage, MAX_ENTITIES, 0);
        std::fill_n(recieveMeteorSeedAmount, MAX_ENTITIES, 0);
    };
    ColliderComponent(const ColliderComponent&) = delete;
    ColliderComponent& operator=(const ColliderComponent&) = delete;

    static constexpr std::string GetName() { return "collider"; }
    static constexpr int GetID() { return 0; }

    void Load(nlohmann::json& data, EntityID entity) {
        radius0[entity] = data["radius0"].get<float>();
        radius1[entity] = GetFloat(data, "radius1");

        properties[entity].set(SendMeteor, GetBoolean(data, "send_meteor"));
        properties[entity].set(RecieveMeteor, GetBoolean(data, "recieve_meteor"));
        properties[entity].set(RecieveProjectile, GetBoolean(data, "recieve_projectile"));
        properties[entity].set(CaptureSeed, GetBoolean(data, "capture_seed"));

        recieveMeteorBehaviors[entity].set(Launch, GetBoolean(data, "launch_on_meteor"));
        recieveMeteorBehaviors[entity].set(Damage, GetBoolean(data, "damage_on_meteor"));
        recieveMeteorBehaviors[entity].set(ReleaseSeed, GetBoolean(data, "release_seed_on_meteor"));

        recieveMeteorDamage[entity] = GetFloat(data, "meteor_damage");
        recieveMeteorSeedAmount[entity] = GetFloat(data, "num_seeds");
    }
};
