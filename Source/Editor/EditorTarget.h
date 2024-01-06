#pragma once
#include "Text/Text.h"

class EntityS;
class EntityListS;

class EditorTarget {
public:
    EditorTarget(EntityListS& entities);
    void Set(EntityS* target);
    EntityS* Get() const { return target_; };
    Text text_;

private:
    EntityListS& entities_;
    EntityS* target_; 
};
