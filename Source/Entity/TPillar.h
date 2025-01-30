#pragma once
#include "Entity.h"
#include "Terrain/Shared_DynamicTerrainModifier.h"

class TPillar : public Entity {
public:
    void Init(InitArgs args);

    static TypeID TYPEID;
    static const char* GetStaticName() { return "e_tpillar"; }
    static EntityDependendies GetStaticDependencies();

    DynamicTerrainModifier* bubble_;
    void OnDestroy();
    void Update();
    void EditorUpdate();
};
