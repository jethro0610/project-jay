#pragma once
#include "Entity.h"
#include "Terrain/DynamicFader.h"

class DynamicBubble;
class CometSpawner;

class Comet: public Entity {
public:
    void Init(InitArgs args);

    static TypeID TYPEID;
    static const char* GetStaticName() { return "e_comet"; }
    static EntityDependendies GetStaticDependencies(); 

    ParticleEmitter* cometTrail_;
    int seeds_;
    DynamicBubble* bubble_;
    DynamicFader fader_;
    bool contact_;
    CometSpawner* spawner_;

    void PreUpdate();
    void Update(); 
    void RenderUpdate();
    void OnDestroy();
    void Start();
    void OnHitlagEnd();
    void OnHurt(HurtArgs args);
    int GetSeeds();
};
