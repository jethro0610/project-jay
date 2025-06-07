#pragma once
#include "Entity.h"

class HoleMarker : public Entity {
public:
    void Init(InitArgs args);
    void Start();
    void EditorUpdate();
    void RenderUpdate();

    static TypeID TYPEID;
    static const char* GetStaticName() { return "e_holemark"; }
    static EntityDependendies GetStaticDependencies();
    EntityProperties GetStaticProperties();

    void EntityFellInHole(Entity* entity);

    Entity* player_;
    float explodeTime_;
};
