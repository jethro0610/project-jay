#pragma once
#include "../Types/Transform.h"
#include "../Types/Inputs.h"
#include "Components/TransformComponents.h"

#define NO_TRACK -1

class Camera {
public:
    Camera(TransformComponents* transformComponents, float trackDistance, vec3 startPosition = vec3(0.0f, 0.0f, 0.0f));
    Camera(TransformComponents* transformComponents, float trackDistance, int trackEntity);
    Transform firstPersonTransform_;
    
    float lookX_;
    float lookY_;

    int trackEntity_;
    float trackDistance_;

    void Update(float deltaTime, Inputs inputs);
    mat4 GetViewMatrix() const;

private:
    TransformComponents* transformComponents_;
};

