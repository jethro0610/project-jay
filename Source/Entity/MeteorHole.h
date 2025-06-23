#pragma once
#include "Entity.h"
#include "Terrain/DynamicFader.h"

class DynamicNegative;
class DynamicBubble;

class MeteorHole : public Entity {
public:
    void Init(InitArgs args);
    void Start();
    void OnDestroy();
    void PreUpdate();
    void EditorUpdate();
    void Boom();

    static TypeID TYPEID;
    static const char* GetStaticName() { return "e_meteorhole"; }
    static EntityDependendies GetStaticDependencies();
    EntityProperties GetStaticProperties();

    DynamicFader fader_;
    DynamicNegative* negative_;
    DynamicBubble* bubble_;

    float radius_;
    float bubbleRadius_;
    float height_;
};
