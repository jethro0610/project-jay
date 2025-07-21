#pragma once
#include <glm/vec4.hpp>
#include <glm/vec2.hpp>
#include <math.h>
#include <FastNoiseLite.h>
#include <glm/gtx/norm.hpp>
#include "StaticTerrainModifier.h"
#include "Editor/EditorTarget.h"

class TerrainNoise : public StaticTerrainModifier {
public:
    static constexpr int ID = 2;
    static constexpr const char* NAME = "e_noise";
    TerrainNoise();

    FastNoiseLite noise_;
    glm::vec2 position_;
    float radius_;
    int seed_;
    float minHeight_;
    float maxHeight_;
    float frequency_;

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
    float GetHeight(const glm::vec2& pos);
    void WriteRenderNodes(vector_const<RenderNode, RenderNode::MAX>& nodes, Terrain& terrain);

    void Save(nlohmann::json& json);
    void Load(const nlohmann::json& json);

    class ETarget : public EditorTarget {
    private:
        friend TerrainNoise;
        TerrainNoise* noise_;

    public:
        std::string GetName() override { return TerrainNoise::NAME; }

        bool CanLabel() override { return true; }
        std::string GetLabel() override { return noise_->label_; }
        void SetLabel(const std::string& label) override { 
            strcpy(noise_->label_, label.c_str());
        }

        glm::vec3 GetPosition() override;
        void SetPosition(const glm::vec3 &pos) override;

        float GetScalar(char id) override;
        float GetScalarDelta(char id) override;
        void SetScalar(char id, float value) override;
        std::unordered_map<char, std::string> GetScalarNames() override {
            return {
                {'1', "Radius"},
                {'2', "Frequency"},
                {'3', "Max Height"},
                {'4', "Min Height"},
            };
        }

        bool UpdateTerrain() override { return true; }
        bool RayHit(const Ray &ray) override;
        void Destroy() override { noise_->destroy_ = true; }
        bool IsModifier() override { return true; }
    };
    ETarget editorTarget_;
};
