#pragma once
#include "Entity.h"

class SnakeHead;

class SnakePod : public Entity {
public:
    void Init(InitArgs args);

    static TypeID TYPEID;
    static const char* GetStaticName() { return "e_snakepod"; }
    static EntityDependendies GetStaticDependencies();

    void Update();
    void PreUpdate();
    void OnHurt(HurtArgs args);

    Entity* prevSegment_;
    Entity* nextSegment_;
    SnakeHead* head_;
};
