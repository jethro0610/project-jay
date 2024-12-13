#pragma once
#include "Entity.h"

class Apple;

class Tree : public Entity {
public:
    void Init(InitArgs args);

    static TypeID TYPEID;
    static const char* GetStaticName() { return "e_tree"; }
    static EntityDependendies GetStaticDependencies();
    static constexpr int MAX_APPLES = 8;

    Apple* apples_[MAX_APPLES];
    void Start();
    void Update();
    void OnHurt(HurtArgs args);
};
