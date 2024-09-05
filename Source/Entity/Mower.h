#pragma once
#include "Entity.h"

class Mower : public Entity {
public:
    void Init(InitArgs args);

    static TypeID TYPEID;
    static constexpr const char* GetName() { return "e_mower"; }
    static EntityDependendies GetDeps();
    enum Mode {
        Cut,
        Sleep
    };

    Entity* target_;
    int numberOfSeeds_;
    Mode mode_;

    void Update(); 
    void RenderUpdate() {};
    void OnHit(HitArgs args) {};
    void OnHurt(HurtArgs args) {};
    void OnCaptureSeed() {};
    void OnDestroy();
    void OnPush(glm::vec3 pushVec) {}
    void OnOverlap(Entity* overlappedEntity) {};

    void SetMode(Mode mode);
    void ToggleMode();
};
