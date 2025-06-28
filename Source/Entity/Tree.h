#pragma once
#include "Entity.h"

class Apple;

class Tree : public Entity {
public:
    void Init(InitArgs args);

    static TypeID TYPEID;
    static const char* GetStaticName() { return "e_tree"; }
    static EntityDependendies GetStaticDependencies();
    EntityProperties GetStaticProperties();
    static constexpr int MAX_APPLES = 20;

    int seedsPerApple_;
    int numApples_;
    Apple* apples_[MAX_APPLES];
    void Start();
    void Update();
    void OnHurt(HurtArgs args);
    int GetSeeds();
};
