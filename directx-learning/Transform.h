#pragma once
#include <glm.hpp>
#include <gtx/quaternion.hpp>

using namespace glm;

class Transform {
public:
    Transform();
    vec3 position_;
    quat rotation_;
    vec3 scale_;

    mat4 GetWorldMatrix() const;
};

