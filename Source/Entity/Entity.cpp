#include "Entity.h"
#include "Time/Time.h"
#include "Terrain/Terrain.h"
#include "EntityTypes.h"
#include "Helpers/Assert.h"
#include "Logging/ScreenText.h"
#include "Collision/Ray.h"
#include "Logging/Logger.h"
#include "EntityList.h"
#include <glm/gtx/compatibility.hpp>
using namespace glm;

void Entity::Construct(
    Camera& camera,
    Clock& clock,
    EntityList& entities,
    Inputs& inputs,
    Level& level,
    SeedManager& seedManager,
    SpreadManager& spreadManager,
    Terrain& terrain
) {
    camera_ = &camera;
    clock_ = &clock;
    entities_ = &entities;
    inputs_ = &inputs;
    level_ = &level;
    seedManager_ = &seedManager;
    spreadManager_ = & spreadManager;
    terrain_ = &terrain;
    trail_ = nullptr;

    editorTarget_ = new ETarget(this);
}

void Entity::Init(
    Entity::InitArgs args
) {
    alive_ = true;
    sleep_ = false;
    hide_ = false;
    destroy_ = false; 
    flags_ = 0; 
    
    lastTransform_ = transform_;
    renderTransform_ = transform_;
    velocity_ = {};
    trail_ = nullptr;

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
    DBG_preview_ = true;
    #endif

    ASSERT((typeId_ != Entity::TYPEID), "Attempted to execute on unassigned entity");

    switch(typeId_) {
        #define ENTITYEXP(TYPE, VAR, ID) case ID: ((TYPE*)this)->Init(args); break;
        EXPANDENTITIES
        #undef ENTITYEXP
    }
    memset(label_, 0, sizeof(char) * 32);
}

void Entity::DoStart() {
    switch(typeId_) {
        #define ENTITYEXP(TYPE, VAR, ID) case ID: ((TYPE*)this)->Start(); break;
        EXPANDENTITIES
        #undef ENTITYEXP
    }
}

void Entity::DoLateStart() {
    switch(typeId_) {
        #define ENTITYEXP(TYPE, VAR, ID) case ID: ((TYPE*)this)->LateStart(); break;
        EXPANDENTITIES
        #undef ENTITYEXP
    }
}

void Entity::Sleep() {
    alive_ = false;
    sleep_ = true;
}

