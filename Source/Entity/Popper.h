#pragma once
#include "Entity.h"
#include "Terrain/DynamicFader.h"

class DynamicBubble;

class Popper : public Entity {
public:
    void Init(InitArgs args);

    static TypeID TYPEID;
    static const char* GetStaticName() { return "e_popper"; }
    static EntityDependendies GetStaticDependencies();
    EntityProperties GetStaticProperties();

    DynamicBubble* bubble_;
    DynamicFader fader_;

    float speed_;
    float targetDistFromEdge_;

    int maxPopTime_;
    int popTime_;

    int popDelay_;
    int popDelayTime_;

    void PreUpdate();
    void Start();
    void OnHurt(HurtArgs args);
    void OnDestroy();
};
