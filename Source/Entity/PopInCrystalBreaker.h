#pragma once
#include "Entity.h"

class PopInCrystal;

class PopInCrystalBreaker : public Entity {
public:
    void Init(InitArgs args);
    void OnHurt(HurtArgs args);
    void Start();

    static TypeID TYPEID;
    static const char* GetStaticName() { return "e_popincrystalbreaker"; }
    static EntityDependendies GetStaticDependencies();
    EntityProperties GetStaticProperties();

    PopInCrystal* target_;
    int targetId_;
};
