#pragma once
#include "Types/Transform.h"
#include "Collision/Collider.h"
#include "Collision/HitboxS.h"
#include "Rendering/Bone.h"
#include "Rendering/Model.h"
#include "Rendering/Skeleton.h"
#include "Rendering/Material.h"
#include <inttypes.h>

class Camera;
class Inputs;
class ParticleManager;
class ResourceManager;
class Terrain;

class ParticleEmitter;


class EntityS {
public:
    typedef int TypeID;
    struct InitArgs {
        ParticleManager& particleManager;
        ResourceManager& resourceManager;
    };

    enum Flag {
        EF_Interpolate,
        EF_GroundCheck,
        EF_StickToGround,
        EF_UseVelocity,
        EF_UseSkeleton,
        EF_SendPush,
        EF_RecievePush,
        EF_HurtRecieveKnockback,
        EF_Count
    };

    void Construct(
        Camera& camera,
        Inputs& inputs,
        Terrain& terrain
    );
    void Init(
        InitArgs args
    );

    static constexpr TypeID GetTypeID() { return 0; };
    static constexpr const char* GetName() { return "e_base"; }

    uint32_t flags_; 

    Transform transform_;
    Transform lastTransform_;
    Transform renderTransform_;
    glm::vec3 velocity_;

    float traceDistance_;
    bool onGround_;
    float groundHeight_;
    glm::vec3 groundNormal_;

    Model* model_;
    Material* materials_[Model::MAX_MESHES_PER_MODEL];

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
    HitboxS hitbox_;
    Collider hurtbox_;

    void SetFlag(Flag flag, bool enable);
    bool GetFlag(Flag flag);

    void Update() {};
    void BaseUpdate();

    void BaseRenderUpdate(float interpTime);
    void RenderUpdate() {};

    Camera* camera_;
    Inputs* inputs_;
    Terrain* terrain_;
};
