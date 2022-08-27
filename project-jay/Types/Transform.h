#pragma once
#include <glm.hpp>
#include <gtx/quaternion.hpp>
#include <gtx/compatibility.hpp>
#include <gtx/string_cast.hpp>
#include <string>

using namespace glm;

class Transform {
public:
    Transform();
    vec3 position_;
    quat rotation_;
    vec3 scale_;

    void GetWorldAndNormalMatrix(mat4& outWorld, mat4& outNormal) const;
    mat4 GetWorldMatrix() const;
    std::string ToString() const;
    static Transform Lerp(const Transform& a, const Transform& b, float t);

    vec3 GetForwardVector() const;
    vec3 GetRightVector() const;
    vec3 GetUpVector() const;

    static vec3 worldForward;
    static vec3 worldRight;
    static vec3 worldUp;
};

