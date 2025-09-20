#pragma once
#include "Entity.h"

class BreakerPillar : public Entity {
public:
    void Init(InitArgs args);

    static TypeID TYPEID;
    static const char* GetStaticName() { return "e_breakerpillar"; }
    static EntityDependendies GetStaticDependencies();
    EntityProperties GetStaticProperties();

    void OnOverlap(Entity* overlappedEntity);
};
