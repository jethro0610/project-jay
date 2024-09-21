#pragma once
#include "Entity.h"

class GroundSpawnProjectile : public Entity {
public:
    void Init(InitArgs args);

    static TypeID TYPEID;
    static const char* GetStaticName() { return "e_groundspawnproj"; }
    static EntityDependendies GetStaticDependencies();
    EntityProperties GetStaticProperties();

    float gravity_;
    TypeID spawn_;
    glm::vec3 spawnScale_;
    EntityPropertiesAssigner spawnProperties_;

    void Update(); 
    void OnDestroy();
};
