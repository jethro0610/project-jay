#include "ModifyEntityMode.h"
#include "Entity/Entity.h"
#include "EditorNotification.h"
#include "EditorTextInput.h"
#include "EditorTargetController.h"

ModifyEntityMode::ModifyEntityMode(EditorModeArgs args):
EditorMode(args) {
    requiresLevel_ = true;
    mouseVisibile_ = false;
    ctrl_ = false;
}

void ModifyEntityMode::OnStart() {
    property_ = "";

    propFloat_ = nullptr;
    propInt_ = nullptr;
    propBool_ = nullptr;

    properties_ = target_.Get().GetProperties();

    modeText_ = "Modify Entity";
    textInput_.SetLabel("Select property: p_");
    state_ = SelectProperty;
    EditorMode::OnStart();
}

void ModifyEntityMode::OnEnd() {
    textInput_.Clear();
    EditorMode::OnEnd();
}

void ModifyEntityMode::OnCancel() {
    EditorMode::OnCancel();
}

void ModifyEntityMode::Update() {
    textInput_.ReadInput();
    EditorMode::Update();
}

ConfirmBehavior ModifyEntityMode::OnConfirm() {
    switch (state_) {
        case ModifyProperty: {
            bool validValue = ModifySelectedProperty(textInput_.Get());
            if (validValue) {
                return CB_Default;
            }
            else {
                notificaiton_.Set("Invalid value");
            }
            break;
        }

        case SelectProperty: {
            std::string property = "p_" + textInput_.Get();
            bool validProperty = SetSelectedProperty(property);
            if (validProperty) {
                textInput_.Clear();
                textInput_.SetLabel(property + ": ");
                textInput_.SetInput(properties_.GetValueAsString(property));
                state_ = ModifyProperty;
            }
            else {
                notificaiton_.Set("Invalid property");
            }
            break;
        }
    }
     

    return CB_Stay;
}

bool ModifyEntityMode::SetSelectedProperty(const std::string& property) {
    float* propFloat = properties_.GetFloat(property);
    if (propFloat != nullptr) {
        property_ = property;
        propFloat_ = propFloat;
        return true;
    }

    int* propInt = properties_.GetInt(property);
    if (propInt != nullptr) {
        property_ = property;
        propInt_ = propInt; 
        return true;
    }

    bool* propBool = properties_.GetBool(property);
    if (propBool != nullptr) {
        property_ = property;
        propBool_ = propBool;
        return true;
    }

    return false;
}

bool ModifyEntityMode::ModifySelectedProperty(const std::string& value) {
    if (propFloat_ != nullptr) {
        try {
            *propFloat_ = std::stof(value);
            return true;
        }
        catch (...) {
            return false;
        }
    }

    if (propInt_ != nullptr) {
        try {
            *propInt_ = std::stoi(value);
            return true;
        }
        catch (...) {
            return false;
        }
    }

    if (propBool_ != nullptr) {
        if (value == "t") {
            *propBool_ = true;
            return true;
        }
        else if (value == "f") {
            *propBool_ = false;
            return true;
        }
        else
            return false;
    }

    return false;
}
