#pragma once
#include "Entity.h"
#include "Item.h"

class ItemContainer : public Entity {
public:
    void Init(InitArgs args);

    static TypeID TYPEID;
    static const char* GetStaticName() { return "e_itemcontainer"; }
    static EntityDependendies GetStaticDependencies();
    EntityProperties GetStaticProperties();

    Item item_;
    int numItem_;

    void OnHurt(HurtArgs args);
};
