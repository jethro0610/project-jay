#pragma once
#include "Entity.h"

class SpeedBoost : public Entity {
public:
    void Init(InitArgs args);

    static TypeID TYPEID;
    static const char* GetStaticName() { return "e_speedboost"; }
    static EntityDependendies GetStaticDependencies();

    float speed_;
    int seed_;
    int cooldown_;
    int timer_;

    void Update();
    void OnOverlap(Entity* overlappedEntity);
};
