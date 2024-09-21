#pragma once
#include "Entity.h"

class ItemBox : public Entity {
public:
    void Init(InitArgs args);

    static TypeID TYPEID;
    static const char* GetStaticName() { return "e_itembox"; }
    static EntityDependendies GetStaticDependencies();

    void OnHurt(HurtArgs args);
};
