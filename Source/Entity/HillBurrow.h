#pragma once
#include "Entity.h"

class DynamicBubble;

class HillBurrow : public Entity {
public:
    void Init(InitArgs args);

    static TypeID TYPEID;
    static const char* GetStaticName() { return "e_hillburrow"; }
    static EntityDependendies GetStaticDependencies();
    EntityProperties GetStaticProperties();

    void Start();
    void PreUpdate();
    void Update();
    void EditorUpdate();
    void OnDestroy();
    void OnOverlap(Entity* overlappedEntity);
    glm::vec3 GetTargetPoint();
    int GetSeeds();

    int timer_;
    float maxHeight_;
    int maxHp_;
    int hp_;
    float boblessHeight_;
    int seedOnHit_;
    int seedOnDead_;

    DynamicBubble* bubble_;
};
