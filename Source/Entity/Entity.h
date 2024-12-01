#pragma once
#include "Types/Transform.h"
#include "Collision/Collider.h"
#include "Collision/Hitbox.h"
#include "Rendering/Bone.h"
#include "Rendering/Model.h"
#include "Rendering/Skeleton.h"
#include "Rendering/Material.h"
#include "Entity/EntityProperties.h"
#include "TypeID.h"
#include <vector>
#include <inttypes.h>
#include <cstring>

class Camera;
class EntityList;
struct Inputs;
class Level;
class ParticleManager;
class ResourceManager;
class SeedManager;
class SpreadManager;
class Terrain;

class ParticleEmitter;

struct EntityDependendies {
    EntityDependendies(std::string mdl, std::vector<std::string> txtrs) {
        hasModel = true;
        model = mdl;
        textures = txtrs;
    }

    EntityDependendies(std::vector<std::string> txtrs) {
        hasModel = false;
        textures = txtrs;
    }

    EntityDependendies(std::string mdl) {
        hasModel = true;
        model = mdl;
        textures = {};
    }

    EntityDependendies() {
        hasModel = false;
        textures = {};
    }

    bool hasModel;
    std::string model;
    std::vector<std::string> textures;
};

class Entity {
public:
    static constexpr int MAX_TRAIL_SIZE = 256;
    struct HitArgs {
        Entity* target;
    };
    struct HurtArgs {
        Entity* attacker;
    };

    static constexpr int MAX_NAME = 32;

    struct InitArgs {
        ParticleManager& particleManager;
        ResourceManager& resourceManager;
    };

    enum Flag {
        EF_Interpolate,
        EF_GroundCheck,
        EF_StickToGround,
        EF_AlignToGround,
        EF_UseVelocity,
        EF_UseSkeleton,
        EF_SendPush,
        EF_RecievePush,
        EF_SendHits,
        EF_RecieveHits,
        EF_Overlap,
        EF_RecieveKnockback,
        EF_HurtFaceForward,
        EF_HurtFaceBack,
        EF_CaptureSeed,
        EF_Targetable,
        EF_UseTrail,
        EF_Count
    };

    void Construct(
        Camera& camera,
        EntityList& entities,
        Inputs& inputs,
        Level& level,
        SeedManager& seedManager,
        SpreadManager& spreadManager,
        Terrain& terrain
    );
    void Init(
        InitArgs args
    );

    static TypeID TYPEID;
    const char* GetName();
    EntityDependendies GetDependencies();
    static EntityDependendies GetDependencies(TypeID typeId);
    EntityProperties GetProperties();
    static TypeID GetTypeIDFromName(const std::string& name);

    TypeID typeId_;
    bool alive_;
    bool destroy_;
    uint32_t flags_; 

    Transform transform_;
    Transform lastTransform_;
    Transform renderTransform_;
    glm::vec3 velocity_;
    glm::vec3* trail_;

    float traceDistance_;
    bool onGround_;
    float groundHeight_;
    glm::vec3 lastGroundNormal_;
    glm::vec3 groundNormal_;
    bool skipGroundCheck_;

    Model* model_;
    Material materials_[Model::MAX_MESHES_PER_MODEL];

    Skeleton* skeleton_;
    Pose pose_;
    Pose renderPose_;
    int animIndex_;
    int prevAnimIndex_;
    float prevAnimTime_; 
    float animTime_;
    float transitionTime_;
    float transitionLength_;

    Collider pushbox_;
    Hitbox hitbox_;
    Collider hurtbox_;
    Collider overlapbox_;
    bool stun_;
    bool initHitlag_;
    int hitlag_;
    int hurtCooldown_;

    #ifdef _DEBUG
    Collider DBG_collider_;
    bool DBG_selected_;
    int DBG_index_;
    bool DBG_persistView_;
    #endif

    void SetFlag(Flag flag, bool enable);
    bool GetFlag(Flag flag);
    void CopyProperties(Entity* from);
    void AssignProperties(EntityPropertiesAssigner& assigner);

    void BaseUpdate();
    void BaseRenderUpdate(float interpTime);

    void DoPreUpdate();
    void DoUpdate();
    void DoRenderUpdate();

    void CalculateBasePose();
    void ChangeAnimation(int index, float transitionLength);

    void DoHit(HitArgs args);
    void DoHurt(HurtArgs args);
    void DoPush(glm::vec3 pushVec);
    void DoOverlap(Entity* overlappedEntity);
    void DoHitlagEnd();

    void DoCaptureSeed();
    void DoDestroy();

    glm::vec3 GetTarget();
    glm::vec3 GetTrailPosition(float t);

    Camera* camera_;
    EntityList* entities_;
    Inputs* inputs_;
    Level* level_;
    SeedManager* seedManager_;
    SpreadManager* spreadManager_;
    Terrain* terrain_;

private:
    void PreUpdate() {};
    void Update() {};
    void RenderUpdate() {};
    void OnHit(HitArgs args) {};
    void OnHurt(HurtArgs args) {};
    void OnPush(glm::vec3 pushVec) {}
    void OnOverlap(Entity* overlappedEntity) {}
    void OnHitlagEnd() {}
    void OnCaptureSeed() {};
    void OnDestroy() {};
    glm::vec3 GetTargetPoint() { return transform_.position; };

    static EntityDependendies GetStaticDependencies() { return {}; }
    static const char* GetStaticName() { return "e_base"; }
    EntityProperties GetStaticProperties() { return {}; }
};
