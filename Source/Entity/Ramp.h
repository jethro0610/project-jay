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

    void PreUpdate();
    void RenderUpdate();
    void EditorUpdate();
};
