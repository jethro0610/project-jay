#pragma once
#include "Entity.h"

class Bumper : public Entity {
public:
    void Init(InitArgs args);

    static TypeID TYPEID;
    static const char* GetStaticName() { return "e_bumper"; }
    static EntityDependendies GetStaticDependencies();
    EntityProperties GetStaticProperties();

    float speed_;
    float friction_;
    float kbMult_;
    float gravity_;

    bool respawnOnFall_;
    int seedOnFall_;
    glm::vec3 spawnPos_;

    Entity* player_;

    void Start();
    void Update();
    void OnHurt(HurtArgs args);
};
