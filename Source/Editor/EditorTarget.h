#pragma once
#include "Text/Text.h"

class Entity;
class EntityList;

class EditorTarget {
public:
    EditorTarget(EntityList& entities);
    void Set(Entity* target);
    Entity* Get() const { return target_; };
    Text name_;
    Text phase_;
    Text persist_;

private:
    EntityList& entities_;
    Entity* target_; 
};
