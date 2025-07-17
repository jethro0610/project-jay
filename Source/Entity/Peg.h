#pragma once
#include "Entity.h"

class Peg : public Entity {
public:
    void Init(InitArgs args);
    void OnDestroy();
    int GetSeeds() { return seeds_; }

    static TypeID TYPEID;
    static const char* GetStaticName() { return "e_peg"; }
    static EntityDependendies GetStaticDependencies();
    EntityProperties GetStaticProperties();

    void OnOverlap(Entity* overlappedEntity);

    Entity* lastOverlap_;
    int seeds_;
    float jumpStrength_;

    glm::vec3 GetTargetPoint();
};
