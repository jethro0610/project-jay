#include "Character.h"
#include "Terrain/Terrain.h"
#include "Time/Time.h"

void Character::ApplyVelocity(Terrain& terrain) {
    float traceDistance = GetGroundTraceDistance();

    float groundHeight = terrain.GetHeight(transform_.position + velocity_);
    groundNormal_ = terrain.GetNormal(transform_.position + velocity_);
    float distanceToSurface = transform_.position.y - groundHeight;

    if (distanceToSurface < traceDistance) {
        onGround_ = true;
    }
    else {
        onGround_ = false;
        // transformComponent.up[i] = Transform::worldUp;
    }

    if (StickToGround()) {
        float stickVelocity = distanceToSurface / GlobalTime::TIMESTEP;
        velocity_.y = stickVelocity;
    }

    transform_.position += velocity_ * GlobalTime::TIMESTEP;
    transform_.rotation = GlobalTime::TIMESTEP * angularVelocity_ * transform_.rotation;
}