void Entity::Wake() {
    alive_ = true;
    sleep_ = false;
    renderTransform_ = transform_;
    lastTransform_ = transform_;
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
        if (distanceToSurface < traceDistance_ && distanceToSurface > -20.0f && !skipGroundCheck_)
            onGround_ = true;
        else 
            onGround_ = false;

        if (GetFlag(EF_StickToGround)) {
            if (!skipGroundCheck_ && onGround_) {
                if (useVelocity) {
                    float stickVel = -distanceToSurface / GlobalTime::TIMESTEP;
                    if (GetFlag(EF_DownStickOnly)) {
                        if (groundHeight_ - transform_.position.y > 5.0f)
                            transform_.position.y = groundHeight_;
                        else if (stickVel > velocity_.y || velocity_.y < 0.0)
                            velocity_.y = stickVel;
                    }
                    else
                        velocity_.y = stickVel;
                }
                else
                    transform_.position.y = groundHeight_;
            }
            else if (transform_.position.y < groundHeight_ && distanceToSurface > -2.0f)
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
        // This zeroes out the renderTransform_ rotation
        renderTransform_ = Transform::Lerp(
            lastTransform_,
            transform_,
            interpTime
        );

        if (GetFlag(EF_AlignToGround)) {
            if (onGround_)
                desiredUp_ = lerp(lastGroundNormal_, groundNormal_, interpTime);
            else
                desiredUp_ = lerp(desiredUp_, Transform::worldUp, 1.0f - expf(-4.0 * GlobalTime::GetDeltaTime()));

            quat delta = rotation(Transform::worldUp, desiredUp_);
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

void Entity::RecordTrail() {
    if (trail_ == nullptr) {
        trail_ = new TrailPoint[TRAIL_SIZE];
        for (int i = 0; i < TRAIL_SIZE; i++) {
            trail_[i].time = GlobalTime::GetTime();
            trail_[i].position = renderTransform_.position;
        }
    }

    for (int i = 0; i < TRAIL_SIZE - 1; i++)
        trail_[i] = trail_[i + 1];

    trail_[TRAIL_SIZE - 1].position = transform_.position;
    trail_[TRAIL_SIZE - 1].time = GlobalTime::GetTime();
}

vec3 Entity::GetTrailPosition(float time) {
    ASSERT(GetFlag(EF_UseTrail), "Attempted to get trail from entity with no trail");
    if (trail_ == nullptr)
        return transform_.position;

    int low = 0;
    int high = TRAIL_SIZE - 1;

    while (low <= high) {
        int mid = low + (high - low) / 2;
        if (trail_[mid].time == time)
            return trail_[mid].position;

        if (trail_[mid].time < time)
            low = mid + 1;
        else
            high = mid - 1;
    }

    TrailPoint pointA = trail_[low - 1];
    TrailPoint pointB = trail_[low];
    float timeRange = pointB.time - pointA.time;
    float t = (time - pointA.time) / timeRange;

    return lerp(pointA.position, pointB.position, t);
}

vec3 Entity::GetTrailPositionNormalized(float time) {
    if (time >= 1.0f)
        return trail_[TRAIL_SIZE - 1].position;
    else if (time <= 0.0)
        return trail_[0].position;

    int target = floor(time * TRAIL_SIZE);
    float targetTime = time / TRAIL_SIZE;

    TrailPoint pointA = trail_[target];
    TrailPoint pointB = trail_[target + 1];

    float timeRange = 1.0f / TRAIL_SIZE;
    float t = (time - targetTime) / timeRange;

    return lerp(pointA.position, pointB.position, timeRange);
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

void Entity::DoSleepUpdate() {
    ASSERT((typeId_ != Entity::TYPEID), "Attempted to execute on unassigned entity");
    switch(typeId_) {
        #define ENTITYEXP(TYPE, VAR, ID) case ID: ((TYPE*)this)->SleepUpdate(); break;
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

#ifdef _DEBUG
void Entity::DoEditorUpdate() {
    ASSERT((typeId_ != Entity::TYPEID), "Attempted to execute on unassigned entity");
    switch(typeId_) {
        #define ENTITYEXP(TYPE, VAR, ID) case ID: ((TYPE*)this)->EditorUpdate(); break;
        EXPANDENTITIES
        #undef ENTITYEXP
    }
}
#endif

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
    if (trail_ != nullptr) 
        delete[] trail_;
    trail_ = nullptr;

    switch(typeId_) {
        #define ENTITYEXP(TYPE, VAR, ID) case ID: ((TYPE*)this)->OnDestroy(); break;
        EXPANDENTITIES
        #undef ENTITYEXP
    }
}

void Entity::DoTrigger() {
    switch(typeId_) {
        #define ENTITYEXP(TYPE, VAR, ID) case ID: ((TYPE*)this)->OnTrigger(); break;
        EXPANDENTITIES
        #undef ENTITYEXP
    }
}

bool Entity::IsTriggered() {
    switch(typeId_) {
        #define ENTITYEXP(TYPE, VAR, ID) case ID: return ((TYPE*)this)->GetIsTriggered(); break;
        EXPANDENTITIES
        #undef ENTITYEXP
    }
    return false;
}

vec3 Entity::GetTarget() {
    switch(typeId_) {
        #define ENTITYEXP(TYPE, VAR, ID) case ID: return ((TYPE*)this)->GetTargetPoint(); break;
        EXPANDENTITIES
        #undef ENTITYEXP
    }
    return vec3(transform_.position);
}

#ifdef _DEBUG
std::string Entity::ETarget::GetName() {
    return std::string(entity_->GetName());
}

vec3 Entity::ETarget::GetPosition() {
    return entity_->transform_.position;
}

void Entity::ETarget::SetPosition(const glm::vec3& pos) {
    entity_->transform_.position.x = pos.x;
    entity_->transform_.position.y = pos.y;
    entity_->transform_.position.z = pos.z;
}

quat Entity::ETarget::GetRotation() {
    return entity_->transform_.rotation;
}

void Entity::ETarget::SetRotation(const glm::quat& rot) {
    entity_->transform_.rotation = rot; 
}

vec4 Entity::ETarget::GetScale() {
    return vec4(entity_->transform_.scale, 0.0f);
}

void Entity::ETarget::SetScale(const glm::vec4& ref, const glm::vec4& delta) {
    entity_->transform_.scale.x = ref.x + delta.x;
    entity_->transform_.scale.y = ref.y + delta.y;
    entity_->transform_.scale.z = ref.z + delta.z;
}

bool Entity::ETarget::RayHit(const Ray& ray) {
    return ray.HitCollider(entity_->pushbox_, entity_->transform_);
}

EditorTarget* Entity::ETarget::Clone() {
    Entity& createdEntity = entity_->entities_->CreateEntity(entity_->typeId_, Transform(), true);
    createdEntity.transform_ = entity_->transform_;
    createdEntity.CopyProperties(entity_);
    return createdEntity.editorTarget_;
}

void Entity::ETarget::Destroy() {
    entity_->destroy_ = true;
}
#endif
