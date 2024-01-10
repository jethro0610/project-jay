#pragma once
#include "Entity.h"

class MagicSeed : public Entity {
public:
    void Init(InitArgs args);

    static constexpr TypeID GetTypeID() { return 5; };
    static constexpr const char* GetName() { return "e_magicseed"; }
    static EntityDependendies GetDeps();

    void Update() {}; 
    void RenderUpdate() {};
    void OnHit() {};
    void OnHurt();
    void OnCaptureSeed() {};
    void OnDestroy() {};
};
