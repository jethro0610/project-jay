#pragma once
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include "Types/Transform.h"
#include "Types/Inputs.h"

class Entity;
class EntityList;
class TransformComponent;

class Camera {
public:
    Camera(float trackDistance, glm::vec3 startPosition = glm::vec3(0.0f, 0.0f, 0.0f));
    Transform transform_;
    glm::vec3 smoothTrackPosition_;
    
    float lookX_;
    float lookY_;

    Entity* target_;
    float trackDistance_;

    void Update(EntityList& entities, Inputs& inputs);
    void Update(Inputs& inputs);
    glm::mat4 GetViewMatrix() const;
    glm::mat4 GetViewOnlyMatrix() const;
};

