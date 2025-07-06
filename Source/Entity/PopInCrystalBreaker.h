#pragma once
#include "Entity.h"

class PopInCrystal;

class PopInCrystalBreaker : public Entity {
public:
    void Init(InitArgs args);
    void OnHurt(HurtArgs args);
    void Start();
    void Update();
    void OnOverlap(Entity* overlappedEntity);

    static TypeID TYPEID;
    static const char* GetStaticName() { return "e_popincrystalbreaker"; }
    static EntityDependendies GetStaticDependencies();
    EntityProperties GetStaticProperties();

    PopInCrystal* target_;
    int targetId_;
    int timer_;
    glm::vec3 launchPos_;
    bool launched_;
};
