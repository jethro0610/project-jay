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
    static const int MAX_SEGMENTS = 32;

    int numSegments_;
    float segmentDist_;
    float speed_;

    float segmentScale_;
    float podScale_;

    void Update();
};
