#pragma once
#include <glm.hpp>
#include <gtx/quaternion.hpp>
#include <gtx/compatibility.hpp>
#include <gtx/string_cast.hpp>
#include <string>

class Transform {
public:
    Transform();
    glm::vec3 position_;
    glm::quat rotation_;
    glm::vec3 scale_;

    void GetWorldAndNormalMatrix(glm::mat4& outWorld, glm::mat4& outNormal) const;
    glm::mat4 GetWorldMatrix() const;
    std::string ToString() const;
    static Transform Lerp(const Transform& a, const Transform& b, float t);

    glm::vec3 GetForwardVector() const;
    glm::vec3 GetRightVector() const;
    glm::vec3 GetUpVector() const;

    static glm::vec3 worldForward;
    static glm::vec3 worldRight;
    static glm::vec3 worldUp;
};

