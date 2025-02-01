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
    static constexpr std::string NAME = "e_noise";
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

        glm::vec3 GetPosition() override;
        void SetPosition(const glm::vec3 &pos) override;

        glm::vec4 GetScale() override;
        void SetScale(const glm::vec4& ref, const glm::vec4 &scale) override;

        float GetScalar(char id) override;
        float GetScalarDelta(char id) override;
        void SetScalar(char id, float value) override;
        std::unordered_map<char, std::string> GetScalarNames() override {
            return {
                {'H', "Max Height"},
                {'M', "Min Height"},
                {'R', "Radius"},
                {'F', "Frequency"},
                {'I', "Ease In Power"},
                {'O', "Ease Out Power"},
            };
        }
        void SetEasing(int easeMode) override { noise_->easeMode_ = (EaseMode)easeMode; }

        bool UpdateTerrain() override { return true; }
        bool RayHit(const Ray &ray) override;
        void Destroy() override { noise_->destroy_ = true; }
        bool IsModifier() override { return true; }
    };
    ETarget editorTarget_;
};
