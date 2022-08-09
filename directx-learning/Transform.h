#pragma once
#include <glm.hpp>
#include <gtx/quaternion.hpp>

struct Transform {
    Transform();
    glm::vec3 position_;
    glm::quat rotation_;
    glm::vec3 scale_;

    glm::mat4 GetWorldMatrix() const;
};

