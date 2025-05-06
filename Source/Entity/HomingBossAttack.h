#pragma once
#include "Entity.h"

class HomingBossAttack : public Entity {
public:
    void Init(InitArgs args);

    static TypeID TYPEID;
    static const char* GetStaticName() { return "e_homingatk"; }
    static EntityDependendies GetStaticDependencies();
    EntityProperties GetStaticProperties();
};
