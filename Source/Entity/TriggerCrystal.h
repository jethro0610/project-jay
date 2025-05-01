#pragma once
#include "Entity.h"

class TriggerCrystal : public Entity {
public:
    void Init(InitArgs args);
    void Start();
    void OnHurt(HurtArgs args);

    static TypeID TYPEID;
    static const char* GetStaticName() { return "e_tcrystal"; }
    static EntityDependendies GetStaticDependencies();
    EntityProperties GetStaticProperties();

    int triggerEntityId_;
    Entity* triggerEntity_;
    bool oneWay_;
    bool inverted_;

    glm::vec3 colorOff_;
    glm::vec3 colorOn_;
};
