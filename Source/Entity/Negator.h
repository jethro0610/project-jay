#pragma once
#include "Terrain/Shared_DynamicTerrainModifier.h"
#include "Entity.h"

class Negator : public Entity {
public:
    void Init(InitArgs args);
    void Start();
    void OnDestroy();
    void Update();

    static TypeID TYPEID;
    static const char* GetStaticName() { return "e_negator"; }
    static EntityDependendies GetStaticDependencies();
    EntityProperties GetStaticProperties();

    DynamicTerrainModifier* negativeModifier_;
    bool initialActive_;

    #ifdef _DEBUG
    void EditorUpdate();
    #endif
};
