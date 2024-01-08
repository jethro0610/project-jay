#pragma once
#include "Entity.h"

class RabbitPole : public Entity {
public:
    void Init(InitArgs args);

    static constexpr TypeID GetTypeID() { return 4; };
    static constexpr const char* GetName() { return "e_rabbit_pole"; }
    void Update() {}; 
    void RenderUpdate() {};
    void OnHit() {};
    void OnHurt() {};
    void OnCaptureSeed() {};
};
