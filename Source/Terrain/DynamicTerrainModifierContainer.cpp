#include "DynamicTerrainModifierContainer.h"
#include "Helpers/Assert.h"

DynamicTerrainModifierContainer::DynamicTerrainModifierContainer() {
    for (int i = 0; i < DYN_MOD_MAX; i++) {
        modifiers_[i] = {};
        modifiersInUse_[i] = false;
    }
}

DynamicTerrainModifier* DynamicTerrainModifierContainer::RequestAvailableModifier() {
    for (int i = 0; i < DYN_MOD_MAX; i++) {
        if (modifiersInUse_[i])
            continue;
        modifiersInUse_[i] = true;
        return (DynamicTerrainModifier*)&modifiers_[i];
    }
    ASSERT(false, "No modifiers available");
    return nullptr;
}

void DynamicTerrainModifierContainer::FreeModifier(DynamicTerrainModifier* modifier) {
    for (int i = 0; i < DYN_MOD_MAX; i++) {
        if (&modifiers_[i] == (glm::mat4*)modifier) {
            ASSERT(modifiersInUse_[i], "Tried to free inactive modifier");
            modifiers_[i] = {};
            modifiersInUse_[i] = false;
            return;
        }
    }
    ASSERT(false, "Tried to free non-existant modifier");
}

void DynamicTerrainModifierContainer::Interpolate(float interpTime) {
    for (int i = 0; i < DYN_MOD_MAX; i++) {
        for (int j = 0; j < 4; j++) {
            for (int k = 0; k < 4; k++) {
                interpolaltedModifiers_[i][j][k] = glm::mix(modifiersLastTick_[i][j][k], modifiers_[i][j][k], interpTime);
            }
        }
    }
}

void DynamicTerrainModifierContainer::RecordLastTick() {
    for (int i = 0; i < DYN_MOD_MAX; i++) {
        modifiersLastTick_[i] = modifiers_[i];
    }
}
