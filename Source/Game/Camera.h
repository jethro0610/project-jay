#pragma once
#include <glm.hpp>
#include "../Types/Transform.h"
#include "../Types/Inputs.h"
class EntityManager;

const int8_t NO_TRACK = -1;

class Camera {
public:
    Camera(EntityManager& entityManager, float trackDistance, glm::vec3 startPosition = glm::vec3(0.0f, 0.0f, 0.0f));
    Camera(EntityManager& entityManager, float trackDistance, int trackEntity);
    Transform transform_;
    glm::vec3 smoothTrackPosition_;
    
    float lookX_;
    float lookY_;

    int trackEntity_;
    float trackDistance_;

    void Update(float deltaTime, Inputs inputs);
    glm::mat4 GetViewMatrix() const;
private:
    EntityManager& entityManager_;
};

