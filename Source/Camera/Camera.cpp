#include "Camera.h"
#include "Time/Time.h"
#include "Terrain/Terrain.h"
#include "Entity/Entity.h"
#include <glm/gtx/compatibility.hpp>
#include "Logging/Logger.h"
#include <glm/gtx/string_cast.hpp>
#include <glm/gtx/vector_angle.hpp>
using namespace glm;

Camera::Camera(Terrain& terrain, vec3 startPosition):
    target_(nullptr),
    lookX_(0.0f),
    lookY_(0.0f),
    smoothLookX_(0.0f),
    trackDistance_(16.0f),
    terrain_(terrain)
{
    transform_.position = startPosition;
    smoothNormal_ = Transform::worldUp;
}

mat4 Camera::GetViewMatrix() const {
    return lookAtRH(
        transform_.position,
        transform_.position + transform_.GetForwardVector(),
        transform_.GetUpVector() 
    );
}

mat4 Camera::GetViewOnlyMatrix() const {
    return lookAtRH(
        vec3(0.0f),
        transform_.GetForwardVector(),
        transform_.GetUpVector()
    );
}

void Camera::Update(EntityList& entities, Inputs& inputs) {
    float deltaTime = GlobalTime::GetDeltaTime();
    if (target_ == nullptr) {
        Update(inputs);
        return;
    }

    if (lockOn_ != nullptr && !lockOn_->alive_)
        lockOn_ = nullptr;

    if (lockOn_ != nullptr) {
        vec3 directionToLockOn = lockOn_->transform_.position - target_->transform_.position;
        directionToLockOn.y = 0.0f;
        directionToLockOn = normalize(directionToLockOn);
        lookX_ = orientedAngle(Transform::worldForward, directionToLockOn, Transform::worldUp);
        if (std::isnan(lookX_))
            lookX_ = 0.0f;
    }
    else {
        lookX_ += inputs.deltaLookX * 1.5f;
    }

    // Wrap the smoothLookX around so we don't end up
    // doing a full 360 lerp
    while (smoothLookX_ - lookX_ >= pi<float>() * 0.5f)
        smoothLookX_ -= 2 * pi<float>();
    while (smoothLookX_ - lookX_ <= -pi<float>() * 0.5f)
        smoothLookX_ += 2 * pi<float>();

    smoothLookX_ = lerp(smoothLookX_, lookX_, 1 - powf(0.0001f, deltaTime));

    // TODO: Mouse keyboard implementation
    float aimLookY = inputs.useLookY ? 40.0f * inputs.lookY : 0.0f;
    float targetLookY = radians(-15.0f + aimLookY);
    lookY_ = lerp(lookY_, targetLookY, 1 - powf(0.005f, deltaTime));

    vec3 trackPosition = target_->renderTransform_.position + vec3(0.0f, 1.25f, 0.0f);
    smoothTrackPosition_ = lerp(smoothTrackPosition_, trackPosition, 1 - powf(0.00000015f, deltaTime));
    if (distance(trackPosition, smoothTrackPosition_) > 3.0f) {
        vec3 delta = normalize(smoothTrackPosition_ - trackPosition) * 3.0f;
        smoothTrackPosition_ = trackPosition + delta;
    }

    quat lookRotation = quat(vec3(lookY_, smoothLookX_, 0.0f));
    vec3 lookForward = lookRotation * Transform::worldForward;
    vec3 planarLookForward = lookForward;
    planarLookForward.y = 0.0f;

    quat upRotation = quat(vec3(-90.0f, 0.0f, 0.0f));
    quat downRotation = quat(vec3(90.0f, 0.0f, 0.0f));

    if (target_->onGround_) {
        vec3 normal = terrain_.GetNormal(target_->renderTransform_.position);
        smoothNormal_ = lerp(smoothNormal_, normal, 1 - powf(0.01f, deltaTime));
    }
    else {
        smoothNormal_ = lerp(smoothNormal_, Transform::worldUp, 1 - powf(0.5f, deltaTime));
    }

    vec3 alignedForward = lookForward;
    float normalDot = dot(planarLookForward, smoothNormal_);
    if (normalDot > 0.0f) {
        normalDot = powf(normalDot, 2.0f);
        alignedForward = lerp(lookForward, -Transform::worldUp, normalDot);
    }
    else {
        normalDot = -normalDot;
        normalDot = powf(normalDot, 2.0f);
        alignedForward = lerp(lookForward, Transform::worldUp, normalDot);
    }
    alignedForward = normalize(alignedForward);

    transform_.position = smoothTrackPosition_ - alignedForward * trackDistance_ + vec3(0.0f, 2.5f, 0.0f);
    transform_.position += vec3(0.0f, 2.0f, 0.0f);
    transform_.rotation = quatLookAtRH(alignedForward, Transform::worldUp);
}

void Camera::Update(Inputs& inputs) {
    float deltaTime = GlobalTime::GetDeltaTime();
    lookX_ += inputs.deltaLookX;
    lookY_ += inputs.deltaLookY;

    lookY_ = clamp(lookY_, radians(-89.0f), radians(89.0f));
    transform_.rotation = quat(vec3(lookY_, lookX_, 0.0f));
    float shiftSpeed = 1.0f; 

    if (inputs.shift)
        shiftSpeed *= 4.0f;
    if (inputs.space)
        shiftSpeed *= 4.0f;

    vec3 forwardMovement = transform_.GetForwardVector() * inputs.forwardInput * 200.0f * deltaTime * shiftSpeed;
    vec3 rightMovement = transform_.GetRightVector() * inputs.sideInput * 200.0f * deltaTime * shiftSpeed;
    transform_.position += forwardMovement + rightMovement;
}

vec3 Camera::GetLookPosition() {
    vec3 pos = terrain_.RaycastTerrain(transform_.position, transform_.rotation * Transform::worldForward);
    if (pos == vec3(0.0f)) {
        pos = transform_.position;
        vec2 terrainDist = terrain_.GetDistance(transform_.position);
        if (terrainDist.x < 0.0f)
            pos.y = terrainDist.y;
    }
    return pos;
}
