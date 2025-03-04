#pragma once
#include "Entity.h"

class Bird : public Entity {
public:
    void Init(InitArgs args);

    static TypeID TYPEID;
    static const char* GetStaticName() { return "e_bird"; }
    static EntityDependendies GetStaticDependencies();

    void OnOverlap(Entity* overlappedEntity);
    void Update();

    glm::vec3 startPos_;
    glm::vec3 sinOffset_;
};
