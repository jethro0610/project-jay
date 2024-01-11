#pragma once
#include "Entity.h"

class MultiHitPod : public Entity {
public:
    void Init(InitArgs args);

    static TypeID TYPEID;
    static constexpr const char* GetName() { return "e_multihitpod"; }
    static EntityDependendies GetDeps();

    static constexpr int MAX_COOLDOWN_TICKS = 5;

    int cooldown_;
    bool disableCollision_;

    void Update(); 
    void RenderUpdate() {};
    void OnHit() {};
    void OnHurt();
    void OnCaptureSeed() {};
    void OnDestroy() {};
    void OnPush(glm::vec3 pushVec) {}
};
