#pragma once
#include "Entity/EntityIDS.h"
#include "Text/Text.h"

class EntityListS;

class EditorTarget {
public:
    EditorTarget(EntityListS& entities);
    void Set(EntityIDS target);
    EntityIDS Get() const { return target_; };
    Text text_;

private:
    EntityListS& entities_;
    EntityIDS target_; 
};
