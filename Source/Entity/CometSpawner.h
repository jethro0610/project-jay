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
    int minInterval_;
    int maxInterval_;

    void Update(); 
};
