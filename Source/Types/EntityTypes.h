#pragma once
#include <stdint.h>
#include <tuple>
class Transform;

typedef uint16_t EntityID;
typedef uint32_t EntityKey;
typedef std::tuple<EntityID, Transform&> EntityReturn;
