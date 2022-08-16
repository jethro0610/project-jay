#include "Transform.h"
#include <sstream>

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

std::string Transform::ToString() const{
    std::stringstream sstrream;
    sstrream << "Position: X: " << position_.x << " Y: " << position_.y << " Z: " << position_.z << '\n';
    sstrream << "Rotation: X: " << rotation_.x << " Y: " << rotation_.y << " Z: " << rotation_.z << " W: " << rotation_.w << '\n';
    sstrream << "Scale: X: " << scale_.x << " Y: " << scale_.y << " Z: " << scale_.z << '\n';
    return sstrream.str();
}