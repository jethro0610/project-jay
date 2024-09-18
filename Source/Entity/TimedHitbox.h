#pragma once
#include "Entity.h"

class TimedHitbox : public Entity {
public:
    void Init(InitArgs args);

    static TypeID TYPEID;
    static const char* GetStaticName() { return "e_timedhitbox"; }
    static EntityDependendies GetStaticDependencies();
    EntityProperties GetStaticProperties();

    int timer_;
    int lifespan_;

    void Update();
    void RenderUpdate() {};
    void OnHit(HitArgs args) {};
    void OnHurt(HurtArgs args) {};
    void OnCaptureSeed() {};
    void OnDestroy() {};
    void OnPush(glm::vec3 pushVec) {};
    void OnOverlap(Entity* overlappedEntity) {};
};
