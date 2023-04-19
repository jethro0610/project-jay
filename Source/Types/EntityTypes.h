#pragma once
#include <stdint.h>
#include <tuple>
class Transform;

typedef uint16_t EntityID;
typedef std::tuple<EntityID, Transform&> EntityReturn;
