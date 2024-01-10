#pragma once
#include "Entity.h"

class RabbitPole : public Entity {
public:
    void Init(InitArgs args);

    static TypeID TYPEID;
    static constexpr const char* GetName() { return "e_rabbit_pole"; }
    static EntityDependendies GetDeps(); 

    void Update() {}; 
    void RenderUpdate() {};
    void OnHit() {};
    void OnHurt() {};
    void OnCaptureSeed() {};
    void OnDestroy() {};
    void OnPush(glm::vec3 pushVec) {};
};
