#pragma once
#include "Entity.h"

class Bomb : public Entity {
public:
    void Init(InitArgs args);

    static TypeID TYPEID;
    static constexpr const char* GetName() { return "e_bomb"; }
    static EntityDependendies GetDeps();
    static constexpr int LIFETIME = 60 * 2;
    static constexpr int RISETIME = 15;
    static constexpr int FALLTIME = 15;
    static constexpr float DEPTH = 20.0f;

    int lifeTimer_;
    int riseTimer_;
    int fallTimer_;
    float upHeight_;
    float downHeight_;

    void Update(); 
    void RenderUpdate() {};
    void OnHit(HitArgs args) {};
    void OnHurt(HurtArgs args) {};
    void OnCaptureSeed() {};
    void OnDestroy() {};
    void OnPush(glm::vec3 pushVec) {};
};
