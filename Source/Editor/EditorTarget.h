#pragma once
#include "Text/Text.h"

class EntityS;
class EntityList;

class EditorTarget {
public:
    EditorTarget(EntityList& entities);
    void Set(EntityS* target);
    EntityS* Get() const { return target_; };
    Text text_;

private:
    EntityList& entities_;
    EntityS* target_; 
};
