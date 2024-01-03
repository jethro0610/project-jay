#pragma once
#include "Collider.h"
#include "Types/Transform.h"
#include <vector_holed.h>

class CollisionManager {
public: 
    static constexpr int MAX_PUSHBOXES = 128;
    static constexpr int MAX_HITBOXES = 128;
    static constexpr int MAX_HURTBOXES = 128;

    Collider* RequestPushbox();
    Collider* RequestHitbox();
    Collider* RequestHurtbox();

    void DoPushes();
    void DoHits();

private:
    vector_holed<Collider, MAX_PUSHBOXES> pushboxes_;   
    vector_holed<Collider, MAX_HITBOXES> hitboxes_;   
    vector_holed<Collider, MAX_HURTBOXES> hurtboxes_;   
};
