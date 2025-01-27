#pragma once
#include <glm/glm.hpp>
#include "Shared_DynamicTerrainModifier.h"

class DynamicTerrainModifierContainer {
public:
    DynamicTerrainModifierContainer();
    void WriteModifiers(DynamicTerrainModifier* arr);
    DynamicTerrainModifier* RequestAvailableModifier();
    void FreeModifier(DynamicTerrainModifier* modifier);
    const void* Data() const { return modifiers_; }

    const DynamicTerrainModifier& operator[](int index) const {
        return modifiers_[index];
    }

private:
    DynamicTerrainModifier modifiers_[DYN_MOD_MAX];
};
