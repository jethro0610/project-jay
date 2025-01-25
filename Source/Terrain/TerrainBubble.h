#pragma once
#include <glm/vec4.hpp>
#include <glm/vec2.hpp>
#include <math.h>
#include <FastNoiseLite.h>
#include <glm/gtx/norm.hpp>
#include "StaticTerrainModifier.h"
#include "Editor/EditorTarget.h"

class TerrainBubble : public StaticTerrainModifier {
public:
    class ETarget : public EditorTarget {
    private:
        friend TerrainBubble;
        TerrainBubble* bubble_;

    public:
        std::string GetName() override { return "e_bubble"; }

        glm::vec3 GetPosition() override;
        void SetPosition(const glm::vec3 &pos) override;

        glm::vec4 GetScale() override;
        void SetScale(const glm::vec4& ref, const glm::vec4& delta) override;

        bool UpdateTerrain() override { return true; }
        bool RayHit(const Ray &ray, Terrain& terrain) override;
    };
    TerrainBubble();
    TerrainBubble(glm::vec2 position, float radius, float height);

    glm::vec2 position_;
    float radius_;
    float height_;

    ETarget editorTarget_;

    float GetHeight(const glm::vec2 &pos) override;
    bool InRange(const glm::vec2 &pos) override;
};
