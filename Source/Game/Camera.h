#pragma once
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include "../Types/Transform.h"
#include "../Types/Inputs.h"
class TransformComponent;

const int8_t NO_TRACK = -1;

class Camera {
public:
    Camera(TransformComponent& transformComponent, float trackDistance, glm::vec3 startPosition = glm::vec3(0.0f, 0.0f, 0.0f));
    Camera(TransformComponent& transformComponent, float trackDistance, int trackEntity);
    Transform transform_;
    glm::vec3 smoothTrackPosition_;
    
    float lookX_;
    float lookY_;

    int trackEntity_;
    float trackDistance_;

    void Update(Inputs inputs);
    glm::mat4 GetViewMatrix() const;
private:
    TransformComponent& transformComponent_;
};

