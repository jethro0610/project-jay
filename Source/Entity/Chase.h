#pragma once
#include "Entity.h"

class Player;

class Chase : public Entity {
public:
    void Init(InitArgs args);

    static TypeID TYPEID;
    static const char* GetStaticName() { return "e_chase"; }
    static EntityDependendies GetStaticDependencies();
    EntityProperties GetStaticProperties();

    float friction_;
    float speed_;
    Player* player_;
    bool returning_;
    int stunEndTimer_;
    glm::vec3 spawnPoint_;

    void Update();
    void Start();
    void OnHurt(HurtArgs args);
};
