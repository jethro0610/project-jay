#pragma once
#include "Entity.h"

class Sleeper : public Entity {
public:
    void Init(InitArgs args);

    static TypeID TYPEID;
    static const char* GetStaticName() { return "e_sleeper"; }
    static EntityDependendies GetStaticDependencies();

    void OnHurt(HurtArgs args);
};
