#pragma once
#include "Entity.h"

class Apple : public Entity {
public:
    void Init(InitArgs args);

    static TypeID TYPEID;
    static const char* GetStaticName() { return "e_apple"; }
    static EntityDependendies GetStaticDependencies();
    static constexpr float GRAVITY = 0.5f;

    float growth_;
    bool active_;
    glm::vec3 initialScale_;
    Entity* overlappedEntity_;
    void Update();
    void OnOverlap(Entity* overlappedEntity);
    void OnHitlagEnd();
};
