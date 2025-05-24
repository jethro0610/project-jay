#pragma once
#include "Entity.h"

class Bumper : public Entity {
public:
    void Init(InitArgs args);

    static TypeID TYPEID;
    static const char* GetStaticName() { return "e_bumper"; }
    static EntityDependendies GetStaticDependencies();
    EntityProperties GetStaticProperties();

    float speed_;
    float friction_;

    void Update();
    void OnHurt(HurtArgs args);
};
