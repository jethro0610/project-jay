#pragma once
#include <glm/vec4.hpp>
#include <glm/vec2.hpp>
#include <math.h>
#include <FastNoiseLite.h>
#include <glm/gtx/norm.hpp>
#include <nlohmann/json.hpp>
#include "Editor/EditorTarget.h"
#include "StaticTerrainModifier.h"

class TerrainBezier : public StaticTerrainModifier {
public:
    struct BezierPoint {
        glm::vec2 position;
        float radius;
        float height;
    };
    union BezierUnion {
        BezierPoint point;
        glm::vec4 vec;
    };
    static constexpr int ID = 3;
    static constexpr std::string NAME = "e_bezier";
    TerrainBezier();

    BezierUnion unionPoints_[4];
    bool customEasing_;
    float inPower_;
    float outPower_;

    void UpdateBounds();
    float minX_;
    float minY_;
    float maxX_;
    float maxY_;

    void Init(const glm::vec3& pos = glm::vec3(0.0f));
    bool InRange(const glm::vec2& pos);
    float GetHeight(const glm::vec2& pos);
    BezierPoint GetPoint(float t);
    BezierPoint GetNearestPoint(const glm::vec2& pos);

    void Save(nlohmann::json& json);
    void Load(const nlohmann::json& json);
    void WriteRenderNodes(vector_const<RenderNode, RenderNode::MAX>& nodes, Terrain& terrain);

    class ETarget : public EditorTarget {
    private:
        friend TerrainBezier;
        TerrainBezier* bezier_;
        BezierPoint* point_;
        int pointIdx_;

    public:
        std::string GetName() override { return TerrainBezier::NAME; }

        bool CanLabel() override { return true; }
        std::string GetLabel() override { return bezier_->label_; }
        void SetLabel(const std::string& label) override { 
            strcpy(bezier_->label_, label.c_str());
        }

        glm::vec3 GetPosition() override;
        void SetPosition(const glm::vec3 &pos) override;

        float GetScalar(char id) override;
        float GetScalarDelta(char id) override;
        void SetScalar(char id, float value) override;
        std::unordered_map<char, std::string> GetScalarNames() override {
            if (!bezier_->customEasing_)
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
        void Destroy() override { bezier_->destroy_ = true; }
        bool IsModifier() override { return true; }
    };
    ETarget bezierTargets_[4];
};

