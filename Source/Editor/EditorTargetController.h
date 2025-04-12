#pragma once
#include "EditorTarget.h"
#include "Text/Text.h"

class EditorTargetController {
private:
    EditorTarget* target_;

public:
    Text name_;

    EditorTarget* GetTarget() {
        return target_;
    }

    EditorTargetController() {
        target_ = nullptr;
    }

    void Select(EditorTarget* target) { 
        Deselect();
        target_ = target;
        target_->selected_ = true;
        name_ = target_->GetName();
    }
    void Deselect() {
        if (target_ == nullptr)
            return;

        target_->selected_ = false;
        target_ = nullptr;
        name_ = "";
    }
    bool HasTarget() { return target_ != nullptr; }

    glm::vec3 GetPosition() { 
        if (target_ == nullptr)
            return glm::vec3(0.0f);

        return target_->GetPosition(); 
    }
    void SetPosition(const glm::vec3& pos) { 
        if (target_ == nullptr)
            return;

        target_->SetPosition(pos); 
    }

    glm::vec4 GetScale() { 
        if (target_ == nullptr)
            return glm::vec4(0.0f);

        return target_->GetScale(); 
    }
    void SetScale(const glm::vec4& ref, const glm::vec4& delta) { 
        if (target_ == nullptr)
            return;

        target_->SetScale(ref, delta); 
    }

    glm::quat GetRotation() { 
        if (target_ == nullptr)
            return glm::quat(0.0f, 0.0f, 0.0f, 0.0f);

        return target_->GetRotation(); 
    }
    void SetRotation(const glm::quat& rot) { 
        if (target_ == nullptr)
            return;

        target_->SetRotation(rot); 
    }

    float GetScalar(char id) {
        if (target_ == nullptr)
            return 0.0f;

        return target_->GetScalar(id);
    }

    float GetScalarDelta(char id) {
        if (target_ == nullptr)
            return 0.0f;

        return target_->GetScalarDelta(id);
    }

    std::vector<char> GetFlagIDs() {
        if (target_ == nullptr)
            return {};

        return target_->GetFlagIDs();
    }

    void SetFlag(char id) {
        if (target_ == nullptr)
            return;

        target_->SetFlag(id);
    }

    void SetScalar(char id, float value) {
        if (target_ == nullptr)
            return;

        target_->SetScalar(id, value);
    }

    std::unordered_map<char, std::string> GetScalarNames() {
        if (target_ == nullptr)
            return {};

        return target_->GetScalarNames();
    }

    EntityProperties GetProperties() {
        if (target_ == nullptr)
            return EntityProperties();

        return target_->GetProperties();
    }

    EditorTarget* Clone() {
        if (target_ == nullptr)
            return nullptr;

        return target_->Clone();
    }

    bool IsEntity() { 
        if (target_ == nullptr)
            return false;

        return target_->IsEntity(); 
    }

    bool IsModifier() {
        if (target_ == nullptr)
            return false;

        return target_->IsModifier();
    }

    bool Clonable() {
        if (target_ == nullptr)
            return false;

        return target_->Clonable();
    }

    bool TerrainAlignable() {
        if (target_ == nullptr)
            return false;

        return target_->TerrainAlignable();
    }

    bool UpdateTerrain() {
        if (target_ == nullptr)
            return false;

       return target_->UpdateTerrain(); 
    }

    void Destroy() { 
        if (target_ == nullptr)
            return;

        target_->Destroy(); 
        target_ = nullptr;
        name_ = "";
    }

    void SetPreview(bool value) {
        if (target_ == nullptr)
            return;

        target_->SetPreview(value);
    }

    bool GetPreview(bool value) {
        if (target_ == nullptr)
            return false;

        return target_->GetPreview();
    }

    void TogglePreview() {
        if (target_ == nullptr)
            return;

        target_->TogglePreview();
    }
};
