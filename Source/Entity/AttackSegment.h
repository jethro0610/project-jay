#pragma once
#include "Entity.h"

class AttackSegment : public Entity {
public:
    void Init(InitArgs args);
    void Start();
    void OnHitlagEnd();

    static TypeID TYPEID;
    static const char* GetStaticName() { return "e_atkseg"; }
    static EntityDependendies GetStaticDependencies();
    EntityProperties GetStaticProperties();

    int targetSegId_;
    int* targetSegCount_;
};
