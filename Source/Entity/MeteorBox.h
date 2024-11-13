#pragma once
#include "Entity.h"

class MeteorBox : public Entity {
public:
    void Init(InitArgs args);

    static TypeID TYPEID;
    static const char* GetStaticName() { return "e_meteor_box"; }
    static EntityDependendies GetStaticDependencies();

    void Update();
};
