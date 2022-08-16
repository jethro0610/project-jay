#pragma once
#include <glm.hpp>
#include <gtx/quaternion.hpp>
#include <string>

using namespace glm;

class Transform {
public:
    Transform();
    vec3 position_;
    quat rotation_;
    vec3 scale_;

    void GetWorldAndNormalMatrix(mat4& outWorld, mat4& outNormal) const;
    std::string ToString() const;
};

