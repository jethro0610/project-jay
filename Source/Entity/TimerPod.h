#pragma once
#include "Entity.h"

class TimerPod : public Entity {
public:
    void Init(InitArgs args);

    static TypeID TYPEID;
    static constexpr const char* GetName() { return "e_timerpod"; }
    static EntityDependendies GetDeps();
    EntityProperties GetProperties();

    int timer_;
    int seedAmount_;

    void Update(); 
    void RenderUpdate() {};
    void OnHit(HitArgs args) {};
    void OnHurt(HurtArgs args);
    void OnCaptureSeed() {};
    void OnDestroy() {};
    void OnPush(glm::vec3 pushVec) {};
    void OnOverlap(Entity* overlappedEntity) {};
};
