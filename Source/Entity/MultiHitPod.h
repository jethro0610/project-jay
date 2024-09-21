#pragma once
#include "Entity.h"

class MultiHitPod : public Entity {
public:
    void Init(InitArgs args);

    static TypeID TYPEID;
    static const char* GetStaticName() { return "e_multihitpod"; }
    static EntityDependendies GetStaticDependencies();

    static constexpr int MAX_COOLDOWN_TICKS = 5;

    int cooldown_;
    bool disableCollision_;

    void Update(); 
    void OnHurt(HurtArgs args);
};
