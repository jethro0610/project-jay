#pragma once
#include "Types/Transform.h"
#include "Collision/Collider.h"
#include "Collision/Hitbox.h"
#include "Rendering/Bone.h"
#include "Rendering/Model.h"
#include "Rendering/Skeleton.h"
#include "Rendering/Material.h"
#include "Editor/ConfirmBehavior.h"
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
class EditorTextInput;
class EditorNotification;

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
    struct HitArgs {
        Entity* target;
    };
    struct HurtArgs {
        Entity* attacker;
    };

    static constexpr int MAX_NAME = 32;
    typedef int TypeID;

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
        EF_RecieveKnockback,
        EF_HurtFaceForward,
        EF_HurtFaceBack,
        EF_CaptureSeed,
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
    static constexpr const char* GetName() { return "e_base"; }
    static EntityDependendies GetDeps() { return {}; };

    TypeID typeId_;
    bool alive_;
    bool destroy_;
    uint32_t flags_; 

    Transform transform_;
    Transform lastTransform_;
    Transform renderTransform_;
    glm::vec3 velocity_;

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
    bool stun_;
    bool initHitlag_;
    int hitlag_;
    int hurtCooldown_;

    #ifdef _DEBUG
    Collider DBG_collider_;
    bool DBG_selected_;
    char DBG_name_[MAX_NAME];
    int DBG_index_;
    bool DBG_persistView_;
    #endif

    void SetFlag(Flag flag, bool enable);
    bool GetFlag(Flag flag);

    void Update() {};
    void BaseUpdate();

    void CalculateBasePose();
    void BaseRenderUpdate(float interpTime);
    void RenderUpdate() {};
    void ChangeAnimation(int index, float transitionLength);

    void OnHit(HitArgs args) {};
    void OnHurt(HurtArgs args) {};
    void OnPush(glm::vec3 pushVec) {};

    void OnCaptureSeed() {};
    void OnDestroy() {};

    #ifdef _DEBUG
    static bool DBG_Modifyable() { return false; };
    void DBG_OnStartModify(EditorTextInput& textInput, EditorNotification& notification) {};
    void DBG_OnCancelModify(EditorTextInput& textInput, EditorNotification& notification) {};
    void DBG_UpdateModify(EditorTextInput& textInput, EditorNotification& notification) {};
    ConfirmBehavior DBG_OnConfirmModify(EditorTextInput& textInput, EditorNotification& notification) { return CB_Default; };
    #endif

    Camera* camera_;
    EntityList* entities_;
    Inputs* inputs_;
    Level* level_;
    SeedManager* seedManager_;
    SpreadManager* spreadManager_;
    Terrain* terrain_;
};
