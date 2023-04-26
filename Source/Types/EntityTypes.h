#pragma once
#include <stdint.h>
#include <tuple>
class Transform;

typedef uint16_t EntityID;
typedef int32_t EntityIDNullable;
typedef uint32_t EntityKey;
typedef std::tuple<EntityID, Transform&> EntityReturn;
const int32_t NO_ENTITY = -1;
