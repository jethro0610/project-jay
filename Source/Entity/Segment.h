#pragma once
#include "Entity.h"

class Segment : public Entity {
public:
    void Init(InitArgs args);

    static TypeID TYPEID;
    static const char* GetStaticName() { return "e_segment"; }
    static EntityDependendies GetStaticDependencies();
    EntityProperties GetStaticProperties();

    void OnOverlap(Entity* overlappedEntity);

    float originalY_;
    float timeOffset_;
    void Update();
    void Start();
    void OnHitlagEnd();

    int targetSegId_;
    int* targetSegments_;
};
