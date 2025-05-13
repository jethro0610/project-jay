#pragma once
#include "EditorTarget.h"
#include "NullTarget.h"
#include "Text/Text.h"

class EditorTargetController {
private:
    EditorTarget* target_;
    NullTarget nullTarget_;

public:
    Text name_;

    EditorTarget& Get() {
        if (target_ == nullptr)
            return nullTarget_;
        else
            return *target_;
    }

    EditorTargetController() {
        target_ = nullptr;
    }

    void Select(EditorTarget* target) { 
        Deselect();
        target_ = target;
        target_->selected_ = true;
        std::string idString = target_->GetId() == - 1 ? 
            "" : 
            " [" + std::to_string(target_->GetId()) + "]";
        name_ = target_->GetName() + idString;
    }
    void Deselect() {
        if (target_ == nullptr)
            return;

        target_->selected_ = false;
        target_ = nullptr;
        name_ = "";
    }
    bool HasTarget() { return target_ != nullptr; }
};
