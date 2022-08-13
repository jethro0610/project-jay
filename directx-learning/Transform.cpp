#include "Transform.h"

Transform::Transform() {
    position_ = vec3(0.0f, 0.0f, 0.0f);
    rotation_ = quat(vec3(0.0f, 0.0f, 0.0f));
    scale_ = vec3(1.0f, 1.0f, 1.0f);
}

void Transform::GetWorldAndNormalMatrix(mat4& outWorld, mat4& outNormal) const {
    outWorld = mat4(1.0f);
    outWorld = translate(outWorld, position_);
    outWorld *= toMat4(rotation_);
    outWorld = scale(outWorld, scale_);
    outNormal = transpose(inverse(mat3(outWorld)));
}