#pragma once
#include "Entity.h"

class HomingBossPoint;
class HomingBossProjectile;
class Player;

class HomingBoss : public Entity {
public:
    void Init(InitArgs args);
    void Start();
    void OnOverlap(Entity* overlappedEntity);
    void Update();
    void ActivatePoints();
    void LaunchProjectile();
    bool AllPointsHit();

    static TypeID TYPEID;
    static const char* GetStaticName() { return "e_homingboss"; }
    static EntityDependendies GetStaticDependencies();
    EntityProperties GetStaticProperties();

    static constexpr int NUM_POINTS = 4;

    int phase_;
    int damage_;
    int overlapCooldown_;
    HomingBossPoint* points_[NUM_POINTS];
    Player* player_;
    HomingBossProjectile* projectile_;
    int projectileTimer_;
};
