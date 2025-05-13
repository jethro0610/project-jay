#pragma once
#include "Entity.h"

class HomingBoss;

class HomingBossProjectile : public Entity {
public:
    void Init(InitArgs args);
    void Start();

    static TypeID TYPEID;
    static const char* GetStaticName() { return "e_homingproj"; }
    static EntityDependendies GetStaticDependencies();
    EntityProperties GetStaticProperties();

    HomingBoss* target_;
    int timer_;
    bool launched_;
    glm::vec3 launchPoint_;
    Transform spawnTransform_;
    int wakeTimer_;
    bool canWake_;

    void Update();
    void SleepUpdate();
    void Reset();
    void OnHurt(HurtArgs args);
};
