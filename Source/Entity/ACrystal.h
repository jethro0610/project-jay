#pragma once
#include "Entity.h"

class ACrystal : public Entity {
public:
    void Init(InitArgs args);
    void Start();
    void Update();

    static TypeID TYPEID;
    static const char* GetStaticName() { return "e_acrystal"; }
    static EntityDependendies GetStaticDependencies();
    EntityProperties GetStaticProperties();

    bool hasTriggered_;

    int numSegments_;
    int segmentId_;
    int minSegments_;

    int triggerEntityId_;
    Entity* triggerEntity_;
};
