#pragma once
#include "Entity.h"
#include "Terrain/DynamicFader.h"

class DynamicNegative;
class DynamicBubble;

class MeteorHole : public Entity {
public:
    void Init(InitArgs args);
    void OnDestroy();
    void PreUpdate();
    void EditorUpdate();

    static TypeID TYPEID;
    static const char* GetStaticName() { return "e_meteorhole"; }
    static EntityDependendies GetStaticDependencies();
    EntityProperties GetStaticProperties();

    DynamicNegative* negative_;
    DynamicBubble* bubble_;
};
