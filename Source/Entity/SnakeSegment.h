#pragma once
#include "Entity.h"

class SnakeSegment : public Entity {
public:
    void Init(InitArgs args);

    static TypeID TYPEID;
    static const char* GetStaticName() { return "e_snakesegment"; }
    static EntityDependendies GetStaticDependencies();

    Entity* target_;
    float dist_;

    void Update();
};
