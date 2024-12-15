#pragma once
#include "Entity.h"

class Apple;

class Pig : public Entity {
public:
    void Init(InitArgs args);

    static TypeID TYPEID;
    static const char* GetStaticName() { return "e_pig"; }
    static EntityDependendies GetStaticDependencies();

    glm::vec3 origin_;
    glm::vec3 wanderTarget_;
    bool wandering_;
    Apple* target_;
    int seeds_;
    int wanderTimer_ = 0;

    void Update();
    void FindTargetApple();
    void OnCaptureSeed();
    void OnHurt(HurtArgs args);
};
