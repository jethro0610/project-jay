#include "Camera.h"
#include "Time/Time.h"
#include "Terrain/Terrain.h"
#include "Entity/Entity.h"
#include <glm/gtx/compatibility.hpp>
using namespace glm;

Camera::Camera(float trackDistance, vec3 startPosition):
    target_(nullptr),
    lookX_(0.0f),
    lookY_(0.0f),
    trackDistance_(trackDistance)
{
    transform_.position = startPosition;
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
    if (target_ == nullptr) {
        Update(inputs);
        return;
    }

    float deltaTime = GlobalTime::GetDeltaTime();
    lookX_ += inputs.deltaLookX;

    // TODO: Mouse keyboard implementation

    float aimLookY = inputs.useLookY ? 40.0f * inputs.lookY : 0.0f;
    float targetLookY = radians(-15.0f + aimLookY);
    lookY_ = lerp(lookY_, targetLookY, 1 - powf(0.005f, deltaTime));

    vec3 trackPosition = target_->renderTransform_.position;
    smoothTrackPosition_ = lerp(smoothTrackPosition_, trackPosition, 1 - powf(0.00000015f, deltaTime));
    if (distance(trackPosition, smoothTrackPosition_) > 3.0f) {
        vec3 delta = normalize(smoothTrackPosition_ - trackPosition) * 3.0f;
        smoothTrackPosition_ = trackPosition + delta;
    }

    quat lookRotation = quat(vec3(lookY_, lookX_, 0.0f));
    vec3 forward = lookRotation * Transform::worldForward;
    vec3 up = lookRotation * Transform::worldUp;

    transform_.position = smoothTrackPosition_ - forward * trackDistance_ + vec3(0.0f, 2.5f, 0.0f);
    transform_.position += vec3(0.0f, 2.0f, 0.0f);
    transform_.rotation = lookRotation;
}

void Camera::Update(Inputs& inputs) {
    float deltaTime = GlobalTime::GetDeltaTime();
    lookX_ += inputs.deltaLookX;
    lookY_ += inputs.deltaLookY;

    lookY_ = clamp(lookY_, radians(-89.0f), radians(89.0f));
    transform_.rotation = quat(vec3(lookY_, lookX_, 0.0f));
    float shiftSpeed = inputs.shift ? 4.0f : 1.0f;
    vec3 forwardMovement = transform_.GetForwardVector() * inputs.forwardInput * 32.0f * deltaTime * shiftSpeed;
    vec3 rightMovement = transform_.GetRightVector() * inputs.sideInput * 32.0f * deltaTime * shiftSpeed;
    transform_.position += forwardMovement + rightMovement;
}

vec3 Camera::GetLookPosition(Terrain& terrain) {
    vec3 pos = terrain.RaycastTerrain(transform_.position, transform_.rotation * Transform::worldForward);
    if (pos == vec3(0.0f)) {
        pos = transform_.position;
        vec2 terrainDist = terrain.GetDistance(transform_.position);
        if (terrainDist.x < 0.0f)
            pos.y = terrainDist.y;
    }
    return pos;
}
