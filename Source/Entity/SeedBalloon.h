#pragma once
#include "Entity.h"

class SeedBalloon : public Entity {
public:
    void Init(InitArgs args);
    void OnOverlap(Entity* overlappedEntity);
    void OnDestroy();

    static TypeID TYPEID;
    static const char* GetStaticName() { return "e_seedballoon"; }
    static EntityDependendies GetStaticDependencies();
    EntityProperties GetStaticProperties();

    Entity* lastOverlapped_;

    int seeds_;
    int GetSeeds();
};
