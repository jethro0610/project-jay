#pragma once
#include "Entity.h"

class HomingBossPoint : public Entity {
public:
    void Init(InitArgs args);
    void OnHurt(HurtArgs args);
    void Activate();
    void Reset();

    static TypeID TYPEID;
    static const char* GetStaticName() { return "e_homingbosspoint"; }
    static EntityDependendies GetStaticDependencies();
    EntityProperties GetStaticProperties();

    bool hit_;
};
