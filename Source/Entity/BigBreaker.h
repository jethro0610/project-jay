#pragma once
#include "Entity.h"

class BigBreaker : public Entity {
public:
    void Init(InitArgs args);
    void OnOverlap(Entity* overlappedEntity);
    void OnDestroy();
    void Update();
    int GetSeeds() { return seeds_; }

    static TypeID TYPEID;
    static const char* GetStaticName() { return "e_bigbreaker"; }
    static EntityDependendies GetStaticDependencies();
    EntityProperties GetStaticProperties();

    int hp_;
    int seeds_;
    int jumpStrength_;
    int timer_;
    Entity* lastOverlapped_;
};
