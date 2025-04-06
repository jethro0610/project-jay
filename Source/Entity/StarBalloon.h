#pragma once
#include "Entity.h"

class StarBalloon : public Entity {
public:
    void Init(InitArgs args);

    static TypeID TYPEID;
    static const char* GetStaticName() { return "e_starballoon"; }
    static EntityDependendies GetStaticDependencies();

    void OnOverlap(Entity* overlappedEntity);

    float originalY_;
    float timeOffset_;
    void Update();
    void Start();
    void OnHitlagEnd();
};
