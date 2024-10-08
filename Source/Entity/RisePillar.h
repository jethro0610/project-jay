#pragma once
#include "Entity.h"

class RisePillar : public Entity {
public:
    void Init(InitArgs args);

    static TypeID TYPEID;
    static const char* GetStaticName() { return "e_risepillar"; }
    static EntityDependendies GetStaticDependencies();

    void Update();
    void RenderUpdate();
    void OnDestroy();
    void OnHitlagEnd();
    void OnOverlap(Entity* overlappedEntity);
    glm::vec3 GetTargetPoint();

    int timer_;
    float initialY_;
    ParticleEmitter* burrowEm_;
    ParticleEmitter* shootEm_;
    ParticleEmitter* burstEm_;
};
