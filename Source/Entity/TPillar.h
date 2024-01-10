#pragma once
#include "Entity.h"

class TPillar : public Entity {
public:
    void Init(InitArgs args);

    static constexpr TypeID GetTypeID() { return 3; };
    static constexpr const char* GetName() { return "e_tpillar"; }
    static EntityDependendies GetDeps();

    void Update() {}; 
    void RenderUpdate() {};
    void OnHit() {};
    void OnHurt() {};
    void OnCaptureSeed() {};
    void OnDestroy() {};
    void OnPush(glm::vec3 pushVec) {};
};
