#pragma once
#include "Entity.h"

class SnakeHead : public Entity {
public:
    void Init(InitArgs args);
    void Start();

    static TypeID TYPEID;
    static const char* GetStaticName() { return "e_snake"; }
    static EntityDependendies GetStaticDependencies();
    EntityProperties GetStaticProperties();

    int numSegments_;
    float segmentDist_;
    float speed_;

    void Update();
};
