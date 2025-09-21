#pragma once
#include "Entity.h"
#include "Terrain/DynamicFader.h"

class DynamicBubble;

class BubbleEater : public Entity {
public:
    void Init(InitArgs args);
    void Start();

    static TypeID TYPEID;
    static const char* GetStaticName() { return "e_bubbleeater"; }
    static EntityDependendies GetStaticDependencies();
    EntityProperties GetStaticProperties();

    DynamicBubble* bubble_;
    DynamicFader fader_;

    void PreUpdate();
    void Update();
    void OnOverlap(Entity* overlappedEntity);
    void OnDestroy();
};
