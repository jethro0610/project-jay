#pragma once
#include <glm/vec4.hpp>
#include <glm/vec2.hpp>
#include <math.h>
#include <FastNoiseLite.h>
#include <glm/gtx/norm.hpp>
#include <nlohmann/json.hpp>
#include "Editor/EditorTarget.h"
#include "StaticTerrainModifier.h"

class TerrainBubble : public StaticTerrainModifier {
public:
    static constexpr int ID = 1;
    static constexpr const char* NAME = "e_bubble";
    TerrainBubble();

    glm::vec2 position_;
    float radius_;
    float height_;

    bool customEasing_;
    float inPower_;
    float outPower_;

    void Init(const glm::vec3& pos = glm::vec3(0.0f));
    inline bool InRange(const glm::vec2& pos) {
        if (pos.x > position_.x + radius_)
            return false;
        if (pos.x < position_.x - radius_)
            return false;
        if (pos.y > position_.y + radius_)
            return false;
        if (pos.y < position_.y - radius_)
            return false;

        float dist = glm::distance2(pos, position_);
        return dist <= radius_ * radius_;
    }
    float GetHeight(const glm::vec2 &pos);

    void Save(nlohmann::json &json);
    void Load(const nlohmann::json &json);
    void WriteRenderNodes(vector_const<RenderNode, RenderNode::MAX>& nodes, Terrain& terrain);

    class ETarget : public EditorTarget {
    private:
        friend TerrainBubble;
        TerrainBubble* bubble_;

    public:
        std::string GetName() override { return TerrainBubble::NAME; }

        bool CanLabel() override { return true; }
        std::string GetLabel() override { return bubble_->label_; }
        void SetLabel(const std::string& label) override { 
            strcpy(bubble_->label_, label.c_str());
        }

        glm::vec3 GetPosition() override;
        void SetPosition(const glm::vec3 &pos) override;

        float GetScalar(char id) override;
        float GetScalarDelta(char id) override;
        void SetScalar(char id, float value) override;
        std::unordered_map<char, std::string> GetScalarNames() override {
            if (!bubble_->customEasing_)
                return {
                    {'1', "Radius"},
                    {'2', "Height"}
                };
            else
                return {
                    {'1', "Radius"},
                    {'2', "Height"},
                    {'3', "Ease In Power"},
                    {'4', "Ease Out Power"},
                };
        }
        void SetFlag(char id) override;
        std::vector<char> GetFlagIDs() override { return {',', '.'}; }

        bool UpdateTerrain() override { return true; }
        bool RayHit(const Ray &ray) override;
        void Destroy() override { bubble_->destroy_ = true; }
        bool IsModifier() override { return true; }
        bool Clonable() override { return true; }
        EditorTarget* Clone() override;
    };
    ETarget editorTarget_;
};
