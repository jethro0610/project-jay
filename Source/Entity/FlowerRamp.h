#pragma once
#include "Entity.h"
#include "Terrain/DynamicFader.h"

class DynamicBubble;

class FlowerRamp : public Entity {
public:
    void Init(InitArgs args);

    static TypeID TYPEID;
    static const char* GetStaticName() { return "e_framp"; }
    static EntityDependendies GetStaticDependencies();
    EntityProperties GetStaticProperties();

    void Update();
    void Start();
    void EditorUpdate();
    void OnDestroy();
    void OnWet();

    DynamicBubble* bubble_;
    DynamicFader fader_;
    int maxWet_;
    int numWet_;
};
