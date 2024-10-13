#include "Entity.h"
#include "Time/Time.h"
#include "Terrain/Terrain.h"
#include "EntityTypes.h"
#include "Helpers/Assert.h"
#include <glm/gtx/compatibility.hpp>
using namespace glm;

void Entity::Construct(
    Camera& camera,
    EntityList& entities,
    Inputs& inputs,
    Level& level,
    SeedManager& seedManager,
    SpreadManager& spreadManager,
    Terrain& terrain
) {
    camera_ = &camera;
    entities_ = &entities;
    inputs_ = &inputs;
    level_ = &level;
    seedManager_ = &seedManager;
    spreadManager_ = & spreadManager;
    terrain_ = &terrain;
}

void Entity::Init(
    Entity::InitArgs args
) {
    alive_ = true;
    destroy_ = false; 
    flags_ = 0; 
    
    lastTransform_ = transform_;
    renderTransform_ = transform_;
    velocity_ = {};

    traceDistance_ = 1.0f;
    onGround_ = false;
    groundHeight_ = 0.0f;
    groundNormal_ = {};

    model_ = nullptr;
    for (int i = 0; i < Model::MAX_MESHES_PER_MODEL; i++)
        materials_[i] = {};

    skeleton_ = nullptr;
    pose_ = {};
    renderPose_ = {};
    animIndex_ = 0;
    prevAnimIndex_ = 0;
    prevAnimTime_ = 0; 
    animTime_ = 0;
    transitionTime_ = 0.0f;
    transitionLength_ = 0.0f;

    pushbox_ = {};
    hitbox_ = {};
    hurtbox_ = {};
    overlapbox_ = {};
    stun_ = false;
    hitlag_ = 0;
    hurtCooldown_ = 0;

    #ifdef _DEBUG
    DBG_selected_ = false;
    DBG_collider_.radius = 1.0f;
    DBG_collider_.top = 1.0f;
    DBG_collider_.bottom = 1.0f;
    DBG_persistView_ = false;
    #endif

    ASSERT((typeId_ != Entity::TYPEID), "Attempted to execute on unassigned entity");

    switch(typeId_) {
        #define ENTITYEXP(TYPE, VAR, ID) case ID: ((TYPE*)this)->Init(args); break;
        EXPANDENTITIES
        #undef ENTITYEXP
    }
}

void Entity::SetFlag(Entity::Flag flag, bool enable) {
    uint32_t bit = 1UL << flag; 

    if (enable)
        flags_ |= bit;
    else
        flags_ &= ~bit;
}

bool Entity::GetFlag(Entity::Flag flag) {
    return (flags_ & 1UL << flag); 
}

void Entity::BaseUpdate() {
    bool useVelocity = GetFlag(EF_UseVelocity);

    if (hitlag_ == 0 && GetFlag(EF_GroundCheck)) {
        lastGroundNormal_ = groundNormal_;
        groundNormal_ = terrain_->GetNormal(transform_.position + velocity_ * GlobalTime::TIMESTEP);
        groundHeight_ = terrain_->GetHeight(transform_.position + velocity_ * GlobalTime::TIMESTEP);
        float distanceToSurface = transform_.position.y - groundHeight_;
        if (distanceToSurface < traceDistance_ && !skipGroundCheck_)
            onGround_ = true;
        else 
            onGround_ = false;

        if (GetFlag(EF_StickToGround)) {
            if (!skipGroundCheck_ && onGround_) {
                if (useVelocity) 
                    velocity_.y = -distanceToSurface / GlobalTime::TIMESTEP;
                else
                    transform_.position.y = groundHeight_;
            }
            else if (transform_.position.y  < groundHeight_)
                transform_.position.y = groundHeight_;
        }
    }

    if (hitlag_ == 0 && velocity_.y < 0.0f)
        skipGroundCheck_ = false;

    if (hitlag_ == 0 && useVelocity)
        transform_.position += velocity_ * GlobalTime::TIMESTEP;

    if ((hitlag_ == 0 || initHitlag_) && GetFlag(EF_UseSkeleton)) {
        int prevAnimFramerate = skeleton_->GetFramerate(prevAnimIndex_);
        int curAnimFramerate = skeleton_->GetFramerate(animIndex_);
        int framerate = transitionTime_ < transitionLength_ ? 
            std::lerp(prevAnimFramerate, curAnimFramerate, clamp(transitionTime_ / transitionLength_, 0.0f, 1.0f)) :
            curAnimFramerate;

        if (transitionTime_ >= transitionLength_ || transitionLength_ == 0.0f) {
            skeleton_->GetPose(
                pose_,
                animIndex_,
                animTime_,
                transform_,
                lastTransform_
            );
        }
        else {
            skeleton_->GetBlendedPose(
                pose_,
                prevAnimIndex_,
                animIndex_,
                prevAnimTime_,
                animTime_,
                transitionTime_ / transitionLength_,
                transform_,
                lastTransform_
            );
        }

        int frame = floor(animTime_ * 60.0f);
        int framesTillUpate = 60 / max(framerate, 1);

        if (frame % framesTillUpate == 0)
            renderPose_ = pose_;

        animTime_ += GlobalTime::TIMESTEP;
        prevAnimTime_ += GlobalTime::TIMESTEP;
        transitionTime_ += GlobalTime::TIMESTEP;
    }

    if (hitlag_ != 0) {
        hitlag_--;
        if (hitlag_ == 0)
            DoHitlagEnd();
    }

    if (hitlag_ == 0 && hurtCooldown_ > 0)
        hurtCooldown_--;

    initHitlag_ = false;
}

