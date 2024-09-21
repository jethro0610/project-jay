#pragma once
#include "Entity.h"

class TimedHitbox : public Entity {
public:
    void Init(InitArgs args);

    static TypeID TYPEID;
    static const char* GetStaticName() { return "e_timedhitbox"; }
    static EntityDependendies GetStaticDependencies();
    EntityProperties GetStaticProperties();

    int timer_;
    int lifespan_;

    void Update();
};
