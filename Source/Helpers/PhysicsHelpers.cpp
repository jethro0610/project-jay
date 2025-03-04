#include "PhysicsHelpers.h"
#include "Time/Time.h"
#include <math.h>

float GetAirtime(float gravity, float yVel, float curHeight, float targetHeight) {
    // Quadratic equation to get airtime
    float a = -0.5 * gravity * GlobalTime::TIMESTEP;
    float b = yVel * GlobalTime::TIMESTEP;
    float bSqr = b * b;
    float c = curHeight - targetHeight;
    return ((-b - sqrt(bSqr - 4 * a * c)) / (2 * a)) * GlobalTime::TIMESTEP;
}
