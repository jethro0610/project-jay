#pragma once
#include "Entity.h"

class Mole : public Entity {
public:
    void Init(InitArgs args);

    static TypeID TYPEID;
    static const char* GetStaticName() { return "e_mole"; }
    static EntityDependendies GetStaticDependencies();

    void Update();
    int timer_;
};
