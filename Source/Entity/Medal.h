#pragma once
#include "Entity.h"

class Medal : public Entity {
public:
    void Init(InitArgs args);
    void OnDestroy();
    void OnOverlap(Entity* overlappedEntity);

    static TypeID TYPEID;
    static const char* GetStaticName() { return "e_medal"; }
    static EntityDependendies GetStaticDependencies();
    EntityProperties GetStaticProperties();
};