void Entity::CalculateBasePose() {
    if (!GetFlag(EF_UseSkeleton)) return;
    skeleton_->GetBasePose(pose_);
    renderPose_ = pose_;
}

void Entity::BaseRenderUpdate(float interpTime) {
    if (GetFlag(EF_Interpolate)) {
        renderTransform_ = Transform::Lerp(
            lastTransform_,
            transform_,
            interpTime
        );

        if (onGround_ && GetFlag(EF_AlignToGround)) {
            vec3 up = lerp(lastGroundNormal_, groundNormal_, interpTime);
            quat delta = rotation(Transform::worldUp, up);
            renderTransform_.rotation = delta * renderTransform_.rotation;
        }
    }
    else {
        renderTransform_ = transform_;
        if (onGround_ && GetFlag(EF_AlignToGround)) {
            quat delta = rotation(Transform::worldUp, groundNormal_);
            renderTransform_.rotation = delta * renderTransform_.rotation;
        }
    }

    if (hitlag_ > 0 && stun_) {
        renderTransform_.position.x += pow(sin(GlobalTime::GetTime() * 100.0f + 8.0f), 2.0f) * 0.5f;
        renderTransform_.position.y += pow(sin(GlobalTime::GetTime() * 100.0f + 16.0f), 2.0f) * 0.5f;
        renderTransform_.position.z += pow(sin(GlobalTime::GetTime() * 100.0f + 32.0f), 2.0f) * 0.5f;
    }
}

void Entity::ChangeAnimation(int index, float transitionLength) {
    prevAnimIndex_ = animIndex_;
    animIndex_ = index; 
    prevAnimTime_ = animTime_;
    animTime_ = 0.0f;

    transitionLength_ = transitionLength;
    transitionTime_ = 0.0f;
}

void Entity::CopyProperties(Entity* from) {
    ASSERT((typeId_ == from->typeId_), "Attempted to copy from entity with different type");

    EntityProperties properties = GetProperties();
    EntityProperties fromProperties = from->GetProperties();

    for (int i = 0; i < properties.floats.size(); i++)
        *properties.floats[i].second = *fromProperties.floats[i].second;

    for (int i = 0; i < properties.ints.size(); i++)
        *properties.ints[i].second = *fromProperties.ints[i].second;

    for (int i = 0; i < properties.bools.size(); i++)
        *properties.bools[i].second = *fromProperties.bools[i].second;
}

const char* Entity::GetName() {
    ASSERT((typeId_ != Entity::TYPEID), "Attempted to execute on unassigned entity");
    switch(typeId_) {
        #define ENTITYEXP(TYPE, VAR, ID) case ID: return TYPE::GetStaticName(); break;
        EXPANDENTITIES
        #undef ENTITYEXP

        default:
            return "e_base";
    }
}

EntityDependendies Entity::GetDependencies() {
    ASSERT((typeId_ != Entity::TYPEID), "Attempted to execute on unassigned entity");
    switch(typeId_) {
        #define ENTITYEXP(TYPE, VAR, ID) case ID: return TYPE::GetStaticDependencies(); break;
        EXPANDENTITIES
        #undef ENTITYEXP

        default:
            return {};
    }
}

EntityDependendies Entity::GetDependencies(TypeID typeId) {
    switch(typeId) {
        #define ENTITYEXP(TYPE, VAR, ID) case ID: return TYPE::GetStaticDependencies(); break;
        EXPANDENTITIES
        #undef ENTITYEXP

        default:
            return {};
    }
}

void Entity::AssignProperties(EntityPropertiesAssigner& assigner) {
    ASSERT((typeId_ != Entity::TYPEID), "Attempted to execute on unassigned entity");
    EntityProperties properties;
    switch(typeId_) {
        #define ENTITYEXP(TYPE, VAR, ID) case ID: properties = ((TYPE*)this)->GetStaticProperties(); break;
        EXPANDENTITIES
        #undef ENTITYEXP
    }
    properties.AssignProperties(assigner);
}

