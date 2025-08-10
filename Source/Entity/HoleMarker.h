#pragma once
#include "Entity.h"
#include "Terrain/DynamicFader.h"

class DynamicAdditive;
class DynamicBubble;

class HoleMarker : public Entity {
public:
    void Init(InitArgs args);
    void Start();
    void Update();
    void EditorUpdate();
    void RenderUpdate();

    static TypeID TYPEID;
    static const char* GetStaticName() { return "e_holemark"; }
    static EntityDependendies GetStaticDependencies();
    EntityProperties GetStaticProperties();

    void EntityFellInHole(Entity* entity);

    DynamicFader fader_;
    DynamicAdditive* additive_;
    DynamicBubble* bubble_;
    Entity* player_;
    float explodeTime_;
    int numEntities_;
    int entityCount_;
    int seedsPerEntity_;
    int seedsEnd_;
    float terrainHeight_;
    int GetSeeds();
    void OnDestroy();
    void OnOverlap(Entity* overlappedEntity);
};
