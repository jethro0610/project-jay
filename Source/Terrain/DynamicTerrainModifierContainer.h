#pragma once
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include "Shared_DynamicTerrainModifier.h"

class DynamicTerrainModifierContainer {
public:
    DynamicTerrainModifierContainer();
    DynamicTerrainModifier* RequestAvailableModifier();
    void FreeModifier(DynamicTerrainModifier* modifier);
    void Interpolate(float interpTime);
    void RecordLastTick();

    inline glm::mat4 GetRenderMatrix(int index) {
        glm::mat4 matrix = interpolaltedModifiers_[index];
        matrix[0][3] = matrix[0][3] != 0.0f ? 1.0f : 0.0f;
        return transpose(matrix);
    }

    inline const glm::mat4& operator[](int index) const {
        return modifiers_[index];
    }

    inline const glm::mat4& GetMatrix(int index) const {
        return modifiers_[index];
    }

    inline const DynamicTerrainModifier& GetModifier(int index) const {
        return *(DynamicTerrainModifier*)&modifiers_[index];
    }

private:
    glm::mat4 modifiers_[DYN_MOD_MAX];
    glm::mat4 modifiersLastTick_[DYN_MOD_MAX];
    glm::mat4 interpolaltedModifiers_[DYN_MOD_MAX];
    bool modifiersInUse_[DYN_MOD_MAX];
};
