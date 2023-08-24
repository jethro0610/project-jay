#include <glm/gtx/compatibility.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <sstream>
#include "Transform.h"
using namespace glm;

vec3 Transform::worldForward = vec3(0.0f, 0.0f, -1.0f);
vec3 Transform::worldRight = vec3(1.0f, 0.0f, 0.0f);
vec3 Transform::worldUp = vec3(0.0f, 1.0f, 0.0f);

Transform::Transform() {
    position = vec3(0.0f, 0.0f, 0.0f);
    rotation = quat(vec3(0.0f, 0.0f, 0.0f));
    scale = vec3(1.0f, 1.0f, 1.0f);
}

Transform::Transform(const mat4& matrix) {
    vec3 skew;
    vec4 persp;
    decompose(matrix, scale, rotation, position, skew, persp);
}

mat4 Transform::ToMatrix() const {
    mat4 outWorld;
    outWorld = mat4(1.0f);
    outWorld = translate(outWorld, position);
    outWorld *= toMat4(rotation);
    outWorld = glm::scale(outWorld, scale);

    return outWorld;
}

std::string Transform::ToString() const{
    std::stringstream sstrream;
    
    sstrream << "Position: X: " << position.x << " Y: " << position.y << " Z: " << position.z << '\n';
    sstrream << "Rotation: X: " << degrees(eulerAngles(rotation).x) << " Y: " << degrees(eulerAngles(rotation).y) << " Z: " << degrees(eulerAngles(rotation).z) << '\n';
    sstrream << "Scale: X: " << scale.x << " Y: " << scale.y << " Z: " << scale.z << '\n';
    return sstrream.str();
}

Transform Transform::Lerp(const Transform& a, const Transform& b, float t) {
    Transform lerpTransform;
    lerpTransform.position = lerp(a.position, b.position, t);
    lerpTransform.rotation = slerp(a.rotation, b.rotation, t);
    lerpTransform.scale = lerp(a.scale, b.scale, t);

    return lerpTransform;
}

vec3 Transform::GetForwardVector() const {
    return rotation * worldForward;
}

vec3 Transform::GetRightVector() const {
    return rotation * worldRight;
}

vec3 Transform::GetUpVector() const {
    return rotation * worldUp;
}
