#include "Camera.h"

Camera::Camera(TransformComponent* transformComponent, float trackDistance, vec3 startPosition) {
    transformComponent_ = transformComponent;
    transform_.position_ = startPosition;
    trackEntity_ = -1;
    lookX_ = 0.0f;
    lookY_ = 0.0f;
    trackDistance_ = trackDistance;
}

Camera::Camera(TransformComponent* transformComponent, float trackDistance, int trackEntity) {
    transformComponent_ = transformComponent;
    trackEntity_ = trackEntity;
    lookX_ = 0.0f;
    lookY_ = 0.0f;
    trackDistance_ = trackDistance;
}

mat4 Camera::GetViewMatrix() const {
    if (trackEntity_ == NO_TRACK) {
        vec3 forward = rotate(transform_.rotation_, Transform::worldForward);
        vec3 up = rotate(transform_.rotation_, Transform::worldUp);

        return lookAtRH(
            transform_.position_,
            transform_.position_ + forward,
            up
        );
    }
    else {
        vec3 trackPosition = transform_.position_;
        quat lookRotation = quat(vec3(lookY_, lookX_, 0.0f));
        vec3 forward = lookRotation * Transform::worldForward;
        vec3 up = lookRotation * Transform::worldUp;

        vec3 cameraPosition = trackPosition - forward * trackDistance_;
        return lookAtRH(
            cameraPosition,
            cameraPosition + forward,
            up
        );
    }
}

void Camera::Update(float deltaTime, Inputs inputs) {
    lookX_ += inputs.deltaLookX;
    lookY_ += inputs.deltaLookY;
    lookY_ = clamp(lookY_, radians(-80.0f), radians(80.0f));

    if (trackEntity_ == NO_TRACK) {
        // Move and use first person look when there is no entity to track
        transform_.rotation_ = quat(vec3(lookY_, lookX_, 0.0f));
        vec3 forwardMovement = transform_.GetForwardVector() * inputs.forwardInput * 15.0f * deltaTime;
        vec3 rightMovement = transform_.GetRightVector() * inputs.sideInput * 15.0f * deltaTime;
        transform_.position_ += forwardMovement + rightMovement;
    }
    else {
        vec3 targetPosition = transformComponent_[trackEntity_].renderTransform->position_;
        vec3 lerpPosition = lerp(transform_.position_, targetPosition, 1 - powf(0.00000015f, deltaTime));
        if (distance(targetPosition, lerpPosition) > 3.0f) {
            vec3 delta = normalize(lerpPosition - targetPosition) * 3.0f;
            lerpPosition = targetPosition + delta;
        }

        transform_.position_ = lerpPosition;
    }
}