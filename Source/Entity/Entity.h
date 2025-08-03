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
#include "Editor/EditorTarget.h"
#include "Collision/Ray.h"
#include <vector>
#include <inttypes.h>
#include <cstring>

class Camera;
class EntityList;
struct Inputs;
class Level;
class LevelController;
class ParticleManager;
class ResourceManager;
class SeedManager;
class SpreadManager;
class Terrain;
class WaterManager;
class WorldText;

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
    static constexpr int TRAIL_SIZE = 2048;
    static constexpr int MAX_NAME = 32;

    struct HitArgs {
        Entity* target;
        Hitbox::HitType hitType;
    };
    struct HurtArgs {
        Entity* attacker;
        Hitbox* hitbox;
        Hitbox::HitType hitType;
        glm::vec3 kbVelocity;
        glm::quat kbRotation;
    };
    struct PushArgs {
        Entity* pushingEntity;
        glm::vec3 pushVec;
    };
    struct TrailPoint {
        float time;
        glm::vec3 position;
    };
    struct InitArgs {
        ParticleManager& particleManager;
        ResourceManager& resourceManager;
    };

    enum Flag {
        EF_Interpolate,
        EF_UseVelocity,

        EF_GroundCheck,
        EF_StickToGround,
        EF_DownStickOnly,
        EF_AlignToGround,

        EF_UseSkeleton,

        EF_SendPush,
        EF_RecievePush,
        EF_Bonkable,

        EF_SendHits,
        EF_RecieveHits,
        EF_RecieveKnockback,
        EF_CustomKnockback,
        EF_HurtFaceForward,
        EF_HurtFaceBack,

        EF_Trackable,
        EF_Overlap,
        EF_CaptureSeed,
        EF_Homeable,
        EF_UseTrail,
        EF_ProjectileLockable,
        EF_Count
    };

    void Construct(
        Camera& camera,
        EntityList& entities,
        Inputs& inputs,
        Level& level,
        LevelController& levelController,
        SeedManager& seedManager,
        SpreadManager& spreadManager,
        Terrain& terrain,
        WaterManager& waterManager
    );
    void Init(
        InitArgs args
    );
    void DoStart();
    void DoLateStart();

    static TypeID TYPEID;
    const char* GetName();
    EntityDependendies GetDependencies();
    static EntityDependendies GetDependencies(TypeID typeId);
    EntityProperties GetProperties();
    static TypeID GetTypeIDFromName(const std::string& name);

    char label_[32];
    TypeID typeId_;
    bool alive_;
    bool destroy_;
    bool hide_;
    bool sleep_;
    uint32_t flags_; 

    Transform transform_;
    Transform lastTransform_;
    Transform renderTransform_;
    glm::vec3 velocity_;
    glm::vec3 desiredUp_;
    TrailPoint* trail_;

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
    bool DBG_preview_;
    int DBG_id_;
    #endif

    void Sleep();
    void Wake();

    void SetFlag(Flag flag, bool enable);
    bool GetFlag(Flag flag);
    void CopyProperties(Entity* from);
    void AssignProperties(EntityPropertiesAssigner& assigner);

    void BaseUpdate();
    void BaseRenderUpdate(float interpTime);

    void DoPreUpdate();
    void DoUpdate();
    void DoSleepUpdate();
    void DoRenderUpdate();
    #ifdef _DEBUG
    void DoEditorUpdate();
    #endif

    void CalculateBasePose();
    void ChangeAnimation(int index, float transitionLength);

    void DoHit(HitArgs args);
    void DoHurt(HurtArgs args);
    void DoPush(PushArgs args);
    void DoOverlap(Entity* overlappedEntity);
    void DoHitlagEnd();

    void DoCaptureSeed();
    void DoDestroy();
    void DoTrigger();
    bool IsTriggered();

    void DoWet();

    glm::vec3 GetTarget();
    glm::vec3 GetTrailPosition(float t);
    glm::vec3 GetTrailPositionNormalized(float t);

    void RecordTrail();
    int DoGetSeeds();

    Camera* camera_;
    EntityList* entities_;
    Inputs* inputs_;
    Level* level_;
    LevelController* levelController_;
    SeedManager* seedManager_;
    SpreadManager* spreadManager_;
    Terrain* terrain_;
    WaterManager* waterManager_;

private:
    void Start() {};
    void LateStart() {};
    void PreUpdate() {};
    void Update() {};
    void SleepUpdate() {};
    void RenderUpdate() {};
    #ifdef _DEBUG
    void EditorUpdate() {};
    #endif
    void OnHit(HitArgs args) {};
    void OnHurt(HurtArgs args) {};
    void OnPush(PushArgs args) {}
    void OnOverlap(Entity* overlappedEntity) {}
    void OnHitlagEnd() {}
    void OnCaptureSeed() {};
    void OnDestroy() {};
    glm::vec3 GetTargetPoint() { return transform_.position; };
    void OnTrigger() {};
    bool GetIsTriggered() { return false; };
    void OnWet() {};
    int GetSeeds() { return 0; }

    static EntityDependendies GetStaticDependencies() { return {}; }
    static const char* GetStaticName() { return "e_base"; }
    EntityProperties GetStaticProperties() { return {}; }

    #ifdef _DEBUG
    public:
    class ETarget : public EditorTarget {
    private:
        friend Entity;
        Entity* entity_;

    public:
        ETarget(Entity* entity) : entity_(entity) {}
        std::string GetName() override;
        int GetId() override { return entity_->DBG_id_; }
        
        bool CanLabel() override { return true; }
        std::string GetLabel() override { return entity_->label_; }
        void SetLabel(const std::string& label) override { 
            strcpy(entity_->label_, label.c_str());
        }

        glm::vec3 GetPosition() override;
        void SetPosition(const glm::vec3 &pos) override;

        glm::quat GetRotation() override;
        void SetRotation(const glm::quat &rot) override;

        glm::vec4 GetScale() override;
        void SetScale(const glm::vec4& ref, const glm::vec4& delta) override;

        EntityProperties GetProperties() override { return entity_->GetProperties(); }

        bool GetPreview() override { return entity_->DBG_preview_; }
        void SetPreview(bool value) override { entity_->DBG_preview_ = value; }

        bool TerrainAlignable() override { return true; }
        bool Selectable() override { return entity_->alive_; }
        bool IsEntity() override { return true; }
        bool Clonable() override { return true; }

        bool RayHit(const Ray& ray) override;
        EditorTarget* Clone() override;
        void Destroy() override;
    };
    EditorTarget* editorTarget_;
    #endif
};
