#pragma once
#include "Entity.h"

class LimitSpawner : public Entity {
public:
    void Init(InitArgs args);

    static TypeID TYPEID;
    static const char* GetStaticName() { return "e_limitspawner"; }
    static EntityDependendies GetStaticDependencies();
    EntityProperties GetStaticProperties();

    int spawnTypeId_;
    float spawnScale_;

    void OnHurt(HurtArgs args);
    void Spawn();
};
