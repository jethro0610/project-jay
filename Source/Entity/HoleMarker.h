#pragma once
#include "Entity.h"

class HoleMarker : public Entity {
public:
    void Init(InitArgs args);

    static TypeID TYPEID;
    static const char* GetStaticName() { return "e_holemark"; }
    static EntityDependendies GetStaticDependencies();
    EntityProperties GetStaticProperties();
};
