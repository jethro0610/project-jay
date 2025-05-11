#include "DynamicFader.h"
#include "Helpers/Shared_Ease.h"
#include "Logging/Logger.h"

DynamicFader::DynamicFader() {
    numModifiers_ = 0;
    active_ = false;
    doActivate_ = false;
    doDeactivate_ = false;
    percent_ = 0.0f;
    activateLength_ = 0.0f;
    deactivateLength_ = 0.0f;
}

void DynamicFader::Update() {
    bool percentChanged = false;
    if (doActivate_) {
        percentChanged = true;
        percent_ += 1.0f / activateLength_; 
        if (percent_ >= 1.0f) {
            percent_ = 1.0f;
            doActivate_ = false;
        }
    }
    else if (doDeactivate_) {
        percentChanged = true;
        percent_ -= 1.0f / deactivateLength_; 
        if (percent_ <= 0.0f) {
            percent_ = 0.0f;
            doDeactivate_ = false;
            DeactivateModifiers();
        }
    }


    if (percentChanged) {
        float easePercent = EaseQuad(percent_);
        for (int i = 0; i < numModifiers_; i++) {
            if (transitionOnly_[i])
                modifiers_[i]->active = true;
            if (percent_ >= 1.0f)  
                *values_[i] = activeOverrides_[i]; 
            else {
                float min = mins_[i];         
                float max = maxs_[i];
                *values_[i] = glm::mix(min, max, easePercent);
            }
        }
    }
    else {
        for (int i = 0; i < numModifiers_; i++) {
            if (transitionOnly_[i])
                modifiers_[i]->active = false;
        }
    }
}

void DynamicFader::StartActivate() {
    if (!active_ || doDeactivate_) {
        if (activateLength_ <= 0.0f)
            ActivateModifiers();
        else {
            doActivate_ = true;
            doDeactivate_ = false;
            ActivateModifiers();
        }
    }
}

void DynamicFader::StartDeactivate() {
    if (active_ || doActivate_) {
        if (deactivateLength_ <= 0.0f)
            DeactivateModifiers();
        else {
            doDeactivate_ = true;
            doActivate_ = false;
        }
    }
}

void DynamicFader::ActivateModifiers(bool initial) {
    active_ = true;
    for (int i = 0; i < numModifiers_; i++) {
        if (!transitionOnly_[i])
            modifiers_[i]->active = true; 
        *values_[i] = activeOverrides_[i];
    }
    if (initial)
        percent_ = 1.0f;
}

void DynamicFader::DeactivateModifiers(bool initial) {
    active_ = false;
    for (int i = 0; i < numModifiers_; i++) {
        if (!transitionOnly_[i])
            modifiers_[i]->active = false; 
    }
    if (initial)
        percent_ = 0.0f;
}

void DynamicFader::AddModifier(DynamicTerrainModifier* modifier, float* value, float min, float max, bool transitionOnly, float activeOverride) {
    modifiers_[numModifiers_] = modifier;
    values_[numModifiers_] = value;
    mins_[numModifiers_] = min;
    maxs_[numModifiers_] = max;
    activeOverrides_[numModifiers_] = activeOverride == -INFINITY ? max : activeOverride;
    transitionOnly_[numModifiers_] = transitionOnly;
    numModifiers_++;
}

bool DynamicFader::IsActive() {
    return ((active_ || doActivate_) && !doDeactivate_);
}

void DynamicFader::Toggle() {
    if (IsActive()) 
        StartDeactivate();
    else 
        StartActivate();
}
