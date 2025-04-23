#pragma once
#include "Entity.h"

class Peg : public Entity {
public:
    void Init(InitArgs args);

    static TypeID TYPEID;
    static const char* GetStaticName() { return "e_peg"; }
    static EntityDependendies GetStaticDependencies();
    EntityProperties GetStaticProperties();

    void OnOverlap(Entity* overlappedEntity);
};
