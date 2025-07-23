#pragma once
#include "Entity.h"

class DynamicBubble;

class HillBurrow : public Entity {
public:
    void Init(InitArgs args);

    static TypeID TYPEID;
    static const char* GetStaticName() { return "e_hillburrow"; }
    static EntityDependendies GetStaticDependencies();
    EntityProperties GetStaticProperties();

    void Start();
    void PreUpdate();
    void Update();
    void OnDestroy();
    void OnOverlap(Entity* overlappedEntity);

    int timer_;
    float maxHeight_;
    int maxHp_;
    int hp_;

    DynamicBubble* bubble_;
};
