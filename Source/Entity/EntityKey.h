#pragma once
#include <stdint.h>
typedef uint32_t EntityKey;

template <class... T>
constexpr EntityKey GetEntityKey() {
    return ((1 << T::GetID()) | ...);
};
