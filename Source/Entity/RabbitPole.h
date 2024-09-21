#pragma once
#include "Entity.h"

class RabbitPole : public Entity {
public:
    void Init(InitArgs args);

    static TypeID TYPEID;
    static const char* GetStaticName() { return "e_rabbitpole"; }
    static EntityDependendies GetStaticDependencies(); 
};
