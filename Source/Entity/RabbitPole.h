#pragma once
#include "Entity.h"

class RabbitPole : public Entity {
public:
    void Init(InitArgs args);

    static constexpr TypeID GetTypeID() { return 4; };
    static constexpr const char* GetName() { return "e_rabbit_pole"; }
    static constexpr EntityDependendies DEPS = {
        true,
        "st_rabbit_pole",
        2,
        {
            "t_granite_c",
            "t_granite_n"
        }
    };
    void Update() {}; 
    void RenderUpdate() {};
    void OnHit() {};
    void OnHurt() {};
    void OnCaptureSeed() {};
};
