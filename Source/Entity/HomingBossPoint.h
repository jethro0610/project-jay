#pragma once
#include "Entity.h"
#include "Terrain/Shared_DynamicNegative.h"
#include "Terrain/Shared_DynamicBubble.h"
#include "Terrain/DynamicFader.h"
#include <vector_const.h>

class HomingBossPoint : public Entity {
public:
    void Init(InitArgs args);
    void Start();
    void LateStart();
    void ActivateInTicks(int ticks);
    void Activate();
    void Reset();
    void OnDestroy();
    void Update();
    void EditorUpdate();

    static TypeID TYPEID;
    static const char* GetStaticName() { return "e_homingbosspoint"; }
    static EntityDependendies GetStaticDependencies();
    EntityProperties GetStaticProperties();

    static const int MAX_CONTAINED = 4;
    DynamicNegative* negative_;
    DynamicBubble* bubble_;
    DynamicFader fader_;
    int activateTicks_;
    vector_const<Entity*, MAX_CONTAINED> containedEntities_;
    glm::vec3 entityScales_[MAX_CONTAINED];
};
