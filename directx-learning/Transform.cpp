#include "Transform.h"

Transform::Transform() {
    position_ = glm::vec3(0.0f, 0.0f, 0.0f);
    rotation_ = glm::quat(glm::vec3(0.0f, 0.0f, 0.0f));
    scale_ = glm::vec3(1.0f, 1.0f, 1.0f);
}

glm::mat4 Transform::GetWorldMatrix() const {
    glm::mat4 worldMat = glm::mat4(1.0f);
    worldMat = glm::translate(worldMat, position_);
    worldMat *= glm::toMat4(rotation_);
    worldMat= glm::scale(worldMat, scale_);

    return worldMat;
}