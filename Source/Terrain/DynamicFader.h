#pragma once
#include "Shared_DynamicTerrainModifier.h"
class DynamicFader {
public:
    DynamicFader();
    static constexpr int MAX_MODIFIERS = 16;
    DynamicTerrainModifier* modifiers_[MAX_MODIFIERS];
    float* values_[MAX_MODIFIERS];
    float mins_[MAX_MODIFIERS];
    float maxs_[MAX_MODIFIERS];
    float activeOverrides_[MAX_MODIFIERS];
    bool transitionOnly_[MAX_MODIFIERS];
    int numModifiers_;

    bool active_;
    bool doActivate_;
    bool doDeactivate_;
    float percent_;
    float activateLength_;
    float deactivateLength_;

    void Update();
    void StartActivate();
    void StartDeactivate();
    void Toggle();
    bool IsActive();
    void ActivateModifiers(bool initial = false);
    void DeactivateModifiers(bool initial = false);
    void AddModifier(
        DynamicTerrainModifier* modifier, 
        float* value, 
        float min, 
        float max, 
        bool transitionOnly,
        float activeOverride_ = -INFINITY
    );
};
