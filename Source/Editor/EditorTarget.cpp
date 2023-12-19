#include "EditorTarget.h"

EditorTarget::EditorTarget(EntityManager& entityManager):
entityManager_(entityManager)
{
    target_ = NULL_ENTITY;
}

void EditorTarget::Set(EntityID target) {
    // Deselect the current target
    if (target_ != NULL_ENTITY)
        entityManager_.entities_[target_].DBG_selected = false;

    // Select the new target
    if (target != NULL_ENTITY) {
        entityManager_.entities_[target].DBG_selected = true;
        text_ = entityManager_.entities_[target].DBG_name + '(' + std::to_string(target) + ')';
    }
    else
        text_ = "";

    target_ = target;
}
