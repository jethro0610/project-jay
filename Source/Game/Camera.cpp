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
    vec3 forward = rotate(transform_.rotation_, Transform::worldForward);
    vec3 up = rotate(transform_.rotation_, Transform::worldUp);

    return lookAtRH(
        transform_.position_,
        transform_.position_ + forward,
        up
    );
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
        vec3 trackPosition = transformComponent_[trackEntity_].renderTransform->position_;
        smoothTrackPosition_ = lerp(smoothTrackPosition_, trackPosition, 1 - powf(0.00000015f, deltaTime));
        if (distance(trackPosition, smoothTrackPosition_) > 3.0f) {
            vec3 delta = normalize(smoothTrackPosition_ - trackPosition) * 3.0f;
            smoothTrackPosition_ = trackPosition + delta;
        }

        quat lookRotation = quat(vec3(lookY_, lookX_, 0.0f));
        vec3 forward = lookRotation * Transform::worldForward;
        vec3 up = lookRotation * Transform::worldUp;

        transform_.position_ = smoothTrackPosition_ - forward * trackDistance_;
        transform_.rotation_ = lookRotation;
    }
}
