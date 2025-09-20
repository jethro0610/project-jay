#pragma once
#include "Entity.h"

class HoleMarker;

class Bumper : public Entity {
public:
    void Init(InitArgs args);

    static TypeID TYPEID;
    static const char* GetStaticName() { return "e_bumper"; }
    static EntityDependendies GetStaticDependencies();
    EntityProperties GetStaticProperties();

    float speed_;
    float timer_;

    glm::vec3 spawnPos_;

    Entity* lastAttacker_;
    Entity* target_;
    glm::vec3 travelPos_;
    int travelTimer_;
    bool canTarget_;
    bool overlappingHole_;
    bool powered_;
    bool fell_;

    void Start();
    void PreUpdate();
    void OnHurt(HurtArgs args);

    void StartTracking(Entity* target);
    void StopTracking();
    void OnOverlap(Entity* overlappedEntity);
    bool OnFallInHole();
    void OnCaptureSeed();
};
