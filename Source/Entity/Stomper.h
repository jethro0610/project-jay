#pragma once
#include "Entity.h"

class Stomper : public Entity {
public:
    void Init(InitArgs args);
    void Update();

    static TypeID TYPEID;
    static const char* GetStaticName() { return "e_stomper"; }
    static EntityDependendies GetStaticDependencies();
    EntityProperties GetStaticProperties();
};
