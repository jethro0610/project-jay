#pragma once
#include "Terrain/Shared_DynamicNegative.h"
#include "Entity.h"

class Negator : public Entity {
public:
    void Init(InitArgs args);
    void Start();
    void OnDestroy();
    void Update();
    void OnTrigger();
    bool GetIsTriggered();

    static TypeID TYPEID;
    static const char* GetStaticName() { return "e_negator"; }
    static EntityDependendies GetStaticDependencies();
    EntityProperties GetStaticProperties();

    DynamicNegative* negativeModifier_;
    bool initialActive_;

    #ifdef _DEBUG
    void EditorUpdate();
    #endif
};
