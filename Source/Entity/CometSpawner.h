#pragma once
#include "Entity.h"

class CometSpawner : public Entity {
public:
    void Init(InitArgs args);

    static TypeID TYPEID;
    static const char* GetStaticName() { return "e_cometspawner"; }
    static EntityDependendies GetStaticDependencies(); 

    int timer_;
    int interval_;

    void Update(); 
};
