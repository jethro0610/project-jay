#pragma once
#include "Entity.h"
#include "Item.h"

class Player;

class ItemToken : public Entity {
public:
    void Init(InitArgs args);

    static TypeID TYPEID;
    static const char* GetStaticName() { return "e_itemtoken"; }
    static EntityDependendies GetStaticDependencies();

    Player* player_;
    Item item_;
    float lifetime_;
    float duration_;
    glm::vec3 basePosition_;
    glm::vec3 spokeVelocity_;

    void Update();
};
