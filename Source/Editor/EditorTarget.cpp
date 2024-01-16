#include "EditorTarget.h"
#include "Entity/EntityList.h"
#include "Entity/EntityUnion.h"

EditorTarget::EditorTarget(EntityList& entities):
entities_(entities)
{
    target_ = nullptr;
}

void EditorTarget::Set(Entity* target) {
    // Deselect the current target
    if (target_ != nullptr)
        target_->DBG_selected_ = false;

    // Select the new target
    if (target != nullptr) {
        target->DBG_selected_ = true;
        name_ = std::string(target->DBG_name_) + '(' + std::to_string(target->DBG_index_) + ')';
        phase_ = "Phase: " + std::to_string(target->phase_);
        persist_ = "Persist: " + std::string((target->persist_ ? "True" : "False"));
    }
    else {
        name_ = "";
        phase_ = "";
        persist_ = "";
    }

    target_ = target;
}
