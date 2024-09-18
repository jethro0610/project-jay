#pragma once
#include "Entity.h"

class JumpPad : public Entity {
public:
    void Init(InitArgs args);

    static TypeID TYPEID;
    static constexpr const char* GetName() { return "e_jumppad"; }
    static EntityDependendies GetDeps();
    EntityProperties GetStaticProperties();

    int numSeeds_;
    int cooldown_;
    int timer_;
    float jumpStregth_;
    Entity* lastOverlappedEntity_;

    void Update(); 
    void RenderUpdate() {};
    void OnHit(HitArgs args) {};
    void OnHurt(HurtArgs args) {};
    void OnCaptureSeed() {};
    void OnDestroy() {};
    void OnPush(glm::vec3 pushVec) {}
    void OnOverlap(Entity* overlappedEntity);
    void OnHitlagEnd();
};
