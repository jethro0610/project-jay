#pragma once
#include "Entity.h"

class PillarPad : public Entity {
public:
    void Init(InitArgs args);

    static TypeID TYPEID;
    static const char* GetStaticName() { return "e_pillarpad"; }
    static EntityDependendies GetStaticDependencies();
    EntityProperties GetStaticProperties();

    void OnOverlap(Entity* overlappedEntity);
    void OnHurt(HurtArgs args);
    glm::vec3 GetTargetPoint();
    float jumpStrength_;
    bool activated_;
};
