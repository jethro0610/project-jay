#pragma once
#include "Entity.h"

class Comet: public Entity {
public:
    void Init(InitArgs args);

    static TypeID TYPEID;
    static const char* GetStaticName() { return "e_comet"; }
    static EntityDependendies GetStaticDependencies(); 

    ParticleEmitter* cometTrail_;
    int seeds_;

    void Update(); 
    void RenderUpdate();
    void OnDestroy();
};
