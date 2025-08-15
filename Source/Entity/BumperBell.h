#pragma once
#include "Entity.h"

class BumperBell : public Entity {
public:
    void Init(InitArgs args);

    static TypeID TYPEID;
    static const char* GetStaticName() { return "e_bumperbell"; }
    static EntityDependendies GetStaticDependencies();
    EntityProperties GetStaticProperties();

    void OnOverlap(Entity* overlappedEntity);
    glm::vec3 GetTargetPoint();
    int timer_;
    void Update();
};
