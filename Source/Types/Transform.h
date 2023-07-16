#pragma once
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/mat3x3.hpp>
#include <tuple>
#include <glm/gtx/quaternion.hpp>
#include <string>

typedef std::tuple<glm::mat4, glm::mat3> WorldNormalReturn;

struct Transform {
    Transform();
    glm::vec3 position;
    glm::quat rotation;
    glm::vec3 scale;

    WorldNormalReturn GetWorldAndNormalMatrix() const;
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

