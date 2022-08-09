#include "Transform.h"

Transform::Transform() {
    position_ = vec3(0.0f, 0.0f, 0.0f);
    rotation_ = quat(vec3(0.0f, 0.0f, 0.0f));
    scale_ = vec3(1.0f, 1.0f, 1.0f);
}

mat4 Transform::GetWorldMatrix() const {
    mat4 worldMat = mat4(1.0f);
    worldMat = translate(worldMat, position_);
    worldMat *= toMat4(rotation_);
    worldMat= scale(worldMat, scale_);

    return worldMat;
}