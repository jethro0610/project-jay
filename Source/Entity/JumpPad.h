#pragma once
#include "Entity.h"

class JumpPad : public Entity {
public:
    void Init(InitArgs args);

    static TypeID TYPEID;
    static constexpr const char* GetStaticName() { return "e_jumppad"; }
    static EntityDependendies GetStaticDependencies();
    EntityProperties GetStaticProperties();

    int numSeeds_;
    int cooldown_;
    int timer_;
    float jumpStregth_;
    Entity* lastOverlappedEntity_;

    void Update(); 
    void OnOverlap(Entity* overlappedEntity);
    void OnHitlagEnd();
};
