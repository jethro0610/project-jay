#pragma once
#include "Entity.h"
#include "Terrain/DynamicFader.h"

class DynamicNegative;

class PopInCrystal : public Entity {
public:
    void Init(InitArgs args);
    void OnDestroy();
    void Start();
    void EditorUpdate();
    void PreUpdate();

    static TypeID TYPEID;
    static const char* GetStaticName() { return "e_popincrystal"; }
    static EntityDependendies GetStaticDependencies();
    EntityProperties GetStaticProperties();

    DynamicFader fader_;
    DynamicNegative* negative_;
    int numBreaks_;
    int maxBreaks_;

    void Break();
};
