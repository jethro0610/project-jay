#include "Camera.h"
#include "Components/TransformComponent.h"
#include "Time/Time.h"
#include <glm/gtx/compatibility.hpp>
using namespace glm;

Camera::Camera(TransformComponent& transformComponent, float trackDistance, vec3 startPosition):
    transformComponent_(transformComponent),
    target_(NULL_ENTITY),
    lookX_(0.0f),
    lookY_(0.0f),
    trackDistance_(trackDistance)
{
    transform_.position = startPosition;
}

Camera::Camera(TransformComponent& transformComponent, float trackDistance, EntityID target):
    transformComponent_(transformComponent),
    target_(target),
    lookX_(0.0f),
    lookY_(0.0f),
    trackDistance_(trackDistance)
{

}

mat4 Camera::GetViewMatrix() const {
    vec3 forward = rotate(transform_.rotation, Transform::worldForward);
    vec3 up = rotate(transform_.rotation, Transform::worldUp);

    return lookAtRH(
        transform_.position,
        transform_.position + forward,
        Transform::worldUp 
    );
}

void Camera::Update(Inputs inputs) {
    float deltaTime = GlobalTime::GetDeltaTime();
    lookX_ += inputs.deltaLookX;
    lookY_ += inputs.deltaLookY;
    lookY_ = clamp(lookY_, radians(-50.0f), radians(5.0f));

    if (target_ == NULL_ENTITY) {
        // Move and use first person look when there is no entity to track
        transform_.rotation = quat(vec3(lookY_, lookX_, 0.0f));
        vec3 forwardMovement = transform_.GetForwardVector() * inputs.forwardInput * 32.0f * deltaTime;
        vec3 rightMovement = transform_.GetRightVector() * inputs.sideInput * 32.0f * deltaTime;
        transform_.position += forwardMovement + rightMovement;
    }
    else {
        vec3 trackPosition = transformComponent_.renderTransform[target_].position;
        smoothTrackPosition_ = lerp(smoothTrackPosition_, trackPosition, 1 - powf(0.00000015f, deltaTime));
        if (distance(trackPosition, smoothTrackPosition_) > 3.0f) {
            vec3 delta = normalize(smoothTrackPosition_ - trackPosition) * 3.0f;
            smoothTrackPosition_ = trackPosition + delta;
        }

        quat lookRotation = quat(vec3(lookY_, lookX_, 0.0f));
        vec3 forward = lookRotation * Transform::worldForward;
        vec3 up = lookRotation * Transform::worldUp;

        transform_.position = smoothTrackPosition_ - forward * trackDistance_;
        transform_.position += vec3(0.0f, 2.0f, 0.0f);
        transform_.rotation = lookRotation;
    }
}
