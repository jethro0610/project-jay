#pragma once
#include "Entity.h"

class Player;

class Medal : public Entity {
public:
    void Init(InitArgs args);
    void Update();
    void OnDestroy();
    void OnOverlap(Entity* overlappedEntity);
    void Start();

    static TypeID TYPEID;
    static const char* GetStaticName() { return "e_medal"; }
    static EntityDependendies GetStaticDependencies();
    EntityProperties GetStaticProperties();

    Player* player_;
    bool collected_;
    int collectTimer_;
    Transform spawnTransform_;
};
