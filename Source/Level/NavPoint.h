#pragma once
#ifdef _DEBUG
#include "Editor/EditorTarget.h"

class NavPoint {
public:
    NavPoint() : target_(this) {};
    std::string label_;
    glm::vec3 position_;
    bool active_;

    class Target : public EditorTarget {
    private:
        NavPoint* navpoint_;

    public:
        Target(NavPoint* navpoint) : navpoint_(navpoint) {}
        std::string GetName() override { return "e_navpoint"; }
        bool Selectable() override { return navpoint_->active_; }

        bool CanLabel() override { return true; }
        std::string GetLabel() override { return navpoint_->label_; }
        void SetLabel(const std::string& label) override { navpoint_->label_ = label; }

        glm::vec3 GetPosition() override { return navpoint_->position_; }
        void SetPosition(const glm::vec3& position) override { navpoint_->position_ = position; }

        bool RayHit(const Ray& ray) override;

        void Destroy() override { navpoint_->active_ = false; }
    };
    Target target_;
};
#endif
