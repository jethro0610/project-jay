#pragma once
#include "Entity.h"

class Apple;

class Pig : public Entity {
public:
    void Init(InitArgs args);

    static TypeID TYPEID;
    static const char* GetStaticName() { return "e_pig"; }
    static EntityDependendies GetStaticDependencies();

    Apple* target_;
    int seeds_;

    void Update();
    void FindTargetApple();
    void OnCaptureSeed();
    void OnHurt(HurtArgs args);
};
