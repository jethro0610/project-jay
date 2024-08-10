#include "TerrainCurve.h"
#include "Helpers/Ease.h"
#include <glm/gtx/compatibility.hpp>
#include <glm/gtx/norm.hpp>
using namespace glm;
using namespace TerrainConsts;

vec4 TerrainCurve::GetPosition(float t) const {
    // t = clamp(t, 0.0f, 1.0f);
    float invT = 1.0f - t;

    vec4 point = vec4(0.0f, 0.0f, 0.0f, 0.0f);
    point += invT * invT * invT * points[0];
    point += (t * 3.0f * invT * invT) * points[1];
    point += (t * t * 3.0f * invT) * points[2];
    point += t * t * t * points[3];

    return point;
}

TerrainInfluence TerrainCurve::GetInfluence(glm::vec2& pos, float offset) const {
    float pivot = 0.0f;
    float nearestSqr = INFINITY;
    float nearestRadius = 0.0f;
    float nearestHeight = 0.0f;
    float notchLength = 0.25f;

    for (int i = 1; i < 4; i++) {
        float t = i * notchLength;
        vec4 bezierPos = GetPosition(t);
        float dSqr = distance2(vec2(bezierPos.x, bezierPos.z), pos);
        if (dSqr < nearestSqr) {
            nearestSqr = dSqr;
            pivot = t;
        }
    }

    for (int i = 0; i < ITERATIONS - 1; i++) {
        float start = pivot - notchLength;
        notchLength *= 0.5f;

        nearestSqr = INFINITY;
        for (int j = 0; j < 5; j++) {
            float t = start + j * notchLength;
            vec4 bezierPos = GetPosition(t);
            float dSqr = distance2(vec2(bezierPos.x, bezierPos.z), pos);
            if (dSqr < nearestSqr) {
                nearestRadius = bezierPos.w;
                nearestHeight = bezierPos.y;
                nearestSqr = dSqr;
                pivot = clamp(t, 0.0f, 1.0f);
            }
        }
    }

    if (pivot == 0.0f) {
        vec4 direction4D = points[0] - GetPosition(0.05f); 

        vec2 direction = vec2(direction4D.x, direction4D.z);
        float directionLen = length(direction);
        float riseRate = direction4D.y / directionLen;
        direction /= directionLen;

        vec2 directionToSample = pos - vec2(points[0].x, points[0].z);
        float d = dot(direction, directionToSample);
        nearestHeight = points[0].y + riseRate * d;
    }
    else if (pivot == 1.0f) {
        vec4 direction4D = GetPosition(0.95f) - points[3]; 

        vec2 direction = vec2(direction4D.x, direction4D.z);
        float directionLen = length(direction);
        float riseRate = direction4D.y / directionLen;
        direction /= directionLen;

        vec2 directionToSample = pos - vec2(points[3].x, points[3].z);
        float d = dot(direction, directionToSample);
        nearestHeight = points[3].y + riseRate * d;
    }

    TerrainInfluence influence;
    influence.distance = (sqrt(nearestSqr) / nearestRadius);
    influence.height = (nearestHeight + offset) * EaseInOutQuad(1.0f - influence.distance);
    return influence;
}
