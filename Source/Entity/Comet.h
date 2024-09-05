#pragma once
#include "Entity.h"

class Comet: public Entity {
public:
    void Init(InitArgs args);

    static TypeID TYPEID;
    static constexpr const char* GetName() { return "e_comet"; }
    static EntityDependendies GetDeps(); 

    ParticleEmitter* cometTrail_;

    void Update(); 
    void RenderUpdate();
    void OnHit(HitArgs args) {};
    void OnHurt(HurtArgs args) {};
    void OnCaptureSeed() {};
    void OnDestroy();
    void OnPush(glm::vec3 pushVec) {};
    void OnOverlap(Entity* overlappedEntity) {};
};
