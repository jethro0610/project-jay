#pragma once
#include "Entity.h"

class TPillar : public Entity {
public:
    void Init(InitArgs args);

    static TypeID TYPEID;
    static const char* GetStaticName() { return "e_tpillar"; }
    static EntityDependendies GetStaticDependencies();
};
