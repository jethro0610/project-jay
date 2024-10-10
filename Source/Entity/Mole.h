#pragma once
#include "Entity.h"

class RisePillar;

class Mole : public Entity {
public:
    void Init(InitArgs args);

    static TypeID TYPEID;
    static const char* GetStaticName() { return "e_mole"; }
    static EntityDependendies GetStaticDependencies();
    static constexpr int NUM_PILLARS = 16;

    void Update();
    RisePillar* pillars_[NUM_PILLARS];

    bool DistantFromPillars(glm::vec3 pos);
};
