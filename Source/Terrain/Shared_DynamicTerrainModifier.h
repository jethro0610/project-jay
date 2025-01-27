#pragma once
#ifndef SHARED_SHADER
#include <glm/glm.hpp>
#define VEC2 glm::vec2
typedef glm::mat4 DynamicTerrainModifier;
#else
#define VEC2 vec2
#endif

#define DYN_MOD_MAX 16
#define DYN_MOD_POS_X(modifier) modifier[0][0]
#define DYN_MOD_POS_Y(modifier) modifier[0][1]
#define DYN_MOD_RADIUS(modifier) modifier[0][2]
#define DYN_MOD_VALUE(modifier) modifier[0][3]
#define DYN_MOD_ACTIVE(modifier) (modifier[1][0] > 0.0f)
#define DYN_MOD_SET_ACTIVE(modifier, acc) (modifier[1][0] = acc ? 1.0f : 0.0f)
