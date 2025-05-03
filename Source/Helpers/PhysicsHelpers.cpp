#include "PhysicsHelpers.h"
#include "Time/Time.h"
#include <math.h>
#include <glm/glm.hpp>
using namespace glm;

float GetAirtime(float gravity, float yVel, float curHeight, float targetHeight) {
    // Quadratic equation to get airtime
    float a = -0.5 * gravity * GlobalTime::TIMESTEP;
    float b = yVel * GlobalTime::TIMESTEP;
    float bSqr = b * b;
    float c = curHeight - targetHeight;
    return ((-b - sqrt(bSqr - 4 * a * c)) / (2 * a)) * GlobalTime::TIMESTEP;
}

vec3 GetProjectileVelocityToPoint(const vec3& position, const vec3& target, float gravity, float yVel) {
    float airtime = GetAirtime(gravity, yVel, position.y, target.y);
    if (isnan(airtime))
        return vec3(0.0f);

    // Get the direction and distance to the point
    vec3 planarPosition = position;
    planarPosition.y = 0.0f;
    vec3 planarTargetPosition = target;
    planarTargetPosition.y = 0.0f;
    float distanceToPoint = distance(planarTargetPosition, planarPosition);
    vec3 directionToPoint = normalize(planarTargetPosition - planarPosition);

    // Then get the required magnitude based on the airtime
    float magToPoint = (distanceToPoint / airtime);

    return directionToPoint * magToPoint + vec3(0.0f, yVel, 0.0f);
}
