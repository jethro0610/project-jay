#pragma once
#include "Entity.h"

class HomingBossPoint;
class HomingBossAttack;
class Player;

class HomingBoss : public Entity {
public:
    void Init(InitArgs args);
    void Start();
    void OnOverlap(Entity* overlappedEntity);
    void Update();
    void ActivatePoints();
    void OnAttackHit();
    void ActivateRandomAttack();
    bool IsValidAttack(HomingBossAttack* attack);

    static TypeID TYPEID;
    static const char* GetStaticName() { return "e_homingboss"; }
    static EntityDependendies GetStaticDependencies();
    EntityProperties GetStaticProperties();

    static constexpr int NUM_POINTS = 4;
    static constexpr int NUM_ATTACKS = 12;

    int phase_;
    int damage_;
    int overlapCooldown_;
    HomingBossPoint* points_[NUM_POINTS];
    Player* player_;
    HomingBossAttack* attacks_[NUM_ATTACKS];
    glm::vec3 centerPoint_;
    float centerPointRot_;
    bool guard_;
};
