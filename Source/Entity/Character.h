#pragma once
#include "EntitySimp.h"
#include <vector_const.h>
#include <glm/glm.hpp>

class Hurtbox;
class Pushbox;
class Terrain;

class Character : public EntitySimp {
public:
    glm::vec3 desiredMovement_;
    glm::vec3 velocity_;
    glm::quat angularVelocity_;
    int hitlagTimer_;

    bool onGround_;
    glm::vec3 groundNormal_;

    virtual void Update();
    virtual void CalculateVelocity() = 0;

    void ApplyVelocity(Terrain& terrain);

protected:
    virtual float GetGroundTraceDistance() { return 1.0f; }
    virtual bool StickToGround() { return false; };
};
