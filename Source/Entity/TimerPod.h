#pragma once
#include "Entity.h"

class TimerPod : public Entity {
public:
    void Init(InitArgs args);

    static TypeID TYPEID;
    static const char* GetStaticName() { return "e_timerpod"; }
    static EntityDependendies GetStaticDependencies();
    EntityProperties GetStaticProperties();

    int timer_;
    int cooldown_;
    int seedAmount_;
    bool hitOnly_;

    void Update(); 
    void OnHurt(HurtArgs args);
    void OnOverlap(Entity* overlappedEntity);
};
