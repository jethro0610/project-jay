#pragma once
#include "Entity/EntityID.h"
#include "Entity/EntityManager.h"
#include "Text/Text.h"

class EditorTarget {
public:
    EditorTarget(EntityManager& entityManager);
    void Set(EntityID target);
    EntityID Get() const { return target_; };
    Text text_;

private:
    EntityManager& entityManager_;
    EntityID target_; 
};
