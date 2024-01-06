#include "EditorTarget.h"
#include "Entity/EntityListS.h"
#include "Entity/EntityUnion.h"

EditorTarget::EditorTarget(EntityListS& entities):
entities_(entities)
{
    target_ = nullptr;
}

void EditorTarget::Set(EntityS* target) {
    // Deselect the current target
    if (target_ != nullptr)
        target_->DBG_selected_ = false;

    // Select the new target
    if (target != nullptr) {
        target->DBG_selected_ = true;
        text_ = std::string(target_->DBG_name_) + '(' + std::to_string(target_->DBG_index_) + ')';
    }
    else
        text_ = "";

    target_ = target;
}
