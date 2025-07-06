#pragma once
#include "Entity.h"

class CometSpawner : public Entity {
public:
    void Init(InitArgs args);

    static TypeID TYPEID;
    static const char* GetStaticName() { return "e_cometspawner"; }
    static EntityDependendies GetStaticDependencies(); 
    EntityProperties GetStaticProperties();

    int timer_;
    float spawnRadius_;
    int interval_;
    int seeds_;
    int simulComets_;
    int numComets_;
    int maxComets_;
    int totalComets_;
    bool doTimer_;

    void Update(); 
    void OnCometDestroyed();
    int GetSeeds();
};
