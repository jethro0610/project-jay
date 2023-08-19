#pragma once
#include <bitset>
#include "Component.h"

const int MAX_COLLIDER_PROPERTIES = 32;
enum ColliderProperties {
    SendMeteor,
    RecieveMeteor,
    RecieveProjectile,
    CaptureSeed,
    SendPush,
    RecievePush
};

const int MAX_RECIEVE_METEOR_BEHAVIORS = 32;
enum RecieveMeteorBehavior {
    Launch,
    Damage,
    ReleaseSeed 
};

class ColliderComponent : public Component {
public:
    std::array<std::bitset<MAX_COLLIDER_PROPERTIES>, MAX_ENTITIES> properties;

    std::array<float, MAX_ENTITIES> radius0;
    std::array<float, MAX_ENTITIES> radius1;

    std::array<int, MAX_ENTITIES> sendMeteorCooldown;
    std::array<int, MAX_ENTITIES> consecutiveMeteorSends;

    std::array<std::bitset<MAX_RECIEVE_METEOR_BEHAVIORS>, MAX_ENTITIES> recieveMeteorBehaviors;
    std::array<int, MAX_ENTITIES> recieveMeteorCooldown;
    std::array<int, MAX_ENTITIES> recieveMeteorDamage;
    std::array<int, MAX_ENTITIES> recieveMeteorSeedAmount;

    ColliderComponent() {
        properties.fill(0);
        radius0.fill(0.0f);
        radius1.fill(0.0f);

        sendMeteorCooldown.fill(0);
        consecutiveMeteorSends.fill(0);

        recieveMeteorBehaviors.fill(0);
        recieveMeteorCooldown.fill(0);
        recieveMeteorDamage.fill(0);
        recieveMeteorSeedAmount.fill(0);
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
        properties[entity].set(SendPush, GetBoolean(data, "send_push"));
        properties[entity].set(RecievePush, GetBoolean(data, "recieve_push"));

        recieveMeteorBehaviors[entity].set(Launch, GetBoolean(data, "launch_on_meteor"));
        recieveMeteorBehaviors[entity].set(Damage, GetBoolean(data, "damage_on_meteor"));
        recieveMeteorBehaviors[entity].set(ReleaseSeed, GetBoolean(data, "release_seed_on_meteor"));

        recieveMeteorDamage[entity] = GetFloat(data, "meteor_damage");
        recieveMeteorSeedAmount[entity] = GetFloat(data, "num_seeds");
    }
};
