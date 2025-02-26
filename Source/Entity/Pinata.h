#pragma once
#include "Entity.h"

class Pinata : public Entity {
public:
    void Init(InitArgs args);

    static TypeID TYPEID;
    static const char* GetStaticName() { return "e_pinata"; }
    static EntityDependendies GetStaticDependencies();
    EntityProperties GetStaticProperties();

    void Update();
    void OnHurt(HurtArgs args);
    int hitsTillBreak_;
    int hits_;
};
