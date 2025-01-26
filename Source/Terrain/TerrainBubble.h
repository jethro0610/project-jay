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
    static constexpr std::string NAME = "e_bubble";
    TerrainBubble();

    float height_;

    void Init(const glm::vec3& pos = glm::vec3(0.0f));
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

        glm::vec3 GetPosition() override;
        void SetPosition(const glm::vec3 &pos) override;

        glm::vec4 GetScale() override;
        void SetScale(const glm::vec4& ref, const glm::vec4& delta) override;

        bool UpdateTerrain() override { return true; }
        bool RayHit(const Ray &ray) override;
        void Destroy() override { bubble_->destroy_ = true; }
    };
    ETarget editorTarget_;
};
