#pragma once
#include "Entity.h"

class WaterGun : public Entity {
public:
    void Init(InitArgs args);

    static TypeID TYPEID;
    static const char* GetStaticName() { return "e_watergun"; }
    static EntityDependendies GetStaticDependencies();
    EntityProperties GetStaticProperties();

    Entity* target_;
    int targetId_;
    int numSeeds_;

    void Start();
    void OnHurt(HurtArgs args);
};
