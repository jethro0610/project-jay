#include "Camera.h"

Camera::Camera(TransformComponents* transformComponents, float trackDistance, vec3 startPosition) {
    transformComponents_ = transformComponents;
    firstPersonTransform_.position_ = startPosition;
    trackEntity_ = -1;
    lookX_ = 0.0f;
    lookY_ = 0.0f;
    trackDistance_ = trackDistance;
}

Camera::Camera(TransformComponents* transformComponents, float trackDistance, int trackEntity) {
    transformComponents_ = transformComponents;
    trackEntity_ = trackEntity;
    lookX_ = 0.0f;
    lookY_ = 0.0f;
    trackDistance_ = trackDistance;
}

mat4 Camera::GetViewMatrix() {
    if (trackEntity_ == NO_TRACK) {
        vec3 forward = rotate(firstPersonTransform_.rotation_, Transform::worldForward);
        vec3 up = rotate(firstPersonTransform_.rotation_, Transform::worldUp);

        return lookAtRH(
            firstPersonTransform_.position_,
            firstPersonTransform_.position_ + forward,
            up
        );
    }
    else {
        vec3 trackPosition = transformComponents_->renderTransform[trackEntity_].position_;
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
        firstPersonTransform_.rotation_ = quat(vec3(lookY_, lookX_, 0.0f));
        vec3 forwardMovement = firstPersonTransform_.GetForwardVector() * inputs.forwardInput * 15.0f * deltaTime;
        vec3 rightMovement = firstPersonTransform_.GetRightVector() * inputs.sideInput * 15.0f * deltaTime;
        firstPersonTransform_.position_ += forwardMovement + rightMovement;
    }
}