#pragma once
#include "Entity.h"

class MagicSeed : public Entity {
public:
    void Init(InitArgs args);

    static TypeID TYPEID;
    static constexpr const char* GetName() { return "e_magicseed"; }
    static EntityDependendies GetDeps();

    void Update() {}; 
    void RenderUpdate() {};
    void OnHit() {};
    void OnHurt();
    void OnCaptureSeed() {};
    void OnDestroy() {};
};