EntityProperties Entity::GetProperties() {
    ASSERT((typeId_ != Entity::TYPEID), "Attempted to execute on unassigned entity");
    EntityProperties returnProps;
    switch(typeId_) {
        #define ENTITYEXP(TYPE, VAR, ID) case ID: returnProps = ((TYPE*)this)->GetStaticProperties(); break;
        EXPANDENTITIES
        #undef ENTITYEXP
    }
    return returnProps;
}

TypeID Entity::GetTypeIDFromName(const std::string& name) {
    TypeID typeId = -1;
    #define ENTITYEXP(TYPE, VAR, ID) if (name == TYPE::GetStaticName()) typeId = ID;
    EXPANDENTITIES
    #undef ENTITYEXP

    return typeId;
}

void Entity::DoPreUpdate() {
    ASSERT((typeId_ != Entity::TYPEID), "Attempted to execute on unassigned entity");
    switch(typeId_) {
        #define ENTITYEXP(TYPE, VAR, ID) case ID: ((TYPE*)this)->PreUpdate(); break;
        EXPANDENTITIES
        #undef ENTITYEXP
    }
}

void Entity::DoUpdate() {
    ASSERT((typeId_ != Entity::TYPEID), "Attempted to execute on unassigned entity");
    switch(typeId_) {
        #define ENTITYEXP(TYPE, VAR, ID) case ID: ((TYPE*)this)->Update(); break;
        EXPANDENTITIES
        #undef ENTITYEXP
    }
}

void Entity::DoRenderUpdate() {
    ASSERT((typeId_ != Entity::TYPEID), "Attempted to execute on unassigned entity");
    switch(typeId_) {
        #define ENTITYEXP(TYPE, VAR, ID) case ID: ((TYPE*)this)->RenderUpdate(); break;
        EXPANDENTITIES
        #undef ENTITYEXP
    }
}

void Entity::DoHit(HitArgs args) {
    ASSERT((typeId_ != Entity::TYPEID), "Attempted to execute on unassigned entity");
    switch(typeId_) {
        #define ENTITYEXP(TYPE, VAR, ID) case ID: ((TYPE*)this)->OnHit(args); break;
        EXPANDENTITIES
        #undef ENTITYEXP
    }
}

void Entity::DoHurt(HurtArgs args) {
    ASSERT((typeId_ != Entity::TYPEID), "Attempted to execute on unassigned entity");
    switch(typeId_) {
        #define ENTITYEXP(TYPE, VAR, ID) case ID: ((TYPE*)this)->OnHurt(args); break;
        EXPANDENTITIES
        #undef ENTITYEXP
    }
}

void Entity::DoPush(glm::vec3 pushVec) {
    ASSERT((typeId_ != Entity::TYPEID), "Attempted to execute on unassigned entity");
    switch(typeId_) {
        #define ENTITYEXP(TYPE, VAR, ID) case ID: ((TYPE*)this)->OnPush(pushVec); break;
        EXPANDENTITIES
        #undef ENTITYEXP
    }
}

void Entity::DoOverlap(Entity* overlappedEntity) {
    ASSERT((typeId_ != Entity::TYPEID), "Attempted to execute on unassigned entity");
    switch(typeId_) {
        #define ENTITYEXP(TYPE, VAR, ID) case ID: ((TYPE*)this)->OnOverlap(overlappedEntity); break;
        EXPANDENTITIES
        #undef ENTITYEXP
    }
}

void Entity::DoHitlagEnd() {
    ASSERT((typeId_ != Entity::TYPEID), "Attempted to execute on unassigned entity");
    switch(typeId_) {
        #define ENTITYEXP(TYPE, VAR, ID) case ID: ((TYPE*)this)->OnHitlagEnd(); break;
        EXPANDENTITIES
        #undef ENTITYEXP
    }
}

void Entity::DoCaptureSeed() {
    ASSERT((typeId_ != Entity::TYPEID), "Attempted to execute on unassigned entity");
    switch(typeId_) {
        #define ENTITYEXP(TYPE, VAR, ID) case ID: ((TYPE*)this)->OnCaptureSeed(); break;
        EXPANDENTITIES
        #undef ENTITYEXP
    }
}

void Entity::DoDestroy() {
    switch(typeId_) {
        #define ENTITYEXP(TYPE, VAR, ID) case ID: ((TYPE*)this)->OnDestroy(); break;
        EXPANDENTITIES
        #undef ENTITYEXP
    }
}

vec3 Entity::GetTarget() {
    switch(typeId_) {
        #define ENTITYEXP(TYPE, VAR, ID) case ID: return ((TYPE*)this)->GetTargetPoint(); break;
        EXPANDENTITIES
        #undef ENTITYEXP
    }
    return vec3(0.0f);
}
