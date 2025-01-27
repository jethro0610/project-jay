#include "DynamicTerrainModifierContainer.h"
#include "Helpers/Assert.h"

DynamicTerrainModifierContainer::DynamicTerrainModifierContainer() {
    for (int i = 0; i < DYN_MOD_MAX; i++) {
        modifiers_[i] = {};
        DYN_MOD_SET_ACTIVE(modifiers_[i], false);
    }
}

DynamicTerrainModifier* DynamicTerrainModifierContainer::RequestAvailableModifier() {
    for (int i = 0; i < DYN_MOD_MAX; i++) {
        if (DYN_MOD_ACTIVE(modifiers_[i]))
            continue;
        DYN_MOD_SET_ACTIVE(modifiers_[i], true);
        return &modifiers_[i];
    }
    ASSERT(false, "No modifiers available");
    return nullptr;
}

void DynamicTerrainModifierContainer::FreeModifier(DynamicTerrainModifier* modifier) {
    for (int i = 0; i < DYN_MOD_MAX; i++) {
        if (&modifiers_[i] == modifier) {
            ASSERT(DYN_MOD_ACTIVE(modifiers_[i]), "Tried to free inactive modifier");
            modifiers_[i] = {};
            DYN_MOD_SET_ACTIVE(modifiers_[i], false);
            return;
        }
    }
    ASSERT(false, "Tried to free non-existant modifier");
}
