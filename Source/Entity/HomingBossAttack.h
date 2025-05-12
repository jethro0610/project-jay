#pragma once
#include "Entity.h"

class HomingBoss;
class Player;

class HomingBossAttack : public Entity {
public:
    void Init(InitArgs args);

    static TypeID TYPEID;
    static const char* GetStaticName() { return "e_homingatk"; }
    static EntityDependendies GetStaticDependencies();
    EntityProperties GetStaticProperties();

    void Activate();
    void Deactivate();
    void OnHurt(HurtArgs args);
    void OnHitlagEnd();

    HomingBoss* homingBoss_;
    Player* player_;
    bool active_;
};
