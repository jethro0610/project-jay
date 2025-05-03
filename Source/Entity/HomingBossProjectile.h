#pragma once
#include "Entity.h"

class HomingBossProjectile : public Entity {
public:
    void Init(InitArgs args);
    void Update();
    void OnHurt(HurtArgs args);

    static TypeID TYPEID;
    static const char* GetStaticName() { return "e_homingproj"; }
    static EntityDependendies GetStaticDependencies();
    EntityProperties GetStaticProperties();

    static constexpr float GRAVITY = 10.0f;
    static constexpr float LAUNCH_STRENGTH = 400.0f;
};
