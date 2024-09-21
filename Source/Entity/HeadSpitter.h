#pragma once
#include "Entity.h"

class HeadSpitter : public Entity {
public:
    void Init(InitArgs args);

    static TypeID TYPEID;
    static const char* GetStaticName() { return "e_headspitter"; }
    static EntityDependendies GetStaticDependencies();
    EntityProperties GetStaticProperties();

    int timer_;

    void Update(); 
};
