#pragma once
#include "Entity.h"

class Water : public Entity {
public:
    void Init(InitArgs args);

    static TypeID TYPEID;
    static const char* GetStaticName() { return "e_water"; }
    static EntityDependendies GetStaticDependencies();
    EntityProperties GetStaticProperties();

    void Update();
    void Shoot(const glm::vec3& target);
    void OnOverlap(Entity* overlappedEntity);
    void OnDestroy();
};
