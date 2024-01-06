#include "EditorTarget.h"
#include "Entity/EntityListS.h"
#include "Entity/EntityUnion.h"

EditorTarget::EditorTarget(EntityListS& entities):
entities_(entities)
{
    target_ = NULL_ENTITY;
}

void EditorTarget::Set(EntityIDS target) {
    // Deselect the current target
    if (target_ != NULL_ENTITY)
        entities_[target_].DBG_selected_ = false;

    // Select the new target
    if (target != NULL_ENTITY) {
        entities_[target].DBG_selected_ = true;
        #define ENTITYEXP(TYPE, VAR) TYPE VAR;
        EXPANDENTITIES
        #undef ENTITYEXP
        text_ = std::string(entities_.GetName(target)) + '(' + std::to_string(target) + ')';
    }
    else
        text_ = "";

    target_ = target;
}
