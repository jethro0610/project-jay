#pragma once
#include "../Types/EntityTypes.h"

template <class... T>
constexpr EntityKey GetEntityKey() {
    return ((1 << T::GetID()) | ...);
};
