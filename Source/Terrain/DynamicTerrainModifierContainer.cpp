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
