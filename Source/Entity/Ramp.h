#pragma once
#include "Entity.h"
#include "Terrain/Shared_DynamicTerrainModifier.h"

class Ramp : public Entity {
public:
    void Init(InitArgs args);

    static TypeID TYPEID;
    static const char* GetStaticName() { return "e_ramp"; }
    static EntityDependendies GetStaticDependencies();
    EntityProperties GetStaticProperties();
    void OnDestroy();

    DynamicTerrainModifier* bubble_;
    bool active_;
    float activeHeight_;
    int riseTimer_;

    void PreUpdate();
    void EditorUpdate();
    void OnActivate();
};
