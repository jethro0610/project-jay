#pragma once
#include "Entity.h"

class SinglePod : public Entity {
public:
    void Init(InitArgs args);

    static TypeID TYPEID;
    static const char* GetStaticName() { return "e_singlepod"; }
    static EntityDependendies GetStaticDependencies();
    EntityProperties GetStaticProperties();

    void OnHurt(HurtArgs args);
    int GetSeeds();
    int seeds_;
};
