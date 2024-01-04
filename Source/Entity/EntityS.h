#pragma once
#include <inttypes.h>
#include "Types/Transform.h"
#include "Rendering/Bone.h"
#include "Rendering/Model.h"
#include "Rendering/Skeleton.h"
#include "Rendering/Material.h"

class Camera;
class Inputs;
class ParticleManager;
class ResourceManager;
class Terrain;

class ParticleEmitter;

class EntityS {
public:
    enum Flag {
        EF_Interpolate,
        EF_GroundCheck,
        EF_StickToGround,
        EF_UseVelocity,
        EF_UseSkeleton,
        EF_Count
    };

    virtual void Init(
        ParticleManager& particleManager,
        ResourceManager& resourceManager 
    ) = 0;

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

    uint32_t flags_; 
    void SetFlag(Flag flag, bool enable);
    bool GetFlag(Flag flag);

    void BaseUpdate();
    virtual void Update() = 0;
    void BaseRenderUpdate(float interpTime);
    virtual void RenderUpdate() = 0;

    Camera* camera_;
    Inputs* inputs_;
    Terrain* terrain_;
};
