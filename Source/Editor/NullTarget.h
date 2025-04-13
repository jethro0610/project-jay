#pragma once
#include "EditorTarget.h"
class NullTarget : public EditorTarget {
    std::string GetName() override { return "e_null"; }
    bool RayHit(const Ray& ray) override { return false; }
};
