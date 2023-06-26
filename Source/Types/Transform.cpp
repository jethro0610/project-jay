#include <glm/gtx/compatibility.hpp>
#include <sstream>
#include "Transform.h"
using namespace glm;

vec3 Transform::worldForward = vec3(0.0f, 0.0f, -1.0f);
vec3 Transform::worldRight = vec3(1.0f, 0.0f, 0.0f);
vec3 Transform::worldUp = vec3(0.0f, 1.0f, 0.0f);

Transform::Transform() {
    position_ = vec3(0.0f, 0.0f, 0.0f);
    rotation_ = quat(vec3(0.0f, 0.0f, 0.0f));
    scale_ = vec3(1.0f, 1.0f, 1.0f);
}

WorldNormalReturn Transform::GetWorldAndNormalMatrix() const {
    mat4 world = GetWorldMatrix();
    mat4 normal = mat3(world);
    normal = transpose(normal);
    normal = inverse(normal);

    return WorldNormalReturn(world, normal);
}

mat4 Transform::GetWorldMatrix() const {
    mat4 outWorld;
    outWorld = mat4(1.0f);
    outWorld = translate(outWorld, position_);
    outWorld *= toMat4(rotation_);
    outWorld = scale(outWorld, scale_);

    return outWorld;
}

std::string Transform::ToString() const{
    std::stringstream sstrream;
    
    sstrream << "Position: X: " << position_.x << " Y: " << position_.y << " Z: " << position_.z << '\n';
    sstrream << "Rotation: X: " << degrees(eulerAngles(rotation_).x) << " Y: " << degrees(eulerAngles(rotation_).y) << " Z: " << degrees(eulerAngles(rotation_).z) << '\n';
    sstrream << "Scale: X: " << scale_.x << " Y: " << scale_.y << " Z: " << scale_.z << '\n';
    return sstrream.str();
}

Transform Transform::Lerp(const Transform& a, const Transform& b, float t) {
    Transform lerpTransform;
    lerpTransform.position_ = lerp(a.position_, b.position_, t);
    lerpTransform.rotation_ = slerp(a.rotation_, b.rotation_, t);
    lerpTransform.scale_ = lerp(a.scale_, b.scale_, t);

    return lerpTransform;
}

vec3 Transform::GetForwardVector() const {
    return rotation_ * worldForward;
}

vec3 Transform::GetRightVector() const {
    return rotation_ * worldRight;
}

vec3 Transform::GetUpVector() const {
    return rotation_ * worldUp;
}
