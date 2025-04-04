#pragma once
#include "Entity.h"

class SnakeHead;

class SnakeSegment : public Entity {
public:
    void Init(InitArgs args);

    static TypeID TYPEID;
    static const char* GetStaticName() { return "e_snakesegment"; }
    static EntityDependendies GetStaticDependencies();

    Entity* prevSegment_;
    float distFromPrev_;
    SnakeHead* head_;

    void Update();
    void PreUpdate();
};
